/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/pgtable.h>

#include <mach/hardware.h>
#include <linux/sprd_cproc.h>
#include <mach/sci.h>
#include <mach/sci_glb_regs.h>

#define CPROC_WDT_TRUE   1
#define CPROC_WDT_FLASE  0
/*used for ioremap to limit vmalloc size, shi yunlong*/
#define CPROC_VMALLOC_SIZE_LIMIT 4096

enum {
	CP_NORMAL_STATUS=0,
	CP_STOP_STATUS,
	CP_WDTIRQ_STATUS,
	CP_MAX_STATUS,
};

const char *cp_status_info[] = {
	"started\n",
	"stopped\n",
	"wdtirq\n",
};

struct cproc_proc_fs;

struct cproc_proc_entry {
	char				*name;
	struct proc_dir_entry	*entry;
	union {
		struct cproc_device		*cproc;
		struct cproc_segments	*seg;
	} data;
};

struct cproc_proc_fs {
	struct proc_dir_entry		*procdir;

	struct cproc_proc_entry		start;
	struct cproc_proc_entry		stop;
	struct cproc_proc_entry		status;
	struct cproc_proc_entry		wdtirq;
	struct cproc_proc_entry		mem;
	struct cproc_proc_entry		*processor;
};

struct cproc_device {
	struct miscdevice		miscdev;
	struct cproc_init_data	*initdata;
	void				*vbase;
	int 				wdtirq;
	int				wdtcnt;
	wait_queue_head_t		wdtwait;
	char *				name;
	int					status;
	struct cproc_proc_fs		procfs;
};

static ssize_t sprd_cproc_seg_dump(uint32_t base, uint32_t maxsz,
	char __user *buf, size_t count, loff_t offset)
{
	void *vmem;
	uint32_t loop = 0;
	uint32_t start_addr;
	uint32_t total;

	if (offset >= maxsz)
		return 0;

	if ((offset + count) > maxsz)
		count = maxsz - offset;

	start_addr = base + offset;
	total = count;

	do {
		uint32_t copy_size = CPROC_VMALLOC_SIZE_LIMIT;

		vmem = ioremap(start_addr + CPROC_VMALLOC_SIZE_LIMIT * loop,
				CPROC_VMALLOC_SIZE_LIMIT);
		if (!vmem) {
			printk(KERN_ERR "Unable to map cproc base: 0x%08x\n",
				start_addr + CPROC_VMALLOC_SIZE_LIMIT * loop);
			if (loop > 0)
				return CPROC_VMALLOC_SIZE_LIMIT * loop;
			else
				return -ENOMEM;

		}
		if (count < CPROC_VMALLOC_SIZE_LIMIT)
			copy_size = count;
		if (copy_to_user(buf, vmem, copy_size)) {
			printk(KERN_ERR "cproc_proc_read copy data to user error !\n");
			iounmap(vmem);
			return -EFAULT;
		}
		iounmap(vmem);
		count -= copy_size;
		loop++;
	} while (count);
	return total;
}

static int sprd_cproc_open(struct inode *inode, struct file *filp)
{
	struct cproc_device *cproc = container_of(filp->private_data,
			struct cproc_device, miscdev);

	filp->private_data = cproc;

	pr_debug("cproc %s opened!\n", cproc->initdata->devname);

	return 0;
}

static int sprd_cproc_release (struct inode *inode, struct file *filp)
{
	struct cproc_device *cproc = filp->private_data;

	pr_debug("cproc %s closed!\n", cproc->initdata->devname);

	return 0;
}

static long sprd_cproc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	/*
	struct cproc_device *cproc = filp->private_data;
	*/
	/* TODO: for general modem download&control */

	return 0;
}

static int sprd_cproc_mmap(struct file *filp, struct vm_area_struct *vma)
{
	/*
	struct cproc_device *cproc = filp->private_data;
	*/
	/* TODO: for general modem download&control */

	return 0;
}

static const struct file_operations sprd_cproc_fops = {
	.owner = THIS_MODULE,
	.open = sprd_cproc_open,
	.release = sprd_cproc_release,
	.unlocked_ioctl = sprd_cproc_ioctl,
	.mmap = sprd_cproc_mmap,
};

static int cproc_proc_open(struct inode *inode, struct file *filp)
{
	struct cproc_proc_entry *entry = (struct cproc_proc_entry *)PDE_DATA(inode);
	/*move remap to writing or reading, shi yunlong*/
/*
	struct cproc_device *cproc = entry->cproc;

	cproc->vbase = ioremap(cproc->initdata->base, cproc->initdata->maxsz);
	if (!cproc->vbase) {
		printk(KERN_ERR "Unable to map cproc base: 0x%08x\n", cproc->initdata->base);
		return -ENOMEM;
	}
*/

	filp->private_data = entry;

	return 0;
}

static int cproc_proc_release(struct inode *inode, struct file *filp)
{
	/*move remap to writing or reading, shi yunlong*/
/*
	struct cproc_proc_entry *entry = (struct cproc_proc_entry *)filp->private_data;
	struct cproc_device *cproc = entry->cproc;

	iounmap(cproc->vbase);
*/

	return 0;
}

static ssize_t cproc_proc_read(struct file *filp,
		char __user *buf, size_t count, loff_t *ppos)
{
	struct cproc_proc_entry *entry = (struct cproc_proc_entry *)filp->private_data;
	struct cproc_device *cproc;
	struct cproc_segments *seg;
	char *type = entry->name;
	unsigned int len;
	void *vmem;
	int rval;
	size_t r, i;/*count ioremap, shi yunlong*/

/*	pr_debug("cproc proc read type: %s ppos %ll\n", type, *ppos);*/

	if (strcmp(type, "mem") == 0) {
		cproc = entry->data.cproc;
		if (*ppos >= cproc->initdata->maxsz) {
			return 0;
		}
		if ((*ppos + count) > cproc->initdata->maxsz) {
			count = cproc->initdata->maxsz - *ppos;
		}

		/*remap and unmap in each read operation, shi yunlong, begin*/
		/*
		vmem = cproc->vbase + *ppos;
		if (copy_to_user(buf, vmem, count)) {
			printk(KERN_ERR "cproc_proc_read copy data to user error !\n");
			return -EFAULT;
		}
		*/
		r = count, i = 0;
		do{
			uint32_t copy_size = CPROC_VMALLOC_SIZE_LIMIT;
			vmem = ioremap(cproc->initdata->base + *ppos + CPROC_VMALLOC_SIZE_LIMIT*i, CPROC_VMALLOC_SIZE_LIMIT);
			if (!vmem) {
				uint32_t addr = cproc->initdata->base + *ppos + CPROC_VMALLOC_SIZE_LIMIT*i;
				printk(KERN_ERR "Unable to map cproc base: 0x%08x\n", addr);
				if(i > 0){
					*ppos += CPROC_VMALLOC_SIZE_LIMIT*i;
					return CPROC_VMALLOC_SIZE_LIMIT*i;
				}else{
					return -ENOMEM;
				}
			}
			if(r < CPROC_VMALLOC_SIZE_LIMIT) copy_size = r;
			if (copy_to_user(buf, vmem, copy_size)) {
				printk(KERN_ERR "cproc_proc_read copy data to user error !\n");
				iounmap(vmem);
				return -EFAULT;
			}
			iounmap(vmem);
			r -= copy_size;
			i++;
		}while(r > 0);
		/*remap and unmap in each read operation, shi yunlong, end*/
	} else if (strcmp(type, "status") == 0) {
		cproc = entry->data.cproc;
		if (cproc->status >= CP_MAX_STATUS) {
			return -EINVAL;
		}
		len = strlen(cp_status_info[cproc->status]);
		if (*ppos >= len) {
			return 0;
		}
		count = (len > count) ? count : len;
		if (copy_to_user(buf, cp_status_info[cproc->status], count)) {
			printk(KERN_ERR "cproc_proc_read copy data to user error !\n");
			return -EFAULT;
		}
	} else if (strcmp(type, "wdtirq") == 0) {
		cproc = entry->data.cproc;
		/* wait forever */
		rval = wait_event_interruptible(cproc->wdtwait, cproc->wdtcnt  != CPROC_WDT_FLASE);
		if (rval < 0) {
			printk(KERN_ERR "cproc_proc_read wait interrupted error !\n");
		}
		len = strlen(cp_status_info[CP_WDTIRQ_STATUS]);
		if (*ppos >= len) {
			return 0;
		}
		count = (len > count) ? count : len;
		if (copy_to_user(buf, cp_status_info[CP_WDTIRQ_STATUS], count)) {
			printk(KERN_ERR "cproc_proc_read copy data to user error !\n");
			return -EFAULT;
		}
	} else if (strcmp(type, "start") == 0) {
		return -EINVAL;
	} else if (strcmp(type, "stop") == 0) {
		return -EINVAL;
	} else {
		seg = entry->data.seg;
		if (strcmp(type, seg->name) != 0) {
			pr_debug("cproc proc read type not match: %s\n", type);
			return -EINVAL;
		}
		ssize_t ret;
		ret = sprd_cproc_seg_dump(seg->base,
			seg->maxsz, buf, count, *ppos);
		if (ret >= 0)
			count = ret;
		else
			return ret;
	}

	*ppos += count;
	return count;
}

static ssize_t cproc_proc_write(struct file *filp,
		const char __user *buf, size_t count, loff_t *ppos)
{
	struct cproc_proc_entry *entry = (struct cproc_proc_entry *)filp->private_data;
	struct cproc_device *cproc;
	struct cproc_segments *seg;
	char *type = entry->name;
	uint32_t base, size, offset;
	void *vmem;
	size_t r, i;/*count ioremap, shi yunlong*/

	pr_debug("cproc proc write type: %s\n!", type);

	if (strcmp(type, "start") == 0) {
		printk(KERN_INFO "cproc_proc_write to map cproc base start\n");
		cproc = entry->data.cproc;
		cproc->initdata->start(cproc);
		cproc->wdtcnt = CPROC_WDT_FLASE;
		cproc->status = CP_NORMAL_STATUS;
		return count;
	} else if (strcmp(type, "stop") == 0) {
		cproc = entry->data.cproc;
		cproc->initdata->stop(cproc);
		cproc->status = CP_STOP_STATUS;
		return count;
	} else {
		seg = entry->data.seg;
		if (strcmp(type, seg->name) != 0) {
			pr_info("cproc proc write type not match: %s\n", type);
			return -EINVAL;
		}
		base = seg->base;
		size = seg->maxsz;
		offset = *ppos;
	}

	if (size <= offset) {
		printk("cproc_proc_write modem dsp write over pos:%0x\n",offset);
		*ppos += count;
		return count;
	}

	pr_debug("cproc proc write: 0x%08x, 0x%08x\n!", base + offset, count);
	count = min((size-offset), count);
	/*remap and unmap in each write operation, shi yunlong, begin*/
	/*
	vmem = cproc->vbase + (base - cproc->initdata->base) + offset;

	if (copy_from_user(vmem, buf, count)) {
		return -EFAULT;
	}
	*/
	r = count, i = 0;
	do{
		uint32_t copy_size = CPROC_VMALLOC_SIZE_LIMIT;
		vmem = ioremap(base + offset + CPROC_VMALLOC_SIZE_LIMIT*i, CPROC_VMALLOC_SIZE_LIMIT);
		if (!vmem) {
			uint32_t addr = base + offset + CPROC_VMALLOC_SIZE_LIMIT*i;
			printk(KERN_ERR "Unable to map cproc base: 0x%08x\n", addr);
			if(i > 0){
				*ppos += CPROC_VMALLOC_SIZE_LIMIT*i;
				return CPROC_VMALLOC_SIZE_LIMIT*i;
			}else{
				return -ENOMEM;
			}
		}
		if(r < CPROC_VMALLOC_SIZE_LIMIT) copy_size = r;
		if (copy_from_user(vmem, buf+CPROC_VMALLOC_SIZE_LIMIT*i, copy_size)) {
			printk(KERN_ERR "cproc_proc_write copy data from user error !\n");
			iounmap(vmem);
			return -EFAULT;
		}
		iounmap(vmem);
		r -= copy_size;
		i++;
	}while(r > 0);
	/*remap and unmap in each write operation, shi yunlong, end*/

	*ppos += count;
	return count;
}

static loff_t cproc_proc_lseek(struct file* filp, loff_t off, int whence )
{
	struct cproc_proc_entry *entry = (struct cproc_proc_entry *)filp->private_data;
	struct cproc_device *cproc;
	char *type = entry->name;
	loff_t new;

	switch (whence) {
	case SEEK_SET:
		new = off;
		filp->f_pos = new;
		break;
	case SEEK_CUR:
		new = filp->f_pos + off;
		filp->f_pos = new;
		break;
	case SEEK_END:
		if (strcmp(type, "mem") == 0) {
			cproc = entry->data.cproc;
			new = cproc->initdata->maxsz + off;
			filp->f_pos = new;
		} else {
			return -EINVAL;
		}
		break;
	default:
		return -EINVAL;
	}
	return (new);
}

static unsigned int cproc_proc_poll(struct file *filp, poll_table *wait)
{
	struct cproc_proc_entry *entry = (struct cproc_proc_entry *)filp->private_data;
	struct cproc_device *cproc;
	char *type = entry->name;
	unsigned int mask = 0;

	pr_debug("cproc proc poll type: %s \n", type);

	if (strcmp(type, "wdtirq") == 0) {
		cproc = entry->data.cproc;
		poll_wait(filp, &cproc->wdtwait, wait);
		if (cproc->wdtcnt  != CPROC_WDT_FLASE) {
			mask |= POLLIN | POLLRDNORM;
		}
	} else {
		printk(KERN_ERR "cproc_proc_poll file don't support poll !\n");
		return -EINVAL;
	}
	return mask;
}

struct file_operations cpproc_fs_fops = {
	.open		= cproc_proc_open,
	.release	= cproc_proc_release,
	.llseek  = cproc_proc_lseek,
	.read		= cproc_proc_read,
	.write	= cproc_proc_write,
	.poll		= cproc_proc_poll,
};

static inline void sprd_cproc_fs_init(struct cproc_device *cproc)
{
	uint8_t i = 0;
	cproc->procfs.procdir = proc_mkdir(cproc->name, NULL);

	cproc->procfs.processor = kzalloc(sizeof(struct cproc_proc_entry)
			* cproc->initdata->segnr, GFP_KERNEL);
	if (!cproc->procfs.processor) {
		printk(KERN_ERR "sprd_cproc_fs_init alloc memory failed !\n");
		return -ENOMEM;
	}

	cproc->procfs.start.name = "start";
	cproc->procfs.start.entry = proc_create_data(cproc->procfs.start.name, S_IWUSR,
			cproc->procfs.procdir, &cpproc_fs_fops, &(cproc->procfs.start));
	cproc->procfs.start.data.cproc = cproc;

	cproc->procfs.stop.name = "stop";
	cproc->procfs.stop.entry = proc_create_data(cproc->procfs.stop.name, S_IWUSR,
			cproc->procfs.procdir, &cpproc_fs_fops, &(cproc->procfs.stop));
	cproc->procfs.stop.data.cproc = cproc;

	cproc->procfs.status.name = "status";
	cproc->procfs.status.entry = proc_create_data(cproc->procfs.status.name, S_IWUSR,
			cproc->procfs.procdir, &cpproc_fs_fops, &(cproc->procfs.status));
	cproc->procfs.status.data.cproc = cproc;

	cproc->procfs.wdtirq.name = "wdtirq";
	cproc->procfs.wdtirq.entry = proc_create_data(cproc->procfs.wdtirq.name, S_IWUSR,
			cproc->procfs.procdir, &cpproc_fs_fops, &(cproc->procfs.wdtirq));
	cproc->procfs.wdtirq.data.cproc = cproc;

	cproc->procfs.mem.name = "mem";
	cproc->procfs.mem.entry = proc_create_data(cproc->procfs.mem.name, S_IWUSR,
			cproc->procfs.procdir, &cpproc_fs_fops, &(cproc->procfs.mem));
	cproc->procfs.mem.data.cproc = cproc;

	for (i = 0; i < cproc->initdata->segnr; i++) {
		cproc->procfs.processor[i].name = cproc->initdata->segs[i].name;
		cproc->procfs.processor[i].data.seg = &cproc->initdata->segs[i];
		cproc->procfs.processor[i].entry = proc_create_data(
				cproc->initdata->segs[i].name,
				S_IWUSR,
				cproc->procfs.procdir,
				&cpproc_fs_fops,
				&cproc->procfs.processor[i]);
	}
	return;
}

static inline void sprd_cproc_fs_exit(struct cproc_device *cproc)
{
	uint8_t i = 0;
	remove_proc_entry(cproc->procfs.start.name, cproc->procfs.procdir);
	remove_proc_entry(cproc->procfs.stop.name, cproc->procfs.procdir);
	remove_proc_entry(cproc->procfs.status.name, cproc->procfs.procdir);
	remove_proc_entry(cproc->procfs.wdtirq.name, cproc->procfs.procdir);
	remove_proc_entry(cproc->procfs.mem.name, cproc->procfs.procdir);
	for (i = 0; i < cproc->initdata->segnr; i++)
		remove_proc_entry(cproc->procfs.processor[i].name, cproc->procfs.procdir);
	kfree(cproc->procfs.processor);
	remove_proc_entry(cproc->name, NULL);
}

static irqreturn_t sprd_cproc_irq_handler(int irq, void *dev_id)
{
	struct cproc_device *cproc = (struct cproc_device *)dev_id;

	printk("sprd_cproc_irq_handler cp watchdog enable !\n");
	cproc->wdtcnt = CPROC_WDT_TRUE;
	cproc->status = CP_WDTIRQ_STATUS;
	wake_up_interruptible_all(&(cproc->wdtwait));
	return IRQ_HANDLED;
}

static int sprd_cproc_probe(struct platform_device *pdev)
{
	struct cproc_device *cproc;
	int rval;

	cproc = kzalloc(sizeof(struct cproc_device), GFP_KERNEL);
	if (!cproc) {
		printk(KERN_ERR "failed to allocate cproc device!\n");
		return -ENOMEM;
	}

	cproc->initdata = pdev->dev.platform_data;

	cproc->miscdev.minor = MISC_DYNAMIC_MINOR;
	cproc->miscdev.name = cproc->initdata->devname;
	cproc->miscdev.fops = &sprd_cproc_fops;
	cproc->miscdev.parent = NULL;
	cproc->name = cproc->initdata->devname;
	rval = misc_register(&cproc->miscdev);
	if (rval) {
		kfree(cproc);
		printk(KERN_ERR "failed to register sprd_cproc miscdev!\n");
		return rval;
	}

	cproc->status = CP_NORMAL_STATUS;
	cproc->wdtcnt = CPROC_WDT_FLASE;
	init_waitqueue_head(&(cproc->wdtwait));

	/* register IPI irq */
	rval = request_irq(cproc->initdata->wdtirq, sprd_cproc_irq_handler,
			0, cproc->initdata->devname, cproc);
	if (rval != 0) {
		printk(KERN_ERR "Cproc failed to request irq %s: %d\n",
				cproc->initdata->devname, cproc->initdata->wdtirq);
		return rval;
	}

	sprd_cproc_fs_init(cproc);

	platform_set_drvdata(pdev, cproc);

	printk(KERN_INFO "cproc %s probed!\n", cproc->initdata->devname);

	return 0;
}

static int sprd_cproc_remove(struct platform_device *pdev)
{
	struct cproc_device *cproc = platform_get_drvdata(pdev);

	sprd_cproc_fs_exit(cproc);
	misc_deregister(&cproc->miscdev);

	printk(KERN_INFO "cproc %s removed!\n", cproc->initdata->devname);
	kfree(cproc);

	return 0;
}

static struct platform_driver sprd_cproc_driver = {
	.probe    = sprd_cproc_probe,
	.remove   = sprd_cproc_remove,
	.driver   = {
		.owner = THIS_MODULE,
		.name = "sprd_cproc",
	},
};

static int __init sprd_cproc_init(void)
{
	if (platform_driver_register(&sprd_cproc_driver) != 0) {
		printk(KERN_ERR "sprd_cproc platform drv register Failed \n");
		return -1;
	}
	return 0;
}

static void __exit sprd_cproc_exit(void)
{
	platform_driver_unregister(&sprd_cproc_driver);
}

module_init(sprd_cproc_init);
module_exit(sprd_cproc_exit);

MODULE_DESCRIPTION("SPRD Communication Processor Driver");
MODULE_LICENSE("GPL");
