/*
 * File:         ap3212c_pls.c
 * Based on:
 * Author:       Kathy Lin <Kathy.Lin@LiteonSemi.com>
 *
 * 思路:
 *		中断中schedule 以下函数 ap3212c_pls_work,在这个函数中采样数据并上报.
 *		也即是说:als和ps均采用中断方式
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

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/earlysuspend.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/i2c/ap32xx_pls.h>
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
#include <linux/cdc_com.h>
#endif


#define PLS_DEBUG		0
#define DEBUG_I2C_DATA	0

#define AP3212C_PLS_ADC_LEVEL9

#if PLS_DEBUG
#define PLS_DBG(format, ...)	\
		printk("AP3212C " format "\n", ## __VA_ARGS__)
#else
#define PLS_DBG(format, ...)
#endif

#define PS_THD_VALUE_LOW		650
#define PS_THD_VALUE_HIGH		730

extern int sprd_3rdparty_gpio_pls_irq;

static atomic_t p_flag;
static atomic_t l_flag;

static struct i2c_client *this_client;
static int ap3212c_pls_opened=0;
/* Attribute */
static unsigned char suspend_flag=0; //0: sleep out; 1: sleep in


static ssize_t ap3212c_pls_show_suspend(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t ap3212c_pls_store_suspend(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t ap3212c_pls_show_version(struct device* cd,struct device_attribute *attr, char* buf);
#if 0 //def CONFIG_HAS_EARLYSUSPEND
static void ap3212c_pls_early_suspend(struct early_suspend *handler);
static void ap3212c_pls_early_resume(struct early_suspend *handler);
#endif
static int ap3212c_pls_write_data(unsigned char addr, unsigned char data);
static int ap3212c_pls_read_data(unsigned char addr, unsigned char *data);
static int ap3212c_pls_enable(SENSOR_TYPE type);
static int ap3212c_pls_disable(SENSOR_TYPE type);
//static void ap3212c_pls_report_init(void);



// AP3216C / AP3212C range
static int ap3216c_range[4] = {28152,7038,1760,440};

/*
static u8 ap3216c_reg[AP3216C_NUM_CACHABLE_REGS] = 
	{0x00,0x01,0x02,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
	 0x10,0x19,0x1a,0x1b,0x1c,0x1d,
	 0x20,0x21,0x22,0x23,0x24,0x28,0x29,0x2a,0x2b,0x2c,0x2d};
*/

//static u8 *reg_array;
//static int *range;
int reg_num = 0;

int cali = 100;

static DEVICE_ATTR(suspend, S_IRUGO | S_IWUSR, ap3212c_pls_show_suspend, ap3212c_pls_store_suspend);
static DEVICE_ATTR(version, S_IRUGO | S_IWUSR, ap3212c_pls_show_version, NULL);

// YC code--------------------------------------------
static int ap3212c_read_reg(u32 reg, uint8_t mask, uint8_t shift)
{
	unsigned char regvalue;
	//struct ap3212c_data *data = i2c_get_clientdata(client);
	ap3212c_pls_read_data(reg,&regvalue);
	//uint8_t idx = 0xff;

	//ADD_TO_IDX(reg,idx)
	//return (data->reg_cache[idx] & mask) >> shift;
	return (regvalue & mask) >> shift;
}

static int ap3212c_write_reg(u32 reg, uint8_t mask, uint8_t shift, uint8_t val)
{
	//unsigned char regvalue;
	//struct ap3212c_data *data = i2c_get_clientdata(client);
	//u8 idx = 0xff;
	int ret = 0;
	u8 tmp;

	//ADD_TO_IDX(reg,idx)
	//if (idx >= reg_num)
	//	return -EINVAL;

	//mutex_lock(&data->lock);
	ap3212c_pls_read_data(reg,&tmp);
	//tmp = data->reg_cache[idx];
	tmp &= ~mask;
	tmp |= val << shift;

	//ret = i2c_smbus_write_byte_data(client, reg, tmp);
	ret = ap3212c_pls_write_data(reg, tmp);

	//if (!ret)
	//	data->reg_cache[idx] = tmp;

	//mutex_unlock(&data->lock);
	return ret;
}
//end YC code---------------------------------------------

#if 0
/* mode from YC code add by kathy*/
static int ap3212c_get_mode(void)
{
	int ret;

	ret = ap3212c_read_reg(AP3212C_MODE_COMMAND,
		AP3212C_MODE_MASK, AP3212C_MODE_SHIFT);
	return ret;
}
#endif

static int ap3212c_set_mode(int mode)
{
	int ret;

	ret = ap3212c_write_reg(AP3212C_MODE_COMMAND,
				AP3212C_MODE_MASK, AP3212C_MODE_SHIFT, mode);
	return ret;
}

#if 0
static int ap3212c_get_aif(void)
{
	int ret;

	ret = ap3212c_read_reg(AP3212C_AIF_COMMAND,
			AP3212C_AIF_MASK, AP3212C_AIF_SHIFT);
	return ret;
}
#endif

static int ap3212c_set_aif(int mode)
{
	int ret;

	ret = ap3212c_write_reg(AP3212C_AIF_COMMAND,
			AP3212C_AIF_MASK, AP3212C_AIF_SHIFT, mode);
	return ret;
}

#if 0
static int ap3212c_get_pif(void)
{
	int ret;

	ret = ap3212c_read_reg(AP3212C_PIF_COMMAND,
			AP3212C_PIF_MASK, AP3212C_PIF_SHIFT);
	return ret;
}
#endif

static int ap3212c_set_pif(int mode)
{
	int ret;

	ret = ap3212c_write_reg(AP3212C_PIF_COMMAND,
			AP3212C_PIF_MASK, AP3212C_PIF_SHIFT, mode);
	return ret;
}

static int ap3212c_get_range(void)
{
	u8 idx = ap3212c_read_reg(AP3212C_RAN_COMMAND,
		AP3212C_RAN_MASK, AP3212C_RAN_SHIFT); 
	PLS_DBG("%s result=%d", __func__,ap3216c_range[idx]);
	return ap3216c_range[idx];
}

#if 0
static int ap3212c_set_range(int range)
{
	return ap3212c_write_reg(AP3212C_RAN_COMMAND,
		AP3212C_RAN_MASK, AP3212C_RAN_SHIFT, range);;
}

static int ap3212c_get_adc_value(void)
{
	//struct ap3212c_data *data = i2c_get_clientdata(client);
	unsigned int lsb, msb, range;
	unsigned long tmp;

	//mutex_lock(&data->lock);
	lsb = ap3212c_read_reg(AP3212C_ADC_LSB,0xFF,0);
	//lsb = i2c_smbus_read_byte_data(client, AP3212C_ADC_LSB);

	if (lsb < 0) {
		//mutex_unlock(&data->lock);
		return lsb;
	}

	msb = ap3212c_read_reg(AP3212C_ADC_MSB,0xFF,0);
	//mutex_unlock(&data->lock);

	if (msb < 0)
		return msb;

	range = ap3212c_get_range();

	tmp = (((msb << 8) | lsb) * range) >> 16;
	tmp *= cali;

	return (tmp / 100);
}

static int ap3212c_get_object(void)
{
	//struct ap3212c_data *data = i2c_get_clientdata(client);
	int val;

	//mutex_lock(&data->lock);
	val = ap3212c_read_reg(AP3212C_OBJ_COMMAND,AP3212C_OBJ_MASK,AP3212C_OBJ_SHIFT);
	//val &= AP3212B_OBJ_MASK;

	//mutex_unlock(&data->lock);

	return val /*>> AP3212B_OBJ_SHIFT*/;
}

static int ap3212c_get_intstat(void)
{
	int val;
	
	val = ap3212c_read_reg(AP3212C_INT_COMMAND,AP3212C_INT_MASK,AP3212C_INT_SHIFT);
	//val &= AP3212C_INT_MASK;

	return val /*>> AP3212B_INT_SHIFT*/;
}


static int ap3212c_get_px_value(void)
{
	//struct ap3212c_data *data = i2c_get_clientdata(client);
	int lsb, msb;

	//mutex_lock(&data->lock);
	lsb = ap3212c_read_reg( AP3212C_PX_LSB, 0xFF,0);

	if (lsb < 0) {
		//mutex_unlock(&data->lock);
		return lsb;
	}

	msb = ap3212c_read_reg( AP3212C_PX_MSB, 0xFF,0);
	//mutex_unlock(&data->lock);

	if (msb < 0)
		return msb;

	return (u32)(((msb & AP3212C_PX_MSB_MASK) << 4) | (lsb & AP3212C_PX_LSB_MASK));
}
#endif

//---------------------------------------------------------------------------------------
static ssize_t ap3212c_pls_show_suspend(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;

	if(suspend_flag==1)
		sprintf(buf, "AP3212C Resume\n");
	else
		sprintf(buf, "AP3212C Suspend\n");
	
	ret = strlen(buf) + 1;

	return ret;
}

static ssize_t ap3212c_pls_store_suspend(struct device* cd, struct device_attribute *attr,
		       const char* buf, size_t len)
{
	unsigned long on_off = simple_strtoul(buf, NULL, 10);
	suspend_flag = on_off;
	
	if(on_off==1)
	{
		printk("AP3212C Entry Resume\n");
		ap3212c_pls_enable(AP3212C_PLS_BOTH);
	}
	else
	{
		printk("AP3212C Entry Suspend\n");
		ap3212c_pls_disable(AP3212C_PLS_BOTH);
	}
	
	return len;
}

static ssize_t ap3212c_pls_show_version(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;
#if PLS_DEBUG
	unsigned char	int_status,enable,datal,datah,pobj;
	unsigned char	als_config=0,ps_config=0;
	int				adata=0,pdata=0;

	ap3212c_pls_read_data(AP3212C_MODE_COMMAND,&enable);
	ap3212c_pls_read_data(AP3212C_INT_COMMAND,&int_status);
	ap3212c_pls_read_data(AP3212C_PX_CONFIGURE,&ps_config);
	ap3212c_pls_read_data(AP3212C_ALS_CONFIGURE,&als_config);
	ap3212c_pls_read_data(AP3212C_ADC_LSB,&datal);
	ap3212c_pls_read_data(AP3212C_ADC_MSB,&datah);
	adata = datah*256+datal;
	ap3212c_pls_read_data(AP3212C_PX_LSB,&datal);
	ap3212c_pls_read_data(AP3212C_PX_MSB,&datah);
	pdata = ((datah&0x3F)<<4) + (datal&0x0F);
	pobj=datah >> 7 ;

	PLS_DBG("%s: enable=0x%x; int_status=0x%x; ps_config=0x%x,als_config=0x%x; als data=%d proximity data=%d obj=%d",
		__func__, enable, int_status, ps_config, als_config, adata, pdata, pobj);
#endif

	sprintf(buf, "AP3212C");	
	ret = strlen(buf) + 1;

	return ret;
}

static int ap3212c_pls_create_sysfs(struct i2c_client *client)
{
	int err;
	struct device *dev = &(client->dev);

	PLS_DBG("%s", __func__);
	
	err = device_create_file(dev, &dev_attr_suspend);
	err = device_create_file(dev, &dev_attr_version);

	return err;
}

static int ap3212c_pls_remove_sysfs(struct i2c_client *client)
{
	struct device *dev = &(client->dev);

	PLS_DBG("%s", __func__);
	
	device_remove_file(dev, &dev_attr_suspend);
	device_remove_file(dev, &dev_attr_version);
}

static int ap3212c_pls_enable(SENSOR_TYPE type)
{
	int ret;
	int pxy_flag, apls_flag; // mickyliang

	PLS_DBG("%s: type=%d", __func__, type);
	switch(type) {
		case AP3212C_PLS_ALPS:
			// mickyliang ++
			/*
			ret=AP3212C_PLS_ALPS_ACTIVE;
			ap3212c_set_mode(ret);
			*/
			pxy_flag = atomic_read(&p_flag);
			if( pxy_flag==1){
				ret=AP3212C_PLS_BOTH_ACTIVE;
				ap3212c_set_mode(ret);
			}else{
				ret=AP3212C_PLS_ALPS_ACTIVE;
				ap3212c_set_mode(ret);
			}
			
			// mickyliang --
			break;
		case AP3212C_PLS_PXY:
			// mickyliang ++
			/*
			ret=AP3212C_PLS_PXY_ACTIVE;
			ap3212c_set_mode(ret);
			*/
			apls_flag = atomic_read(&l_flag);
			if(apls_flag==1){
				ret=AP3212C_PLS_BOTH_ACTIVE;
				ap3212c_set_mode(ret);
			}else{
				ret=AP3212C_PLS_PXY_ACTIVE;
				ap3212c_set_mode(ret);
			}
			// mickyliang --
			break;
		case AP3212C_PLS_BOTH:
			ret=AP3212C_PLS_BOTH_ACTIVE;
			ap3212c_set_mode(ret);
			break;
		default:
			ret=-1;
			break;
	}
	PLS_DBG("%s: ret=%d", __func__, ret);
	return ret;
}

static int ap3212c_pls_disable(SENSOR_TYPE type)
{
	int ret;
	int pxy_flag, apls_flag;

	PLS_DBG("%s: type=%d", __func__, type);
	switch(type) {
		case AP3212C_PLS_ALPS:
			pxy_flag = atomic_read(&p_flag);
			if( pxy_flag==0){
				ret=AP3212C_PLS_BOTH_DEACTIVE;
				ap3212c_set_mode(ret);
			}else{
				ret=AP3212C_PLS_PXY_ACTIVE;
				ap3212c_set_mode(ret);
			}

			break;
		case AP3212C_PLS_PXY:
			apls_flag = atomic_read(&l_flag);
			if(apls_flag==0){
				ret=AP3212C_PLS_BOTH_DEACTIVE;
				ap3212c_set_mode(ret);
			}else{
				ret=AP3212C_PLS_ALPS_ACTIVE;
				ap3212c_set_mode(ret);
			}
			break;
		case AP3212C_PLS_BOTH:
			ret=AP3212C_PLS_BOTH_DEACTIVE;
			ap3212c_set_mode(ret);
			break;
		default:
			ret = -1;
			break;

	}
	PLS_DBG("%s: ret=%d", __func__, ret);
	return ret;

}

static int ap3212c_pls_open(struct inode *inode, struct file *file)
{
	PLS_DBG("%s\n", __func__);
	if (ap3212c_pls_opened)
		return -EBUSY;
	ap3212c_pls_opened = 1;
	return 0;
}

static int ap3212c_pls_release(struct inode *inode, struct file *file)
{
	PLS_DBG("%s", __func__);
	ap3212c_pls_opened = 0;
	return ap3212c_pls_disable(AP3212C_PLS_BOTH);
}

static long ap3212c_pls_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int flag;
	unsigned char data;
	
	PLS_DBG("%s: cmd %d", __func__, _IOC_NR(cmd));

	//get ioctl parameter
	switch (cmd) {
	case LTR_IOCTL_SET_PFLAG:
	case LTR_IOCTL_SET_LFLAG:
		if (copy_from_user(&flag, argp, sizeof(flag))) {
			return -EFAULT;
		}
		if (flag < 0 || flag > 1) {
			return -EINVAL;
		}
		PLS_DBG("%s: set flag=%d", __func__, flag);
		break;
	default:
		break;
	} 

	//handle ioctl
	switch (cmd) {
	case LTR_IOCTL_GET_PFLAG:
		flag = atomic_read(&p_flag);
		break;
		
	case LTR_IOCTL_GET_LFLAG:
		flag = atomic_read(&l_flag);
		break;

	case LTR_IOCTL_GET_DATA:
		break;
		
	case LTR_IOCTL_SET_PFLAG:
		atomic_set(&p_flag, flag);	
		if(flag==1){
			ap3212c_pls_enable(AP3212C_PLS_PXY);
		}
		else if(flag==0) {
			ap3212c_pls_disable(AP3212C_PLS_PXY);
		}
		break;
		
	case LTR_IOCTL_SET_LFLAG:
		atomic_set(&l_flag, flag);
		if(flag==1){
			ap3212c_pls_enable(AP3212C_PLS_ALPS);
		}
		else if(flag==0) {
			ap3212c_pls_disable(AP3212C_PLS_ALPS);
		}
		break;
		
	default:
		pr_err("%s: invalid cmd %d\n", __func__, _IOC_NR(cmd));
		return -EINVAL;
	}

	//report ioctl
	switch (cmd) {
	case LTR_IOCTL_GET_PFLAG:
	case LTR_IOCTL_GET_LFLAG:
		if (copy_to_user(argp, &flag, sizeof(flag))) {
			return -EFAULT;
		}
		PLS_DBG("%s: get flag=%d", __func__, flag);
		break;
		
	case LTR_IOCTL_GET_DATA:
		//ap3212c_pls_read_data(AP3212C_PLS_REG_DATA,&data);
		if (copy_to_user(argp, &data, sizeof(data))) {
			return -EFAULT;
		}
		PLS_DBG("%s: get data=%d", __func__, flag);
		break;
		
	default:
		break;
	}

	return 0;
}

static struct file_operations ap3212c_pls_fops = {
	.owner				= THIS_MODULE,
	.open				= ap3212c_pls_open,
	.release			= ap3212c_pls_release,
	.unlocked_ioctl		= ap3212c_pls_ioctl,
};

static struct miscdevice ap3212c_pls_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = AP3212C_PLS_DEVICE,
	.fops = &ap3212c_pls_fops,
};

static int ap3212c_pls_rx_data(char *buf, int len)
{
	uint8_t i;
#if DEBUG_I2C_DATA
	char addr = buf[0];
#endif
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
			.len	= len,
			.buf	= buf,
		}
	};

	for (i = 0; i < AP3212C_PLS_RETRY_COUNT; i++) {
		if (i2c_transfer(this_client->adapter, msgs, 2) >= 0) {
			break;
		}
		mdelay(10);
	}

	if (i >= AP3212C_PLS_RETRY_COUNT) {
		return -EIO;
	}


#if DEBUG_I2C_DATA
 	printk("ts %s receive to address[0x%x] data_len[%d] retry_times[%d]: \n", __func__, addr, len,i);
	for(i=0; i<len; i++)
		printk("%x, ",buf[i]);
	printk("\n");
#endif	
 

	return 0;
}

static int ap3212c_pls_tx_data(char *buf, int len)
{
	uint8_t i;
#if DEBUG_I2C_DATA
	char addr=buf[0];
#endif
	struct i2c_msg msg[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= len,
			.buf	= buf,
		}
	};
	
	for (i = 0; i < AP3212C_PLS_RETRY_COUNT; i++) {
		if (i2c_transfer(this_client->adapter, msg, 1) >= 0) {
			break;
		}
		mdelay(10);
	}

	if (i >= AP3212C_PLS_RETRY_COUNT) {
		return -EIO;
	}

#if DEBUG_I2C_DATA
	printk("ts %s send to address[0x%x] data_len[%d] retry_times[%d]: \n", __func__, addr, len,i);
	for(i=0; i<len-1; i++)
		printk("%x, ",buf[i+1]);
	printk("\n");
#endif	

	
	return 0;
}

/*******************************************************************************
* Function    :  ap3212c_pls_write_data
* Description :  write data to IC
* Parameters  :  addr: register address, data: register data
* Return      :  none
*******************************************************************************/
static int ap3212c_pls_write_data(unsigned char addr, unsigned char data)
{
	unsigned char buf[2];
	buf[0]=addr;
	buf[1]=data;
	return ap3212c_pls_tx_data(buf, 2); 
}

/*******************************************************************************
* Function    :  ap3212c_pls_read_data
* Description :  read data from IC
* Parameters  :  addr: register address, data: read data
* Return      :    status
*******************************************************************************/
static int ap3212c_pls_read_data(unsigned char addr, unsigned char *data)
{
	int ret;
	unsigned char buf;
	
	buf=addr;
	ret = ap3212c_pls_rx_data(&buf, 1);
	*data = buf;
	
	return ret;;
}


#if PLS_DEBUG
static void ap3212c_pls_reg_dump(void)
{
	unsigned char config,intstatus,alslsb,alsmsb,pslsb,psmsb,alsconfig,psconfig;
	ap3212c_pls_read_data(AP3212C_MODE_COMMAND,&config);
	ap3212c_pls_read_data(AP3212C_INT_COMMAND,&intstatus);
	ap3212c_pls_read_data(AP3212C_ADC_LSB,&alslsb);
	ap3212c_pls_read_data(AP3212C_ADC_MSB,&alsmsb);
	ap3212c_pls_read_data(AP3212C_PX_LSB,&pslsb);
	ap3212c_pls_read_data(AP3212C_PX_MSB,&psmsb);
	ap3212c_pls_read_data(AP3212C_RAN_COMMAND,&alsconfig);
	ap3212c_pls_read_data(AP3212C_PIF_COMMAND,&psconfig);

	PLS_DBG("%s: config=0x%x,intstatus=0x%x,alslsb=0x%x,alsmsb=0x%x,pslsb=0x%x,psmsb=0x%x,alsconfig=0x%x, \
		psconfig=0x%x", __func__,  config,intstatus,alslsb,alsmsb,pslsb,psmsb,alsconfig,psconfig);
}
#endif

/*******************************************************************************
* Function    :  ap3212c_pls_reg_init
* Description :  set ap3212c registers
* Parameters  :  none
* Return      :  void
*******************************************************************************/
static int ap3212c_pls_reg_init(void)
{
	int ret=0;
	//enter power down mode
/*	if(ap3212c_pls_write_data(AP3212C_PLS_REG_CONFIG,AP3212C_PLS_BOTH_DEACTIVE)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
	}

	//set window loss
	if(ap3212c_pls_write_data(AP3212C_PLS_REG_DLS_WIN,0x0D)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
	}

	//set time control
	if(ap3212c_pls_write_data(AP3212C_PLS_REG_TIME_CTRL,0x11)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
	}	

	//set alps level
#if defined(AP3212C_PLS_ADC_LEVEL64)
	ap3212c_pls_write_data(AP3212C_PLS_REG_DLS_CTRL,0xA0);
#elif defined(AP3212C_PLS_ADC_LEVEL9)
	ap3212c_pls_write_data(AP3212C_PLS_REG_DLS_CTRL,0x40);
#elif defined(AP3212C_PLS_ADC_LEVEL5)
	ap3212c_pls_write_data(AP3212C_PLS_REG_DLS_CTRL,0x20);
#endif

	//set dps distance
	if(ap3212c_pls_write_data(AP3212C_PLS_REG_DPS_CTRL,0x10)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
	}	
*/
	if(ap3212c_pls_write_data(AP3212C_ALS_LTHL ,0x0)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	
	if(ap3212c_pls_write_data(AP3212C_ALS_LTHH ,0x0)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	
	if(ap3212c_pls_write_data(AP3212C_ALS_HTHL ,0x0)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	
	if(ap3212c_pls_write_data(AP3212C_ALS_HTHH ,0x0)<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	
	ap3212c_set_aif( 0x02);
	ap3212c_set_pif( 0x01);
	if(ap3212c_pls_write_data(AP3212C_PX_LTHL ,((PS_THD_VALUE_HIGH>>8)&0x3))<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	
	if(ap3212c_pls_write_data(AP3212C_PX_LTHH ,(PS_THD_VALUE_HIGH&0xFF))<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	
	if(ap3212c_pls_write_data(AP3212C_PX_HTHL ,((PS_THD_VALUE_LOW>>8)&0x3))<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	
	if(ap3212c_pls_write_data(AP3212C_PX_HTHH ,(PS_THD_VALUE_LOW&0xFF))<0) {
		printk("%s: I2C Write Config Failed\n", __func__);
		ret = -EIO;
		goto exit;
	}	

#if 1	// Alex.shi reduce gain 
	// PS Configuration
	//ret = ap3212c_pls_write_data(AP3212C_PX_CONFIGURE,0x01);
	// led turn 3 pulse , 100%
	ret = ap3212c_pls_write_data(AP3212C_PX_LED,0x13);
#else
	// led turn 3 pulse , 100%
	ret = ap3212c_pls_write_data(AP3212C_PX_LED,0x33);
#endif
	// ALS interrupt filter 0x02--. 8 TIMES 
	ret = ap3212c_pls_write_data(AP3212C_ALS_CONFIGURE,0x02);
	// ALS Calibration Register 放大adata 80-256
	ret = ap3212c_pls_write_data(AP3212C_ALS_CALI,0xC0);

#if PLS_DEBUG
	ap3212c_pls_reg_dump();
#endif

exit:
	return ret;
}

#if 0 //def CONFIG_HAS_EARLYSUSPEND
/*******************************************************************************
* Function    :  ap3212c_pls_early_suspend
* Description :  cancel the delayed work and put ts to shutdown mode
* Parameters  :  handler
* Return      :    none
*******************************************************************************/
static void ap3212c_pls_early_suspend(struct early_suspend *handler)
{
	PLS_DBG("%s\n", __func__);
#if 0
	ap3212c_pls_disable(AP3212C_PLS_BOTH);
#endif
}

/*******************************************************************************
* Function    :  ap3212c_pls_early_resume
* Description :  ts re-entry the normal mode and schedule the work, there need to be  a litte time 
                      for ts ready
* Parameters  :  handler
* Return      :    none
*******************************************************************************/
static void ap3212c_pls_early_resume(struct early_suspend *handler)
{	
	PLS_DBG("%s\n", __func__);
#if 0
	ap3212c_pls_enable(AP3212C_PLS_BOTH);
#endif
}
#endif

#if 0
static void ap3212c_pls_report_init(void)
{
	ap3212c_pls_t *ap3212c_pls = (ap3212c_pls_t *)i2c_get_clientdata(this_client);
	PLS_DBG("%s\n",__func__);
	input_report_abs(ap3212c_pls->input, ABS_DISTANCE, 1); //report far
	input_sync(ap3212c_pls->input);
}
#endif

/*******************************************************************************
* Function    :  ap3212c_pls_report_dps
* Description :  reg data,input dev
* Parameters  :  report proximity sensor data to input system
* Return      :  none
*******************************************************************************/
static void ap3212c_pls_report_dps(unsigned char data, struct input_dev *input)
{
	unsigned char dps_data = (data==1) ? 0: 1;
	PLS_DBG("%s: proximity=%d", __func__, dps_data);
	
	input_report_abs(input, ABS_DISTANCE, dps_data);
	input_sync(input);
}

/*******************************************************************************
* Function    :  ap3212c_pls_report_dls
* Description :  reg data,input dev
* Parameters  :  report light sensor data to input system
* Return      :  none
*******************************************************************************/
static void ap3212c_pls_report_dls(int data, struct input_dev *input)
{
	int lux; 

	lux = data* ap3212c_get_range() >> 16;
	PLS_DBG("%s: adc=%d, lux=%d", __func__, data, lux);
	input_report_abs(input, ABS_MISC, lux);
	input_sync(input);
}

/*******************************************************************************
* Function    :  ap3212c_pls_work
* Description :  handler current data and report coordinate
* Parameters  :  work
* Return      :    none
*******************************************************************************/

static void ap3212c_pls_work(struct work_struct *work)
{
	unsigned char int_status,/* data,*/ enable,datal,datah,pobj;
	int adata=0,pdata=0;
	ap3212c_pls_t *pls = container_of(work, ap3212c_pls_t, work);

	//ap3212c_pls_read_data(AP3212C_PLS_REG_CONFIG,&enable);
	//ap3212c_pls_read_data(AP3212C_PLS_REG_INT_STATUS,&int_status);
	//ap3212c_pls_read_data(AP3212C_PLS_REG_DATA,&data);
	ap3212c_pls_read_data(AP3212C_MODE_COMMAND,&enable);
	ap3212c_pls_read_data(AP3212C_INT_COMMAND,&int_status);
	ap3212c_pls_read_data(AP3212C_ADC_LSB,&datal);
	ap3212c_pls_read_data(AP3212C_ADC_MSB,&datah);
	adata = datah*256+datal;
	ap3212c_pls_read_data(AP3212C_PX_LSB,&datal);
	ap3212c_pls_read_data(AP3212C_PX_MSB,&datah);
	pdata = ((datah&0x3F)<<4) + (datal&0x0F);
	pobj=datah >> 7 ;

	PLS_DBG("\033[33;1m %s: enable=0x%x; int_status=0x%x; als data=%d proximity data=%d obj=%d\033[m",
		__func__, enable, int_status, adata, pdata, pobj);
	//PLS_DBG("\033[33;1m %s: enable=0x%x; int_status=0x%x; data=0x%x \033[m", __func__, enable, int_status, data);
        
	switch(int_status & AP3212C_PLS_BOTH_ACTIVE) {
		case AP3212C_PLS_PXY_ACTIVE:
			ap3212c_pls_report_dps(pobj, pls->input);
			break;

		case AP3212C_PLS_ALPS_ACTIVE:
			ap3212c_pls_report_dls(adata, pls->input);
			break;

		case AP3212C_PLS_BOTH_ACTIVE:
			ap3212c_pls_report_dps(pobj, pls->input);
			ap3212c_pls_report_dls(adata, pls->input);
			break;

		default:
			break;
	}
	
	enable_irq(pls->client->irq);
	PLS_DBG("%s: int_status=%d\n", __func__, int_status);
}


/*******************************************************************************
* Function    :  ap3212c_pls_irq_handler
* Description :  handle ts irq
* Parameters  :  handler
* Return      :    none
*******************************************************************************/
static irqreturn_t ap3212c_pls_irq_handler(int irq, void *dev_id)
{
	ap3212c_pls_t *pls = (ap3212c_pls_t *)dev_id;

	disable_irq_nosync(pls->client->irq);
	queue_work(pls->ltr_work_queue,&pls->work);
	return IRQ_HANDLED;
}

static void ap3212c_pls_pininit(int irq_pin)
{
	printk(KERN_INFO "%s [irq=%d]\n",__func__, irq_pin);
	gpio_request(irq_pin, AP3212C_PLS_IRQ_PIN);
}

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
static ssize_t cdc_als_enable_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int flag;
	flag = atomic_read(&l_flag);

	flag = flag ? 1:0;

	PLS_DBG("%s:als_enable = 0x%x\n", __FUNCTION__, flag);

	return sprintf(buf, "%d\n", flag);
}

static ssize_t cdc_als_enable_store(struct device *dev,
				struct device_attribute *attr, const char *buf, size_t count)
{
	int flag=simple_strtoul(buf, NULL, 10), flag_old=atomic_read(&l_flag);

	PLS_DBG("%s:als_enable = 0x%x -> 0x%x\n", __FUNCTION__, flag_old, flag);
	if (flag < 0 || flag > 1) {
		return -EINVAL;
	}

	atomic_set(&l_flag, flag);
	if(flag==1){
		ap3212c_pls_enable(AP3212C_PLS_ALPS);
	}
	else if(flag==0) {
		ap3212c_pls_disable(AP3212C_PLS_ALPS);
	}

	return 0;
}

static struct device_attribute cdc_als_enable_attribute = __ATTR(als_enable,0666,cdc_als_enable_show,cdc_als_enable_store);

static ssize_t cdc_ps_enable_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int flag;
	flag = atomic_read(&p_flag);

	flag = flag ? 1:0;

	PLS_DBG("%s:ps_enable = 0x%x\n", __FUNCTION__, flag);

	return sprintf(buf, "%d\n", flag);
}

static ssize_t cdc_ps_enable_store(struct device *dev,
				struct device_attribute *attr, const char *buf, size_t count)
{
	int flag=simple_strtoul(buf, NULL, 10), flag_old=atomic_read(&p_flag);

	PLS_DBG("%s:ps_enable = 0x%x -> 0x%x\n", __FUNCTION__, flag_old, flag);
	if (flag < 0 || flag > 1) {
		return -EINVAL;
	}

	atomic_set(&p_flag, flag);
	if(flag==1){
		ap3212c_pls_enable(AP3212C_PLS_PXY);
	}
	else if(flag==0) {
		ap3212c_pls_disable(AP3212C_PLS_PXY);
	}

	return 0;
}

static struct device_attribute cdc_ps_enable_attribute = __ATTR(ps_enable,0666,cdc_ps_enable_show,cdc_ps_enable_store);

static ssize_t chip_id_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", 0x3212);
}

static struct device_attribute cdc_chip_id_attribute = __ATTR(chip_id,0666,chip_id_show,NULL);


static struct attribute* sensors_pls_cdc_attrs [] =
{
	&cdc_als_enable_attribute.attr,
	&cdc_ps_enable_attribute.attr,
	&cdc_chip_id_attribute.attr,
	NULL,
};

static const struct attribute_group pls_cdc_attrs_group = {
	.attrs = sensors_pls_cdc_attrs,
};

static int check_plsensor_chip(void)
{
	CDC_Plsensor_Device_Id(0x3212);

	return 0;
}

static int remove_plsensor_chip(void)
{
	CDC_Plsensor_Device_Id(0xFFFF);

	return 0;
}
#endif

static int ap3212c_pls_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int		err = 0;
	struct input_dev	*input_dev;
	ap3212c_pls_t		*ap3212c_pls;
	struct ap3212c_pls_platform_data	*pdata = client->dev.platform_data;

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(CDC_Plsensor_Device_Id(0)!=0)
	{
		printk("PlSensor(0x%x)Exist!", CDC_Plsensor_Device_Id(0));
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
#endif

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		printk("%s: functionality check failed\n", __func__);
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	ap3212c_pls = kzalloc(sizeof(ap3212c_pls_t), GFP_KERNEL);
	if (!ap3212c_pls)
	{
		printk("%s: request memory failed\n", __func__);
		err= -ENOMEM;
		goto exit_check_functionality_failed;
	}

	ap3212c_pls->client = client;
	this_client = client;
	i2c_set_clientdata(client, ap3212c_pls);

	/*pin init*/
	ap3212c_pls_pininit(pdata->irq_gpio_number);

	/*get irq*/
	client->irq = gpio_to_irq(pdata->irq_gpio_number);
	ap3212c_pls->irq = client->irq;
	PLS_DBG("I2C name=%s, addr=0x%x, gpio=%d, irq=%d", client->name, client->addr, \
		pdata->irq_gpio_number, client->irq);

	//init AP3212C_PLS
	if(ap3212c_pls_reg_init()<0)
	{
		printk("%s: device init failed\n", __func__);
		err=-1;
		goto exit_device_init_failed;
	}

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(check_plsensor_chip()<0)
	{
		err = -ENODEV;
		printk("Fail to find ap32xx");
		goto exit_check_chip_failed;
	}
#endif

	//register device
	err = misc_register(&ap3212c_pls_device);
	if (err) {
		printk("%s: ap3212c_pls_device register failed\n", __func__);
		goto exit_check_chip_failed;
	}

	// register input device 
	input_dev = input_allocate_device();
	if (!input_dev) {
		printk("%s: input allocate device failed\n", __func__);
		err = -ENOMEM;
		goto exit_input_dev_allocate_failed;
	}

	ap3212c_pls->input = input_dev;

	input_dev->name = AP3212C_PLS_INPUT_DEV;
	input_dev->phys  = AP3212C_PLS_INPUT_DEV;
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &client->dev;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0010;

	__set_bit(EV_ABS, input_dev->evbit);	
	//for proximity
	input_set_abs_params(input_dev, ABS_DISTANCE, 0, 1, 0, 0);
	//for lightsensor
	input_set_abs_params(input_dev, ABS_MISC, 0, 100001, 0, 0);

	err = input_register_device(input_dev);
	if (err < 0)
	{
	    printk("%s: input device regist failed\n", __func__);
	    goto exit_input_register_failed;
	}

	//create work queue
	INIT_WORK(&ap3212c_pls->work, ap3212c_pls_work);
	ap3212c_pls->ltr_work_queue= create_singlethread_workqueue(AP3212C_PLS_DEVICE);	 

#if 0 //def CONFIG_HAS_EARLYSUSPEND
	//register early suspend
	ap3212c_pls->ltr_early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ap3212c_pls->ltr_early_suspend.suspend = ap3212c_pls_early_suspend;
	ap3212c_pls->ltr_early_suspend.resume = ap3212c_pls_early_resume;
	register_early_suspend(&ap3212c_pls->ltr_early_suspend);
#endif

	if(client->irq > 0) 
	{  
		err =  request_irq(client->irq, ap3212c_pls_irq_handler, IRQ_TYPE_LEVEL_LOW, client->name,ap3212c_pls);
		if (err <0)
		{
			printk("%s: IRQ setup failed %d\n", __func__, err);
			goto irq_request_err;
		}		
	}

	//create attribute files
	err = ap3212c_pls_create_sysfs(client);
	if (err)
	{
	    printk("%s: ap3212c_pls_create_sysfs failed\n", __func__);
	    goto exit_create_sysfs_failed;
	}

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	err = sysfs_create_group(&(input_dev->dev.kobj), &pls_cdc_attrs_group);
	if (err) {
		dev_err(&(input_dev->dev), "create device file failed!\n");
		err = -EINVAL;
		goto err_create_sysfs;
	}
#endif

	printk("%s: Probe Success!\n",__func__);

	return 0;

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
err_create_sysfs:
	sysfs_remove_group(&(input_dev->dev.kobj), &pls_cdc_attrs_group);
	ap3212c_pls_remove_sysfs(client);
#endif
exit_create_sysfs_failed:
	free_irq(client->irq, ap3212c_pls);
irq_request_err:
#if 0 //def CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ap3212c_pls->ltr_early_suspend);
#endif
	flush_workqueue(ap3212c_pls->ltr_work_queue);
	destroy_workqueue(ap3212c_pls->ltr_work_queue);
	input_unregister_device(input_dev);
exit_input_register_failed:
	input_free_device(input_dev);
exit_input_dev_allocate_failed:
	misc_deregister(&ap3212c_pls_device);
exit_check_chip_failed:
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	remove_plsensor_chip();
#endif
exit_device_init_failed:
	ap3212c_pls->irq = client->irq = -1;
	gpio_free(pdata->irq_gpio_number);
	i2c_set_clientdata(client, NULL);
	this_client = NULL;
	ap3212c_pls->client = NULL;
	kfree(ap3212c_pls);	
exit_check_functionality_failed:
	printk("%s: Probe Fail!\n",__func__);
	return err;
}

static int ap3212c_pls_remove(struct i2c_client *client)
{
	ap3212c_pls_t						*ap3212c_pls = i2c_get_clientdata(client);
	struct ap3212c_pls_platform_data	*pdata = client->dev.platform_data;

	printk("%s\n", __func__);

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	sysfs_remove_group(&(ap3212c_pls->input->dev.kobj), &pls_cdc_attrs_group);
#endif
	ap3212c_pls_remove_sysfs(client);
	free_irq(client->irq, ap3212c_pls);
#if 0 //def CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ap3212c_pls->ltr_early_suspend);
#endif
	flush_workqueue(ap3212c_pls->ltr_work_queue);
	destroy_workqueue(ap3212c_pls->ltr_work_queue);
	input_unregister_device(ap3212c_pls->input);
	input_free_device(ap3212c_pls->input);
	misc_deregister(&ap3212c_pls_device);
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	remove_plsensor_chip();
#endif
	ap3212c_pls->irq = client->irq = -1;
	gpio_free(pdata->irq_gpio_number);
	i2c_set_clientdata(client, NULL);
	this_client = NULL;
	ap3212c_pls->client = NULL;
	kfree(ap3212c_pls);	

	return 0;
}

static const struct i2c_device_id ap3212c_pls_id[] = {
	{ AP3212C_PLS_DEVICE, 2 },
	{ }
};
/*----------------------------------------------------------------------------*/
static struct i2c_driver ap3212c_pls_driver = {
    .driver = {
        .owner = THIS_MODULE, 
        .name  = AP3212C_PLS_DEVICE,
    },
	.probe      = ap3212c_pls_probe,
	.remove     = ap3212c_pls_remove,
	.id_table = ap3212c_pls_id,
};
/*----------------------------------------------------------------------------*/

static int __init ap3212c_pls_init(void)
{
	printk("%s\n", __func__);
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(CDC_Plsensor_Device_Id(0)!=0)
	{
		printk("PlSensor(0x%x)Exist!", CDC_Plsensor_Device_Id(0));
		return -ENODEV;
	}
#endif
	return i2c_add_driver(&ap3212c_pls_driver);
}

static void __exit ap3212c_pls_exit(void)
{
	printk("%s\n", __func__);
	i2c_del_driver(&ap3212c_pls_driver);
}

module_init(ap3212c_pls_init);
module_exit(ap3212c_pls_exit);


MODULE_AUTHOR("Alex Shi <sikinzen@126.com>");
MODULE_DESCRIPTION("Proximity&Light Sensor AP3212C DRIVER");
MODULE_LICENSE("GPL");

