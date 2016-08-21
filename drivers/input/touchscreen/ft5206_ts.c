/*
 * drivers/input/touchscreen/ft5x0x_ts.c
 *
 * FocalTech ft5x0x TouchScreen driver.
 *
 * Copyright (c) 2010  Focal tech Ltd.
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
 *    1.0		  2010-01-05			WenFS
 *
 * note: only support mulititouch	Wenfs 2010-10-01
 */

#include <linux/firmware.h>
#include <linux/platform_device.h>
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
#include <linux/i2c/ft5206_ts.h>
#include <mach/regulator.h>
#include <mach/hardware.h>
#include <mach/globalregs.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/clk.h>
#ifdef CONFIG_I2C_SPRD
#include <mach/i2c-sprd.h>
#endif

#include <linux/cdc_com.h>
#if defined(FTS_CTL_IIC)
#include "focaltech_ctl.h"
#endif
#include "focaltech.h"

// Alex.shi 支持的TP列表
// 格式:CONFIG_ZYT_IC_项目_客户_模组厂_序号
#if defined(CONFIG_LCD_QHD)&&defined(CONFIG_MACH_SP7730EC_7685)
	// 7685 QHD调试时用展讯7730EC的TP  
	#define CONFIG_ZYT_FT6206_7685_ZYT_SPRD_0
#else
	// 默认采用TP标准,即:menu(40,980), home(120,980), back(200,980), search(280,980)  
#endif

#define I2C_BOARD_INFO_METHOD   1
#define TS_DATA_THRESHOLD_CHECK	0
#define TS_WIDTH_MAX			539
#define	TS_HEIGHT_MAX			1060
#define TOUCH_VIRTUAL_KEYS
#define CONFIG_FT5X0X_MULTITOUCH 1

#ifdef CONFIG_ZYT_SUPPORT
#define CONFIG_FT5X0X_INIT_NOT_UPDATE	// Alex.shi 不升级固件
#endif

static int debug_level=0;

#define TS_DBG(format, ...)	\
	if(debug_level == 1)	\
		printk(KERN_INFO "FT5X0X " format "\n", ## __VA_ARGS__)

//------------------------------proximity----------------------------------
/*
0xb0
*/
#if (7001 == CONFIG_ZYT_PRJ) && (63 == CONFIG_ZYT_CUST)  //fs001-财富1部	
#define TP_PROXIMITY_SENSOR
#elif (7673 == CONFIG_ZYT_PRJ)  //7673-财富11部	
#define TP_PROXIMITY_SENSOR
#elif (7675 == CONFIG_ZYT_PRJ)
#define TP_PROXIMITY_SENSOR
#elif (7271 == CONFIG_ZYT_PRJ) && ((3 == CONFIG_ZYT_CUST) || (70 == CONFIG_ZYT_CUST) || (73 == CONFIG_ZYT_CUST))
#define TP_PROXIMITY_SENSOR
#elif (7003 == CONFIG_ZYT_PRJ)
#define TP_PROXIMITY_SENSOR
#elif (7011 == CONFIG_ZYT_PRJ)
#define TP_PROXIMITY_SENSOR
#elif (7006 == CONFIG_ZYT_PRJ)
#define TP_PROXIMITY_SENSOR
#elif (7012 == CONFIG_ZYT_PRJ)
#define TP_PROXIMITY_SENSOR
#endif

#ifdef TP_PROXIMITY_SENSOR
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#ifdef CONFIG_UDC_GPIO
#include <linux/udc.h>
#endif

#if (7616 == CONFIG_ZYT_PRJ) && (29 == CONFIG_ZYT_CUST)  //7616-天锐祥  
// 部分区域由单指来模拟接近  
//#define TP_PROXIMITY_FINGER_SIMULATOR
#endif

#ifdef CONFIG_ZYT_SUPPORT
#define PROXIMITY_DEVICE		"gtp_proximity"

//ioctl cmd
#define FT_IOCTL_MAGIC			0X5D
#define FT_IOCTL_PROX_ON		_IO(FT_IOCTL_MAGIC, 7)
#define FT_IOCTL_PROX_OFF		_IO(FT_IOCTL_MAGIC, 8)

#define FT_REGS_PS_CTL			0xB0
#else
#define PROXIMITY_DEVICE		"FT6206_proximity"

//ioctl cmd
#define FT_IOCTL_MAGIC			0xB0
#define FT_IOCTL_PROX_ON		_IO(FT_IOCTL_MAGIC, 1)
#define FT_IOCTL_PROX_OFF		_IO(FT_IOCTL_MAGIC, 2)

#define FT_REGS_PS_CTL			0xB0
#endif

#define TPD_PROXIMITY_DEVICE			"FT6206"
#define TPD_PROXIMITY_DMESG(a,arg...)	printk(TPD_PROXIMITY_DEVICE ": " a,##arg)

//#define TPD_PROXIMITY_DEBUG
#if defined(TPD_PROXIMITY_DEBUG)
//#undef TPD_PROXIMITY_DEBUG
#define TPD_PROXIMITY_DEBUG(a,arg...)	printk(TPD_PROXIMITY_DEVICE ": " a,##arg)
#else
#define TPD_PROXIMITY_DEBUG(arg...)
#endif
static u8 tpd_proximity_flag = 0;
static u8	PROXIMITY_STATE = 0;
static int tpd_halt = 0;

static u8 tpd_proximity_dir_faraway = 0;//0-->close ; 1--> far away
static u8 tpd_proximity_need_report = 0;
#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
static u8 tpd_proximity_got_near = 1;
#else
static u8 tpd_proximity_got_near = 1;
#endif
#endif
//------------------------------proximity----------------------------------
static const char *ft5206_input_name = "SPRD_WW_CTP";//goodix-ts


struct sprd_i2c_setup_data {
	unsigned i2c_bus;  //the same number as i2c->adap.nr in adapter probe function
	unsigned short i2c_address;
	int irq;
	char type[I2C_NAME_SIZE];
};

struct ft5x0x_ts_data *g_ft5x0x_ts;
static struct i2c_client *this_client;
static struct sprd_i2c_setup_data ft5x0x_ts_setup={0, FT5206_TS_ADDR, 0, FT5206_TS_NAME};
/* Attribute */
static unsigned char suspend_flag=0; //0: sleep out; 1: sleep in

static ssize_t ft5x0x_show_suspend(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t ft5x0x_store_suspend(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t ft5x0x_show_version(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t ft5x0x_update(struct device* cd, struct device_attribute *attr, const char* buf, size_t len);
static ssize_t ft5x0x_show_debug(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t ft5x0x_store_debug(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
#ifdef TP_PROXIMITY_SENSOR
static ssize_t ft5x0x_enable_show(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t ft5x0x_enable_store(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
#endif
static unsigned char ft5x0x_read_fw_ver(void);
static void ft5x0x_ts_suspend(struct early_suspend *handler);
static void ft5x0x_ts_resume(struct early_suspend *handler);
//static int fts_ctpm_fw_update(void);
static int fts_ctpm_fw_upgrade_with_i_file(void);
void ft5x0x_ts_reset(void);
struct ts_event {
	u16	x1;
	u16	y1;
	u16	x2;
	u16	y2;
	u16	x3;
	u16	y3;
	u16	x4;
	u16	y4;
	u16	x5;
	u16	y5;
	u16	pressure;
    u8  touch_point;
};

struct ft5x0x_ts_data {
	struct input_dev	*input_dev;
	struct i2c_client	*client;
	struct ts_event	event;
	struct work_struct	pen_event_work;
	struct workqueue_struct	*ts_workqueue;
	struct work_struct	 resume_work;
	struct workqueue_struct	*ts_resume_workqueue;
	struct early_suspend	early_suspend;
	struct ft5x0x_ts_platform_data	*platform_data;
//	struct timer_list touch_timer;
};
 struct Upgrade_Info fts_updateinfo[] =
{
    {0x55,"FT5x06",TPD_MAX_POINTS_5,AUTO_CLB_NEED,50, 30, 0x79, 0x03, 10, 2000},
    {0x08,"FT5606",TPD_MAX_POINTS_5,AUTO_CLB_NEED,50, 10, 0x79, 0x06, 100, 2000},
	{0x0a,"FT5x16",TPD_MAX_POINTS_5,AUTO_CLB_NEED,50, 30, 0x79, 0x07, 10, 1500},
	{0x06,"FT6x06",TPD_MAX_POINTS_2,AUTO_CLB_NONEED,100, 30, 0x79, 0x08, 10, 2000},
	{0x36,"FT6x36",TPD_MAX_POINTS_2,AUTO_CLB_NONEED,10, 10, 0x79, 0x18, 10, 2000},
	{0x55,"FT5x06i",TPD_MAX_POINTS_5,AUTO_CLB_NEED,50, 30, 0x79, 0x03, 10, 2000},
	{0x14,"FT5336",TPD_MAX_POINTS_5,AUTO_CLB_NONEED,30, 30, 0x79, 0x11, 10, 2000},
	{0x13,"FT3316",TPD_MAX_POINTS_5,AUTO_CLB_NONEED,30, 30, 0x79, 0x11, 10, 2000},
	{0x12,"FT5436i",TPD_MAX_POINTS_5,AUTO_CLB_NONEED,30, 30, 0x79, 0x11, 10, 2000},
	{0x11,"FT5336i",TPD_MAX_POINTS_5,AUTO_CLB_NONEED,30, 30, 0x79, 0x11, 10, 2000},
	{0x54,"FT5x46",TPD_MAX_POINTS_5,AUTO_CLB_NONEED,2, 2, 0x54, 0x2c, 10, 2000},
};

struct Upgrade_Info fts_updateinfo_curr;

extern int fts_ctpm_auto_upgrade(struct i2c_client *client);

static DEVICE_ATTR(suspend, S_IRUGO | S_IWUSR, ft5x0x_show_suspend, ft5x0x_store_suspend);
static DEVICE_ATTR(update, S_IRUGO | S_IWUSR, ft5x0x_show_version, ft5x0x_update);
static DEVICE_ATTR(debug, S_IRUGO | S_IWUSR, ft5x0x_show_debug, ft5x0x_store_debug);
#ifdef TP_PROXIMITY_SENSOR
static DEVICE_ATTR(enable, S_IRWXU | S_IRWXG | S_IRWXO,ft5x0x_enable_show, ft5x0x_enable_store);


static struct attribute *ft5x0x_attributes[] = {
	&dev_attr_suspend.attr,
	&dev_attr_update.attr,
	&dev_attr_debug.attr,
	&dev_attr_enable.attr,
	NULL
};

static const struct attribute_group ft5x0x_attr_group = {
	.attrs = ft5x0x_attributes,
};
#endif
static ssize_t ft5x0x_show_debug(struct device* cd,struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;

	sprintf(buf, "FT5206 Debug %d\n",debug_level);

	ret = strlen(buf) + 1;

	return ret;
}

static ssize_t ft5x0x_store_debug(struct device* cd, struct device_attribute *attr,
		       const char* buf, size_t len)
{
	unsigned long on_off = simple_strtoul(buf, NULL, 10);
	debug_level = on_off;

	TS_DBG("%s: debug_level=%d\n",__func__, debug_level);

	return len;
}

static ssize_t ft5x0x_show_suspend(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;

	if(suspend_flag==1)
		sprintf(buf, "FT5206 Suspend\n");
	else
		sprintf(buf, "FT5206 Resume\n");

	ret = strlen(buf) + 1;

	return ret;
}

static ssize_t ft5x0x_store_suspend(struct device* cd, struct device_attribute *attr,
		       const char* buf, size_t len)
{
	unsigned long on_off = simple_strtoul(buf, NULL, 10);
	suspend_flag = on_off;

	if(on_off==1)
	{
		TS_DBG("FT5206 Entry Suspend\n");
		ft5x0x_ts_suspend(NULL);
	}
	else
	{
		TS_DBG("FT5206 Entry Resume\n");
		ft5x0x_ts_resume(NULL);
	}

	return len;
}

static ssize_t ft5x0x_show_version(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;
	unsigned char uc_reg_value; 
	
	//get some register information
	uc_reg_value = ft5x0x_read_fw_ver();

	sprintf(buf, "ft5x0x firmware id is V%x\n", uc_reg_value);

	ret = strlen(buf) + 1;

	return ret;
}

static ssize_t ft5x0x_update(struct device* cd, struct device_attribute *attr,
		       const char* buf, size_t len)
{
	unsigned long on_off = simple_strtoul(buf, NULL, 10);
	unsigned char uc_reg_value;

	//get some register information
	uc_reg_value = ft5x0x_read_fw_ver();

	if(on_off==1)
	{
		TS_DBG("ft5x0x update, current firmware id is V%x\n", uc_reg_value);
		//fts_ctpm_fw_update();
		fts_ctpm_fw_upgrade_with_i_file();
	}

	return len;
}
#ifdef TP_PROXIMITY_SENSOR
static ssize_t ft5x0x_enable_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	int						enable;
	//struct ft5x0x_ts_data	*acc = g_ft5x0x_ts;

	enable = tpd_proximity_flag;
		printk("%s\n", __func__);
	return sprintf(buf, "%d\n", enable);
}
static int FT6206_enable_ps(int enable);

static ssize_t ft5x0x_enable_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct ft5x0x_ts_data	*acc = g_ft5x0x_ts;
	int						ret;
	unsigned long			enable;
		printk("%s\n", __func__);
	enable = simple_strtoul(buf, NULL, 10);    

	enable = (enable > 0) ? 1 : 0;
	if(enable)
	{  
			FT6206_enable_ps(1);   
	}
	else
	{
			FT6206_enable_ps(0);   
	}

	return count;
}
#endif
static int ft5x0x_create_sysfs(struct i2c_client *client)
{
	int err;
	struct device *dev = &(client->dev);

	TS_DBG("%s", __func__);

	err = device_create_file(dev, &dev_attr_suspend);
	err = device_create_file(dev, &dev_attr_update);
	err = device_create_file(dev, &dev_attr_debug);

	return err;
}

static int ft5x0x_remove_sysfs(struct i2c_client *client)
{
	struct device *dev = &(client->dev);

	TS_DBG("%s", __func__);
	
	device_remove_file(dev, &dev_attr_suspend);
	device_remove_file(dev, &dev_attr_update);
	device_remove_file(dev, &dev_attr_debug);
	return 0;
}


#if defined(FTS_CTL_IIC)
//add jinq start
void ft5x0x_reset_tp(int HighOrLow)
{
	printk("set tp reset pin to %d\n", HighOrLow);
//	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	//mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
//	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
 //	msleep(50);  

  //	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
   //	msleep(10);
     //  mt_set_gpio_out(GPIO_CTP_RST_PIN, HighOrLow);
     ft5x0x_ts_reset();
}

/*
*ft5x0x_i2c_Read-read data and write data by i2c
*@client: handle of i2c
*@writebuf: Data that will be written to the slave
*@writelen: How many bytes to write
*@readbuf: Where to store data read from slave
*@readlen: How many bytes to read
*
*Returns negative errno, else the number of messages executed
*
*
*/
int ft5x0x_i2c_Read(struct i2c_client *client, char *writebuf,
		    int writelen, char *readbuf, int readlen)
{
	int ret;

	if (writelen > 0) {
		struct i2c_msg msgs[] = {
			{
			 .addr = client->addr,
			 .flags = 0,
			 .len = writelen,
			 .buf = writebuf,
			 },
			{
			 .addr = client->addr,
			 .flags = I2C_M_RD,
			 .len = readlen,
			 .buf = readbuf,
			 },
		};
		ret = i2c_transfer(client->adapter, msgs, 2);
		if (ret < 0)
			dev_err(&client->dev, "f%s: i2c read error.\n",
				__func__);
	} else {
		struct i2c_msg msgs[] = {
			{
			 .addr = client->addr,
			 .flags = I2C_M_RD,
			 .len = readlen,
			 .buf = readbuf,
			 },
		};
		ret = i2c_transfer(client->adapter, msgs, 1);
		if (ret < 0)
			dev_err(&client->dev, "%s:i2c read error.\n", __func__);
	}
	return ret;
}
/*write data by i2c*/
int ft5x0x_i2c_Write(struct i2c_client *client, char *writebuf, int writelen)
{
	int ret;

	struct i2c_msg msg[] = {
		{
		 .addr = client->addr,
		 .flags = 0,
		 .len = writelen,
		 .buf = writebuf,
		 },
	};

	ret = i2c_transfer(client->adapter, msg, 1);
	if (ret < 0)
		dev_err(&client->dev, "%s i2c write error.\n", __func__);

	return ret;
}


//add jinq end
#endif


static int ft5x0x_i2c_rxdata(char *rxdata, int length)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= rxdata,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= length,
			.buf	= rxdata,
		},
	};

	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);

	return ret;
}

static int ft5x0x_i2c_txdata(char *txdata, int length)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= length,
			.buf	= txdata,
		},
	};

	ret = i2c_transfer(this_client->adapter, msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}
/***********************************************************************************************
Name	:	 ft5x0x_write_reg

Input	:	addr -- address
                     para -- parameter

Output	:

function	:	write register of ft5x0x

***********************************************************************************************/
static int ft5x0x_write_reg(u8 addr, u8 para)
{
	u8 buf[3];
	int ret = -1;

	buf[0] = addr;
	buf[1] = para;
	ret = ft5x0x_i2c_txdata(buf, 2);
	if (ret < 0) {
		pr_err("write reg failed! %#x ret: %d", buf[0], ret);
		return -1;
	}

	return 0;
}


/***********************************************************************************************
Name	:	ft5x0x_read_reg

Input	:	addr
                     pdata

Output	:

function	:	read register of ft5x0x

***********************************************************************************************/
static int ft5x0x_read_reg(u8 addr, u8 *pdata)
{
	int ret;
	u8 buf[2] = {0};

	buf[0] = addr;

	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= buf,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= buf+1,
		},
	};

	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);
	else if (2!=ret) {
		pr_err("msg %s i2c read error: %d\n", __func__, ret);
		ret = -EIO;
	}

	*pdata = buf[1];
	return ret;
}

#ifdef TOUCH_VIRTUAL_KEYS
static ssize_t virtual_keys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	#if defined(CONFIG_ZYT_FT6206_7685_ZYT_SPRD_0)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":100:1020:80:65"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":280:1020:80:65"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":470:1020:80:65"
			"\n");
	#elif defined(CONFIG_MACH_SP6815GA_7271) || defined(CONFIG_MACH_SP6815EA_7271)
	#if (CONFIG_ZYT_CUST==3)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":90:1010:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":270:1010:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":450:1010:60:40"
			"\n");
	#elif (CONFIG_ZYT_CUST==67)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":40:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":120:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:50:30"
			"\n");
	#elif (CONFIG_ZYT_CUST==37)//yingmai -s8
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":160:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":240:530:60:40"
			"\n");
	#elif (CONFIG_ZYT_CUST==18)
				return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"//wanjinta
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
	#elif (CONFIG_ZYT_CUST==73)//youdexun
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":160:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":240:530:60:40"
			"\n");
	#elif (CONFIG_ZYT_CUST==74)//yfnd
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":200:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":40:980:60:40"
			"\n");
	#else
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");

	#endif
	#elif defined(CONFIG_MACH_SP6815GA_7271_WVGA)
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
	#elif defined(CONFIG_MACH_SP6815GA_FS011_WVGA) || defined(CONFIG_MACH_SP6815EA_FS011_WVGA) 
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":180:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":300:900:60:40"
			"\n");
	#elif defined(CONFIG_MACH_SP7715EA_7671)
		#if 0 // 后寄的TP
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":210:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
		#else
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");
		#endif
	#elif defined(CONFIG_MACH_SP7715GA_7672) 
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");
	#elif defined(CONFIG_MACH_SP7715GA_7675) || defined(CONFIG_MACH_SP7715EA_7675)
#if (CONFIG_ZYT_CUST==67)
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
#elif (CONFIG_ZYT_CUST==3)//B930
				return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
#elif (CONFIG_ZYT_CUST==37)
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
#elif (CONFIG_ZYT_CUST==75)
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
#elif (CONFIG_ZYT_CUST==78)
				return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
#else
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");
#endif
	#elif defined(CONFIG_MACH_SP7715GA_7675_HVGA)
	#if (CONFIG_ZYT_CUST==67)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":40:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":120:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:50:30"
			"\n");
	#elif (CONFIG_ZYT_CUST==3)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":90:1010:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":270:1010:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":450:1010:60:40"
			"\n");
	#else
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");
	#endif
	#elif defined(CONFIG_MACH_SP7715EA_7675_HVGA) 
	#if (CONFIG_ZYT_CUST==37)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":160:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":240:530:60:40"
			"\n");
	#elif (CONFIG_ZYT_CUST==18)
				return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"//wanjinta
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
	#elif (CONFIG_ZYT_CUST==67)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":160:530:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":240:530:60:40"
			"\n");
	#elif (CONFIG_ZYT_CUST==3)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":90:1010:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":270:1010:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":450:1010:60:40"
			"\n");
	#else
			return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");
	#endif
	#elif defined(CONFIG_MACH_SP7715GA_7673)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");
	/*#elif defined(CONFIG_MACH_SP7715GA_5701)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":400:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":80:900:60:40"
			"\n");*/
	#elif defined(CONFIG_MACH_SP7715GA_7676)
		#if (CONFIG_ZYT_CUST == 61) // 调试用，后续要求客户用统一配置
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":60:560:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:560:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":180:560:60:40"
			"\n");
		#else
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":120:980:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:60:40"
			"\n");
		#endif
	#elif defined(CONFIG_MACH_SP7715GA_FS003)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:530:60:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":230:530:60:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":270:530:60:30"
			"\n");
	#elif defined(CONFIG_MACH_SP7715GA_FS005)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
	#elif defined(CONFIG_MACH_SP7715EA_FS006)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":60:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":180:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":300:900:60:40"
			"\n");
	#elif defined(CONFIG_MACH_SP7715EA_FS003_HVGA)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":40:530:60:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":230:530:60:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":270:530:60:30"
			"\n");
	#elif (7012 == CONFIG_ZYT_PRJ)
	#if (CONFIG_ZYT_CUST == 76)
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU)   ":80:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE)   ":240:900:60:40"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":400:900:60:40"
			"\n");
	#else
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":40:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":120:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:50:30"
			"\n");
	#endif
	#else
		return sprintf(buf,
			__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":40:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":120:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:50:30"
			":" __stringify(EV_KEY) ":" __stringify(KEY_SEARCH) ":280:980:50:30"
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

static struct kobject *properties_kobj;

static void ft5x0x_ts_virtual_keys_init(void)
{
    int ret;

    TS_DBG("%s\n",__func__);

    properties_kobj = kobject_create_and_add("board_properties", NULL);
    if (properties_kobj)
        ret = sysfs_create_group(properties_kobj,
                     &properties_attr_group);
    if (!properties_kobj || ret)
        pr_err("failed to create board_properties\n");
}

static void ft5x0x_ts_virtual_keys_destroy(void)
{
	if (properties_kobj)
	{
		sysfs_remove_group(properties_kobj, &properties_attr_group);
		kobject_del(properties_kobj);
	}
}
#endif

/***********************************************************************************************
Name	:	 ft5x0x_read_fw_ver

Input	:	 void

Output	:	 firmware version

function	:	 read TP firmware version

***********************************************************************************************/
static unsigned char ft5x0x_read_fw_ver(void)
{
	unsigned char ver;
	ft5x0x_read_reg(FT5X0X_REG_FIRMID, &ver);
	return(ver);
}

#ifdef TP_PROXIMITY_SENSOR
static int FT6206_get_ps_value(void)
{
	TPD_PROXIMITY_DEBUG("FT6206_get_ps_value %d!\n", tpd_proximity_dir_faraway);
	return tpd_proximity_dir_faraway;
}

static int FT6206_enable_ps(int enable)
{
	TPD_PROXIMITY_DEBUG("FT6206_enable_ps %d!\n", enable);
	if(enable==1)
	{
		tpd_proximity_flag =1;
		tpd_proximity_need_report = 1;
		ft5x0x_write_reg(FT_REGS_PS_CTL, 0x1);
	}
	else
	{	
		tpd_proximity_flag =0;
		tpd_proximity_need_report = 0;	
		ft5x0x_write_reg(FT_REGS_PS_CTL, 0x0);
	}

    return 1;
}

static int FT6206_ioctl_operate(struct i2c_client *client, unsigned int cmd, void *arg)
{
	int err = 0;
	int value;

	TPD_PROXIMITY_DEBUG("FT6206_ioctl_operate %d!\n", cmd);

	switch(cmd)
	{
		case FT_IOCTL_PROX_ON:
			FT6206_enable_ps(1);   
			break;

		case FT_IOCTL_PROX_OFF:
			FT6206_enable_ps(0);
			break;

		default:
			pr_err("%s: invalid cmd %d\n", __func__, _IOC_NR(cmd));
			return -EINVAL;
	}
	return 0;

}
static struct file_operations FT6206_proximity_fops = {
	.owner = THIS_MODULE,
	.open = NULL,
	.release = NULL,
	.unlocked_ioctl = FT6206_ioctl_operate
};

struct miscdevice FT6206_proximity_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = PROXIMITY_DEVICE,		//match the hal's name 
	.fops = &FT6206_proximity_fops,
};

#endif


#define CONFIG_SUPPORT_FTS_CTP_UPG


#ifdef CONFIG_SUPPORT_FTS_CTP_UPG

typedef enum
{
    ERR_OK,
    ERR_MODE,
    ERR_READID,
    ERR_ERASE,
    ERR_STATUS,
    ERR_ECC,
    ERR_DL_ERASE_FAIL,
    ERR_DL_PROGRAM_FAIL,
    ERR_DL_VERIFY_FAIL
}E_UPGRADE_ERR_TYPE;

typedef unsigned char         FTS_BYTE;     //8 bit
typedef unsigned short        FTS_WORD;    //16 bit
typedef unsigned int          FTS_DWRD;    //16 bit
typedef unsigned char         FTS_BOOL;    //8 bit

#define FTS_NULL                0x0
#define FTS_TRUE                0x01
#define FTS_FALSE              0x0

#define I2C_CTPM_ADDRESS       0x70

/*
[function]:
    callback: read data from ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[out]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/
FTS_BOOL i2c_read_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    int ret;

    ret=i2c_master_recv(this_client, pbt_buf, dw_lenth);

    if(ret<=0)
    {
        printk("[TSP]i2c_read_interface error\n");
        return FTS_FALSE;
    }

    return FTS_TRUE;
}

/*
[function]:
    callback: write data to ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[in]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/
FTS_BOOL i2c_write_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    int ret;
    ret=i2c_master_send(this_client, pbt_buf, dw_lenth);
    if(ret<=0)
    {
        printk("[TSP]i2c_write_interface error line = %d, ret = %d\n", __LINE__, ret);
        return FTS_FALSE;
    }

    return FTS_TRUE;
}

/*
[function]:
    send a command to ctpm.
[parameters]:
    btcmd[in]        :command code;
    btPara1[in]    :parameter 1;
    btPara2[in]    :parameter 2;
    btPara3[in]    :parameter 3;
    num[in]        :the valid input parameter numbers, if only command code needed and no parameters followed,then the num is 1;
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL cmd_write(FTS_BYTE btcmd,FTS_BYTE btPara1,FTS_BYTE btPara2,FTS_BYTE btPara3,FTS_BYTE num)
{
    FTS_BYTE write_cmd[4] = {0};

    write_cmd[0] = btcmd;
    write_cmd[1] = btPara1;
    write_cmd[2] = btPara2;
    write_cmd[3] = btPara3;
    return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, num);
}

/*
[function]:
    write data to ctpm , the destination address is 0.
[parameters]:
    pbt_buf[in]    :point to data buffer;
    bt_len[in]        :the data numbers;
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL byte_write(FTS_BYTE* pbt_buf, FTS_DWRD dw_len)
{
    return i2c_write_interface(I2C_CTPM_ADDRESS, pbt_buf, dw_len);
}

/*
[function]:
    read out data from ctpm,the destination address is 0.
[parameters]:
    pbt_buf[out]    :point to data buffer;
    bt_len[in]        :the data numbers;
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL byte_read(FTS_BYTE* pbt_buf, FTS_BYTE bt_len)
{
    return i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len);
}


/*
[function]:
    burn the FW to ctpm.
[parameters]:(ref. SPEC)
    pbt_buf[in]    :point to Head+FW ;
    dw_lenth[in]:the length of the FW + 6(the Head length);
    bt_ecc[in]    :the ECC of the FW
[return]:
    ERR_OK        :no error;
    ERR_MODE    :fail to switch to UPDATE mode;
    ERR_READID    :read id fail;
    ERR_ERASE    :erase chip fail;
    ERR_STATUS    :status error;
    ERR_ECC        :ecc error.
*/


#define    FTS_PACKET_LENGTH        128

static unsigned char FT5316_FW[]=
{
#include "ft5316_qHD.i"
};

static unsigned char FT5306_FW[]=
{
#include "ft5306_qHD.i"
};

static unsigned char *CTPM_FW = FT5306_FW;
static int fm_size;

E_UPGRADE_ERR_TYPE  ft5x0xs_ctpm_fw_upgrade(FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    FTS_BYTE reg_val[2] = {0};
    FTS_DWRD i = 0;

    FTS_DWRD  packet_number;
    FTS_DWRD  j;
    FTS_DWRD  temp;
    FTS_DWRD  lenght;
    FTS_BYTE  packet_buf[FTS_PACKET_LENGTH + 6];
    FTS_BYTE  auc_i2c_write_buf[10];
    FTS_BYTE bt_ecc;
    int      i_ret;

    /*********Step 1:Reset  CTPM *****/
    /*write 0xaa to register 0xfc*/
    ft5x0x_write_reg(0xfc,0xaa);
    msleep(50);
     /*write 0x55 to register 0xfc*/
    ft5x0x_write_reg(0xfc,0x55);
    TS_DBG("[TSP] Step 1: Reset CTPM test, bin-length=%d\n",dw_lenth);

    msleep(100);

    /*********Step 2:Enter upgrade mode *****/
    do
    {
        i ++;
        auc_i2c_write_buf[0] = 0x55;
	i_ret = ft5x0x_i2c_txdata(auc_i2c_write_buf, 1);
        i_ret |= auc_i2c_write_buf[0] = 0xaa;
        ft5x0x_i2c_txdata(auc_i2c_write_buf, 1);
        msleep(5);
    }while(i_ret <= 0 && i < 5 );
    /*********Step 3:check READ-ID***********************/
    cmd_write(0x90,0x00,0x00,0x00,4);
    byte_read(reg_val,2);
    if (reg_val[0] == 0x79 && (reg_val[1] == 0x07 || reg_val[1] == 0x03))
    {
        printk("[TSP] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
    }
    else
    {
        printk("%s: ERR_READID, ID1 = 0x%x,ID2 = 0x%x\n", __func__,reg_val[0],reg_val[1]);
        return ERR_READID;
        //i_is_new_protocol = 1;
    }

     /*********Step 4:erase app*******************************/
    cmd_write(0x61,0x00,0x00,0x00,1);

    msleep(1500);
    TS_DBG("[TSP] Step 4: erase.\n");

    /*********Step 5:write firmware(FW) to ctpm flash*********/
    bt_ecc = 0;
    TS_DBG("[TSP] Step 5: start upgrade.\n");
    dw_lenth = dw_lenth - 8;
    packet_number = (dw_lenth) / FTS_PACKET_LENGTH;
    packet_buf[0] = 0xbf;
    packet_buf[1] = 0x00;
    for (j=0;j<packet_number;j++)
    {
        temp = j * FTS_PACKET_LENGTH;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;
        lenght = FTS_PACKET_LENGTH;
        packet_buf[4] = (FTS_BYTE)(lenght>>8);
        packet_buf[5] = (FTS_BYTE)lenght;

        for (i=0;i<FTS_PACKET_LENGTH;i++)
        {
            packet_buf[6+i] = pbt_buf[j*FTS_PACKET_LENGTH + i];
            bt_ecc ^= packet_buf[6+i];
        }

        byte_write(&packet_buf[0],FTS_PACKET_LENGTH + 6);
        msleep(FTS_PACKET_LENGTH/6 + 1);
        if ((j * FTS_PACKET_LENGTH % 1024) == 0)
        {
              printk("[TSP] upgrade the 0x%x th byte.\n", ((unsigned int)j) * FTS_PACKET_LENGTH);
        }
    }

    if ((dw_lenth) % FTS_PACKET_LENGTH > 0)
    {
        temp = packet_number * FTS_PACKET_LENGTH;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;

        temp = (dw_lenth) % FTS_PACKET_LENGTH;
        packet_buf[4] = (FTS_BYTE)(temp>>8);
        packet_buf[5] = (FTS_BYTE)temp;

        for (i=0;i<temp;i++)
        {
            packet_buf[6+i] = pbt_buf[ packet_number*FTS_PACKET_LENGTH + i]; 
            bt_ecc ^= packet_buf[6+i];
        }

        byte_write(&packet_buf[0],temp+6);
        msleep(20);
    }

    //send the last six byte
    for (i = 0; i<6; i++)
    {
        temp = 0x6ffa + i;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;
        temp =1;
        packet_buf[4] = (FTS_BYTE)(temp>>8);
        packet_buf[5] = (FTS_BYTE)temp;
        packet_buf[6] = pbt_buf[ dw_lenth + i];
        bt_ecc ^= packet_buf[6];

        byte_write(&packet_buf[0],7);
        msleep(20);
    }

    /*********Step 6: read out checksum***********************/
    /*send the opration head*/
    cmd_write(0xcc,0x00,0x00,0x00,1);
    byte_read(reg_val,1);
    TS_DBG("[TSP] Step 6:  ecc read 0x%x, new firmware 0x%x. \n", reg_val[0], bt_ecc);
    if(reg_val[0] != bt_ecc)
    {
        printk("%s: ERR_ECC\n", __func__);
        return ERR_ECC;
    }

    /*********Step 7: reset the new FW***********************/
    cmd_write(0x07,0x00,0x00,0x00,1);

    return ERR_OK;
}

int ft5x0xs_ctpm_auto_clb(void)
{
    unsigned char uc_temp;
    unsigned char i ;

    TS_DBG("[FTS] start auto CLB.\n");
    msleep(200);
    ft5x0x_write_reg(0, 0x40);
    msleep(100);   //make sure already enter factory mode
    ft5x0x_write_reg(2, 0x4);  //write command to start calibration
    msleep(300);
    for(i=0;i<100;i++)
    {
        ft5x0x_read_reg(0,&uc_temp);
        if ( ((uc_temp&0x70)>>4) == 0x0)  //return to normal mode, calibration finish
        {
            break;
        }
        msleep(200);
        TS_DBG("[FTS] waiting calibration %d\n",i);
    }
    TS_DBG("[FTS] calibration OK.\n");

    msleep(300);
    ft5x0x_write_reg(0, 0x40);  //goto factory mode
    msleep(100);   //make sure already enter factory mode
    ft5x0x_write_reg(2, 0x5);  //store CLB result
    msleep(300);
    ft5x0x_write_reg(0, 0x0); //return to normal mode
    msleep(300);
    TS_DBG("[FTS] store CLB result OK.\n");
    return 0;
}

#if 1
int fts_ctpm_fw_upgrade_with_i_file(void)
{
   FTS_BYTE*     pbt_buf = FTS_NULL;
   int i_ret;

    //=========FW upgrade========================*/
   pbt_buf = CTPM_FW;
   /*call the upgrade function*/
   i_ret =  ft5x0xs_ctpm_fw_upgrade(pbt_buf,fm_size);
   if (i_ret != 0)
   {
	printk("[FTS] upgrade failed i_ret = %d.\n", i_ret);
       //error handling ...
       //TBD
   }
   else
   {
       printk("[FTS] upgrade successfully.\n");
       ft5x0xs_ctpm_auto_clb();  //start auto CLB
   }

   return i_ret;
}
#endif

#if 0
static int fts_ctpm_fw_update(void)
{
    int ret = 0;
    const struct firmware *fw;
    unsigned char *fw_buf;
    struct platform_device *pdev;

    pdev = platform_device_register_simple("ft5206_ts", 0, NULL, 0);
    if (IS_ERR(pdev)) {
        printk("%s: Failed to register firmware\n", __func__);
        return -1;
    }

    ret = request_firmware(&fw, "ft5306_fw.bin", &pdev->dev);
    if (ret) {
		printk("%s: request_firmware error\n",__func__);
		platform_device_unregister(pdev);
        return -1;
    }

    platform_device_unregister(pdev);
    printk("%s:fw size=%d\n", __func__,fw->size);

    fw_buf = kzalloc(fw->size, GFP_KERNEL | GFP_DMA);
    memcpy(fw_buf, fw->data, fw->size);

    fts_ctpm_fw_upgrade(fw_buf, fw->size);

    printk("%s: update finish\n", __func__);
    release_firmware(fw);
    kfree(fw_buf);

    return 0;
}
#endif

#endif

static void ft5x0x_ts_release(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
#ifdef CONFIG_FT5X0X_MULTITOUCH
	//input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
	input_report_key(data->input_dev, BTN_TOUCH, 0);
#else
	input_report_abs(data->input_dev, ABS_PRESSURE, 0);
	input_report_key(data->input_dev, BTN_TOUCH, 0);
#endif

#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
	if ((tpd_proximity_flag) && (1 == tpd_proximity_got_near)) {
		TPD_PROXIMITY_DEBUG("tpd_proximity:faraway\n");
		tpd_proximity_got_near = 0;
		input_report_abs(data->input_dev, ABS_DISTANCE, 1);
		input_mt_sync(data->input_dev);
	}
#endif

	input_sync(data->input_dev);
	TS_DBG("%s",__func__);
}

void focaltech_get_upgrade_array(struct i2c_client *client)
{

	u8 chip_id;
	u32 i;

	i2c_smbus_read_i2c_block_data(client,FT_REG_CHIP_ID,1,&chip_id);

	printk("%s chip_id = %x\n", __func__, chip_id);

	for(i=0;i<sizeof(fts_updateinfo)/sizeof(struct Upgrade_Info);i++)
	{
		if(chip_id==fts_updateinfo[i].CHIP_ID)
		{
			memcpy(&fts_updateinfo_curr, &fts_updateinfo[i], sizeof(struct Upgrade_Info));
			break;
		}
	}

	if(i >= sizeof(fts_updateinfo)/sizeof(struct Upgrade_Info))
	{
		memcpy(&fts_updateinfo_curr, &fts_updateinfo[0], sizeof(struct Upgrade_Info));
	}
}

static int ft5x0x_read_data(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
//	u8 buf[14] = {0};
	u8 buf[32] = {0};
	int ret = -1;

#ifdef CONFIG_FT5X0X_MULTITOUCH
//	ret = ft5x0x_i2c_rxdata(buf, 13);
	ret = ft5x0x_i2c_rxdata(buf, 31);
#else
    ret = ft5x0x_i2c_rxdata(buf, 7);
#endif
    if (ret < 0) {
		printk("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
		return ret;
	}

	memset(event, 0, sizeof(struct ts_event));
//	event->touch_point = buf[2] & 0x03;// 0000 0011
	event->touch_point = buf[2] & 0x07;// 000 0111

#if defined(TP_PROXIMITY_SENSOR) && !defined(TP_PROXIMITY_FINGER_SIMULATOR)
	{
		u8 proximity_flag=0;
		//  0x01-->C0 是靠近  E0 是远离
		ft5x0x_read_reg(0x01, &proximity_flag);
		TPD_PROXIMITY_DEBUG("proximity_flag =0x%x tpd_proximity_got_near %d\n", proximity_flag, tpd_proximity_got_near);
		if (0xE0 == proximity_flag)// leave
		{
			if(tpd_proximity_got_near)
			{
				tpd_proximity_dir_faraway = 1;
				TPD_PROXIMITY_DEBUG("1111111111111111111111111111111111111\n");
				return 0;
			}
			else
			{
				TPD_PROXIMITY_DEBUG("11111111 dirty faraway received\n");
			}
		}
		else if (0xC0 == proximity_flag)// close to
		{
			tpd_proximity_dir_faraway = 0;
			tpd_proximity_got_near = 1;
			TPD_PROXIMITY_DEBUG("2222222222222222222222222222222222222\n");
			return 0;
		}
	}
#endif

    if (event->touch_point == 0) {
        ft5x0x_ts_release();
        return 1;
    }

#ifdef CONFIG_FT5X0X_MULTITOUCH
    switch (event->touch_point) {
		case 5:
			event->x5 = (s16)(buf[0x1b] & 0x0F)<<8 | (s16)buf[0x1c];
			event->y5 = (s16)(buf[0x1d] & 0x0F)<<8 | (s16)buf[0x1e];
		#if defined(CONFIG_MACH_SP6825GA) || defined(CONFIG_MACH_SP6825GB) || defined(CONFIG_MACH_SP8825GB) || defined(CONFIG_MACH_SP8825GA)
			event->x5 = event->x5*8/9;
			event->y5 = event->y5*854/960;
		#endif
			TS_DBG("===x5 = %d,y5 = %d ====",event->x5,event->y5);
		case 4:
			event->x4 = (s16)(buf[0x15] & 0x0F)<<8 | (s16)buf[0x16];
			event->y4 = (s16)(buf[0x17] & 0x0F)<<8 | (s16)buf[0x18];
		#if defined(CONFIG_MACH_SP6825GA) || defined(CONFIG_MACH_SP6825GB) || defined(CONFIG_MACH_SP8825GB) || defined(CONFIG_MACH_SP8825GA)
			event->x4 = event->x4*8/9;
			event->y4 = event->y4*854/960;
		#endif
			TS_DBG("===x4 = %d,y4 = %d ====",event->x4,event->y4);
		case 3:
			event->x3 = (s16)(buf[0x0f] & 0x0F)<<8 | (s16)buf[0x10];
			event->y3 = (s16)(buf[0x11] & 0x0F)<<8 | (s16)buf[0x12];
		#if defined(CONFIG_MACH_SP6825GA) || defined(CONFIG_MACH_SP6825GB) || defined(CONFIG_MACH_SP8825GB) || defined(CONFIG_MACH_SP8825GA)
			event->x3 = event->x3*8/9;
			event->y3 = event->y3*854/960;
		#endif
			TS_DBG("===x3 = %d,y3 = %d ====",event->x3,event->y3);
		case 2:
			event->x2 = (s16)(buf[9] & 0x0F)<<8 | (s16)buf[10];
			event->y2 = (s16)(buf[11] & 0x0F)<<8 | (s16)buf[12];
		#if defined(CONFIG_MACH_SP6825GA) || defined(CONFIG_MACH_SP6825GB) || defined(CONFIG_MACH_SP8825GB) || defined(CONFIG_MACH_SP8825GA)
			event->x2 = event->x2*8/9;
			event->y2 = event->y2*854/960;
		#endif
			TS_DBG("===x2 = %d,y2 = %d ====",event->x2,event->y2);
		case 1:
			event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
			event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
		#if defined(CONFIG_MACH_SP6825GA) || defined(CONFIG_MACH_SP6825GB) || defined(CONFIG_MACH_SP8825GB) || defined(CONFIG_MACH_SP8825GA)
			event->x1 = event->x1*8/9;
			event->y1 = event->y1*854/960;
		#endif
			TS_DBG("===x1 = %d,y1 = %d ====",event->x1,event->y1);
            break;
		default:
		    return -1;
	}
#else
    if (event->touch_point == 1) {
        event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
        event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
    }
#endif
    event->pressure = 200;

	//printk("%d (%d, %d), (%d, %d)\n", event->touch_point, event->x1, event->y1, event->x2, event->y2);
#if defined(CONFIG_MACH_SP7715EA_7671)
	#if 0  // 后寄的TP
	event->x1 = 480 - event->x1;
	#endif
#endif

#if TS_DATA_THRESHOLD_CHECK
	#ifdef CONFIG_FT5X0X_MULTITOUCH
		if((event->x1>TS_WIDTH_MAX || event->y1>TS_HEIGHT_MAX)||
		   (event->x2>TS_WIDTH_MAX || event->y2>TS_HEIGHT_MAX)||
		   (event->x3>TS_WIDTH_MAX || event->y3>TS_HEIGHT_MAX)||
		   (event->x4>TS_WIDTH_MAX || event->y4>TS_HEIGHT_MAX)||
		   (event->x5>TS_WIDTH_MAX || event->y5>TS_HEIGHT_MAX)) {
				//printk("%s: get dirty data x1=%d,y1=%d\n",__func__, event->x1, event->y1);
				return -1;
		}
	#else
		if(event->x1>TS_WIDTH_MAX || event->y1>TS_HEIGHT_MAX){
				//printk("%s: get dirty data x1=%d,y1=%d\n",__func__, event->x1, event->y1);
				return -1;
		}
	#endif
#endif

	return 0;
}

static void ft5x0x_report_value(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
	u8		tp_ps_close=1;
#endif
	
#ifdef TP_PROXIMITY_SENSOR
	TPD_PROXIMITY_DEBUG("report tpd_proximity_need_report %d!\n", tpd_proximity_need_report);
#endif

//	printk("==ft5x0x_report_value =\n");
#if CTP_NEGATIVE_Y
	event->y1 = (SCREEN_MAX_Y-event->y1 >= 0) ? (SCREEN_MAX_Y-event->y1): event->y1;
	event->y2 = (SCREEN_MAX_Y-event->y2 >= 0) ? (SCREEN_MAX_Y-event->y2): event->y2;
	event->y3 = (SCREEN_MAX_Y-event->y3 >= 0) ? (SCREEN_MAX_Y-event->y3): event->y3;
	event->y4 = (SCREEN_MAX_Y-event->y4 >= 0) ? (SCREEN_MAX_Y-event->y4): event->y4;
	event->y5 = (SCREEN_MAX_Y-event->y5 >= 0) ? (SCREEN_MAX_Y-event->y5): event->y5;
#endif

	if(event->touch_point)
		input_report_key(data->input_dev, BTN_TOUCH, 1);
#ifdef CONFIG_FT5X0X_MULTITOUCH
	switch(event->touch_point) {
		case 5:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x5);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y5);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
	#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
			if (tpd_proximity_flag) {
				if ( (80 > event->y5) || (event->y5 > 500))
					tp_ps_close = 0;
			}
	#endif
			
		case 4:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x4);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y4);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
	#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
			if (tpd_proximity_flag) {
				if ( (80 > event->y4) || (event->y4 > 500))
					tp_ps_close = 0;
			}
	#endif
			
		case 3:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x3);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y3);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
	#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
			if (tpd_proximity_flag) {
				if ( (80 > event->y3) || (event->y3 > 500))
					tp_ps_close = 0;
			}
	#endif
			
		case 2:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x2);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y2);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
	#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
			if (tpd_proximity_flag) {
				if ( (80 > event->y2) || (event->y2 > 500))
					tp_ps_close = 0;
			}
	#endif
			
		case 1:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x1);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y1);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
	#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
			if (tpd_proximity_flag) {
				if ( (80 > event->y1) || (event->y1 > 500))
					tp_ps_close = 0;
			}
	#endif

		default:
//			printk("==touch_point default =\n");
			break;
	}
	
	#if defined(TP_PROXIMITY_SENSOR) && !defined(TP_PROXIMITY_FINGER_SIMULATOR)
	if (tpd_proximity_need_report == 1)
	{
		//get raw data
		TPD_PROXIMITY_DEBUG("tpd_proximity_dir_faraway=%d\n", tpd_proximity_dir_faraway);

		if(tpd_proximity_dir_faraway)
		{
			tpd_proximity_got_near = 0;
		}
		
		//let up layer to know
		input_report_abs(data->input_dev, ABS_DISTANCE, tpd_proximity_dir_faraway);
		input_mt_sync(data->input_dev);
	}
	#endif
#else	/* CONFIG_FT5X0X_MULTITOUCH*/
	if (event->touch_point == 1) {
		input_report_abs(data->input_dev, ABS_X, event->x1);
		input_report_abs(data->input_dev, ABS_Y, event->y1);
		input_report_abs(data->input_dev, ABS_PRESSURE, event->pressure);
	}
	input_report_key(data->input_dev, BTN_TOUCH, 1);
	#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
	if (tpd_proximity_flag) {
		if ( (80 > event->y1) || (event->y1 > 500))
			tp_ps_close = 0;
	}
	#endif
#endif	/* CONFIG_FT5X0X_MULTITOUCH*/

#if defined(TP_PROXIMITY_FINGER_SIMULATOR)
	if ((tpd_proximity_flag) && (1 == tp_ps_close)) {
		TPD_PROXIMITY_DEBUG("tpd_proximity:close\n");
		tpd_proximity_got_near = 1;
		input_report_abs(data->input_dev, ABS_DISTANCE, 0);
		input_mt_sync(data->input_dev);
	}
#endif

	input_sync(data->input_dev);
}	/*end ft5x0x_report_value*/

static void ft5x0x_ts_pen_irq_work(struct work_struct *work)
{

	int ret = -1;

	ret = ft5x0x_read_data();
	if (ret == 0) {
		ft5x0x_report_value();
	}

	enable_irq(this_client->irq);
}

static irqreturn_t ft5x0x_ts_interrupt(int irq, void *dev_id)
{

	//struct ft5x0x_ts_data *ft5x0x_ts = (struct ft5x0x_ts_data *)dev_id;
	int ret = -1;

#if 0
	if (!work_pending(&ft5x0x_ts->pen_event_work)) {
		queue_work(ft5x0x_ts->ts_workqueue, &ft5x0x_ts->pen_event_work);
	}

#endif
	ret = ft5x0x_read_data();
	if (ret == 0) {
		ft5x0x_report_value();
	}

	return IRQ_HANDLED;
}

#if 0
void ft5x0x_tpd_polling(void)
{
    struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);

   // if (!work_pending(&data->pen_event_work)) {
    	queue_work(data->ts_workqueue, &data->pen_event_work);
    //}
	data->touch_timer.expires = jiffies + 2;
	add_timer(&data->touch_timer);
}
#endif

/*******************************************************************************
* Function	  :  ft5x0x_ts_config_pins
* Description :  config gpio pins and set relative TS pins
* Parameters  :  void
* Return	  :    void
*******************************************************************************/
static int ft5x0x_ts_config_pins(void)
{
	unsigned gpio_irq=0, gpio_rst=0;
	int ft5x0x_irq;
#ifdef I2C_BOARD_INFO_METHOD
	struct ft5x0x_ts_platform_data	*pdata = g_ft5x0x_ts->platform_data;

	TS_DBG("%s [irq=%d];[rst=%d]\n", __func__, pdata->irq_gpio_number, pdata->reset_gpio_number);
	gpio_irq = pdata->irq_gpio_number;
	gpio_rst = pdata->reset_gpio_number;
#else
	gpio_irq = GPIO_TOUCH_IRQ;
	gpio_rst = GPIO_TOUCH_RESET;
#endif

	
	gpio_request(gpio_irq, "ts_irq");
	gpio_request(gpio_rst, "ts_rst");
	gpio_direction_output(gpio_rst, 0);
	msleep(10);
	gpio_direction_input(gpio_irq);
	ft5x0x_irq=gpio_to_irq(gpio_irq);

	msleep(10); //wait for stable

	return ft5x0x_irq;
}

void ft5x0x_ts_reset(void)
{
	struct ft5x0x_ts_platform_data *pdata = g_ft5x0x_ts->platform_data;

	gpio_direction_output(pdata->reset_gpio_number, 1);
	msleep(1);
	gpio_set_value(pdata->reset_gpio_number, 0);
	msleep(10);
	gpio_set_value(pdata->reset_gpio_number, 1);
	msleep(200);
}

static void ft5x0x_ts_suspend(struct early_suspend *handler)
{
	TS_DBG("==ft5x0x_ts_suspend==\n");
#ifdef TP_PROXIMITY_SENSOR
	TPD_PROXIMITY_DEBUG("ft5x0x_ts_suspend: tpd_proximity_flag=%d, tpd_halt=%d\n", tpd_proximity_flag, tpd_halt);
	if(tpd_proximity_flag == 1)
	{
		return;
	}

	tpd_halt = 1;
#endif

	disable_irq_nosync(this_client->irq);
	ft5x0x_write_reg(FT5X0X_REG_PMODE, PMODE_HIBERNATE);
}

static void ft5x0x_ts_resume(struct early_suspend *handler)
{
	TS_DBG("==ft5x0x_ts_resume==\n");
#ifdef TP_PROXIMITY_SENSOR
	TPD_PROXIMITY_DEBUG("ft5x0x_ts_resume: tpd_proximity_flag=%d, tpd_halt=%d\n", tpd_proximity_flag, tpd_halt);
	if ((1 == tpd_proximity_flag) && (0 == tpd_halt))
	{
		return;
	}
#endif

#if 0
	printk("==%s==\n", __FUNCTION__);
	ft5x0x_ts_reset();
	ft5x0x_write_reg(FT5X0X_REG_PERIODACTIVE, 7);//about 70HZ
	enable_irq(this_client->irq);
#endif
	struct ft5x0x_ts_data  *ft5x0x_ts = (struct ft5x0x_ts_data *)i2c_get_clientdata(this_client);
	queue_work(ft5x0x_ts->ts_resume_workqueue, &ft5x0x_ts->resume_work);
}

static void ft5x0x_ts_resume_work(struct work_struct *work)
{
	TS_DBG("==%s==\n", __FUNCTION__);
	ft5x0x_ts_reset();
	ft5x0x_write_reg(FT5X0X_REG_PERIODACTIVE, 7);//about 70HZ
	enable_irq(this_client->irq);
#ifdef TP_PROXIMITY_SENSOR
	msleep(100);
	tpd_halt = 0;

	TPD_PROXIMITY_DEBUG("ft5x0x_ts_resume_work: tpd_proximity_flag=%d\n", tpd_proximity_flag);
	if(tpd_proximity_flag == 1)
	{
		FT6206_enable_ps(1);	
	}
#endif
}

static int ft5x0x_ts_hw_init(struct ft5x0x_ts_data *ft5x0x_ts)
{
	int ft5x0x_irq;
	struct regulator *reg_vdd;
	struct i2c_client *client = ft5x0x_ts->client;
	struct ft5x0x_ts_platform_data *pdata = ft5x0x_ts->platform_data;

	TS_DBG("%s [irq=%d];[rst=%d]\n",__func__,pdata->irq_gpio_number,pdata->reset_gpio_number);
	gpio_request(pdata->irq_gpio_number, "ts_irq_pin");
	gpio_request(pdata->reset_gpio_number, "ts_rst_pin");
	gpio_direction_output(pdata->reset_gpio_number, 1);
	msleep(300);
	gpio_direction_input(pdata->irq_gpio_number);
	//vdd power on
	reg_vdd = regulator_get(&client->dev, pdata->vdd_name);
#if defined(CONFIG_MACH_SP7715GA_7676)
	regulator_set_voltage(reg_vdd, 2800000, 2800000);
#else
	regulator_set_voltage(reg_vdd, 1800000, 1800000);
#endif
	regulator_enable(reg_vdd);
	ft5x0x_irq = gpio_to_irq(pdata->irq_gpio_number);
	msleep(100);
	//reset
	ft5x0x_ts_reset();

	return ft5x0x_irq;
}

static int check_ctp_chip(void)
{
	ctp_lock_mutex();
	tp_device_id(0x5206);
	ctp_unlock_mutex();
	return 0;
}

static int remove_ctp_chip(void)
{
	ctp_lock_mutex();
	tp_device_id(0xFFFF);
	ctp_unlock_mutex();
	return 0;
}

static int ft5x0x_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ft5x0x_ts_data *ft5x0x_ts;
	struct input_dev *input_dev;
	struct ft5x0x_ts_platform_data *pdata = client->dev.platform_data;
	int err = 0;
	unsigned char uc_reg_value;

	TS_DBG("%s: probe\n",__func__);

	if(tp_device_id(0)!=0)
	{
		printk("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		printk(KERN_ERR "%s: i2c_check_functionality failed\n",__func__);
		goto exit_check_functionality_failed;
	}

	ft5x0x_ts = kzalloc(sizeof(*ft5x0x_ts), GFP_KERNEL);
	if (!ft5x0x_ts)	{
		err = -ENOMEM;
		printk(KERN_ERR "%s: kzalloc failed\n",__func__);
		goto exit_alloc_data_failed;
	}

	g_ft5x0x_ts = ft5x0x_ts;
	ft5x0x_ts->platform_data = pdata;
	this_client = client;
	ft5x0x_ts->client = client;
	client->irq = ft5x0x_ts_hw_init(ft5x0x_ts);
	i2c_set_clientdata(client, ft5x0x_ts);
	//client->irq = gpio_to_irq(pdata->irq_gpio_number);

	#ifdef CONFIG_I2C_SPRD
	sprd_i2c_ctl_chg_clk(client->adapter->nr, 400000);
	#endif

	err = ft5x0x_read_reg(FT5X0X_REG_CIPHER, &uc_reg_value);
	if (err < 0)
	{
		printk("read chip id error %x\n", uc_reg_value);
		err = -ENODEV;
		goto exit_read_chipid;
	}
	TS_DBG("[FST] FT5X0X_REG_CIPHER is 0x%x\n",uc_reg_value);

#if !defined(CONFIG_FT5X0X_INIT_NOT_UPDATE)
	/*
	  Normally FT5306 chip id is 0x55, FT5316 chip id is 0x0a,
	  but if the tp device power off when downloading firmware,
	  FT5306 chip id changes to 0xa3, FT5316 changes to 0x0,
	  and the firmware need to be downloaded first next time.
	*/
	if (uc_reg_value == 0x55 || uc_reg_value == 0xa3) {
		CTPM_FW = FT5306_FW;
		fm_size = sizeof(FT5306_FW);
		if(uc_reg_value == 0xa3) {
			msleep(100);
			fts_ctpm_fw_upgrade_with_i_file();
		}
	} else if (uc_reg_value == 0x0a || uc_reg_value == 0x0) {
		CTPM_FW = FT5316_FW;
		fm_size = sizeof(FT5316_FW);
		if(uc_reg_value == 0x0) {
			msleep(100);
			fts_ctpm_fw_upgrade_with_i_file();
		}
	} else {
		printk("unknown chip id %x\n", uc_reg_value);
		err = -ENODEV;
		goto exit_read_chipid;
	}
#endif

	err = check_ctp_chip();
	if (err<0) {
		dev_err(&client->dev,
			"ft5x0x_ts_probe failed(%d): failed to check_ctp_chip: %s\n", err,
			dev_name(&client->dev));
		goto exit_check_ctp_chip;
	}
		
	ft5x0x_write_reg(FT5X0X_REG_PERIODACTIVE, 7);//about 70HZ

	INIT_WORK(&ft5x0x_ts->pen_event_work, ft5x0x_ts_pen_irq_work);

	ft5x0x_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!ft5x0x_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}

	INIT_WORK(&ft5x0x_ts->resume_work, ft5x0x_ts_resume_work);
	ft5x0x_ts->ts_resume_workqueue = create_singlethread_workqueue("ft5x0x_ts_resume_work");
	if (!ft5x0x_ts->ts_resume_workqueue) {
		err = -ESRCH;
		goto create_singlethread_workqueue_resume;
	}


	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
#ifdef TOUCH_VIRTUAL_KEYS
	ft5x0x_ts_virtual_keys_init();
#endif
	ft5x0x_ts->input_dev = input_dev;

#ifdef TP_PROXIMITY_SENSOR
	input_set_abs_params(input_dev, ABS_DISTANCE, 0, 1, 0, 0);
#endif

#ifdef CONFIG_FT5X0X_MULTITOUCH
	__set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	__set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	__set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	__set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);

	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);
#else
	__set_bit(ABS_X, input_dev->absbit);
	__set_bit(ABS_Y, input_dev->absbit);
	__set_bit(ABS_PRESSURE, input_dev->absbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);

	input_set_abs_params(input_dev, ABS_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_PRESSURE, 0, PRESS_MAX, 0 , 0);
#endif

	set_bit(EV_ABS, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);
	__set_bit(KEY_MENU,  input_dev->keybit);
	__set_bit(KEY_BACK,  input_dev->keybit);
	__set_bit(KEY_HOMEPAGE,  input_dev->keybit);
	__set_bit(KEY_SEARCH,  input_dev->keybit);

	input_dev->name		= ft5206_input_name;		//dev_name(&client->dev)
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"ft5x0x_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}

	err = request_threaded_irq(client->irq, NULL, ft5x0x_ts_interrupt, IRQF_TRIGGER_FALLING | IRQF_ONESHOT, client->name, ft5x0x_ts);
	if (err < 0) {
		dev_err(&client->dev, "ft5x0x_probe: request irq failed %d\n",err);
		goto exit_irq_request_failed;
	}

	disable_irq_nosync(client->irq);

#ifdef CONFIG_HAS_EARLYSUSPEND
	TS_DBG("==register_early_suspend =");
	ft5x0x_ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ft5x0x_ts->early_suspend.suspend = ft5x0x_ts_suspend;
	ft5x0x_ts->early_suspend.resume	= ft5x0x_ts_resume;
	register_early_suspend(&ft5x0x_ts->early_suspend);
#endif
#ifdef SPRD_AUTO_UPGRADE
   focaltech_get_upgrade_array(client);
#endif
	msleep(100);
	//get some register information
	uc_reg_value = ft5x0x_read_fw_ver();
	TS_DBG("[FST] Firmware version = 0x%x\n", uc_reg_value);
	TS_DBG("[FST] New Firmware version = 0x%x\n", CTPM_FW[fm_size-2]);

#if !defined(CONFIG_FT5X0X_INIT_NOT_UPDATE)
	if(uc_reg_value != CTPM_FW[fm_size-2])
	{
		fts_ctpm_fw_upgrade_with_i_file();
	}
#endif

//add jinq
#ifdef FTS_CTL_IIC
		if (ft_rw_iic_drv_init(client) < 0)
			dev_err(&client->dev, "%s:[FTS] create fts control iic driver failed\n",
					__func__);
#endif
#ifdef SPRD_AUTO_UPGRADE
	printk("********************Enter CTP Auto Upgrade********************\n");
	fts_ctpm_auto_upgrade(client);
#endif   

#if 0
	ft5x0x_ts->touch_timer.function = ft5x0x_tpd_polling;
	ft5x0x_ts->touch_timer.data = 0;
	init_timer(&ft5x0x_ts->touch_timer);
	ft5x0x_ts->touch_timer.expires = jiffies + HZ*3;
	add_timer(&ft5x0x_ts->touch_timer);
#endif

#ifdef TP_PROXIMITY_SENSOR
    err = sysfs_create_group(&(ft5x0x_ts->input_dev->dev.kobj), &ft5x0x_attr_group);
	if (err) {
		dev_err(&client->dev, "create device file failed!\n");
		err = -EINVAL;
		goto exit_create_sysfs_failed;
	}
#endif
	enable_irq(client->irq);
	TS_DBG("%s: probe Success!\n",__func__);
	return 0;
#ifdef TP_PROXIMITY_SENSOR
exit_create_sysfs_failed:
#endif
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ft5x0x_ts->early_suspend);
#endif
	free_irq(client->irq, ft5x0x_ts);
exit_irq_request_failed:
	input_unregister_device(input_dev);
exit_input_register_device_failed:
	input_free_device(input_dev);
#ifdef TOUCH_VIRTUAL_KEYS
	ft5x0x_ts_virtual_keys_destroy();
#endif
exit_input_dev_alloc_failed:
	destroy_workqueue(ft5x0x_ts->ts_resume_workqueue);
create_singlethread_workqueue_resume:
	cancel_work_sync(&ft5x0x_ts->resume_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
#ifdef TP_PROXIMITY_SENSOR
err_mis_reg:
		remove_ctp_chip();
#endif
exit_create_singlethread:
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
exit_check_ctp_chip:
	remove_ctp_chip();
exit_read_chipid:
	i2c_set_clientdata(client, NULL);
	if (pdata->reset_gpio_number>0)
		gpio_free(pdata->reset_gpio_number);
	if (pdata->irq_gpio_number>0)
		gpio_free(pdata->irq_gpio_number);
	kfree(ft5x0x_ts);
exit_alloc_data_failed:
exit_check_functionality_failed:
	return err;
}

static int ft5x0x_ts_remove(struct i2c_client *client)
{

	struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(client);

	TS_DBG("==ft5x0x_ts_remove=\n");

	//ft5x0x_remove_sysfs(client);
#ifdef TP_PROXIMITY_SENSOR
	sysfs_remove_group(&(ft5x0x_ts->input_dev->dev.kobj), &ft5x0x_attr_group);
#endif
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ft5x0x_ts->early_suspend);
#endif
	free_irq(client->irq, ft5x0x_ts);
	input_unregister_device(ft5x0x_ts->input_dev);
	input_free_device(ft5x0x_ts->input_dev);
#ifdef TOUCH_VIRTUAL_KEYS
	ft5x0x_ts_virtual_keys_destroy();
#endif
	destroy_workqueue(ft5x0x_ts->ts_resume_workqueue);
	cancel_work_sync(&ft5x0x_ts->resume_work);
	if (ft5x0x_ts->ts_workqueue)
		destroy_workqueue(ft5x0x_ts->ts_workqueue);
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	remove_ctp_chip();
	i2c_set_clientdata(client, NULL);
	if (ft5x0x_ts->platform_data->reset_gpio_number>0)
		gpio_free(ft5x0x_ts->platform_data->reset_gpio_number);
	if (ft5x0x_ts->platform_data->irq_gpio_number>0)
		gpio_free(ft5x0x_ts->platform_data->irq_gpio_number);
	kfree(ft5x0x_ts);

	return 0;
}

static const struct i2c_device_id ft5x0x_ts_id[] = {
	{ FT5X0X_NAME, 0 },{ }
};


MODULE_DEVICE_TABLE(i2c, ft5x0x_ts_id);

static struct i2c_driver ft5x0x_ts_driver = {
	.probe		= ft5x0x_ts_probe,
	.remove		= ft5x0x_ts_remove,
	.id_table	= ft5x0x_ts_id,
	.driver	= {
		.name	= FT5X0X_NAME,
		.owner	= THIS_MODULE,
	},
};

#if I2C_BOARD_INFO_METHOD
static int __init ft5x0x_ts_init(void)
{
	TS_DBG("==ft5x0x_ts_init==\n");
	
	if(tp_device_id(0)!=0)
	{
		printk("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	return i2c_add_driver(&ft5x0x_ts_driver);
}

static void __exit ft5x0x_ts_exit(void)
{
	TS_DBG("==ft5x0x_ts_exit==\n");
	i2c_del_driver(&ft5x0x_ts_driver);
}
#else //register i2c device&driver dynamicly

static int sprd_add_i2c_device(struct sprd_i2c_setup_data *i2c_set_data, struct i2c_driver *driver)
{
	struct i2c_board_info info;
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	int ret,err;


	TS_DBG("%s : i2c_bus=%d; slave_address=0x%x; i2c_name=%s\n",__func__,i2c_set_data->i2c_bus, \
		    i2c_set_data->i2c_address, i2c_set_data->type);

	memset(&info, 0, sizeof(struct i2c_board_info));
	info.addr = i2c_set_data->i2c_address;
	strlcpy(info.type, i2c_set_data->type, I2C_NAME_SIZE);
	if(i2c_set_data->irq > 0)
		info.irq = i2c_set_data->irq;

	adapter = i2c_get_adapter( i2c_set_data->i2c_bus);
	if (!adapter) {
		printk("%s: can't get i2c adapter %d\n",
			__func__,  i2c_set_data->i2c_bus);
		err = -ENODEV;
		goto err_adapter;
	}

	client = i2c_new_device(adapter, &info);
	if (!client) {
		printk("%s:  can't add i2c device at 0x%x\n",
			__func__, (unsigned int)info.addr);
		err = -ENODEV;
		goto err_device;
	}

	i2c_put_adapter(adapter);

	ret = i2c_add_driver(driver);
	if (ret != 0) {
		printk("%s: can't add i2c driver\n", __func__);
		err = -ENODEV;
		goto err_driver;
	}

	return 0;

err_driver:
	i2c_unregister_device(client);
	return err;
err_device:
	i2c_put_adapter(adapter);
err_adapter:
	return err;
}

static void sprd_del_i2c_device(struct i2c_client *client, struct i2c_driver *driver)
{
	TS_DBG("%s : slave_address=0x%x; i2c_name=%s",__func__, client->addr, client->name);
	if (client!=NULL)
		i2c_unregister_device(client);
	i2c_del_driver(driver);
}

static int __init ft5x0x_ts_init(void)
{

	TS_DBG("%s\n", __func__);

	if(tp_device_id(0)!=0)
	{
		printk("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	ft5x0x_ts_setup.i2c_bus = 2;
	ft5x0x_ts_setup.i2c_address = FT5206_TS_ADDR;
	strcpy (ft5x0x_ts_setup.type,FT5206_TS_NAME);
	return sprd_add_i2c_device(&ft5x0x_ts_setup, &ft5x0x_ts_driver);
}

static void __exit ft5x0x_ts_exit(void)
{
	TS_DBG("%s\n", __func__);
	sprd_del_i2c_device(this_client, &ft5x0x_ts_driver);
}
#endif

module_init(ft5x0x_ts_init);
module_exit(ft5x0x_ts_exit);

MODULE_AUTHOR("<wenfs@Focaltech-systems.com>");
MODULE_DESCRIPTION("FocalTech ft5x0x TouchScreen driver");
MODULE_LICENSE("GPL");
