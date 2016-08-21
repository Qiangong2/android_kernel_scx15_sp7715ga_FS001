/* 
 * drivers/input/touchscreen/gslX680.c
 *
 * Sileadinc gslX680 TouchScreen driver. 
 *
 * Copyright (c) 2012  Sileadinc
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
 * VERSION      	DATE			AUTHOR
 *   1.0		 2012-04-18		   leweihua
 *
 * note: only support mulititouch	Wenfs 2010-10-01
 */



#include <linux/init.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/byteorder/generic.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/earlysuspend.h>
#include <linux/regulator/consumer.h>
#include <mach/regulator.h>
#include <mach/hardware.h>
#include <mach/globalregs.h>
#include <linux/kernel.h>
#include <linux/firmware.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/i2c/gslX680_ts.h>


#include <linux/input/mt.h>

#ifdef CONFIG_UDC_GPIO
#include <linux/udc.h>
#endif

#include <linux/cdc_com.h>
#if (CONFIG_ZYT_PRJ==7675) || (CONFIG_ZYT_PRJ==7271) ||(defined(CONFIG_MACH_SP7715EA_FS003_HVGA)) || (defined(CONFIG_MACH_SP7715GA_FS003))
#define USE_TP_PSENSOR
#endif
#ifdef USE_TP_PSENSOR
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include "tp_psensor.h"
#endif
#ifdef USE_TP_PSENSOR
//#define PS_DEBUG

#ifdef PS_DEBUG
#define PS_DBG(format, ...)	\
		printk(KERN_INFO "TP_PSENSOR " format "\n", ## __VA_ARGS__)
#else
#define PS_DBG(format, ...)
#endif  
static int gsl_ts_write(struct i2c_client *client, u8 addr, u8 *pdata, int datalen);
static int tp_ps_opened = 0;
static atomic_t ps_flag;
static tp_ps_t *tp_ps = 0;
static int ps_en = 0;
static u8 gsl_psensor_data[8]={0};

#endif //USE_TP_PSENSOR

//#define GSL_DEBUG

//将device信息放在board.c中
#define I2C_BOARD_INFO_METHOD
//#define CONFIG_ZYT_CTP_PS_CDC
#define MAX_FINGERS	  5
#define GSL_MONITOR

#define MAX_CONTACTS	10
#define DMA_TRANS_LEN	0x20

#define PRESS_MAX    255
#if (CONFIG_ZYT_PRJ==7675) && (CONFIG_ZYT_CUST ==64)//shangbaiyi
//#define FILTER_POINT
#else
//#define FILTER_POINT
#endif
#ifdef FILTER_POINT
#define FILTER_MAX	9
#endif

#ifdef GSL_MONITOR
static struct delayed_work gsl_monitor_work;
static struct workqueue_struct *gsl_monitor_workqueue = NULL;
static u8 int_1st[4] = {0};
static u8 int_2nd[4] = {0};
static char dac_counter = 0;
static char b0_counter = 0;
static char bc_counter = 0;
static char i2c_lock_flag = 0;
#endif 

#ifdef GSL_DEBUG 
#define print_info(fmt, args...)   \
        do{                              \
                printk(fmt, ##args);     \
        }while(0)
#else
#define print_info(fmt, args...)   //
#endif


struct sprd_i2c_setup_data {
	unsigned i2c_bus;  //the same number as i2c->adap.nr in adapter probe function
	unsigned short i2c_address;
	int irq;
	char type[I2C_NAME_SIZE];
};

static u32 id_sign[MAX_CONTACTS+1] = {0};
static u8 id_state_flag[MAX_CONTACTS+1] = {0};
static u8 id_state_old_flag[MAX_CONTACTS+1] = {0};
static u16 x_old[MAX_CONTACTS+1] = {0};
static u16 y_old[MAX_CONTACTS+1] = {0};
static u16 x_new = 0;
static u16 y_new = 0;

static struct i2c_client *this_client;
static  u8 tp_suspend = 0;

#ifdef CONFIG_UDC_TP
#define UPDATE_BUF_MAX 80
static udc_tp* g_tp = NULL; 
udc_t value_count = 0; 
udc_t line_buf[UPDATE_BUF_MAX];
struct fw_data fireware_buf[UPDATE_BUF_MAX];
#endif


//#define TPD_PROC_DEBUG
#ifdef TPD_PROC_DEBUG
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>
static struct proc_dir_entry *gsl_config_proc = NULL;
#define GSL_CONFIG_PROC_FILE "gsl_config"
#define CONFIG_LEN 31
static char gsl_read[CONFIG_LEN];
static u8 gsl_data_proc[8] = {0};
static u8 gsl_proc_flag = 0;
#endif

struct gslX680_ts_data {
	struct input_dev	*input_dev;
	u8 touch_data[44];	
	struct work_struct 	pen_event_work;
	struct workqueue_struct *ts_workqueue;
	struct early_suspend	early_suspend;
#ifdef I2C_BOARD_INFO_METHOD
	struct gslX680_ts_platform_data	*platform_data;
#endif
};
#ifdef I2C_BOARD_INFO_METHOD
static struct gslX680_ts_data	*g_gslX680_ts=NULL;
#else
//static struct sprd_i2c_setup_data gslX680_ts_setup={0, GSLX680_TS_ADDR, 0, GSLX680_TS_NAME};
#endif

#define TOUCH_VIRTUAL_KEYS





#define TS_IRQ_PIN			"ts_irq_pin"
#define TS_RESET_PIN		"ts_rst_pin"

//extern void set_i2c_clk(struct sprd_i2c *i2c,unsigned int freq);
extern void sprd_i2c_ctl_chg_clk(unsigned int id_nr, unsigned int freq);
#ifdef TOUCH_VIRTUAL_KEYS
static ssize_t virtual_keys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	print_info("==%s==\n", __func__);
	#if (CONFIG_ZYT_PRJ==7675) && (CONFIG_ZYT_CUST ==64)
	//shang bai yi
	return sprintf(buf,
	     __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":72:868:120:40"
	 ":"__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":240:868:120:40"
	 ":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":408:868:120:40"
	 "\n");
	#elif defined(CONFIG_LCD_WVGA)
	return sprintf(buf,
	     __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":72:868:120:40"
	 ":"__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":240:868:120:40"
	 ":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":408:868:120:40"
	 "\n");
	#else
	return sprintf(buf,
	     __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":53:500:60:40"
	 ":"__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":160:500:60:40"
	 ":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":266:500:60:40"
	 "\n");
	#endif
}

static struct kobj_attribute virtual_keys_attr = {
    .attr = {
        .name = "virtualkeys.SPRD_WW_CTP",
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

static void gslX680_ts_virtual_keys_init(void)
{
    int ret;
    struct kobject *properties_kobj;	
	print_info("==%s==\n", __func__);
	
    properties_kobj = kobject_create_and_add("board_properties", NULL);
    if (properties_kobj)
        ret = sysfs_create_group(properties_kobj,
                     &properties_attr_group);
    if (!properties_kobj || ret)
        print_info("gslX680 failed to create board_properties\n");   

	print_info("gslX680 success to create board_properties\n");
}
#endif

#ifdef  USE_TP_PSENSOR
static int gsl_ts_read(struct i2c_client *client, u8 addr, u8 *pdata, unsigned int datalen);
static int tp_ps_ioctl(struct i2c_client *client, unsigned int cmd, void *arg);

static void gsl_gain_psensor_data(struct i2c_client *client)
{
//	printk("----------------gsl_gain_psensor_data-start----tony------------\n");	

	int tmp = 0;
	u8 buf[4]={0};
	
	buf[0]=0x3;
	gsl_ts_write(client,0xf0,buf,4);
	tmp = gsl_ts_read(client,0x0,&gsl_psensor_data[0],4);
	if(tmp <= 0)
	{
		 gsl_ts_read(client,0x0,&gsl_psensor_data[0],4);
	}
	

	buf[0]=0x4;
	gsl_ts_write(client,0xf0,buf,4);
	tmp = gsl_ts_read(client,0x0,&gsl_psensor_data[4],4);
	if(tmp <= 0)
	{
		gsl_ts_read(client,0x0,&gsl_psensor_data[4],4);
	}

//		printk("----------------gsl_gain_psensor_data-end----tony------------\n");
}
       
static int gsl_ts_read(struct i2c_client *client, u8 addr, u8 *pdata, unsigned int datalen);

/*customer implement: do something like read data from TP IC*/
static int tp_ps_getdata(char *data)
{
	unsigned char read_buf[4];
	gsl_ts_read(this_client, 0xac, read_buf, sizeof(read_buf));
	*data = !(read_buf[0]);
	PS_DBG("read_buf[0]=%d\n\n",read_buf[0]);
	return 0;
}

static int tp_ps_enable()
{	
    u8 buf[4];
    int i;
        
	PS_DBG("%s\n", __func__);

    for(i = 0; i < 10; i ++)
    {
        if(i2c_lock_flag == 0)
        {   
            i2c_lock_flag = 1;
            break;
        }
        else
            msleep(10);
    }
    if(i >= 10)
	goto ps_enable;

       gsl_gain_psensor_data(this_client);
	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x00;
	buf[0] = 0x3;
	gsl_ts_write(this_client, 0xf0, buf,4);
	buf[3] = 0x5a;
	buf[2] = 0x5a;
	buf[1] = 0x5a;
	buf[0] = 0x5a;
	gsl_ts_write(this_client, 0x00, buf,4);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x00;
	buf[0] = 0x4;
	gsl_ts_write(this_client, 0xf0, buf,4);
	buf[3] = 0x0;
	buf[2] = 0x0;
	buf[1] = 0x0;
	buf[0] = 0x3;
	gsl_ts_write(this_client, 0x00, buf,4);  

ps_enable:
   i2c_lock_flag = 0;
	ps_en = 1;
	return 0;
}

static int tp_ps_disable()
{

	u8 buf[4];
    int i;
	PS_DBG("%s\n", __func__);

    for(i = 0; i < 10; i ++)
    {
        if(i2c_lock_flag == 0)
        {   
            i2c_lock_flag = 1;
            break;
        }
        else
            msleep(10);
    }
    if(i >= 10)
	goto ps_disable;


	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x00;
	buf[0] = 0x3;
	gsl_ts_write(this_client, 0xf0, buf,4);
	buf[3] = gsl_psensor_data[3];
	buf[2] = gsl_psensor_data[2];
	buf[1] = gsl_psensor_data[1];
	buf[0] = gsl_psensor_data[0];

	gsl_ts_write(this_client, 0x00, buf,4);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x00;
	buf[0] = 0x4;

	gsl_ts_write(this_client, 0xf0,  buf,4);
	buf[3] = gsl_psensor_data[7];
	buf[2] = gsl_psensor_data[6];
	buf[1] = gsl_psensor_data[5];
	buf[0] = gsl_psensor_data[4];
	gsl_ts_write(this_client, 0x00, buf,4);	

ps_disable:
    i2c_lock_flag = 0;
	ps_en = 0;
	return 0;
}

static int tp_ps_open(struct inode *inode, struct file *file)
{
	PS_DBG("%s\n", __func__);
	if (tp_ps_opened)
		return -EBUSY;
	tp_ps_opened = 1;
	return 0;
}

static int tp_ps_release(struct inode *inode, struct file *file)
{
	PS_DBG("%s", __func__);
	tp_ps_opened = 0;
	return tp_ps_disable();
}
//static long tp_ps_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
static int tp_ps_ioctl(struct i2c_client *client, unsigned int cmd, void *arg)

{
	switch (cmd) {
	case GTP_IOCTL_PROX_ON:
		tp_ps_enable();
		printk("%s:GTP_IOCTL_PROX_ON", __FUNCTION__);
		break;

	case GTP_IOCTL_PROX_OFF:
		tp_ps_disable();
		printk("%s:GTP_IOCTL_PROX_OFF", __FUNCTION__);
		break;

	default:
		pr_err("%s: invalid cmd %d\n", __func__, _IOC_NR(cmd));
		return -EINVAL;
	}

	return 0;
}

static struct file_operations tp_ps_fops = {
	.owner		= THIS_MODULE,
	.open		= NULL,
	.release	= NULL,
	.unlocked_ioctl		= tp_ps_ioctl,
};

static struct miscdevice tp_ps_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = TP_PS_DEVICE,
	.fops = &tp_ps_fops,
};
static void tp_ps_report_dps()
{
	unsigned char dps_data = 0;
	
	tp_ps_getdata(&dps_data);
	
	PS_DBG("%s: proximity=%d", __func__, dps_data);
	
	input_report_abs(tp_ps->input, ABS_DISTANCE, dps_data);
	input_sync(tp_ps->input);
}
static int tp_ps_init(struct i2c_client *client)
{
	int err = 0;
		struct gslX680_ts_data *gslX680_ts = i2c_get_clientdata(client);
	ps_en = 0;
	
	tp_ps = kzalloc(sizeof(tp_ps_t), GFP_KERNEL);
	if (!tp_ps)
	{
		PS_DBG("%s: request memory failed\n", __func__);
		err= -ENOMEM;
		goto exit_mem_fail;
	}
		
	//register device


	tp_ps->input = gslX680_ts->input_dev;
	__set_bit(EV_ABS, tp_ps->input->evbit);	
	//for proximity
	input_set_abs_params(tp_ps->input, ABS_DISTANCE, 0, 1, 0, 0);


	PS_DBG("%s: Probe Success!\n",__func__);
	return 0;
#ifndef CONFIG_ZYT_CTP_PS_CDC
exit_input_register_failed:
	input_free_device(input_dev);
exit_input_dev_allocate_failed:
#endif
exit_misc_reg_fail:
	kfree(tp_ps);
exit_mem_fail:
	return err;
}

static int tp_ps_uninit()
{
	//free input
#ifndef CONFIG_ZYT_CTP_PS_CDC
	input_unregister_device(tp_ps->input);
	input_free_device(tp_ps->input);
#endif
	//free alloc
	kfree(tp_ps);
	tp_ps = 0;
}



static ssize_t gslX680_ts_enable_show(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t gslX680_ts_enable_store(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);


static DEVICE_ATTR(enable, S_IRWXU | S_IRWXG | S_IRWXO,gslX680_ts_enable_show, gslX680_ts_enable_store);


static struct attribute *gslX680_ts_attributes[] = {
	&dev_attr_enable.attr,
	NULL
};

static const struct attribute_group gslX680_ts_attr_group = {
	.attrs = gslX680_ts_attributes,
};


static ssize_t gslX680_ts_enable_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	int						enable;
	enable = ps_en;
		printk("%s\n", __func__);
	return sprintf(buf, "%d\n", enable);
}

static ssize_t gslX680_ts_enable_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
//	struct gslX680_ts_data	*acc = g_ft5x0x_ts;
	int						ret;
	unsigned long			enable;
		printk("%s\n", __func__);
	enable = simple_strtoul(buf, NULL, 10);    

	enable = (enable > 0) ? 1 : 0;
	if(enable)
	{  
			tp_ps_enable();   
	}
	else
	{
			tp_ps_disable();   
	}

	return count;
}
#endif //USE_TP_PSENSOR


/*******************************************************************************
* Function    :  gslX680_ts_config_pins
* Description :  config gpio pins and set relative TS pins
* Parameters  :  void
* Return      :    int irq_num;
*******************************************************************************/
static void gslX680_ts_pwron(void)
{
	print_info("==%s==\n", __func__);
	//LDO_SetVoltLevel(LDO_LDO_SIM2, LDO_VOLT_LEVEL0);
	//LDO_TurnOnLDO(LDO_LDO_SIM2);
	//msleep(20);
	/*
	LDO_SetVoltLevel(LDO_LDO_SIM2, LDO_VOLT_LEVEL2);
	LDO_TurnOnLDO(LDO_LDO_SIM2);
	msleep(20);

	LDO_SetVoltLevel(LDO_LDO_SIM3, LDO_VOLT_LEVEL0);
	LDO_TurnOnLDO(LDO_LDO_SIM3);
	msleep(20);
	*/
} 
static void gslX680_ts_pwroff(void)
{
	print_info("==%s==\n", __func__);
	/*
	//LDO_TurnOffLDO(LDO_LDO_SIM2);
	msleep(5);
	LDO_TurnOffLDO(LDO_LDO_SIM2);
	msleep(5);
	LDO_TurnOffLDO(LDO_LDO_SIM3);
	msleep(5);
	*/
}
static void gslX680_ts_pininit(int irq_pin, int rst_pin)
{
	print_info(KERN_INFO "====%s==== [irq=%d];[rst=%d]\n",__func__,irq_pin,rst_pin);
	gpio_request(irq_pin, TS_IRQ_PIN);
	gpio_request(rst_pin, TS_RESET_PIN);
}
static void gslX680_ts_config_pins(int* client_irq)
{
#ifdef I2C_BOARD_INFO_METHOD
	unsigned gpio_irq=0, gpio_rst=0;
	struct gslX680_ts_platform_data *pdata = g_gslX680_ts->platform_data;

	printk(KERN_INFO "%s [irq=%d];[rst=%d]\n", __func__, pdata->irq_gpio_number, pdata->reset_gpio_number);
	gpio_irq = pdata->irq_gpio_number;
	gpio_rst = pdata->reset_gpio_number;

	gpio_request(gpio_irq, TS_IRQ_PIN);
	gpio_request(gpio_rst, TS_RESET_PIN);
	
	gpio_direction_output(gpio_rst, 1);
	gpio_direction_input(gpio_irq);	
	gpio_set_value(gpio_rst, 1);
	print_info("client_irq=%d\n", *client_irq);
	*client_irq = gpio_to_irq(gpio_irq);;
	print_info("client_irq=%d\n", *client_irq);
#else

	int gslX680_irq;
	gslX680_ts_pwron();
	gslX680_ts_pininit(sprd_3rdparty_gpio_tp_irq,sprd_3rdparty_gpio_tp_rst);

	gpio_direction_output(sprd_3rdparty_gpio_tp_rst, 1);
	gpio_direction_input(sprd_3rdparty_gpio_tp_irq);	
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
	//gslX680_irq=sprd_alloc_gpio_irq(sprd_3rdparty_gpio_tp_irq);
#endif
	msleep(100); //wait for stable
	
	print_info("==%s out==\n", __func__);

	return ;
}

static inline u16 join_bytes(u8 a, u8 b)
{
	u16 ab = 0;
	ab = ab | a;
	ab = ab << 8 | b;
	return ab;
}

static u32 gsl_read_interface(struct i2c_client *client, u8 reg, u8 *buf, u32 num)
{
	struct i2c_msg xfer_msg[2];

	xfer_msg[0].addr = client->addr;
	xfer_msg[0].len = 1;
	xfer_msg[0].flags = client->flags & I2C_M_TEN;
	xfer_msg[0].buf = &reg;

	xfer_msg[1].addr = client->addr;
	xfer_msg[1].len = num;
	xfer_msg[1].flags |= I2C_M_RD;
	xfer_msg[1].buf = buf;

	if (reg < 0x80) {
		i2c_transfer(client->adapter, xfer_msg, ARRAY_SIZE(xfer_msg));
		msleep(5);
	}

	return i2c_transfer(client->adapter, xfer_msg, ARRAY_SIZE(xfer_msg)) == ARRAY_SIZE(xfer_msg) ? 0 : -EFAULT;
}

static u32 gsl_write_interface(struct i2c_client *client, const u8 reg, u8 *buf, u32 num)
{
	struct i2c_msg xfer_msg[1];

	buf[0] = reg;

	xfer_msg[0].addr = client->addr;
	xfer_msg[0].len = num + 1;
	xfer_msg[0].flags = client->flags & I2C_M_TEN;
	xfer_msg[0].buf = buf;

	return i2c_transfer(client->adapter, xfer_msg, 1) == 1 ? 0 : -EFAULT;
}

static __inline__ void fw2buf(u8 *buf, const u32 *fw)
{
	u32 *u32_buf = (int *)buf;
	*u32_buf = *fw;
}

static void gsl_load_fw(struct i2c_client *client)
{
	u8 buf[DMA_TRANS_LEN*4 + 1] = {0};
	u8 send_flag = 1;
	u8 *cur = buf + 1;
	u32 source_line = 0;
	u32 source_len = ARRAY_SIZE(GSLX680_FW);
    struct fw_data *ptr_fw;
#ifdef CONFIG_UDC_TP
    u32 update_line = 0;
    u32 update_len = 0;
#endif

	print_info("=============gsl_load_fw start==============\n");

    ptr_fw = GSLX680_FW;
#ifdef CONFIG_UDC_TP
    if(g_tp->config.addr == GSLX680_TS_ADDR)
    {
        update_len = value_count;
    
        print_info("gsl_load_fw contect: %d=0x%x, 0x%x, %d=0x%x, 0x%x, %d=0x%x, 0x%x\n", 
            line_buf[0], fireware_buf[0].offset, fireware_buf[0].val,  
            line_buf[1], fireware_buf[1].offset, fireware_buf[1].val, 
            line_buf[2], fireware_buf[2].offset, fireware_buf[2].val);
    }
    update_line = 0;
#endif	

	for (source_line = 0; source_line < source_len; source_line++) 
	{
		/* init page trans, set the page val */
		if (0xf0 == ptr_fw[source_line].offset)
		{
			fw2buf(cur, &ptr_fw[source_line].val);
			gsl_write_interface(client, 0xf0, buf, 4);
			send_flag = 1;
		}
		else 
		{
			if (1 == send_flag % (DMA_TRANS_LEN < 0x20 ? DMA_TRANS_LEN : 0x20))
	    			buf[0] = (u8)ptr_fw[source_line].offset;

#ifdef CONFIG_UDC_TP
            if(g_tp->config.addr == GSLX680_TS_ADDR && line_buf[update_line] == (source_line+1) && update_line < update_len)
            {
                fw2buf(cur, &fireware_buf[update_line].val);
                update_line++;
            }
            else
#endif
			fw2buf(cur, &ptr_fw[source_line].val);
			cur += 4;

			if (0 == send_flag % (DMA_TRANS_LEN < 0x20 ? DMA_TRANS_LEN : 0x20)) 
			{
	    			gsl_write_interface(client, buf[0], buf, cur - buf - 1);
	    			cur = buf + 1;
			}

			send_flag++;
		}
	}

	print_info("=============gsl_load_fw end==============\n");

}


static int gsl_ts_write(struct i2c_client *client, u8 addr, u8 *pdata, int datalen)
{
	int ret = 0;
	u8 tmp_buf[128];
	unsigned int bytelen = 0;
	if (datalen > 125)
	{
		print_info("%s too big datalen = %d!\n", __func__, datalen);
		return -1;
	}
	
	tmp_buf[0] = addr;
	bytelen++;
	
	if (datalen != 0 && pdata != NULL)
	{
		memcpy(&tmp_buf[bytelen], pdata, datalen);
		bytelen += datalen;
	}
	
	ret = i2c_master_send(client, tmp_buf, bytelen);
	return ret;
}

static int gsl_ts_read(struct i2c_client *client, u8 addr, u8 *pdata, unsigned int datalen)
{
	int ret = 0;

	if (datalen > 126)
	{
		print_info("%s too big datalen = %d!\n", __func__, datalen);
		return -1;
	}

	ret = gsl_ts_write(client, addr, NULL, 0);
	if (ret < 0)
	{
		print_info("%s set data address fail!\n", __func__);
		return ret;
	}
	
	return i2c_master_recv(client, pdata, datalen);
}

static int test_i2c(struct i2c_client *client)
{
	u8 read_buf = 0;
	u8 write_buf = 0x12;
	int ret, rc = 1;
	print_info("gslX680 %s\n", __func__);
	
	ret = gsl_ts_read( client, 0xf0, &read_buf, sizeof(read_buf) );
	if  (ret  < 0)  
    		rc --;
	else
		printk("gslX680 read reg 0xf0 is %x\n", read_buf);
	
	msleep(2);
	ret = gsl_ts_write(client, 0xf0, &write_buf, sizeof(write_buf));
	if(ret  >=  0 )
		printk("gslX680 write reg 0xf0 0x12\n");
	
	msleep(2);
	ret = gsl_ts_read( client, 0xf0, &read_buf, sizeof(read_buf) );
	if(ret <  0 )
		rc --;
	else
		printk("gslX680 read reg 0xf0 is 0x%x\n", read_buf);

	if(rc < 0)
	{
		printk("check_ctp_chip failed(%d)\n", ret);
	}
	else if (read_buf != write_buf)
	{
		rc=-1;
		printk("check_ctp_chip failed(%d) %d\n", read_buf, rc);
	}
	else
	{
		printk("check_ctp_chip success(0x%x)\n", 0x1680);
		tp_device_id(0x1680);
	}

	return rc;
}

static int remove_ctp_chip(void)
{
	print_info("gslX680 %s\n", __func__);
	tp_device_id(0xFFFF);
	return 0;
}

static void startup_chip(struct i2c_client *client)
{
	u8 tmp = 0x00;
	print_info("gslX680 %s\n", __func__);
#ifdef GSL_NOID_VERSION
	gsl_DataInit(gsl_config_data_id);
#endif


	gsl_ts_write(client, 0xe0, &tmp, 1);
	msleep(10);	
}

static void reset_chip(struct i2c_client *client)
{
	u8 tmp = 0x88;
	u8 buf[4] = {0x00};
	print_info("gslX680 %s\n", __func__);
	
	gsl_ts_write(client, 0xe0, &tmp, sizeof(tmp));
	msleep(20);
	tmp = 0x04;
	gsl_ts_write(client, 0xe4, &tmp, sizeof(tmp));
	msleep(10);
	gsl_ts_write(client, 0xbc, buf, sizeof(buf));
	msleep(10);
}

static void clr_reg(struct i2c_client *client)
{
	u8 write_buf[4]	= {0};

	print_info("gslX680 %s\n", __func__);
	write_buf[0] = 0x88;
	gsl_ts_write(client, 0xe0, &write_buf[0], 1); 	
	msleep(20);
	write_buf[0] = 0x03;
	gsl_ts_write(client, 0x80, &write_buf[0], 1); 	
	msleep(5);
	write_buf[0] = 0x04;
	gsl_ts_write(client, 0xe4, &write_buf[0], 1); 	
	msleep(5);
	write_buf[0] = 0x00;
	gsl_ts_write(client, 0xe0, &write_buf[0], 1); 	
	msleep(20);
}

static void init_chip(struct i2c_client *client)
{
	int rc;
	print_info("gslX680 %s reset_gpio_number=%d\n", __func__, g_gslX680_ts->platform_data->reset_gpio_number);
#ifdef I2C_BOARD_INFO_METHOD
	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 0);
	msleep(20); 	
	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 1);
#else
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 0);
	msleep(20); 	
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
#endif
	printk("gslX680 after gpio_set_value-reset_gpio_number\n");
	msleep(20); 	
	printk("invoke test_i2c in init_chip\n");	
	rc = test_i2c(client);
	if(rc < 0)
	{
		printk("------gslX680 test_i2c error------\n");	
		return;
	}	
	clr_reg(client);
	reset_chip(client);
	gsl_load_fw(client);			
	startup_chip(client);
	reset_chip(client);
	startup_chip(client);		
}

static void check_mem_data(struct i2c_client *client)
{
	u8 read_buf[4]  = {0};
	
	msleep(30);
	gsl_ts_read(client,0xb0, read_buf, sizeof(read_buf));
	
	if (read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
	{
		printk("#########check mem read 0xb0 = %x %x %x %x #########\n", read_buf[3], read_buf[2], read_buf[1], read_buf[0]);
		init_chip(client);
	}
}

#ifdef TPD_PROC_DEBUG
static int char_to_int(char ch)
{
	if(ch>='0' && ch<='9')
		return (ch-'0');
	else
		return (ch-'a'+10);
}

//static int gsl_config_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
static int gsl_config_read_proc(struct seq_file *m,void *v)
{
	char temp_data[5] = {0};
	unsigned int tmp=0;
	
	if('v'==gsl_read[0]&&'s'==gsl_read[1])
	{
#ifdef GSL_NOID_VERSION
		tmp=gsl_version_id();
#else 
		tmp=0x20121215;
#endif
		seq_printf(m,"version:%x\n",tmp);
	}
	else if('r'==gsl_read[0]&&'e'==gsl_read[1])
	{
		if('i'==gsl_read[3])
		{
#ifdef GSL_NOID_VERSION 
			tmp=(gsl_data_proc[5]<<8) | gsl_data_proc[4];
			seq_printf(m,"gsl_config_data_id[%d] = ",tmp);
			if(tmp>=0&&tmp<ARRAY_SIZE(gsl_config_data_id))
			{
					seq_printf(m,"%d\n",gsl_config_data_id[tmp]); 
			}
#endif
		}
		else 
		{
			gsl_ts_write(this_client,0Xf0,&gsl_data_proc[4],4);
			if(gsl_data_proc[0] < 0x80)
				gsl_ts_read(this_client,gsl_data_proc[0],temp_data,4);
			gsl_ts_read(this_client,gsl_data_proc[0],temp_data,4);

			seq_printf(m,"offset : {0x%02x,0x",gsl_data_proc[0]);
			seq_printf(m,"%02x",temp_data[3]);
			seq_printf(m,"%02x",temp_data[2]);
			seq_printf(m,"%02x",temp_data[1]);
			seq_printf(m,"%02x};\n",temp_data[0]);
		}
	}
//	*eof = 1;
	return (0);
}
static int gsl_config_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	u8 buf[8] = {0};
	char temp_buf[CONFIG_LEN];
	char *path_buf;
	int tmp = 0;
	int tmp1 = 0;
	print_info("[tp-gsl][%s] \n",__func__);
	if(count > 512)
	{
		print_info("size not match [%d:%ld]\n", CONFIG_LEN, count);
        return -EFAULT;
	}
	path_buf=kzalloc(count,GFP_KERNEL);
	if(!path_buf)
	{
		printk("alloc path_buf memory error \n");
		return -1;
	}
	if(copy_from_user(path_buf, buffer, count))
	{
		print_info("copy from user fail\n");
		goto exit_write_proc_out;
	}
	memcpy(temp_buf,path_buf,(count<CONFIG_LEN?count:CONFIG_LEN));
	print_info("[tp-gsl][%s][%s]\n",__func__,temp_buf);
	
	buf[3]=char_to_int(temp_buf[14])<<4 | char_to_int(temp_buf[15]);	
	buf[2]=char_to_int(temp_buf[16])<<4 | char_to_int(temp_buf[17]);
	buf[1]=char_to_int(temp_buf[18])<<4 | char_to_int(temp_buf[19]);
	buf[0]=char_to_int(temp_buf[20])<<4 | char_to_int(temp_buf[21]);
	
	buf[7]=char_to_int(temp_buf[5])<<4 | char_to_int(temp_buf[6]);
	buf[6]=char_to_int(temp_buf[7])<<4 | char_to_int(temp_buf[8]);
	buf[5]=char_to_int(temp_buf[9])<<4 | char_to_int(temp_buf[10]);
	buf[4]=char_to_int(temp_buf[11])<<4 | char_to_int(temp_buf[12]);
	if('v'==temp_buf[0]&& 's'==temp_buf[1])//version //vs
	{
		memcpy(gsl_read,temp_buf,4);
		printk("gsl version\n");
	}
	else if('s'==temp_buf[0]&& 't'==temp_buf[1])//start //st
	{
	#ifdef GSL_MONITOR
		cancel_delayed_work_sync(&gsl_monitor_work);
	#endif
		gsl_proc_flag = 1;
		reset_chip(this_client);
	}
	else if('e'==temp_buf[0]&&'n'==temp_buf[1])//end //en
	{
		msleep(20);
		reset_chip(this_client);
		startup_chip(this_client);
		gsl_proc_flag = 0;
	}
	else if('r'==temp_buf[0]&&'e'==temp_buf[1])//read buf //
	{
		memcpy(gsl_read,temp_buf,4);
		memcpy(gsl_data_proc,buf,8);
	}
	else if('w'==temp_buf[0]&&'r'==temp_buf[1])//write buf
	{
		gsl_ts_write(this_client,buf[4],buf,4);
	}
#ifdef GSL_NOID_VERSION
	else if('i'==temp_buf[0]&&'d'==temp_buf[1])//write id config //
	{
		tmp1=(buf[7]<<24)|(buf[6]<<16)|(buf[5]<<8)|buf[4];
		tmp=(buf[3]<<24)|(buf[2]<<16)|(buf[1]<<8)|buf[0];
		if(tmp1>=0 && tmp1<ARRAY_SIZE(gsl_config_data_id))
		{
			gsl_config_data_id[tmp1] = tmp;
		}
	}
#endif
exit_write_proc_out:
	kfree(path_buf);
	return count;
}
static int gsl_server_list_open(struct inode *inode,struct file *file)
{
	return single_open(file,gsl_config_read_proc,NULL);
}
static const struct file_operations gsl_seq_fops = {
	.open = gsl_server_list_open,
	.read = seq_read,
	.release = single_release,
	.write = gsl_config_write_proc,
	.owner = THIS_MODULE,
};
#endif

#ifdef FILTER_POINT
static void filter_point(u16 x, u16 y , u8 id)
{
	u16 x_err =0;
	u16 y_err =0;
	u16 filter_step_x = 0, filter_step_y = 0;
	
	id_sign[id] = id_sign[id] + 1;
	if(id_sign[id] == 1)
	{
		x_old[id] = x;
		y_old[id] = y;
	}
	
	x_err = x > x_old[id] ? (x -x_old[id]) : (x_old[id] - x);
	y_err = y > y_old[id] ? (y -y_old[id]) : (y_old[id] - y);

	if( (x_err > FILTER_MAX && y_err > FILTER_MAX/3) || (x_err > FILTER_MAX/3 && y_err > FILTER_MAX) )
	{
		filter_step_x = x_err;
		filter_step_y = y_err;
	}
	else
	{
		if(x_err > FILTER_MAX)
			filter_step_x = x_err; 
		if(y_err> FILTER_MAX)
			filter_step_y = y_err;
	}

	if(x_err <= 2*FILTER_MAX && y_err <= 2*FILTER_MAX)
	{
		filter_step_x >>= 2; 
		filter_step_y >>= 2;
	}
	else if(x_err <= 3*FILTER_MAX && y_err <= 3*FILTER_MAX)
	{
		filter_step_x >>= 1; 
		filter_step_y >>= 1;
	}	
	else if(x_err <= 4*FILTER_MAX && y_err <= 4*FILTER_MAX)
	{
		filter_step_x = filter_step_x*3/4; 
		filter_step_y = filter_step_y*3/4;
	}
	
	x_new = x > x_old[id] ? (x_old[id] + filter_step_x) : (x_old[id] - filter_step_x);
	y_new = y > y_old[id] ? (y_old[id] + filter_step_y) : (y_old[id] - filter_step_y);

	x_old[id] = x_new;
	y_old[id] = y_new;
}
#else

static void record_point(u16 x, u16 y , u8 id)
{
	u16 x_err =0;
	u16 y_err =0;

	id_sign[id]=id_sign[id]+1;
	
	if(id_sign[id]==1){
		x_old[id]=x;
		y_old[id]=y;
	}

	x = (x_old[id] + x)/2;
	y = (y_old[id] + y)/2;
		
	if(x>x_old[id]){
		x_err=x -x_old[id];
	}
	else{
		x_err=x_old[id]-x;
	}

	if(y>y_old[id]){
		y_err=y -y_old[id];
	}
	else{
		y_err=y_old[id]-y;
	}

	if( (x_err > 3 && y_err > 1) || (x_err > 1 && y_err > 3) ){
		x_new = x;     x_old[id] = x;
		y_new = y;     y_old[id] = y;
	}
	else{
		if(x_err > 3){
			x_new = x;     x_old[id] = x;
		}
		else
			x_new = x_old[id];
		if(y_err> 3){
			y_new = y;     y_old[id] = y;
		}
		else
			y_new = y_old[id];
	}

	if(id_sign[id]==1){
		x_new= x_old[id];
		y_new= y_old[id];
	}	
}
#endif


#ifdef USE_TP_PSENSOR
static void report_data(struct gslX680_ts_data *ts, u16 x, u16 y, u8 pressure, u8 id)
{	
		input_mt_slot(ts->input_dev, id);
    input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, pressure);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_X, x);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, y);
    input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, 1);
    input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, id);	
}
#else
static void report_data(struct gslX680_ts_data *ts, u16 x, u16 y, u8 pressure, u8 id)
{
	input_report_key(ts->input_dev, BTN_TOUCH, 1);  //jihb

	input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, pressure);
	input_report_abs(ts->input_dev, ABS_MT_POSITION_X, x);
	input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, y);	
	input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, 1);
    print_info("report_data: id %d, x %d, y %d \n",id, x, y);
	input_mt_sync(ts->input_dev);
}
#endif

#ifdef USE_TP_PSENSOR
static void process_gslX680_data(struct gslX680_ts_data *ts)
{
	u8 id, touches;
	u16 x, y;
	int i = 0;
#ifdef GSL_NOID_VERSION
	struct gsl_touch_info cinfo={0};
	u32 tmp1=0;
	u8 buf[4]={0};
#endif
	touches = ts->touch_data[0];
#ifdef GSL_NOID_VERSION
	cinfo.finger_num = touches;
	print_info("tp-gsl  finger_num = %d\n",cinfo.finger_num);
	for(i = 0; i < (touches < MAX_CONTACTS ? touches : MAX_CONTACTS); i ++)
	{
		cinfo.x[i] = join_bytes( ( ts->touch_data[4 * i + 7] & 0xf),ts->touch_data[4 * i + 6]);
		cinfo.y[i] = join_bytes(ts->touch_data[4 * i + 5],ts->touch_data[4 * i +4]);
		cinfo.id[i] = ((ts->touch_data[4 * i + 7]  & 0xf0)>>4);
		print_info("tp-gsl  x = %d y = %d \n",cinfo.x[i],cinfo.y[i]);
	}
	cinfo.finger_num=(ts->touch_data[3]<<24)|(ts->touch_data[2]<<16)
		|(ts->touch_data[1]<<8)|(ts->touch_data[0]);
	gsl_alg_id_main(&cinfo);
	tmp1=gsl_mask_tiaoping();
	print_info("[tp-gsl] tmp1=%x\n",tmp1);
	if(tmp1>0&&tmp1<0xffffffff)
	{
		buf[0]=0xa;buf[1]=0;buf[2]=0;buf[3]=0;
		gsl_ts_write(this_client,0xf0,buf,4);
		buf[0]=(u8)(tmp1 & 0xff);
		buf[1]=(u8)((tmp1>>8) & 0xff);
		buf[2]=(u8)((tmp1>>16) & 0xff);
		buf[3]=(u8)((tmp1>>24) & 0xff);
		print_info("tmp1=%08x,buf[0]=%02x,buf[1]=%02x,buf[2]=%02x,buf[3]=%02x\n",
			tmp1,buf[0],buf[1],buf[2],buf[3]);
		gsl_ts_write(this_client,0x8,buf,4);
	}
	touches = cinfo.finger_num;
#endif
#ifdef USE_TP_PSENSOR
	if(ps_en)
	{
		tp_ps_report_dps();
	}
#endif
	print_info("report_data: touches %d \n",touches);
	for(i=1;i<=MAX_CONTACTS;i++)
	{
		if(touches == 0)
			id_sign[i] = 0;	
		id_state_flag[i] = 0;
	}

	for(i=0;i<(touches > MAX_FINGERS ? MAX_FINGERS : touches);i++)
	{
#ifdef GSL_NOID_VERSION
		id = cinfo.id[i];
		x =  cinfo.x[i];
		y =  cinfo.y[i];	
#else
		id = ts->touch_data[4 * i + 7] >> 4;
		x = join_bytes( ( ts->touch_data[4 * i + 7] & 0xf),ts->touch_data[4 * i + 6]);
		y = join_bytes(ts->touch_data[4 * i + 5],ts->touch_data[4 * i +4]);
#endif	
		if(1 <= id && id <= MAX_CONTACTS)
		{
		#ifdef FILTER_POINT
			filter_point(x, y ,id);
		#else
			record_point(x, y , id);
		#endif
			report_data(ts, x_new, y_new, 10, id);		
			id_state_flag[id] = 1;
		}
	}
	for(i=1;i<=MAX_CONTACTS;i++)
	{	
		if( (0 == touches) || ((0 != id_state_old_flag[i]) && (0 == id_state_flag[i])) )
		{
			input_mt_slot(ts->input_dev, i);
		      input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, -1);
		      input_mt_report_slot_state(ts->input_dev, MT_TOOL_FINGER, false);
			id_sign[i]=0;
		}
		id_state_old_flag[i] = id_state_flag[i];
	}
	if(0 == touches)
	{
		{
			input_mt_slot(ts->input_dev, i);
	      input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, -1);
	      input_mt_report_slot_state(ts->input_dev, MT_TOOL_FINGER, false);
		}

	}	
	input_sync(ts->input_dev);
}
#else
static void process_gslX680_data(struct gslX680_ts_data *ts)
{
	u8 id, touches;
	u16 x, y;
	int i = 0;

	touches = ts->touch_data[0];
	print_info("report_data: touches %d \n",touches);
	for(i=1;i<=MAX_CONTACTS;i++)
	{
		if(touches == 0)
		id_sign[i] = 0;	
		id_state_flag[i] = 0;
	}
	for(i=0;i<(touches > MAX_FINGERS ? MAX_FINGERS : touches);i++)
	{
		id = ts->touch_data[4 * i + 7] >> 4;
		x = join_bytes( ( ts->touch_data[4 * i + 7] & 0xf),ts->touch_data[4 * i + 6]);
		y = join_bytes(ts->touch_data[4 * i + 5],ts->touch_data[4 * i +4]);
		if(1 <= id && id <= MAX_CONTACTS)
		{
		#ifdef FILTER_POINT
			filter_point(x, y ,id);
		#else
			record_point(x, y , id);
		#endif
			report_data(ts, x_new, y_new, 200, id);		
			id_state_flag[id] = 1;
		}
	}
	for(i=1;i<=MAX_CONTACTS;i++)
	{	
		if( (0 == touches) || ((0 != id_state_old_flag[i]) && (0 == id_state_flag[i])) )
		{
			id_sign[i]=0;
		}
		id_state_old_flag[i] = id_state_flag[i];
	}
	if(0 == touches)
	{
		input_report_key(ts->input_dev, BTN_TOUCH, 0);  //jihb
		input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0);
		input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0);
		input_mt_sync(ts->input_dev);
	}	
	input_sync(ts->input_dev);

}
#endif
#if 1
static void gslX680_ts_pen_irq_work(struct work_struct *work)
{
	int rc;
	u8 read_buf[4] = {0};
	struct gslX680_ts_data *ts = i2c_get_clientdata(this_client);

	print_info("gslX680_ts_pen_irq_work \n");

#ifdef GSL_MONITOR
	if(i2c_lock_flag != 0)
		goto i2c_lock_schedule;
	else
		i2c_lock_flag = 1;
#endif

#ifdef TPD_PROC_DEBUG
	if(gsl_proc_flag == 1){
		goto schedule;		
	}
#endif	

	/* read data from DATA_REG */
	rc = gsl_ts_read(this_client, 0x80, ts->touch_data, sizeof(ts->touch_data));

	if (rc < 0) 
	{
		dev_err(&this_client->dev, "read failed\n");
		goto schedule;
	}

	if (ts->touch_data[0] == 0xff) {
		goto schedule;
	}

	process_gslX680_data(ts);
	
schedule:
#ifdef GSL_MONITOR
	i2c_lock_flag = 0;
i2c_lock_schedule:
#endif
	//enable_irq(this_client->irq);
	if(this_client->irq > 0)
	{
		enable_irq(this_client->irq);
	}
	else
	{
		#ifdef I2C_BOARD_INFO_METHOD
		enable_irq(g_gslX680_ts->platform_data->irq_gpio_number);
		#else
		enable_irq(gslX680_ts_setup.irq);
		#endif
	}

}
#endif


#ifdef GSL_MONITOR
static void gsl_monitor_worker(void)
{
	u8 write_buf[4] = {0};
	u8 read_buf[4]  = {0};
	char init_chip_flag = 0;
	
	print_info("----------------gsl_monitor_worker-----------------\n");	

	if(i2c_lock_flag != 0)
		goto queue_monitor_work;
	else
		i2c_lock_flag = 1;
	
	gsl_ts_read(this_client, 0xb0, read_buf, 4);
	if(read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
		b0_counter ++;
	else
		b0_counter = 0;

	if(b0_counter > 1)
	{
		printk("======read 0xb0: %x %x %x %x ======\n",read_buf[3], read_buf[2], read_buf[1], read_buf[0]);
		init_chip_flag = 1;
		b0_counter = 0;
		goto queue_monitor_init_chip;
	}
	
	gsl_ts_read(this_client, 0xb4, read_buf, 4);	
	int_2nd[3] = int_1st[3];
	int_2nd[2] = int_1st[2];
	int_2nd[1] = int_1st[1];
	int_2nd[0] = int_1st[0];
	int_1st[3] = read_buf[3];
	int_1st[2] = read_buf[2];
	int_1st[1] = read_buf[1];
	int_1st[0] = read_buf[0];

	if(int_1st[3] == int_2nd[3] && int_1st[2] == int_2nd[2] &&int_1st[1] == int_2nd[1] && int_1st[0] == int_2nd[0]) 
	{
		printk("======int_1st: %x %x %x %x , int_2nd: %x %x %x %x ======\n",int_1st[3], int_1st[2], int_1st[1], int_1st[0], int_2nd[3], int_2nd[2],int_2nd[1],int_2nd[0]);
		init_chip_flag = 1;
		goto queue_monitor_init_chip;
	}

	gsl_ts_read(this_client, 0xbc, read_buf, 4);
	if(read_buf[3] != 0 || read_buf[2] != 0 || read_buf[1] != 0 || read_buf[0] != 0)
		bc_counter++;
	else
		bc_counter = 0;
	if(bc_counter > 1)
	{
		printk("======read 0xbc: %x %x %x %x======\n",read_buf[3], read_buf[2], read_buf[1], read_buf[0]);
		init_chip_flag = 1;
		bc_counter = 0;
	}

queue_monitor_init_chip:
	if(init_chip_flag)
      {   
          sprd_i2c_ctl_chg_clk(1,400*1000);
		init_chip(this_client);
           sprd_i2c_ctl_chg_clk(1,100*1000);
       }
	
	i2c_lock_flag = 0;
	
queue_monitor_work:	
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 50);
}
#endif

static irqreturn_t gslX680_ts_interrupt(int irq, void *dev_id)
{

	struct gslX680_ts_data *gslX680_ts = (struct gslX680_ts_data *)dev_id;

	print_info("gslX680_ts_interrupt\n");
	if(this_client->irq > 0)
	{
		disable_irq_nosync(this_client->irq);
	}
	else
	{
		#ifdef I2C_BOARD_INFO_METHOD
		disable_irq_nosync(g_gslX680_ts->platform_data->irq_gpio_number);
		#else
		disable_irq_nosync(gslX680_ts_setup.irq);
		#endif
	}
    	//disable_irq_nosync(this_client->irq);
	if (!work_pending(&gslX680_ts->pen_event_work)) {
		queue_work(gslX680_ts->ts_workqueue, &gslX680_ts->pen_event_work);
	}

	return IRQ_HANDLED;
}

static void gslX680_ts_suspend(struct early_suspend *handler)
{
	print_info("==gslX680_ts_suspend=\n");

#ifdef USE_TP_PSENSOR
    if (ps_en == 1)
    {
		printk("==gslX680_ts_suspend=USE_TP_PSENSOR,  do nothing.\n");
		return;
    }
#endif
#ifdef GSL_MONITOR
	printk( "gsl_ts_suspend () : cancel gsl_monitor_work\n");
	cancel_delayed_work_sync(&gsl_monitor_work);
#endif
	if(this_client->irq > 0)
	{
		disable_irq_nosync(this_client->irq);
	}
	else
	{
		#ifdef I2C_BOARD_INFO_METHOD
		disable_irq_nosync(g_gslX680_ts->platform_data->irq_gpio_number);
		#else
		disable_irq_nosync(gslX680_ts_setup.irq);
		#endif
	}
	#ifdef I2C_BOARD_INFO_METHOD
	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 0);
	#else
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 0);
	#endif
	tp_suspend = 1;
}

static void gslX680_ts_resume(struct early_suspend *handler)
{	
	print_info("==gslX680_ts_resume=\n");
#ifdef USE_TP_PSENSOR
	if (ps_en == 1 && tp_suspend == 0)
	{
		printk("==gslX680_ts_resume=USE_TP_PSENSOR,  do nothing.\n");
		return;
	}
#endif

	#ifdef I2C_BOARD_INFO_METHOD
	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 1);
	#else
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
	#endif
	msleep(30);
	reset_chip(this_client);
	startup_chip(this_client);		
	check_mem_data(this_client);
    
#ifdef USE_TP_PSENSOR
	if (ps_en == 1)
            tp_ps_enable();
#endif

#ifdef GSL_MONITOR
	printk( "gsl_ts_resume () : queue gsl_monitor_work\n");
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 300);
#endif	
	if(this_client->irq > 0)
	{
		enable_irq(this_client->irq);
	}
	else
	{
		#ifdef I2C_BOARD_INFO_METHOD
		enable_irq(g_gslX680_ts->platform_data->irq_gpio_number);
		#else
		enable_irq(gslX680_ts_setup.irq);
		#endif
	}
	tp_suspend = 0;
}
static void gslX680_ts_reset(void)
{
	printk("\n%s\n",__func__);

#ifdef I2C_BOARD_INFO_METHOD
	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 1);
	msleep(1);
	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 0);
	msleep(10);
	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 1);
	msleep(1);
#else
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
	msleep(1);
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 0);
	msleep(10);
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
	msleep(1);
#endif
	printk("\n%s out \n",__func__);
}
static int gslX680_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct gslX680_ts_data *gslX680_ts;
	struct input_dev *input_dev;
	int err = 0;
	unsigned char uc_reg_value=0; 
#ifdef CONFIG_UDC_TP
	u16 i = 0;
    udc_t *fireware;
#endif



#ifdef I2C_BOARD_INFO_METHOD
	struct gslX680_ts_platform_data *pdata = client->dev.platform_data;
#endif

	printk("\n%s\n",__func__);

	if(tp_device_id(0)!=0)
	{
		printk("CTP(0x%x)Exist!", tp_device_id(0));
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
	
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
	printk("invoke test_i2c in probe\n");	
	err = test_i2c(client);
	if(err < 0)
	{
		printk("------gslX680 test_i2c error, no silead chip------\n");	
		goto exit_check_functionality_failed;
	}	
	print_info("==kzalloc=\n");
	gslX680_ts = kzalloc(sizeof(*gslX680_ts), GFP_KERNEL);
	if (!gslX680_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}
#ifdef TOUCH_VIRTUAL_KEYS
	gslX680_ts_virtual_keys_init();
#endif
	this_client = client;
	i2c_set_clientdata(client, gslX680_ts);

	print_info("I2C addr=%x \n", client->addr);
#if 1
	INIT_WORK(&gslX680_ts->pen_event_work, gslX680_ts_pen_irq_work);
#else
//	INIT_WORK(&gslX680_ts->pen_event_work, gslX680_ts_worker);
#endif
	gslX680_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!gslX680_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}
	
	print_info("%s: ==request_irq=\n",__func__);
	print_info("%s IRQ number is %d\n", client->name, client->irq);
#ifdef I2C_BOARD_INFO_METHOD
	gslX680_ts->platform_data = pdata;
	g_gslX680_ts = gslX680_ts;

	gslX680_ts_config_pins(&client->irq);
	gslX680_ts_reset();

	print_info("%s irq_gpio_number %d\n", client->name, g_gslX680_ts->platform_data->irq_gpio_number);
	if(client->irq > 0)
	{
		err = request_irq(client->irq, gslX680_ts_interrupt, IRQF_TRIGGER_RISING, client->name, gslX680_ts);
	}
	else
	{
		err = request_irq(g_gslX680_ts->platform_data->irq_gpio_number, gslX680_ts_interrupt, IRQF_TRIGGER_RISING, client->name, gslX680_ts);
	}
	
	if (err < 0) 
	{
		dev_err(&client->dev, "gslX680_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}
	else
	{
		print_info("gslX680_probe: request irq success\n");
	}
	if(client->irq > 0)
	{
		disable_irq(client->irq);
	}
	else
	{
		disable_irq(g_gslX680_ts->platform_data->irq_gpio_number);
	}	
#else
	gslX680_ts_setup.irq = sprd_alloc_gpio_irq(sprd_3rdparty_gpio_tp_irq);
	if(client->irq > 0)
	{
		err = request_irq(client->irq, gslX680_ts_interrupt, IRQF_TRIGGER_RISING, client->name, gslX680_ts);
	}
	else
	{
		err = request_irq(gslX680_ts_setup.irq, gslX680_ts_interrupt, IRQF_TRIGGER_RISING, client->name, gslX680_ts);
	}			
	if (err < 0) {
		dev_err(&client->dev, "gslX680_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}
	if(client->irq > 0)
	{
		disable_irq(client->irq);
	}
	else
	{
		disable_irq(gslX680_ts_setup.irq);
	}	
#endif
	print_info("==input_allocate_device=\n");
	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
	
	gslX680_ts->input_dev = input_dev;
#ifdef USE_TP_PSENSOR
	set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);
	
	input_set_abs_params(input_dev,	ABS_MT_TRACKING_ID, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, TS_WIDTH_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, TS_HEIGHT_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);
	
	set_bit(EV_ABS, input_dev->evbit);	
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(INPUT_PROP_DIRECT, input_dev->propbit);
	input_mt_init_slots(input_dev, 11,0);
	print_info("gslX680 width=%d height=%d\n", TS_WIDTH_MAX, TS_HEIGHT_MAX);
#else
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(EV_ABS, input_dev->evbit);
	set_bit(EV_SYN, input_dev->evbit);
	set_bit(BTN_TOUCH, input_dev->keybit);
	/*
	__set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	__set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	__set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	__set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);
	*/

	set_bit(KEY_MENU,  input_dev->keybit);
	set_bit(KEY_BACK,  input_dev->keybit);
	set_bit(KEY_HOMEPAGE,  input_dev->keybit);
	set_bit(KEY_SEARCH,  input_dev->keybit);

	print_info("gslX680 width=%d height=%d\n", TS_WIDTH_MAX, TS_HEIGHT_MAX);

	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, TS_WIDTH_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, TS_HEIGHT_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);

#endif

	input_set_drvdata(input_dev, gslX680_ts);

	input_dev->name = GSLX680_NAME;//dev_name(&client->dev);
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"gslX680_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}
#ifdef USE_TP_PSENSOR
	err = tp_ps_init(client);
	if (err) {
		dev_err(&client->dev,
		"gslX680_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_tp_ps_init;
	}
#endif

#ifdef CONFIG_UDC_TP
        //get udc config
        g_tp = udc_tp_create();
    
        g_tp->early_init(g_tp);
        
        g_tp->config.addr = GSLX680_TS_ADDR;
        g_tp->config.device_id = 0;
        err = g_tp->identify(g_tp,SEC_TP);
        if(err <= 0)
            g_tp->config.addr = 0;
        else
        {
            if(g_tp->config.addr == GSLX680_TS_ADDR)
            {
                fireware = udc_tp_get_string(TP_OPEN , &value_count);
            }
        
            print_info("Config Groups\' Lengths: %x, %x, %d\n", 
                    g_tp->config.device_id, g_tp->config.addr, value_count);
            if(value_count > 0)
            {
                memset(&fireware_buf[0], 0, sizeof(fireware_buf));
                value_count = value_count/4;
                if(value_count > UPDATE_BUF_MAX)
                    value_count = UPDATE_BUF_MAX;
                for(i = 0; i < value_count; i++)
                {
                    line_buf[i] = fireware[i*4];
                    fireware_buf[i].offset = (u8)fireware[i*4+1];
                    fireware_buf[i].val = (u32)fireware[i*4+2];
                    fireware_buf[i].val = fireware_buf[i].val << 16;
                    fireware_buf[i].val += (u32)fireware[i*4+3] ;
                }
            }
        }
#endif

	print_info("Invoke init_chip in probe\n");

	init_chip(this_client);
	check_mem_data(this_client);

	print_info("==register_early_suspend =");
	gslX680_ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	gslX680_ts->early_suspend.suspend = gslX680_ts_suspend;
	gslX680_ts->early_suspend.resume	= gslX680_ts_resume;
	register_early_suspend(&gslX680_ts->early_suspend);

#ifdef TPD_PROC_DEBUG


	proc_create(GSL_CONFIG_PROC_FILE,0666,NULL,&gsl_seq_fops);

	gsl_proc_flag = 0;
#endif

#ifdef GSL_MONITOR
	printk( "gsl_ts_probe () : queue gsl_monitor_workqueue\n");

	INIT_DELAYED_WORK(&gsl_monitor_work, gsl_monitor_worker);
	gsl_monitor_workqueue = create_singlethread_workqueue("gsl_monitor_workqueue");
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 1000);
#endif

#ifdef USE_TP_PSENSOR

    err = sysfs_create_group(&(g_gslX680_ts->input_dev->dev.kobj), &gslX680_ts_attr_group);
	if (err) {
		dev_err(&client->dev, "create device file failed!\n");
		err = -EINVAL;
		goto exit_create_sysfs_failed;
	}


#endif

#ifdef I2C_BOARD_INFO_METHOD
	if(client->irq > 0)
	{
		enable_irq(client->irq);
	}
	else
	{
		enable_irq(g_gslX680_ts->platform_data->irq_gpio_number);
	}
#else
   if(client->irq > 0){
        enable_irq(client->irq);
    }else{
        enable_irq(gslX680_ts_setup.irq);
    }
#endif
//	gpio_set_value(g_gslX680_ts->platform_data->reset_gpio_number, 0);
//	msleep(10);
//#ifdef USE_TP_PSENSOR
//	gsl_gain_psensor_data(client);
//#endif
	print_info("%s: ==probe over =\n",__func__);
    return 0;
#ifdef USE_TP_PSENSOR
exit_create_sysfs_failed:
exit_tp_ps_init:
	tp_ps_uninit();
	input_unregister_device(input_dev);
#endif

exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
	free_irq(client->irq, gslX680_ts);
exit_irq_request_failed:
	cancel_work_sync(&gslX680_ts->pen_event_work);
	destroy_workqueue(gslX680_ts->ts_workqueue);
exit_create_singlethread:
	print_info("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(gslX680_ts);
exit_alloc_data_failed:
	remove_ctp_chip();
exit_check_functionality_failed:
	#ifdef I2C_BOARD_INFO_METHOD
	//sprd_free_gpio_irq(g_gslX680_ts->platform_data->irq_gpio_number);
	
    gpio_free(TS_IRQ_PIN);    
    gpio_free(TS_RESET_PIN);
	#else
	sprd_free_gpio_irq(gslX680_ts_setup.irq);
	
    gpio_free(sprd_3rdparty_gpio_tp_irq);    
    gpio_free(sprd_3rdparty_gpio_tp_rst);
	#endif
	return err;
}

static int gslX680_ts_remove(struct i2c_client *client)
{

	struct gslX680_ts_data *gslX680_ts = i2c_get_clientdata(client);

	print_info("==gslX680_ts_remove=\n");
#ifdef GSL_MONITOR
	cancel_delayed_work_sync(&gsl_monitor_work);
	destroy_workqueue(gsl_monitor_workqueue);
#endif
#ifdef USE_TP_PSENSOR
	tp_ps_uninit();
	sysfs_remove_group(&(gslX680_ts->input_dev->dev.kobj), &gslX680_ts_attr_group);
#endif	
	unregister_early_suspend(&gslX680_ts->early_suspend);
	free_irq(client->irq, gslX680_ts);
	#ifdef I2C_BOARD_INFO_METHOD
	//sprd_free_gpio_irq(g_gslX680_ts->platform_data->irq_gpio_number);
	
    gpio_free(TS_IRQ_PIN);    
    gpio_free(TS_RESET_PIN);
	#else
	sprd_free_gpio_irq(gslX680_ts_setup.irq);
    gpio_free(sprd_3rdparty_gpio_tp_irq);    
    gpio_free(sprd_3rdparty_gpio_tp_rst);
	#endif
	input_unregister_device(gslX680_ts->input_dev);
	kfree(gslX680_ts);
	remove_ctp_chip();
	cancel_work_sync(&gslX680_ts->pen_event_work);
	destroy_workqueue(gslX680_ts->ts_workqueue);
	i2c_set_clientdata(client, NULL);

	//LDO_TurnOffLDO(LDO_LDO_SIM2);

	return 0;
}

static const struct i2c_device_id gslX680_ts_id[] = {
	{ GSLX680_NAME, 0 },{ }
};


MODULE_DEVICE_TABLE(i2c, gslX680_ts_id);

static struct i2c_driver gslX680_ts_driver = {
	.probe		= gslX680_ts_probe,
	.remove		= gslX680_ts_remove,
	.id_table	= gslX680_ts_id,
	.driver	= {
		.name	= GSLX680_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init gslX680_ts_init(void)
{
	int ret;
	print_info("==gslX680_ts_init==\n");
	
	if(tp_device_id(0)!=0)
	{
		printk("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	
	ret = i2c_add_driver(&gslX680_ts_driver);
	return ret;
}

static void __exit gslX680_ts_exit(void)
{
	print_info("==gslX680_ts_exit==\n");
	i2c_del_driver(&gslX680_ts_driver);
}

module_init(gslX680_ts_init);
module_exit(gslX680_ts_exit);

MODULE_AUTHOR("leweihua");
MODULE_DESCRIPTION("GSLX680 TouchScreen Driver");
MODULE_LICENSE("GPL");



