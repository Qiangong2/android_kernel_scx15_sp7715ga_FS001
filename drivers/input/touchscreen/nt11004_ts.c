/*
 * Driver for nt11004 I2C touchscreen controllers.
 *
 * Copyright (C) 2010-2011 nt11004, Inc.
 *
 * nt11004_i2c_ts.c V3.0	from v3.0 support TangoC solution and remove the previous soltutions
 *
 * nt11004_i2c_ts.c V3.1	Add bootloader function	7
 *			Add RESET_TP		9
 * 			Add ENABLE_IRQ		10
 *			Add DISABLE_IRQ		11
 * 			Add BOOTLOADER_STU	16
 *			Add ATTB_VALUE		17
 *			Add Write/Read Interface for APP software
 *
 * nt11004_i2c_ts.c V3.2.0A	for INT_MODE 0x0A
 *				arrange to nt11004 10 slot
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */ 

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/earlysuspend.h>
#include <linux/regulator/consumer.h>
#include <linux/i2c/nt11004_ts.h>
#include <mach/regulator.h>

#include <mach/hardware.h>
#include <mach/globalregs.h>

#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/irq.h> 
#include <linux/proc_fs.h>
#include <linux/kthread.h>
#include <linux/i2c/al3006_pls.h>

#include <linux/cdc_com.h>

/*********************************Bee-0928-TOP****************************************/


//#define TPD_DEBUG_FUNC

#ifdef TPD_DEBUG_FUNC
#define	TPD_DEBUG(format, ...)	\
		printk(KERN_INFO "nt11004 " format "\n", ## __VA_ARGS__)
#else
#define TPD_DEBUG(format, ...)
#endif


#define TS_IRQ_PIN			"ts_irq_pin"
#define TS_RESET_PIN		"ts_rst_pin"


#ifndef I2C_MAJOR
#define I2C_MAJOR 		125
#endif

#define I2C_MINORS 		256


int global_irq;

struct timer_list tp_timer;		/* "no irq" timer */

static struct i2c_driver ntp_i2c_driver;
static struct class *i2c_dev_class;
static LIST_HEAD( i2c_dev_list);


static struct i2c_client *this_client = NULL;
static int suspend_flag = 0;
static struct early_suspend	ntp_early_suspend;
static struct ntp_i2c_data *g_nt11004_ts;


#ifdef NTP_WITH_BUTTON

#define TPD_KEY_COUNT			3

int ntp_keys[] = 
{
	KEY_MENU,
	KEY_HOMEPAGE,
	KEY_BACK 
};
#endif 	// HAVE_BUTTON

static int ntp_i2c_read_bytes(char *buf, int len)
{
	int ret;
	
	struct i2c_msg msgs[] = 
	{
		{
			.addr	= this_client->addr,
			.flags	= !I2C_M_RD,
			.len	= 1,
			.buf	= buf,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= len,
			.buf	= buf,
		}
	};

	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret != 2)
	{
		pr_err("%s i2c write error: %d\n", __func__, ret);
	}

	return ret;
}

static int ntp_i2c_write_bytes(char *buf, int len)
{
	int ret;
	
	struct i2c_msg msg[] = 
	{
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= len,
			.buf	= buf,
		},
	};
	
	ret = i2c_transfer(this_client->adapter, msg, 1);
	if (ret != 1)
	{
		pr_err("%s i2c write error: %d\n", __func__, ret);
	}
	
	return ret;
}

#ifdef TOUCH_VIRTUAL_KEYS
static ssize_t virtual_keys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":40:980:50:30"
		":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":120:980:50:30"
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:50:30"
		":" __stringify(EV_KEY) ":" __stringify(KEY_SEARCH) ":280:980:50:30"
		"\n");
}

static struct kobj_attribute virtual_keys_attr = {
	.attr = {
		.name = "virtualkeys.nt11004_ts",
		.mode = S_IRUGO,
	},
	.show = &virtual_keys_show,
};

static struct attribute *properties_attrs[] = {
	&virtual_keys_attr.attr,
	NULL
};

static struct attribute_group properties_attr_group = {
	.attrs = properties_attrs,
};

static struct kobject *properties_kobj;

static void ntp_virtual_keys_init(void)
{
	int ret;

	TPD_DEBUG("%s\n",__func__);

	properties_kobj = kobject_create_and_add("board_properties", NULL);
	if (properties_kobj)
		ret = sysfs_create_group(properties_kobj,
					 &properties_attr_group);
	if (!properties_kobj || ret)
		pr_err("failed to create board_properties\n");
}

static void ntp_virtual_keys_destroy(void)
{
	if (properties_kobj)
	{
		sysfs_remove_group(properties_kobj, &properties_attr_group);
		kobject_del(properties_kobj);
	}
}
#endif

static void ntp_reset(void)
{
	gpio_direction_output(GPIO_TOUCH_RESET, 1);
	msleep(20);
	gpio_set_value(GPIO_TOUCH_RESET, 0);
	msleep(20);
	gpio_set_value(GPIO_TOUCH_RESET,1);
	msleep(50);
}


#ifdef NTP_APK_DRIVER_FUNC_SUPPORT

#define NTP_DEVICE_NAME	"NVTflash"

struct ntp_flash_data
{
	rwlock_t lock;
	unsigned char bufferIndex;
	unsigned int length;
	struct i2c_client *client;
};

static int ntp_apk_mode = 0;
static struct proc_dir_entry *ntp_proc_entry;
static struct ntp_flash_data *ntp_flash_priv;

/*******************************************************
Description:
	Novatek touchscreen control driver initialize function.

Parameter:
	priv:	i2c client private struct.
	
return:
	Executive outcomes.0---succeed.
*******************************************************/
int ntp_flash_write(struct file *file, const char __user *buff, size_t count, loff_t *offp)
{
	struct i2c_msg msgs[2];	
	char *str;
	int ret = -1;
	int retries = 0;
	
	//file->private_data = (uint8_t *)kmalloc(64, GFP_KERNEL);
	//str = file->private_data;
	str = (uint8_t *)kmalloc(64, GFP_KERNEL);
	
	ret = copy_from_user(str, buff, count);

	//set addr
	if((str[0] == 0x7F)||(str[0] == (0x7F<<1)))
	{
		msgs[0].addr = NOVATEK_HW_ADDR;
	}
	else if((str[0] == 0x01)||(str[0] == (0x01<<1)))
	{
		msgs[0].addr = 0x01;
	}	
	else
	{
		msgs[0].addr = NOVATEK_TS_ADDR;
	}
	
	msgs[0].flags = !I2C_M_RD;
	//msgs[0].addr  = str[0];
	msgs[0].len   = str[1];
	msgs[0].buf   = &str[2];

	while(retries < 20)
	{
		ret = i2c_transfer(ntp_flash_priv->client->adapter, msgs, 1);
		if(ret == 1)
		{	
			break;
		}
		else
		{
			pr_info("write error %d\n", retries);
		}
		
		retries++;
	}

	kfree(str);
	
	return ret;
}

int ntp_flash_read(struct file *file, char __user *buff, size_t count, loff_t *offp)
{
	struct i2c_msg msgs[2];	 
	char *str;
	int ret = -1;
	int retries = 0;
	
	//file->private_data = (uint8_t *)kmalloc(64, GFP_KERNEL);
	//str = file->private_data;
	str = (uint8_t *)kmalloc(64, GFP_KERNEL);

	if(copy_from_user(str, buff, count))
	{
		return -EFAULT;
	}

	//set addr
	if((str[0] == 0x7F)||(str[0] == (0x7F<<1)))
	{
		msgs[0].addr = NOVATEK_HW_ADDR;
		msgs[1].addr = NOVATEK_HW_ADDR;
	}
	else if((str[0] == 0x01)||(str[0] == (0x01<<1)))
	{
		msgs[0].addr = 0x01;
		msgs[1].addr = 0x01;
	}
	else
	{
		msgs[0].addr = NOVATEK_TS_ADDR;
		msgs[1].addr = NOVATEK_TS_ADDR;
	}
	
	msgs[0].flags = !I2C_M_RD;
	//msgs[0].addr  = str[0];
	msgs[0].len   = 1;
	msgs[0].buf   = &str[2];

	msgs[1].flags = I2C_M_RD;
	//msgs[1].addr  = str[0];
	msgs[1].len   = str[1]-1;
	msgs[1].buf   = &str[3];

	while(retries < 20)
	{
		ret = i2c_transfer(ntp_flash_priv->client->adapter, msgs, 2);
		if(ret == 2)
		{
			break;
		}
		else
		{
			pr_info("read error %d\n", retries);
		}
		
		retries++;
	}

	ret = copy_to_user(buff, str, count);

	kfree(str);
	
	return ret;
}

int ntp_flash_open(struct inode *inode, struct file *file)
{
	struct ntp_flash_data *dev;

	dev = kmalloc(sizeof(struct ntp_flash_data), GFP_KERNEL);
	
	if (dev == NULL) 
	{
		return -ENOMEM;
	}

	rwlock_init(&dev->lock);
	file->private_data = dev;

	ntp_apk_mode = 1;

	return 0;
}

int ntp_flash_close(struct inode *inode, struct file *file)
{
	struct ntp_flash_data *dev = file->private_data;

	ntp_apk_mode = 0;

	if (dev) 
	{
		kfree(dev);
	}
	
	return 0;   
}

struct file_operations ntp_flash_fops = 
{
	.owner 	= THIS_MODULE,
	.open 	= ntp_flash_open,
	.release= ntp_flash_close,
	.write 	= ntp_flash_write,
	.read 	= ntp_flash_read,
};

static int ntp_flash_init(void)
{		
	int ret = 0;
	
	ntp_proc_entry = proc_create(NTP_DEVICE_NAME, 0666, NULL, &ntp_flash_fops);
	if(ntp_proc_entry == NULL)
	{
		pr_info("Couldn't create proc entry!\n");
		ret = -ENOMEM;
		return ret;
	}

	ntp_flash_priv = kzalloc(sizeof(*ntp_flash_priv), GFP_KERNEL);	
	
	ntp_flash_priv->client = this_client;
	
	pr_info("NVT_flash driver loaded\n");
	
	return 0;
}

#endif // NTP_APK_DRIVER_FUNC_SUPPORT

#ifdef NTP_BOOTLOADER_FUNC_SUPPORT

enum
{
	RS_OK		= 0,
	RS_INIT_ER	= 10,	// Init bootloader error
	RS_VERI_ER	= 11,	// Verify firmware error  
	RS_ERAS_ER	= 12,	// Erase flash error
	RS_WRDA_ER	= 13,	// Write data to flash error
	RS_UPFW_ER	= 14	// Update firmware error
};

#include "nt11004_firmware.h"
#define NTP_FLASH_SIZE		(0x4000)	// 16K bytes
#define NTP_UPDATE_SIZE		(0x4000)	// 16K bytes
#define NTP_SECTOR_SIZE		(0x1000)	// 4K bytes
#define NTP_PAGE_SIZE		(0x80)		// 128 bytes

#define NTP_SECTOR_NUM			(NTP_FLASH_SIZE/NTP_SECTOR_SIZE)
#define NTP_WIRTE_BYTES			8


static unsigned char *ntp_fw_data_ptr;


static void ntp_bl_delay_ms(unsigned long ms)
{
	msleep(ms);
}


static void ntp_bl_reset(void)
{
	ntp_reset();
}


static int ntp_bl_read_bytes(struct i2c_client *client, unsigned char addr, unsigned char *buf, int len)
{
	unsigned char reg;
	unsigned char *data;
	int length;
	unsigned char tmpaddr;
	int ret;

	reg = buf[0];
	data = &buf[1];
	length = len - 1;
	
	tmpaddr = client->addr;
	client->addr = addr;
	ret = i2c_smbus_read_i2c_block_data(client, reg, length, data);
	client->addr = tmpaddr;

	if(ret < 0)
	{
	    return -1;
	}
	else
	{
	    return RS_OK;
	}
}


static int ntp_bl_write_bytes(struct i2c_client *client, unsigned char addr, unsigned char *buf, int len)
{
	unsigned char reg;
	unsigned char *data;
	int length;
	unsigned char tmpaddr;
	int ret;

	reg = buf[0];
	data = &buf[1];
	length = len - 1;
	
	tmpaddr = client->addr;
	client->addr = addr;
	ret = i2c_smbus_write_i2c_block_data(client, reg, len, data);
	client->addr = tmpaddr;

	if(ret < 0)
	{
	    return -1;
	}
	else
	{
	    return RS_OK;
	}
}


static int ntp_bl_init_bootloader(struct i2c_client *client)
{
	unsigned char buf[4] = {0};
	int i = 0;
	int ret = RS_OK;

	printk("ntp ntp_bl_init_bootloader\n");

	while(i++ < 5)
	{	
		// Software Reset Command
		// Host Controller write a A5H to NT11004 I2C addr 7FH
		buf[0] = 0x00;
		buf[1] = 0xA5;

		ret = ntp_bl_write_bytes(client, NOVATEK_HW_ADDR, buf, 2);
		if( ret != RS_OK )   		
		{
			printk("ntp Send Software Reset Command Error \n");			
			return RS_INIT_ER;
		}
		ntp_bl_delay_ms(10);

		// Initiation Command
		// Host Controller write a 00H to NT11004 I2C address 7F
		// Enter flash mode
		buf[0] = 0x00;
		buf[1] = 0x00; 
		ret = ntp_bl_write_bytes(client, NOVATEK_HW_ADDR, buf, 2);
		if( ret != RS_OK )   		
		{	
			return RS_INIT_ER;
		}
		ntp_bl_delay_ms(5);
		
		// Host Controller read NT11004 status form I2C address 7FH
		// Read Status
		buf[0] = 0x00;
		ret = ntp_bl_read_bytes(client, NOVATEK_HW_ADDR, buf, 2);
		if( ret != RS_OK )   		
	  	{	
			return RS_INIT_ER;
		}

		if(buf[1] == 0xAA)
		{
			//printk("buf[1] == 0xAA\n");	
			return RS_OK;
		}
	}

	return RS_INIT_ER;
}


// function 
// return 1: need update; 0: do not need update
static int ntp_bl_verify_fw(struct i2c_client *client)
{
	unsigned short checksum1, checksum2;
	unsigned char buf[4] = {0};
	int i, count = 0;
	int ret;

	printk("ntp ntp_bl_verify_fw \n");

	// SW RST and then CPU normally run or HW RST
	buf[0] = 0x00;
	buf[1] = 0x5A;
	ret = ntp_bl_write_bytes(client, NOVATEK_HW_ADDR, buf, 2);
	if( ret != RS_OK )			
	{
		printk("ntp i2c communication problem, do not need update \n"); 		
		return RS_OK;
	}
	ntp_bl_delay_ms(100);

	// I2C特別指令切換至Address至0x0EF9
	buf[0] = 0xFF;
	buf[1] = 0x0E;
	buf[2] = 0xF9;	
	ret = ntp_bl_write_bytes(client, NOVATEK_TS_ADDR, buf, 3);
	if( ret != RS_OK )			
	{
		printk("ntp NOVATEK_TS_ADDR Error, need update\n"); 		
		return RS_VERI_ER;
	}

	// I2C Write Short Test Command (0xE1)
	buf[0] = 0x00;
	buf[1] = 0xE1;
	ret = ntp_bl_write_bytes(client, NOVATEK_TS_ADDR, buf, 2);
	if( ret != RS_OK )			
	{
		printk("ntp NOVATEK_TS_ADDR Error, need update\n"); 		
		return RS_VERI_ER;
	}
	ntp_bl_delay_ms(100);

	// I2C特別指令切換至Address至0x0CD7
	buf[0] = 0xFF;
	buf[1] = 0x0C;
	buf[2] = 0xD7;	
	ret = ntp_bl_write_bytes(client, NOVATEK_TS_ADDR, buf, 3);
	if( ret != RS_OK )			
	{
		printk("ntp NOVATEK_TS_ADDR Error, need update\n"); 		
		return RS_VERI_ER;
	}

	// I2C Read 1 Byte, 重覆直到收到0xAA
	count = 0;
	while(count++ < 5)
	{
		buf[0] = 0x00;
		ret = ntp_bl_read_bytes(client, NOVATEK_TS_ADDR, buf, 2);
		if( ret != RS_OK )			
		{
			printk("ntp NOVATEK_TS_ADDR Error, need update\n"); 		
			return RS_VERI_ER;
		}

		if(buf[1] == 0xAA)
		{
			printk("ntp buf[1] == 0XAA \n");
			break;
		}

		ntp_bl_delay_ms(2);
	}

	if(count >= 5)
	{
		printk("ntp NOVATEK_TS_ADDR Error, need update\n"); 		
		return RS_VERI_ER;
	}

	buf[0] = 0xFF;
	buf[1] = 0x0C;
	buf[2] = 0xD8;	
	ret = ntp_bl_write_bytes(client, NOVATEK_TS_ADDR, buf, 3);
	if( ret != RS_OK )			
	{
		printk("ntp NOVATEK_TS_ADDR Error, need update\n"); 		
		return RS_VERI_ER;
	}

	// I2C Read 2 Byte Result
	buf[0] = 0x00;
	ret = ntp_bl_read_bytes(client, NOVATEK_TS_ADDR, buf, 3);
	if( ret != RS_OK )			
	{
		printk("ntp NOVATEK_TS_ADDR Error, need update\n"); 		
		return RS_VERI_ER;
	}

	checksum1 = (((unsigned short)buf[1]) << 8)|((unsigned short)buf[2]);
	checksum1 = checksum1&0xFFFF;

	// get checksum of bin file
	checksum2 = 0;
	for(i = 0; i < NTP_FLASH_SIZE; i++)
	{
		checksum2 = checksum2 + ntp_fw_data_ptr[i];
	}
	checksum2 = checksum2&0xFFFF;

	printk("ntp checksum1 = %x; checksum2 = %x\n", checksum1, checksum2);
	
	//Compare the chechsum
	if (checksum1 != checksum2)
	{
		return RS_VERI_ER;
	}
	
	return RS_OK; 							// Boot loader function is completed.
}


static int ntp_bl_disable_flash_protect(struct i2c_client *client)
{
	unsigned char buf[6] = {0};
	unsigned char ret,i;

	printk("ntp ntp_bl_disable_flash_protect \n");

	
	// disable flash protect
	buf[0] = 0xFF;
	buf[1] = 0xF0;
	buf[2] = 0xAC;
	ret = ntp_bl_write_bytes(client, 0x01, buf, 3);
    if( ret != RS_OK )   		
  	{
	    return RS_ERAS_ER;
    }
    ntp_bl_delay_ms(20);   //Delay 

	buf[0] = 0x00;
	buf[1] = 0x21;
	ret = ntp_bl_write_bytes(client, 0x01, buf, 2);
    if( ret != RS_OK )   		
  	{
	    return RS_ERAS_ER;
    }
    ntp_bl_delay_ms(20);   //Delay 
    
    buf[0] = 0x00;
	buf[1] = 0x99;
	buf[2] = 0x00;
	buf[3] = 0x0E;
	buf[4] = 0X01;
	ret = ntp_bl_write_bytes(client, NOVATEK_HW_ADDR, buf, 5);
    if( ret != RS_OK )   		
  	{
	    return RS_ERAS_ER;
    }	
    ntp_bl_delay_ms(20);   //Delay 
    
	buf[0] = 0x00;
	buf[1] = 0x81;
	ret = ntp_bl_write_bytes(client, 0x01, buf, 2);
    if( ret != RS_OK )   		
  	{
	    return RS_ERAS_ER;
    }	
    ntp_bl_delay_ms(20);   //Delay 
       
	//NT11004_TRACE("[FangMS] %s,%d  \r\n",__func__,__LINE__);
	buf[0] = 0x00;
	buf[1] = 0x99;
	buf[2] = 0x00;
	buf[3] = 0x0F;
	buf[4] = 0X01;
	ret = ntp_bl_write_bytes(client, NOVATEK_HW_ADDR, buf, 5);
    if( ret != RS_OK )   		
  	{
		return RS_ERAS_ER;
    }	
    ntp_bl_delay_ms(20);   //Delay 
    
	buf[0] = 0x00;
	buf[1] = 0x01;
	ret = ntp_bl_write_bytes(client, 0x01, buf, 2);
    if( ret != RS_OK )   		
  	{
	    return RS_ERAS_ER;
    }	
    ntp_bl_delay_ms(20);   //Delay 
    
	buf[0] = 0xFF;
	buf[1] = 0x00;
	buf[2] = 0x00;
	ret = ntp_bl_write_bytes(client, 0x01, buf, 3);
    if( ret != RS_OK )   		
  	{
	    return RS_ERAS_ER;
    }

    return RS_OK;
}


static int ntp_bl_erase_flash_sector(struct i2c_client *client, int sec)
{
	unsigned char buf[6] = {0};
	int i = 0;
	int ret = RS_ERAS_ER;

	printk("ntp ntp_bl_erase_flash_sector sec = %x \n", sec);

	for(i = 0; i < 5; i++)
	{
		buf[0] = 0x00;	// offset address
		buf[1] = 0x33;	// erase flash command 33H
		buf[2] = (unsigned char)((sec*NTP_SECTOR_SIZE) >> 8); 		// section_addr high 8 bit
		buf[3] = (unsigned char)((sec*NTP_SECTOR_SIZE) & 0xFF); 	// section_addr low 8 bit

		ret = ntp_bl_write_bytes(client, NOVATEK_HW_ADDR, buf, 4);
		if( ret != RS_OK )   		
		{
			return RS_ERAS_ER;
		}

		// Delay 40mS
		ntp_bl_delay_ms(40);

		// Read status
		buf[0] = 0x00;	// offset address	
		ret = ntp_bl_read_bytes(client, NOVATEK_HW_ADDR, buf, 2);
		if( ret != RS_OK )   		
		{
			return RS_ERAS_ER;
		}

		if(buf[1] == 0xAA)
		{
			return RS_OK;
		}
	}

	return RS_ERAS_ER;
}


static int ntp_bl_erase_flash(struct i2c_client *client)
{
	int i;
	int ret = RS_ERAS_ER;

	printk("ntp ntp_bl_erase_flash \n");
	
	ntp_bl_disable_flash_protect(client);
    
	for(i = 0; i < NTP_SECTOR_NUM; i++)
	{
		ret = ntp_bl_erase_flash_sector(client, i);

		if(ret != RS_OK)
		{
			return RS_ERAS_ER;
		}
	}
	
	return RS_OK;
}


static int ntp_bl_write_data_to_flash(struct i2c_client *client)
{
	unsigned char buf[16] = {0};
	unsigned char checksum = 0;
	unsigned char datasum = 0;
	unsigned int addr = 0;
	unsigned int page = 0;
	int i,j;
	int ret = RS_OK;
		
	printk("ntp ntp_bl_write_data_to_flash\n");

	do
	{	
		// write 8 bytes to flash start
		for(i = 0; i < (NTP_PAGE_SIZE/NTP_WIRTE_BYTES); i++)
		{	
			// Write Data to flash
			buf[0] = 0x00;
			buf[1] = 0x55;								// flash write command
			buf[2] = (unsigned char)(addr >> 8);		// address high 8 bit
			buf[3] = (unsigned char)(addr & 0x00FF);	// address low 8 bit
			buf[4] = NTP_WIRTE_BYTES;					// length
			//buf[5] = 0;									// checksum

			datasum = 0;
			for(j = 0; j < NTP_WIRTE_BYTES; j++)		// data
			{
				buf[6+j] = ntp_fw_data_ptr[addr+j];			
				datasum = datasum + buf[6+j];
			}

			checksum = (~(buf[2]+buf[3]+buf[4]+datasum))+1;
			buf[5] = checksum;
			
			ret = ntp_bl_write_bytes(client, NOVATEK_HW_ADDR, buf, (6+NTP_WIRTE_BYTES));
			if( ret != RS_OK )			
			{
				printk("\n ntp Write data to flash error 1, i = %d\n", i);
				return RS_WRDA_ER;
			}

			//ntp_bl_delay_ms(1);	//Delay 0.1ms
			
			addr = addr + NTP_WIRTE_BYTES;
		}

		ntp_bl_delay_ms(10);	//Delay 6ms

		#if 1
		buf[0] = 0x00;
		ret = ntp_bl_read_bytes(client, NOVATEK_HW_ADDR, buf, 2);		
		if( ret != RS_OK )			
		{
			printk("ntp Write data to flash error 2\n");
			return RS_WRDA_ER;
		}
								
		if(buf[1] != 0xAA)
		{
		
			printk("ntp Write data to flash error 3  buf[1] = %x\n", buf[1]);
			return RS_WRDA_ER;
		}
		#endif

		printk("ntp data writing ... page=%d, buf[1]=%x\n", page, buf[1]);
		
		page++;
	}
	while(addr < NTP_UPDATE_SIZE);

	return RS_OK;
}


static int ntp_bl_update_fw(struct i2c_client *client)
{
	int i;
	int ret = RS_OK;
	
	printk("ntp ntp_bl_update_fw\n");

START_UPDATE:
	
	// step 1 init bootloader
	ret = ntp_bl_init_bootloader(client);

	if(ret != RS_OK)
	{
		printk("ntp Init boot loader Error;\n");
		return RS_INIT_ER;
	}
	
	// step 2 erase flash
	ret = ntp_bl_erase_flash(client);

	if( ret != RS_OK )   		
	{
		printk("ntp Erase flash Error;\n");
		return RS_ERAS_ER;
	}

	// step 3 write data to flash
	ret = ntp_bl_write_data_to_flash(client);
	if(ret != RS_OK)
	{
		printk("ntp write data to flash Error;\n");
		return RS_WRDA_ER;
	}

	// step 4 confirm update is success
	#if 1
	ntp_bl_reset();
	if(ntp_bl_verify_fw(client) != RS_OK)
	{
		if(i < 3)
		{
			i++;
			printk("ntp ntp_bl_update_fw: i = %d\n", i);
			goto START_UPDATE;
		}
		else
		{
			printk("Update firmare fail!\n");
			return RS_UPFW_ER;
		}
	}
	#endif
	
	printk("ntp Update firmware success!\n");
	
	return RS_OK;
}


static int ntp_bl_bootloader(struct i2c_client *client)
{
	int ret = RS_OK;

	printk("\n ntp ntp_bl_bootloader NTP_WIRTE_BYTES = %d\n", NTP_WIRTE_BYTES);

	ntp_fw_data_ptr = firmware_data_0x34;

	if(ntp_fw_data_ptr == NULL)
	{
		return -1;
	}
	
	if(ntp_bl_verify_fw(client) != RS_OK)	// check if need update
	{
		printk("ntp_bl_bootloader: Start to update firmware\n");
		
		ret = ntp_bl_update_fw(client);

		if( RS_OK != ret )
		{
			printk("ntp ntp_bl_bootloader update firmware fail\n");
		}
		else
		{
			printk("ntp ntp_bl_bootloader update firmware success\n");
		}	
	}
	else
	{
		printk("ntp ntp_bl_bootloader do not need to update firmware\n");
	}
		
    ntp_bl_reset();

	return ret;
}

#endif //NTP_BOOTLOADER_FUNC_SUPPORT


#ifdef NTP_WAKEUP_SYSTEM_SUPPORT
//extern struct input_dev *kpd_input_dev;
static int ntp_wakeup_system_event(unsigned char buf1, unsigned char buf2)
{
	struct ntp_i2c_data *tsdata = g_nt11004_ts;
	int ret = -1;
	int index,pos,id;
	int x,y;

	TPD_DEBUG("ntp_wakeup_system_event\n");
	
	if(suspend_flag == 1)
	{
		if((buf1 = 0xF6)&&(buf2 = 0x01))
		{
			// wakeup system
			printk("wakeup system\n");

			#if 1
			input_report_key(tsdata->input, KEY_POWER, 1);
			msleep(10);
			input_report_key(tsdata->input, KEY_POWER, 0);
			#endif
			
			#if 0
			input_report_key(kpd_tpd_dev, KPD_PWRKEY_MAP, 1);
			msleep(10);
			input_report_key(kpd_tpd_dev, KPD_PWRKEY_MAP, 0);
			#endif
			input_sync(tsdata->input);
		}
		else
		{
			//printk("write wakeup system command!\n");
			int i = 0;
			unsigned char res = 0;
			unsigned char data[2] = {0};

			printk("write wakeup system command to tp!\n");
			
			while(i++<10)
			{
				data[0] = 0x88;
				data[1] = 0x5A;
				ntp_i2c_write_bytes(data, 2);
				
				msleep(2);
				
				data[0] = 0x98;
				ntp_i2c_read_bytes(data, 1);
				
				printk("data[0] = %x\n", data[0]);
			
				if(data[0] == 0xAA)
				{
					printk("write wakeup cmd OK\n");
					break;
				}
			}
		}
		ret = 1;
	}
	
	return ret;
}
#endif // NTP_WAKEUP_SYSTEM_SUPPORT


static void ntp_i2c_suspend(struct early_suspend *handler)
{
#ifdef NTP_WAKEUP_SYSTEM_SUPPORT
	unsigned char buf[2]={0x88, 0x5A};
	int i = 0;
	
	while(i++ <10)
	{
		buf[0] = 0x88;
		buf[1] = 0x5A;
		ntp_i2c_write_bytes(buf, 2);
		msleep(2);

		buf[0] = 0x98;
		ntp_i2c_read_bytes(buf, 2);
		
		printk("ntp_i2c_suspend buf[0] = %x\n", buf[0]);

		if(buf[0] == 0xAA)
		{
			printk("wakeup cmd OK\n");
			break;
		}
	}
#else
	unsigned char buf[2]={0x88, 0xAA};

	ntp_i2c_write_bytes(buf, 2);
	disable_irq_nosync(global_irq);
#endif

	TPD_DEBUG("==%s==\n", __func__);

	suspend_flag = 1;
}


static void ntp_i2c_resume(struct early_suspend *handler)
{
	TPD_DEBUG("==%s==\n", __func__);
	
	ntp_reset();

	suspend_flag = 0;
	
	enable_irq(global_irq);
}


static void ntp_poweron(void)
{
	int err = 0;
	struct regulator *tsp_regulator_28 = NULL;

	tsp_regulator_28 = regulator_get(NULL, "vddsim2");
	if (IS_ERR(tsp_regulator_28)) 
	{
		pr_err("zinitix:could not get 3.3v regulator\n");
		return;
	}
	
	err =regulator_set_voltage(tsp_regulator_28,2700000,2800000);
	if (err)
	{
		pr_err("zinitix:could not set to 3300mv.\n");
	}
	
	regulator_enable(tsp_regulator_28);
	msleep(20);
}  


static int ntp_config_pins(void)
{
	int irq;

	TPD_DEBUG("ntp_config_pins");
	
	gpio_request(GPIO_TOUCH_IRQ, TS_IRQ_PIN);
	gpio_request(GPIO_TOUCH_RESET, TS_RESET_PIN);

	ntp_poweron();
	ntp_reset();

	gpio_direction_input(GPIO_TOUCH_IRQ);	
	irq = gpio_to_irq(GPIO_TOUCH_IRQ);

	return irq;
}


static void ntp_down(int id, int x, int y, int w, int p)
{
	struct ntp_i2c_data *tsdata = g_nt11004_ts;

	TPD_DEBUG("ntp_down: track_id=%d, x=%d, y=%d\n", id, x, y);

	// ntp down
	input_report_key(tsdata->input, BTN_TOUCH, 1);
	
	input_report_abs(tsdata->input, ABS_MT_POSITION_X,	x);
	input_report_abs(tsdata->input, ABS_MT_POSITION_Y,	y);
	input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 1);
	//input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
	
	input_mt_sync(tsdata->input);
}

static void ntp_up(int id, int x, int y, int w, int p)
{
	struct ntp_i2c_data *tsdata = g_nt11004_ts;

	TPD_DEBUG("ntp_up\n");

	// ntp up
	input_report_key(tsdata->input, BTN_TOUCH, 0);	
	
	input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 0);
	//input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 0);
	
	input_mt_sync(tsdata->input);
}

static irqreturn_t ntp_isr_handler(int irq, void *dev_id)
{
	int ret = 0;
	struct ntp_i2c_data *tsdata = (struct ntp_i2c_data *)dev_id;

	disable_irq_nosync(irq);
	
	if (!work_pending(&tsdata->pen_event_work)) 
	{
		queue_work(tsdata->ts_workqueue, &tsdata->pen_event_work);
	}
	
	return IRQ_HANDLED;
}


#ifdef NTP_WITH_BUTTON
static int ntp_button_event(u8 buf1, u8 buf2)
{
	struct ntp_i2c_data *tsdata = g_nt11004_ts;

	static int key_status = 0;
	int i, ret;

	TPD_DEBUG("ntp_button_event: buf2 = 0x%x\n", buf2);
	
	switch(buf2)
	{
	case 0x00:
		if(key_status == 1)
		{
			TPD_DEBUG("key released\n");
#ifdef TOUCH_VIRTUAL_KEYS
			input_report_key(tsdata->input, BTN_TOUCH, 0);
			input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 0);
			input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 0);
			input_mt_sync(tsdata->input);
			input_sync(tsdata->input);
#else
			for(i=0; i < TPD_KEY_COUNT; i++)
			{
				input_report_key(tsdata->input, ntp_keys[i], 0);
				input_sync(tsdata->input);
			}
#endif
			key_status = 0;
			ret = 1;
		}			
        break;
		
    case 0x01:
        TPD_DEBUG("KEY[0] press\n");
#ifdef TOUCH_VIRTUAL_KEYS
		input_report_key(tsdata->input, BTN_TOUCH, 1);
		input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 1);
		input_report_abs(tsdata->input, ABS_MT_POSITION_X, 40);
		input_report_abs(tsdata->input, ABS_MT_POSITION_Y, 980);
		input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(tsdata->input);
		input_sync(tsdata->input);
#else
        input_report_key(tsdata->input, ntp_keys[0], 0x01);
		input_sync(tsdata->input);
#endif
		key_status = 1;
		ret = 1;
        break;
		
    case 0x02:
        TPD_DEBUG("KEY[1] press\n");
#ifdef TOUCH_VIRTUAL_KEYS
		input_report_key(tsdata->input, BTN_TOUCH, 1);
		input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 1);
		input_report_abs(tsdata->input, ABS_MT_POSITION_X, 120);
		input_report_abs(tsdata->input, ABS_MT_POSITION_Y, 980);
		input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(tsdata->input);
		input_sync(tsdata->input);
#else
        input_report_key(tsdata->input, ntp_keys[1], 0x01);
		input_sync(tsdata->input);
#endif
		key_status = 1;
		ret = 1;
        break;
		
    case 0x04:
        TPD_DEBUG("KEY[2] press\n");
#ifdef TOUCH_VIRTUAL_KEYS
		input_report_key(tsdata->input, BTN_TOUCH, 1);
		input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 1);
		input_report_abs(tsdata->input, ABS_MT_POSITION_X, 200);
		input_report_abs(tsdata->input, ABS_MT_POSITION_Y, 980);
		input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(tsdata->input);
		input_sync(tsdata->input);
#else
        input_report_key(tsdata->input, ntp_keys[2], 0x01);
		input_sync(tsdata->input);
#endif
		key_status = 1;
		ret = 1;
        break;
		
	default:
		break;
    }
	
	return ret;
}
#endif


static void ntp_work_func(struct work_struct *work)
{
	struct ntp_i2c_data *tsdata = g_nt11004_ts;
	unsigned char buf[14] = {0};
	unsigned char point_count = 0;
	unsigned char track_id = 0;
	int index, pos, x, y, w, p;
	int ret;

	TPD_DEBUG("ntp_work_func\n");

	ret = ntp_i2c_read_bytes(buf, 14);
	TPD_DEBUG("buf: %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],buf[12],buf[13]);

#ifdef NTP_WAKEUP_SYSTEM_SUPPORT
	ret = ntp_wakeup_system_event(buf[12], buf[13]);
	if(ret == 1)
	{
		goto END_WORK_FUNC;
	}
#endif

#ifdef NTP_WITH_BUTTON
	ret = ntp_button_event(buf[12], buf[13]);
#endif

	ret = 0;

    for (index = 0; index < MAX_FINGER_NUM; index++)
    {
	    pos = 6*index;

        track_id = (buf[pos]>>3) - 1;
            
        TPD_DEBUG("index=%x, pos=%x, track_id=%x\n", index, pos, track_id);

        if((track_id < MAX_FINGER_NUM)&&(track_id >= 0))
        {
        	if(((buf[pos]&0x03) == 0x01)||((buf[pos]&0x03) == 0x02))
            {
            	point_count ++;
				
            	x = (int)((buf[pos+1]<<4) | ((buf[pos+3] & 0xf0)>>4));
        		y = (int)((buf[pos+2]<<4) | ((buf[pos+3] & 0x0f)));
				x = x * LCD_MAX_WIDTH / TPD_MAX_WIDTH;
				y = y * LCD_MAX_HEIGHT / TPD_MAX_HEIGHT;
				
                // point press
                ntp_down(track_id, x ,y, w, p);
				ret = 1;
		    }
        }
    }

	TPD_DEBUG("point_count=%d\n", point_count);
	
	if((point_count == 0)&&(buf[0] != 0xff)&&(buf[1] != 0xff))
	{
		// point release
		ntp_up(track_id, x ,y, w, p);
		ret = 1;
	}

	if(ret == 1)
	{
		input_sync(tsdata->input);
	}
	
END_WORK_FUNC:
	enable_irq(tsdata->client->irq);
}

static int check_ctp_chip(void)
{	
	int	ret = -1;
	unsigned char buf[14] = {0};

	ctp_lock_mutex();
	ret = ntp_i2c_read_bytes(buf, 14);
	if (ret > 0)
	{
		tp_device_id(0x1104);
	}
	ctp_unlock_mutex();
	return ret;
}

static int remove_ctp_chip(void)
{
	ctp_lock_mutex();
	tp_device_id(0xFFFF);
	ctp_unlock_mutex();
	return 0;
}

static int ntp_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	//const struct novatek_platform_data *pdata = client->dev.platform_data;
	struct ntp_i2c_data *tsdata;
	struct input_dev *input;
	struct device *dev;
	struct i2c_dev *i2c_dev;
	struct novatek_platform_data *pdata = client->dev.platform_data;
	int i, error;
	u32 retval;

	TPD_DEBUG("nt1104 : ntp_i2c_probe!!! \n");
	
	if(tp_device_id(0)!=0)
	{
		printk("ntp(0x%x)Exist!", tp_device_id(0));
		error = -ENODEV;
		goto exit_check_ctp_chip;
	}
	
	this_client = client;
	client->irq = ntp_config_pins(); //reset pin set to 0 or 1 and platform init

	msleep(200);
	
	tsdata = kzalloc(sizeof(*tsdata), GFP_KERNEL);
	input = input_allocate_device();
	if (!tsdata || !input) 
	{
		dev_err(&client->dev, "Failed to allocate driver data!\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

	TPD_DEBUG("nt1104 : client->name=== %s\n", client->name);

	tsdata->client = client;
	tsdata->input = input;
	global_irq = client->irq;
	g_nt11004_ts = tsdata;
	tsdata->client->dev.platform_data = pdata;

	input->name = client->name;
	input->id.bustype = BUS_I2C;
	input->dev.parent = &client->dev;

	error = check_ctp_chip();
	if (error<0) {
		dev_err(&client->dev,
			"ntp_i2c_probe failed(%d): failed to check_ctp_chip: %s\n", error,
			dev_name(&client->dev));
		goto exit_check_ctp_chip;
	}

#ifdef TOUCH_VIRTUAL_KEYS
	ntp_virtual_keys_init();
#endif

	__set_bit(EV_KEY, input->evbit);
	__set_bit(EV_ABS, input->evbit);
	__set_bit(EV_SYN, input->evbit);
	__set_bit(BTN_TOUCH, input->keybit);

	__set_bit(ABS_MT_TOUCH_MAJOR, input->absbit);
	__set_bit(ABS_MT_POSITION_X, input->absbit);
	__set_bit(ABS_MT_POSITION_Y, input->absbit);
	__set_bit(ABS_MT_WIDTH_MAJOR, input->absbit);

	__set_bit(KEY_MENU,  input->keybit);
	__set_bit(KEY_BACK,  input->keybit);
	__set_bit(KEY_HOMEPAGE,  input->keybit);
	__set_bit(KEY_SEARCH,input->keybit);	
	__set_bit(KEY_POWER, input->keybit);

#ifdef TOUCH_VIRTUAL_KEYS
	input_set_capability(input, EV_KEY, KEY_MENU);
	input_set_capability(input, EV_KEY, KEY_HOMEPAGE);
	input_set_capability(input, EV_KEY, KEY_BACK);
	input_set_capability(input, EV_KEY, KEY_SEARCH);
#endif

	input_set_abs_params(input, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_X, 0, TPD_MAX_WIDTH, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_Y, 0, TPD_MAX_HEIGHT, 0, 0);
	input_set_abs_params(input, ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);

	input_set_drvdata(input, tsdata);

	INIT_WORK(&tsdata->pen_event_work, ntp_work_func);
	tsdata->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	error = request_irq(client->irq, ntp_isr_handler, IRQF_TRIGGER_FALLING, client->name, tsdata);

	if (error) 
	{
		dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
		goto err_free_mem;
	}
	disable_irq_nosync(client->irq);


	error = input_register_device(input);
	if (error)
	{
		TPD_DEBUG("nt1104 :input_register_device error\n");
		goto err_free_irq;
	}

	i2c_set_clientdata(client, tsdata);
	device_init_wakeup(&client->dev, 1);

	ntp_early_suspend.level   = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ntp_early_suspend.suspend = ntp_i2c_suspend;
	ntp_early_suspend.resume  = ntp_i2c_resume;
	register_early_suspend(&ntp_early_suspend);
	
	dev_err(&tsdata->client->dev, "insmod successfully!\n");

#ifdef NTP_APK_DRIVER_FUNC_SUPPORT
	ntp_flash_init();
#endif

#ifdef NTP_BOOTLOADER_FUNC_SUPPORT
	ntp_bl_bootloader(this_client);
#endif

    ntp_i2c_resume(NULL);

	return 0;

exit_check_ctp_chip:
	remove_ctp_chip();
	global_irq = 0;
	tsdata->client->irq = this_client->irq = 0;
	g_nt11004_ts->client->irq = 0;
	if (pdata->reset_gpio_number>0)
		gpio_free(pdata->reset_gpio_number);
	if (pdata->irq_gpio_number>0)
		gpio_free(pdata->irq_gpio_number);
	this_client = NULL;
err_free_irq:
	free_irq(client->irq, tsdata);
err_free_mem:
	input_free_device(input);
#ifdef TOUCH_VIRTUAL_KEYS
	ntp_virtual_keys_destroy();
#endif
	kfree(tsdata);
	
	return error;
}

static int ntp_i2c_remove(struct i2c_client *client)
{
	int error;
	struct i2c_dev *i2c_dev;
	struct ntp_i2c_data *tsdata = i2c_get_clientdata(client);
	struct novatek_platform_data *pdata = client->dev.platform_data;

	device_init_wakeup(&client->dev, 0);

//	tsdata->exiting = true;
	mb();
	free_irq(client->irq, tsdata);

	unregister_early_suspend(&ntp_early_suspend);
	input_unregister_device(tsdata->input);
#ifdef TOUCH_VIRTUAL_KEYS
	ntp_virtual_keys_destroy();
#endif
	remove_ctp_chip();
	tsdata->client->irq = global_irq = 0;
	if (pdata->reset_gpio_number>0)
		gpio_free(pdata->reset_gpio_number);
	if (pdata->irq_gpio_number>0)
		gpio_free(pdata->irq_gpio_number);
	this_client = NULL;
	i2c_set_clientdata(client, NULL);
	kfree(tsdata);

	return 0;
}


static const struct i2c_device_id ntp_i2c_id[] = 
{
	{NOVATEK_TS_NAME, 0},
	{ }
};

MODULE_DEVICE_TABLE(i2c, ntp_i2c_id);


static unsigned short force[] = {0, NOVATEK_TS_ADDR, I2C_CLIENT_END, I2C_CLIENT_END};
static const unsigned short * const forces[] = { force, NULL };

static struct i2c_driver ntp_i2c_driver = 
{
	.driver = 
	{
		.owner	= THIS_MODULE,
		.name	= NOVATEK_TS_NAME,
	},
	.probe		= ntp_i2c_probe,
	.remove		= ntp_i2c_remove,
	.id_table	= ntp_i2c_id,
};

static int __init ntp_i2c_init(void)
{
	int ret = -1;
	
    printk("nt11004 init");
	if(tp_device_id(0)!=0)
	{
		printk("ntp(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	ret = i2c_add_driver(&ntp_i2c_driver);
	
	return ret;
}

static void __exit ntp_i2c_exit(void)
{
	i2c_del_driver(&ntp_i2c_driver);
}

module_init(ntp_i2c_init);
module_exit(ntp_i2c_exit);


MODULE_AUTHOR("x_j_chen@novatek.com.cn>");
MODULE_DESCRIPTION("nt11004 I2C Touchscreen Driver");
MODULE_LICENSE("GPL");

