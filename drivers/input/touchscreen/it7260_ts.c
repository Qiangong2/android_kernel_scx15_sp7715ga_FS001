/*
 * File:         It7260_ts.c
 * Based on:
 * Author:       Yunlong Wang <Yunlong.Wang @spreadtrum.com>
 *
 * Created:	  2011-03-09
 * Description:  TouchScreen IC driver for sc8800g Processors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
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
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/earlysuspend.h>
#include <linux/regulator/consumer.h>
#include <linux/i2c/it7260_ts.h>
#include <mach/regulator.h>
#include <mach/i2c-sprd.h>
#include <mach/board.h>
#include <linux/cdc_com.h>

#undef TS_DEBUG_MSG
#ifdef TS_DEBUG_MSG
#define TS_DBG(format, ...)	\
		printk(KERN_INFO "IT7260 " format "\n", ## __VA_ARGS__)
#else
#define TS_DBG(format, ...)
#endif

#define TS_INFO(format, ...)	\
		printk(KERN_INFO "IT7260 " format "\n", ## __VA_ARGS__)

// Alex.shi 支持的TP列表
// 格式:CONFIG_ZYT_IC_项目_客户_模组厂_序号
#if defined(CONFIG_MACH_SP6825EB_7661) || defined(CONFIG_MACH_SP8825EB_7661)
	// 7629 打包的普星光电TP  
	#define CONFIG_ZYT_IT7252_7629_ZYT_PUXING_0
#else
	// 7616万达 安浙TP MG05001-W XFY.这个宏在内核配置文件中打开  
	// CONFIG_ZYT_IT7252_7616_WD_ANZHE_0

	// 7616洲际通 安浙TP  
	// #define CONFIG_ZYT_IT7252_7616_ZJT_ANZHE_0

	// 默认采用TP标准,即:MENU(1)；BACK(2);HOME(3);SEARCH(4)    
#endif

#ifdef CONFIG_ZYT_SUPPORT
#define TOUCH_VIRTUAL_KEYS
#endif

#if defined(CONFIG_LCD_QHD)
#define TS_X_MAX	539
#define TS_Y_MAX	959
#elif defined(CONFIG_LCD_FWVGA)
#define TS_X_MAX	479
#define TS_Y_MAX	853
#elif defined(CONFIG_LCD_WVGA)
#define TS_X_MAX	479
#define TS_Y_MAX	799
#elif defined(CONFIG_LCD_HVGA)
#define TS_X_MAX	319
#define TS_Y_MAX	479
#elif defined(CONFIG_LCD_QVGA)
#define TS_X_MAX	239
#define TS_Y_MAX	319
#else
#error "You do not set IT72XX's resolution."
#endif

#define PRESS_MAX				255

#if defined(CONFIG_ZYT_IT7252_7616_WD_ANZHE_0)
#define TOUCH_VIRTUAL_KEY_HOME		3
#define TOUCH_VIRTUAL_KEY_MENU		1
#define TOUCH_VIRTUAL_KEY_BACK		2
#define TOUCH_VIRTUAL_KEY_SEARCH	4
#elif defined(CONFIG_ZYT_IT7252_7629_ZYT_PUXING_0)
#define TOUCH_VIRTUAL_KEY_HOME		1
#define TOUCH_VIRTUAL_KEY_MENU		2
#define TOUCH_VIRTUAL_KEY_BACK		3
#define TOUCH_VIRTUAL_KEY_SEARCH	4
#elif defined(CONFIG_ZYT_IT7252_7616_ZJT_ANZHE_0)
#define TOUCH_VIRTUAL_KEY_HOME		2
#define TOUCH_VIRTUAL_KEY_MENU		1
#define TOUCH_VIRTUAL_KEY_BACK		3
#define TOUCH_VIRTUAL_KEY_SEARCH	4
#else	//  默认采用TP标准,即:MENU(1)；BACK(2);HOME(3);SEARCH(4)    
#define TOUCH_VIRTUAL_KEY_HOME		3
#define TOUCH_VIRTUAL_KEY_MENU		1
#define TOUCH_VIRTUAL_KEY_BACK		2
#define TOUCH_VIRTUAL_KEY_SEARCH	4
#endif

static int it7260_i2c_rxdata(char *rxdata, int length);
static int it7260_i2c_txdata(char *txdata, int length);

struct sprd_i2c_setup_data {
	unsigned i2c_bus;  //the same number as i2c->adap.nr in adapter probe function
	unsigned short i2c_address;
	int irq;
	char type[I2C_NAME_SIZE];
};

struct it7260_ts_t{
	struct input_dev *input;
	struct i2c_client *client;
	struct work_struct	work;
	struct workqueue_struct *ts_work_queue;
	struct early_suspend ts_early_suspend;
#ifdef I2C_BOARD_INFO_METHOD
	struct it7260_ts_platform_data	*platform_data;
#endif
};

#ifdef I2C_BOARD_INFO_METHOD
static struct it7260_ts_t	*g_it7260_ts=NULL;
#else
static struct sprd_i2c_setup_data it7260_ts_setup={2, IT7260_TS_ADDR, 0, IT7260_TS_NAME};
#endif
static struct i2c_client *this_client;
static int it7260_irq=0;

static unsigned char rdbuf[14];
#ifdef TOUCH_VIRTUAL_KEYS
static struct kobject *properties_kobj;
#endif

static int x[2] = { (int) -1, (int) -1 };
static int y[2] = { (int) -1, (int) -1 };
static bool finger[2]={0, 0};
static int cnt[4] = { 0, 0, 0, 0 };
//static int pos[2]={0, 0};

static void it7260_chip_vdd_input(bool turn_on)
{
#if 0
	struct regulator	*reg_vdd;
	struct i2c_client	*client = this_client;
#ifdef I2C_BOARD_INFO_METHOD
	struct it7260_ts_platform_data	*pdata = g_it7260_ts->platform_data;
	printk(KERN_INFO "%s [irq=%d];[rst=%d]\n", __func__, pdata->irq_gpio_number, pdata->reset_gpio_number);
#endif

	if (turn_on) {	//vdd power on
#ifdef I2C_BOARD_INFO_METHOD
		reg_vdd = regulator_get(&client->dev, pdata->vdd_name);
#else
		reg_vdd = regulator_get(&client->dev, "vdd28");
#endif
		regulator_set_voltage(reg_vdd, 2800000, 2800000);
		regulator_enable(reg_vdd);
		//msleep(100);
	}
	else {
		// not do anything
	}
#endif
}

static int it7260_ts_open(struct inode *inode, struct file *file)
{
	return nonseekable_open(inode, file);
}
 
static int it7260_ts_release(struct inode *inode, struct file *file)
{
	return 0;
}

struct ite7260_data {
	rwlock_t		lock;
	unsigned short	bufferIndex;
	unsigned short	length;
	unsigned short	buffer[MAX_BUFFER_SIZE];
};

static int it7260_ts_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
//	struct ite7260_data *dev = filp->private_data;
	int retval = 0;
	int i;
//	unsigned char ucQuery;
	unsigned char buffer[MAX_BUFFER_SIZE];
	struct ioctl_cmd168 data;
	unsigned char datalen;
	unsigned char ent[] = {0x60, 0x00, 0x49, 0x54, 0x37, 0x32};
	unsigned char ext[] = {0x60, 0x80, 0x49, 0x54, 0x37, 0x32};

	memset(&data, 0, sizeof(struct ioctl_cmd168));

	switch (cmd) {
	case IOCTL_SET:
		//printk("IOCTL_SET\n");
		if (!access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd))) {
			retval = -EFAULT;
			goto done;
		}
		if ( copy_from_user(&data, (int __user *)arg, sizeof(struct ioctl_cmd168)) ) {
			retval = -EFAULT;
			goto done;
		}
		/*
		printk("  bufferIndex: %02X\n", data.bufferIndex);
		printk("  length: %02X\n", data.length);
		printk("  buffer: ");
		for (i = 0; i < data.length; i++) {
			printk("%02X ", data.buffer[i]);
		}
		printk("\n");
		*/
		buffer[0] = (unsigned char) data.bufferIndex;
		for (i = 1; i < data.length + 1; i++) {
			buffer[i] = (unsigned char) data.buffer[i - 1];
		}
		if (!memcmp(&(buffer[1]), ent, sizeof(ent))) {
			pr_info("Disabling IRQ.\n");
			disable_irq_nosync(this_client->irq);
		}
		if (!memcmp(&(buffer[1]), ext, sizeof(ext))) {
			pr_info("Enabling IRQ.\n");
			enable_irq(this_client->irq);
		}
		datalen = (unsigned char) (data.length + 1);
		buffer[0]=0x20;
		/*
		printk("TX datalen %d: ", datalen);
		for (i = 0; i < datalen; i++) {
			printk("%02X ", buffer[i]);
		}
		printk("\n");
		*/
		it7260_i2c_txdata(buffer, datalen);
		//write_unlock(&dev->lock);
		retval = 0;
		break;

	case IOCTL_GET:
		//printk("IOCTL_GET\n");
		if (!access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd))) {
			retval = -EFAULT;
			goto done;
		}
		if (!access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd))) {
			retval = -EFAULT;
			goto done;
		}
		if ( copy_from_user(&data, (int __user *)arg, sizeof(struct ioctl_cmd168)) ) {
			retval = -EFAULT;
			goto done;
		}

		//pr_info("=================================================\n");
		buffer[0] = (unsigned char) data.bufferIndex;

		it7260_i2c_rxdata((char *) buffer, (int) data.length);
		/*
		printk("RX data.length %d: ", data.length);
		for (i = 0; i < data.length; i++) {
			printk("%02X ", buffer[i]);
		}
		printk("\n");
		*/
		retval = 0;
		for (i = 0; i < data.length; i++) {
			data.buffer[i] = (unsigned short) buffer[i];
		}
		/*		
		printk("  bufferIndex: %02X\n", data.bufferIndex);
		printk("  length: %02X\n", data.length);
		printk("  buffer: ");
		for (i = 0; i < data.length; i++) {
			printk("%02X ", data.buffer[i]);
		}
		printk("\n");
		*/		
		if ( copy_to_user((int __user *)arg, &data, sizeof(struct ioctl_cmd168)) ) {
			retval = -EFAULT;
			goto done;
		}
		break;

	default:
		retval = -ENOTTY;
		break;
	}

done:
	return (retval);
}

static struct file_operations it7260_ts_fops = {
	.owner		= THIS_MODULE,
	.open		= it7260_ts_open,
	.release	= it7260_ts_release,
	.unlocked_ioctl		= it7260_ts_ioctl,
};

static struct miscdevice it7260_ts_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = IT7260_TS_DEVICE,
	.fops = &it7260_ts_fops,
};

#if 0
static int it7260_read_block_data(struct i2c_client *client, u8 reg,
				   int count, u8 *addr)
{
	if (i2c_smbus_read_i2c_block_data
			(client, reg, count, addr) < count) {
		dev_err(&client->dev, "i2c block read failed\n");
		return -1;
	}

	return count;
}

static int it7260_ts_check(void)
{
	char	buffer[10];    
	int		status=0;

	status = 1;

	TS_DBG("it7260_ts_check!!\n");      
	status = it7260_read_block_data(this_client, 0x00, 10, &(buffer[0]));    
	if(status<0) 
	{        
		printk("cy8ctma340 tpd_i2c_probe failed!!\n");        
		status = it7260_read_block_data(this_client, 0x00, 10, &(buffer[0]));        
		if(status<0) 
		{            
			printk("cy8ctma340 tpd_i2c_probe retry failed!!\n");          
			return status;        
		}
	}

	{
		int i =0;
		for (i=0;i<10;i++)
			printk("IT7260 identify =%c",buffer[i]);
	}
	printk("\n");
	return status;
}
#endif

/*******************************************************************************
* Function    :  it7260_ts_config_pins
* Description :  config gpio pins and set relative TS pins
* Parameters  :  void
* Return      :    void
*******************************************************************************/
static void it7260_ts_config_pins(void)
{
	unsigned gpio_irq=0, gpio_rst=0;
#ifdef I2C_BOARD_INFO_METHOD
	struct it7260_ts_platform_data	*pdata = g_it7260_ts->platform_data;

	printk(KERN_INFO "%s [irq=%d];[rst=%d]\n", __func__, pdata->irq_gpio_number, pdata->reset_gpio_number);
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
	it7260_irq=gpio_to_irq(gpio_irq);

	msleep(10); //wait for stable
}

/*******************************************************************************
* Function    :  it7260_ts_early_suspend
* Description :  cancel the delayed work and put ts to shutdown mode
* Parameters  :  handler
* Return      :    none
*******************************************************************************/
static void it7260_ts_early_suspend(struct early_suspend *handler)
{
	unsigned char writeSleepBuf[] = {0x20, 0x04, 0x00, 0x02};
	finger[0] = finger[1] = 0;

	TS_DBG("%s\n", __FUNCTION__);

	//disable_irq_nosync(it7260_irq);    
	it7260_i2c_txdata(writeSleepBuf, 4);

	mdelay(5);    

	//it7260_chip_vdd_input(false);	//Alex.shi 不能重复 regulator_get
}

/*******************************************************************************
* Function    :  it7260_ts_early_resume
* Description :  ts re-entry the normal mode and schedule the work, there need to be  a litte time 
                      for ts ready
* Parameters  :  handler
* Return      :    none
*******************************************************************************/
static void it7260_ts_early_resume(struct early_suspend *handler)
{	
	unsigned gpio_rst=0;
#ifdef I2C_BOARD_INFO_METHOD
	struct it7260_ts_platform_data	*pdata = g_it7260_ts->platform_data;

	printk(KERN_INFO "%s [irq=%d];[rst=%d]\n", __func__, pdata->irq_gpio_number, pdata->reset_gpio_number);
	gpio_rst = pdata->reset_gpio_number;
#else
	gpio_rst = GPIO_TOUCH_RESET;
#endif
	TS_DBG("%s\n", __FUNCTION__);

	//it7260_chip_vdd_input(true);	//Alex.shi 不能重复 regulator_get

	mdelay(1);
	rdbuf[0] = 0x80;
	it7260_i2c_rxdata(rdbuf, 1);
	mdelay(10);
	gpio_direction_output(gpio_rst, 0);
	msleep(10);
	//gpio_set_value(gpio_rst, 0);
	//msleep(20);
	//gpio_set_value(gpio_rst, 1);
	//enable_irq(it7260_irq);
}

#if 0
/*******************************************************************************
* Function    :  tpd_down
* Description :  handler current data and report coordinate
* Parameters  :  work
* Return      :    none
*******************************************************************************/
static void tpd_down(int x, int y, int p) 
{
	struct it7260_ts_t *data = i2c_get_clientdata(this_client);

	input_report_abs(data->input, ABS_PRESSURE,p);
	input_report_key(data->input, BTN_TOUCH, 1);
	//input_report_abs(data->input_dev,ABS_MT_TRACKING_ID,i);
	input_report_abs(data->input, ABS_MT_TOUCH_MAJOR, 1);
	input_report_abs(data->input, ABS_MT_POSITION_X, x);
	input_report_abs(data->input, ABS_MT_POSITION_Y, y);
	TS_DBG("Down x:%4d, y:%4d, p:%4d \n ", x, y, p);
	input_mt_sync(data->input);
}

static void tpd_up(int x, int y,int p) 
{
	struct it7260_ts_t *data = i2c_get_clientdata(this_client);

	input_report_abs(data->input, ABS_PRESSURE, 0);
	input_report_key(data->input, BTN_TOUCH, 0);

	input_report_abs(data->input, ABS_MT_TOUCH_MAJOR, 0);
	input_report_abs(data->input, ABS_MT_POSITION_X, x);
	input_report_abs(data->input, ABS_MT_POSITION_Y, y);
	TS_DBG("Up x:%4d, y:%4d, p:%4d \n", x, y, 0);
	input_mt_sync(data->input);
}

static int tpd_touchinfo(struct touch_info *cinfo, struct touch_info *pinfo)
{
	u32 retval;
	//static u8 tt_mode;

	memcpy(pinfo, cinfo, sizeof(struct touch_info));
	TS_DBG("tpd_touchinfo pinfo.count=%d,pinfo.x1=%d,pinfo.y1=%d\n", pinfo->count,pinfo->x1,pinfo->y1);
	TS_DBG("tpd_touchinfo cinfo.count=%d,cinfo.x1=%d,cinfo.y1=%d\n", cinfo->count,cinfo->x1,cinfo->y1);
	memset(cinfo, 0, sizeof(struct touch_info));

	retval =  it7260_read_block_data(this_client, TPD_REG_BASE, 8, (u8 *)&g_operation_data);
	retval += it7260_read_block_data(this_client, TPD_REG_BASE + 8, 8, (((u8 *)(&g_operation_data)) + 8));
	retval += it7260_read_block_data(this_client, TPD_REG_BASE + 16, 8, (((u8 *)(&g_operation_data)) + 16));
	retval += it7260_read_block_data(this_client, TPD_REG_BASE + 24, 4, (((u8 *)(&g_operation_data)) + 24));

	cinfo->count = (g_operation_data.tt_stat & 0x0f) ; //point count

	TS_DBG("cinfo->count =%d\n",cinfo->count);

	TS_DBG("Procss raw data...\n");

	cinfo->x1 = (( g_operation_data.x1_M << 8) | ( g_operation_data.x1_L)); //point 1		
	cinfo->y1  = (( g_operation_data.y1_M << 8) | ( g_operation_data.y1_L));
	cinfo->p1 = g_operation_data.z1;

	if(cinfo->x1 < 1) cinfo->x1 = 1;
	if(cinfo->y1 < 1) cinfo->y1 = 1;

	//cinfo->id1 = ((g_operation_data.touch12_id & 0xf0) >>4) -1;

	TS_DBG("Before: cinfo->x1= %3d, 	cinfo->y1 = %3d, cinfo->p1 = %3d\n", cinfo->x1, cinfo->y1 , cinfo->p1);		

	//cinfo->x1 =  cinfo->x1 * 480 >> 11; //calibrate
	//cinfo->y1 =  cinfo->y1 * 800 >> 11; 

	TS_DBG("After:	cinfo->x1 = %3d, cinfo->y1 = %3d, cinfo->p1 = %3d\n", cinfo->x1 ,cinfo->y1 ,cinfo->p1);

	if(cinfo->count >1) {
		cinfo->x2 = (( g_operation_data.x2_M << 8) | ( g_operation_data.x2_L)); //point 2
		cinfo->y2 = (( g_operation_data.y2_M << 8) | ( g_operation_data.y2_L));
		cinfo->p2 = g_operation_data.z2;

		if(cinfo->x2< 1) cinfo->x2 = 1;
		if(cinfo->y2 < 1) cinfo->y2 = 1;

		//cinfo->id2 = (g_operation_data.touch12_id & 0x0f)-1;

		TS_DBG("before:	 cinfo->x2 = %3d, cinfo->y2 = %3d,  cinfo->p2 = %3d\n", cinfo->x2, cinfo->y2,  cinfo->p2);	  
		//cinfo->x2 =  cinfo->x2 * 480 >> 11; //calibrate
		//cinfo->y2 =  cinfo->y2 * 800 >> 11; 
		TS_DBG("After:	 cinfo->x2 = %3d, cinfo->y2 = %3d,  cinfo->p2 = %3d\n", cinfo->x2, cinfo->y2, cinfo->p2);	
	}

	if (!cinfo->count) return true; // this is a touch-up event

	/*
	if (g_operation_data.tt_mode & 0x20) {
	TS_DBG("uffer is not ready for use!\n");
	memcpy(cinfo, pinfo, sizeof(struct touch_info));
	return false;
	}//return false; // buffer is not ready for use// buffer is not ready for use
	*/

	it7260_read_block_data(this_client, TPD_REG_BASE, 1, (u8*)&g_operation_data);

	if((g_operation_data.hst_mode & 0x80)==0)
		g_operation_data.hst_mode = g_operation_data.hst_mode|0x80;
	else
		g_operation_data.hst_mode = g_operation_data.hst_mode & (~0x80);

	i2c_smbus_write_i2c_block_data(this_client, TPD_REG_BASE, sizeof(g_operation_data.hst_mode), &g_operation_data.hst_mode);

	/*
	if (tt_mode == g_operation_data.tt_mode) {
		TS_DBG("sampling not completed!\n");
		memcpy(cinfo, pinfo, sizeof(struct touch_info));
		return false; 
	}// sampling not completed
	else 
		tt_mode = g_operation_data.tt_mode;
	*/	
	return true;
};
#endif

static int it7260_i2c_rxdata(char *rxdata, int length)
{
	int ret;
	struct i2c_msg msgs[] = {
		{
			.addr   = this_client->addr,
			.flags  = I2C_M_NOSTART,
			.len    = 1,
			.buf    = rxdata,
		},{
			.addr   = this_client->addr,
			.flags  = I2C_M_RD,
			.len    = length,
			.buf    = rxdata,
		},
	};

	ret = i2c_transfer(this_client->adapter, msgs,2);
	if (ret < 0)
		pr_err("%s i2c read error: %d\n", __func__, ret);
	else if (2!=ret) {
		pr_err("%s i2c read error: %d\n", __func__, ret);
		ret = -EIO;
	}

	return ret;
}

static int it7260_i2c_txdata(char *txdata, int length)
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

static void it7260_ts_work(struct work_struct *work)
{
	struct it7260_ts_t *ts = container_of(work, struct it7260_ts_t, work);
	struct it7260_ts_t *tsdata = ts;

	int xraw = -1;
	int yraw = -1;
	int ret, i;

	rdbuf[0] = 0x80;
	it7260_i2c_rxdata(rdbuf, 1);

	TS_DBG("  it7260   ts %s:  %d", __FUNCTION__,__LINE__);

	if( rdbuf[0] & 0x80 )
	{
		rdbuf[0] = 0xE0;
		ret = it7260_i2c_rxdata(rdbuf, 14);
		for (i=0; i<14; i++)
			TS_DBG("rdbuf[%d]=%x, ", i, rdbuf[i]);
		TS_DBG("\n");
		if( rdbuf[0] &0xF0 )
		{
			if( ( rdbuf[0] & 0x40 ) && ( rdbuf[0] & 0x01 ) )
			{	// Button / Gesture
				if (rdbuf[2]) {//Button down
					switch (rdbuf[1]) {
					case TOUCH_VIRTUAL_KEY_SEARCH:
						{
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 1);
							input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 8);
							input_report_abs(tsdata->input, ABS_MT_POSITION_X, 280);
							input_report_abs(tsdata->input, ABS_MT_POSITION_Y, 980);
							input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
							//input_report_abs(tsdata->input, ABS_PRESSURE, 200);
							input_mt_sync(tsdata->input);
							input_sync(tsdata->input);
#else
							input_report_key(tsdata->input, KEY_SEARCH, 1);
							input_sync(tsdata->input);
							//input_report_key(tsdata->input, KEY_SEARCH, 0);
							//input_sync(tsdata->input);
#endif
						}
						break;

					case TOUCH_VIRTUAL_KEY_BACK:
						{
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 1);
							input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 8);
							input_report_abs(tsdata->input, ABS_MT_POSITION_X, 200);
							input_report_abs(tsdata->input, ABS_MT_POSITION_Y, 980);
							input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
							//input_report_abs(tsdata->input, ABS_PRESSURE, 200);
							input_mt_sync(tsdata->input);
							input_sync(tsdata->input);
#else
							input_report_key(tsdata->input, KEY_BACK, 1);
							input_sync(tsdata->input);
							//input_report_key(tsdata->input, KEY_BACK, 0);
							//input_sync(tsdata->input);
#endif
						}
						break;

					case TOUCH_VIRTUAL_KEY_MENU:
						{ 
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 1);
							input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 8);
							input_report_abs(tsdata->input, ABS_MT_POSITION_X, 40);
							input_report_abs(tsdata->input, ABS_MT_POSITION_Y, 980);
							input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
							//input_report_abs(tsdata->input, ABS_PRESSURE, 200);
							input_mt_sync(tsdata->input);
							input_sync(tsdata->input);
#else
							input_report_key(tsdata->input, KEY_MENU, 1);
							input_sync(tsdata->input);
							//input_report_key(tsdata->input, KEY_MENU, 0);
							//input_sync(tsdata->input);
#endif
						}
						break;

					case TOUCH_VIRTUAL_KEY_HOME:
						{
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 1);
							input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 8);
							input_report_abs(tsdata->input, ABS_MT_POSITION_X, 120);
							input_report_abs(tsdata->input, ABS_MT_POSITION_Y, 980);
							input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
							//input_report_abs(tsdata->input, ABS_PRESSURE, 200);
							input_mt_sync(tsdata->input);
							input_sync(tsdata->input);
#else
							input_report_key(tsdata->input, KEY_HOMEPAGE, 1);
							input_sync(tsdata->input);
							//input_report_key(tsdata->input, KEY_HOMEPAGE, 0);
							//input_sync(tsdata->input);
#endif
						}
						break;
					}
				}
				else {
					switch (rdbuf[1]) {
					case TOUCH_VIRTUAL_KEY_SEARCH:
						{
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 0);
							//input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 0);
							//input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 0);
							input_sync(tsdata->input);
#else
							//input_report_key(tsdata->input, KEY_SEARCH, 1);
							//input_sync(tsdata->input);
							input_report_key(tsdata->input, KEY_SEARCH, 0);
							input_sync(tsdata->input);
#endif
						}
						break;

					case TOUCH_VIRTUAL_KEY_BACK:
						{
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 0);
							//input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 0);
							//input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 0);
							input_sync(tsdata->input);
#else
							//input_report_key(tsdata->input, KEY_BACK, 1);
							//input_sync(tsdata->input);
							input_report_key(tsdata->input, KEY_BACK, 0);
							input_sync(tsdata->input);
#endif
						}
						break;

					case TOUCH_VIRTUAL_KEY_MENU:
						{ 
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 0);
							//input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 0);
							//input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 0);
							input_sync(tsdata->input);
#else
							//input_report_key(tsdata->input, KEY_MENU, 1);
							//input_sync(tsdata->input);
							input_report_key(tsdata->input, KEY_MENU, 0);
							input_sync(tsdata->input);
#endif
						}
						break;

					case TOUCH_VIRTUAL_KEY_HOME:
						{
#ifdef TOUCH_VIRTUAL_KEYS
							input_report_key(tsdata->input, BTN_TOUCH, 0);
							//input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 0);
							//input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 0);
							input_sync(tsdata->input);
#else
							//input_report_key(tsdata->input, KEY_HOMEPAGE, 1);
							//input_sync(tsdata->input);
							input_report_key(tsdata->input, KEY_HOMEPAGE, 0);
							input_sync(tsdata->input);
#endif
						}
						break;
					}
				}
			}
			enable_irq(ts->client->irq);
			return;
		}

		if( rdbuf[1 ] &0x01 )	//palm
		{
			TS_DBG("palm\n");
			enable_irq(ts->client->irq);
			return;
		}

		if( !( rdbuf[0] & 0x08 ) )	//no more data
		{
			TS_DBG("no more data push pen up\n");
			input_report_key(tsdata->input, BTN_TOUCH, 0);
			//input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 0);
			//input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 0);
			//input_mt_sync(tsdata->input);
			finger[0] =	finger[1] = 0;
			cnt[0] = cnt[1] = cnt[2] = cnt[3] = 0;
			input_sync(tsdata->input);
			enable_irq(ts->client->irq);
			return;
		}

		if( rdbuf[0] & 0x04 ){	// 3 finger
			TS_DBG("3point detect,enable irq and return");
			enable_irq(ts->client->irq);
			return ;
		}

		if( rdbuf[0] & 0x01 ){	// Finger 1
			xraw = ((( rdbuf[3] & 0x0F ) << 8 ) + rdbuf[2] );
			yraw = ((( rdbuf[3] & 0xF0 ) << 4 ) + rdbuf[4] );
			
			if( finger[0] ){
				if( abs( x[0] - xraw ) < 7 ){
					cnt[0]+=1;
					if( cnt[0] > 6 ) cnt[0] = 7;
				}else{
					cnt[0]-=1;
					if( cnt[0] < 0 ) cnt[0] = 0;
		  		}
	  		//----------------------------------------
				if( abs( y[0] - yraw ) < 7 ){
					cnt[1]+=1;
					if( cnt[1] > 6 ) cnt[1] = 7;
				}else{
					cnt[1]-=1;
					if( cnt[1] < 0 ) cnt[1] = 0;
		  		}
	  		//----------------------------------------
				if( cnt[0] > 5 && cnt[1] > 5 ){
						xraw = x[0];
						yraw = y[0];
				}
			}
			
			TS_DBG("push pen 1 down x=%d,y=%d\n", xraw, yraw);
			input_report_key(tsdata->input, BTN_TOUCH, 1);
			input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 200);
			input_report_abs(tsdata->input, ABS_MT_POSITION_X,  xraw);
			input_report_abs(tsdata->input, ABS_MT_POSITION_Y,  yraw);
			input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
			//input_report_abs(tsdata->input, ABS_PRESSURE, 200);
			finger[0] = 1;
			x[0] = xraw;
			y[0] = yraw;

			input_mt_sync( tsdata->input );
		}else{
			finger[0] = 0;
			cnt[0] = cnt[1] = 0;
		}

		if( rdbuf[0] & 0x02 ){	// Finger 2
			finger[1] = 1;
			xraw = ((( rdbuf[7] & 0x0F ) << 8 ) + rdbuf[6] );
			yraw = ((( rdbuf[7] & 0xF0 ) << 4 ) + rdbuf[8] );

			if( finger[1] ){
				if( abs( x[1] - xraw ) < 7 ){
					cnt[2]+=1;
					if( cnt[2] > 6 ) cnt[2] = 7;
				}else{
					cnt[2]-=1;
				if( cnt[2] < 0 ) cnt[2] = 0;
		  		}
	  		//---------------------------------------
				if( abs( y[1] - yraw ) < 7 ){
					cnt[3]+=1;
					if( cnt[3] > 6 ) cnt[3] = 7;
				}else{
					cnt[3]-=1;
					if( cnt[3] < 0 ) cnt[3] = 0;
		  		}
	  		//----------------------------------------
				if( cnt[2] > 5 && cnt[3] > 5 ){
						xraw = x[1];
						yraw = y[1];						
		  		}
			}
			
			TS_DBG("push pen 2 down x=%d,y=%d\n", xraw, yraw);
			input_report_key(tsdata->input, BTN_TOUCH, 1);
			input_report_abs(tsdata->input, ABS_MT_TOUCH_MAJOR, 200);
			input_report_abs(tsdata->input, ABS_MT_POSITION_X,  xraw);
			input_report_abs(tsdata->input, ABS_MT_POSITION_Y,  yraw);
			input_report_abs(tsdata->input, ABS_MT_WIDTH_MAJOR, 1);
			//input_report_abs(tsdata->input, ABS_PRESSURE, 200);
			finger[1] = 1;
			x[1] = xraw;
			y[1] = yraw;

			input_mt_sync(tsdata->input);
		}else{
			finger[1] = 0;
			cnt[2] = cnt[3] = 0;
		}
		input_sync(tsdata->input);
	}

	enable_irq(ts->client->irq);
	return ;
}

/*******************************************************************************
* Function    :  it7260_ts_irq_handler
* Description :  handle ts irq
* Parameters  :  handlerit7260_ts_irq_handler
* Return      :    none
*******************************************************************************/
static irqreturn_t it7260_ts_irq_handler(int irq, void *dev_id)
{
	struct it7260_ts_t *ts = (struct it7260_ts_t *)dev_id;
	disable_irq_nosync(ts->client->irq);
	queue_work(ts->ts_work_queue,&ts->work);

	return IRQ_HANDLED;
}

#ifdef TOUCH_VIRTUAL_KEYS
static ssize_t virtual_keys_show(struct kobject *kobj,
             struct kobj_attribute *attr, char *buf)
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
		.name = "virtualkeys.it7260_ts",
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

static void virtual_keys_init(void)
{
	int ret;

	properties_kobj = kobject_create_and_add("board_properties", NULL);
	if (properties_kobj)
		ret = sysfs_create_group(properties_kobj,&properties_attr_group);
	if (!properties_kobj || ret)
		pr_err("failed to create board_properties\n");    
}

static void virtual_keys_destroy(void)
{
	kobject_del(properties_kobj);
}
#endif

static int check_ctp_chip(void)
{
	int	ret=-1;

	rdbuf[0] = 0x80;
	ctp_lock_mutex();
	ret=it7260_i2c_rxdata(rdbuf, 1);
	if(ret>0)
	{
		tp_device_id(0x7260);
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

static int it7260_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int					err = 0;
	struct input_dev	*input_dev;
	struct it7260_ts_t	*it7260_ts;
#ifdef I2C_BOARD_INFO_METHOD
	struct it7260_ts_platform_data *pdata = client->dev.platform_data;
#endif

	printk("  it7260   ts %s:  %d", __FUNCTION__, __LINE__);
	if(tp_device_id(0)!=0)
	{
		TS_INFO("CTP(0x%x)Exist!", tp_device_id(0));
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		printk("ts %s: functionality check failed\n", __FUNCTION__);
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	it7260_ts = kzalloc(sizeof(struct it7260_ts_t), GFP_KERNEL);
	if (!it7260_ts)
	{
		printk("ts %s: request memory failed\n", __FUNCTION__);
		err= -ENOMEM;
		goto exit_request_memory_failed;
	}

	i2c_set_clientdata(client, it7260_ts);
	it7260_ts->client = client;

#ifdef I2C_BOARD_INFO_METHOD
	it7260_ts->platform_data = pdata;
	g_it7260_ts = it7260_ts;
#endif
	this_client = client;
	it7260_chip_vdd_input(true);
	it7260_ts_config_pins();
#ifndef I2C_BOARD_INFO_METHOD
	it7260_ts_setup.irq = it7260_irq;
#endif
	client->irq = it7260_irq;

	//sprd_i2c_ctl_chg_clk(client->adapter->nr, 400000);

	if(check_ctp_chip()<0)
	{
		err = -ENODEV;
		TS_INFO("Fail to find chip");
		goto exit_check_chip_failed;
	}

	//check i2c & chip
	/*
	if(it7260_ts_check()<0)
	{
		printk("ts %s: device check failed\n", __FUNCTION__);
		err=-1;
		goto exit_device_check_failed;
	}
	TS_DBG("it7260_ts_check success!!\n");
	*/

	//register device
	err = misc_register(&it7260_ts_device);
	if (err) {
		printk("ts %s: adbs_a320_device register failed\n", __FUNCTION__);
		goto exit_device_register_failed;
	}

	// register input device 
	input_dev = input_allocate_device();
	if (!input_dev) 
	{
		printk("ts %s: input allocate device failed\n", __FUNCTION__);
		err = -ENOMEM;
		goto exit_input_dev_allocate_failed;
	}

	it7260_ts->input = input_dev;

	input_dev->name = "it7260_ts";
	input_dev->phys  = "it7260_ts";
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &client->dev;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0010;

	__set_bit(EV_ABS, input_dev->evbit);	
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, TS_X_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, TS_Y_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);	

	set_bit(EV_ABS,     input_dev->evbit);
	set_bit(EV_KEY,     input_dev->evbit);
	set_bit(BTN_TOUCH,  input_dev->keybit);
	set_bit(EV_SYN,     input_dev->evbit);


#if 1	//def TOUCH_VIRTUAL_KEYS
	set_bit(KEY_HOMEPAGE,  input_dev->keybit);
	set_bit(KEY_MENU,  input_dev->keybit);
	set_bit(KEY_BACK,  input_dev->keybit);
	set_bit(KEY_SEARCH,  input_dev->keybit);

	input_set_capability(input_dev, EV_KEY, KEY_HOMEPAGE);
	input_set_capability(input_dev, EV_KEY, KEY_MENU);
	input_set_capability(input_dev, EV_KEY, KEY_BACK);
	input_set_capability(input_dev, EV_KEY, KEY_SEARCH);
#endif

	err = input_register_device(input_dev);
	if (err < 0)
	{
		printk("ts %s: input device regist failed\n", __FUNCTION__);
		goto exit_input_register_failed;
	}

	TS_DBG("  it7260   ts %s:  %d", __FUNCTION__,__LINE__);

	//create work queue
	INIT_WORK(&it7260_ts->work, it7260_ts_work);
	it7260_ts->ts_work_queue= create_singlethread_workqueue("it7260_ts");	 
	TS_DBG("  it7260   ts %s:  %d", __FUNCTION__,__LINE__);

#ifdef CONFIG_HAS_EARLYSUSPEND
	//register early suspend
	it7260_ts->ts_early_suspend.suspend = it7260_ts_early_suspend;
	it7260_ts->ts_early_suspend.resume = it7260_ts_early_resume;
	register_early_suspend(&it7260_ts->ts_early_suspend);
#endif

	//register irq
	TS_DBG("IRQ number is %d", client->irq);

	if(client->irq > 0)
	{
		err =  request_irq(client->irq, it7260_ts_irq_handler, IRQF_TRIGGER_LOW ,client->name,it7260_ts);
		if (err <0)
		{
			printk("ts %s: IRQ setup failed %d\n", __FUNCTION__, err);
			goto irq_request_err;
		}
	}
	else 
	{
		printk("ts %s: client->irq<0\n", __FUNCTION__);
		goto irq_request_err;
	}

#ifdef TOUCH_VIRTUAL_KEYS
	virtual_keys_init();
#endif
	printk("ts %s: Probe Success!\n",__FUNCTION__);
	return 0;

irq_request_err:
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&it7260_ts->ts_early_suspend);
#endif
	free_irq(it7260_ts->client->irq, it7260_ts);
	if (it7260_ts->ts_work_queue)
		destroy_workqueue(it7260_ts->ts_work_queue);
	cancel_work_sync(&it7260_ts->work);
	input_unregister_device(input_dev);
exit_input_register_failed:
	input_free_device(input_dev);
exit_input_dev_allocate_failed:
	misc_deregister(&it7260_ts_device);
exit_device_register_failed:	
//exit_device_check_failed:
	remove_ctp_chip();
exit_check_chip_failed:
	it7260_irq = client->irq = 0;
	it7260_ts->client->irq = this_client->irq = 0;
	//sprd_free_gpio_irq(it7260_ts_setup.irq);
#ifdef I2C_BOARD_INFO_METHOD
	g_it7260_ts->client->irq = 0;
	gpio_free(pdata->irq_gpio_number);
	gpio_free(pdata->reset_gpio_number);
#else
	it7260_ts_setup.irq = 0;
	gpio_free(GPIO_TOUCH_IRQ);
	gpio_free(GPIO_TOUCH_RESET);
#endif
	this_client = NULL;
	i2c_set_clientdata(client, NULL);
	kfree(it7260_ts);
exit_request_memory_failed:
exit_check_functionality_failed:
	printk("%s: Probe Fail!\n",__func__);

	return err;
}

static int it7260_ts_remove(struct i2c_client *client)
{
	struct it7260_ts_t *it7260_ts = i2c_get_clientdata(client);

	printk("ts: %s\n", __FUNCTION__);

#ifdef TOUCH_VIRTUAL_KEYS
	virtual_keys_destroy();
#endif
	//free irq
	free_irq(it7260_ts->client->irq, it7260_ts);
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&it7260_ts->ts_early_suspend);
#endif
	if (it7260_ts->ts_work_queue)
	{	//remove queue
		flush_workqueue(it7260_ts->ts_work_queue);
		destroy_workqueue(it7260_ts->ts_work_queue);
	}
	cancel_work_sync(&it7260_ts->work);
	input_unregister_device(it7260_ts->input);
	input_free_device(it7260_ts->input);
	misc_deregister(&it7260_ts_device);
	remove_ctp_chip();
	it7260_irq = client->irq = 0;
	//sprd_free_gpio_irq(it7260_ts->client->irq);
#ifdef I2C_BOARD_INFO_METHOD
	if (g_it7260_ts) {
		gpio_free(g_it7260_ts->platform_data->irq_gpio_number);
		gpio_free(g_it7260_ts->platform_data->reset_gpio_number);
	}
#else
	it7260_ts_setup.irq = 0;
	gpio_free(GPIO_TOUCH_IRQ);
	gpio_free(GPIO_TOUCH_RESET);
#endif
	this_client = NULL;
	i2c_set_clientdata(client, NULL);
	kfree(it7260_ts);	

	return 0;
}

static const struct i2c_device_id it7260_ts_id[] = {
	{IT7260_TS_NAME, 0},
	{}
};

static struct i2c_driver it7260_ts_driver = {
	.driver = {
		.owner	= THIS_MODULE, 
		.name	= IT7260_TS_NAME,
	},

	.probe		= it7260_ts_probe,
	.remove		= it7260_ts_remove,
	.id_table	= it7260_ts_id,
};

#ifdef I2C_BOARD_INFO_METHOD
static int __init it7260_ts_init(void)
{
	printk("call it7260_ts_init\n");
	if(tp_device_id(0)!=0)
	{
		TS_INFO("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	return i2c_add_driver(&it7260_ts_driver);
}

static void __exit it7260_ts_exit(void)
{
	printk("call it7260_ts_exit\n");
	i2c_del_driver(&it7260_ts_driver);
}

#else
static int sprd_add_i2c_device(struct sprd_i2c_setup_data *i2c_set_data, struct i2c_driver *driver)
{
	struct i2c_board_info	info;
	struct i2c_adapter		*adapter;
	struct i2c_client		*client;
	int						ret,err;

	TS_DBG("%s : i2c_bus=%d; slave_address=0x%x; i2c_name=%s",__FUNCTION__,i2c_set_data->i2c_bus, \
		i2c_set_data->i2c_address, i2c_set_data->type);

	memset(&info, 0, sizeof(struct i2c_board_info));
	info.addr = i2c_set_data->i2c_address;
	strlcpy(info.type, i2c_set_data->type, I2C_NAME_SIZE);
	if(i2c_set_data->irq > 0)
		info.irq = i2c_set_data->irq;

	adapter = i2c_get_adapter( i2c_set_data->i2c_bus);
	if (!adapter) {
		printk("%s: can't get i2c adapter %d\n",
		__FUNCTION__,  i2c_set_data->i2c_bus);
		err = -ENODEV;
		goto err_adapter;
	}

	client = i2c_new_device(adapter, &info);
	if (!client) {
		printk("%s:  can't add i2c device at 0x%x\n",
		__FUNCTION__, (unsigned int)info.addr);
		err = -ENODEV;
		goto err_device;
	}

	i2c_put_adapter(adapter);

	ret = i2c_add_driver(driver);
	if (ret != 0) {
		printk("%s: can't add i2c driver\n", __FUNCTION__);
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
	TS_DBG("%s : slave_address=0x%x; i2c_name=%s",__FUNCTION__, client->addr, client->name);
	if (client!=NULL)
		i2c_unregister_device(client);
	i2c_del_driver(driver);
}

static int __init it7260_ts_init(void)
{
	printk("call it7260_ts_init\n");
	if(tp_device_id(0)!=0)
	{
		TS_INFO("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	it7260_ts_setup.i2c_bus = 0;
	it7260_ts_setup.i2c_address = IT7260_TS_ADDR;
	strcpy (it7260_ts_setup.type, IT7260_TS_NAME);
	return sprd_add_i2c_device(&it7260_ts_setup, &it7260_ts_driver);
}

static void __exit it7260_ts_exit(void)
{
	printk("call it7260_ts_exit\n");
	sprd_del_i2c_device(this_client, &it7260_ts_driver);
}
#endif

module_init(it7260_ts_init);
module_exit(it7260_ts_exit);

MODULE_AUTHOR("Feng Tan <Feng.Tan@revoview.com>");
MODULE_DESCRIPTION("TouchScreen IT7260 DRIVER");
MODULE_LICENSE("GPL");

