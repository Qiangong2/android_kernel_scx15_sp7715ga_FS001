/* He Fei 2012-4-5 modify to solve TYLL-NO ，Declare the code command*/
/*
 *  apds990x.c - Linux kernel modules for ambient light + proximity sensor
 *
 *  Copyright (C) 2010 Lee Kai Koon <kai-koon.lee@avagotech.com>
 *  Copyright (C) 2010 Avago Technologies
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * 思路概述  
 *	PS采用中断方式，最终调用 apds990x_work_handler;
 * 	ALS采用poll,最终调用apds990x_als_polling_work_handler.间隔时间为data->als_poll_delay,
 *		单位为ms,默认为600ms(在 apds990x_probe 中设置),这个值上层可以修改
 */
/* Change History 
 *
 * 1.0.1	Functions apds990x_show_rev(), apds990x_show_id() and apds990x_show_status()
 *			have missing CMD_BYTE in the i2c_smbus_read_byte_data(). APDS-990x needs
 *			CMD_BYTE for i2c write/read byte transaction.
 *
 *
 * 1.0.2	Include PS switching threshold level when interrupt occurred
 *
 *
 * 1.0.3	Implemented ISR and delay_work, correct PS threshold storing
 *
 * 1.0.4	Added Input Report Event
 */
 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/i2c/apds990x.h>
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
#include <linux/cdc_com.h>
#endif

// Alex.shi 支持的PLSensor列表
// 格式:CONFIG_ZYT_IC_项目_客户_模组厂_序号
//#if (defined(CONFIG_MACH_SP6825GA_7660) || defined(CONFIG_MACH_SP8825GA_7660)) && (53 == CONFIG_ZYT_CUST)
	// 默认采用7660松铭+APDS9900  
	#define CONFIG_ZYT_APDS990X_7660_SM_AV_0
//#endif

#undef PLS_DEBUG
#if defined(PLS_DEBUG)
#define PLS_DBG(format, args...) \
	printk(KERN_INFO "%s: " format, APDS990x_DRV_NAME, ## args)
#else
#define PLS_DBG(format, ...)
#endif
#define PLS_INFO(format, args...) \
	printk(KERN_INFO "%s: " format, APDS990x_DRV_NAME, ## args)

#define APDS990x_DRV_NAME	"apds990x"
#define DRIVER_VERSION		"1.0.4"
#undef SPRD_ANDROID_235
// Alex.shi Self-Defined Macro
// Alex.shi join als_input_dev and ps_input_dev together  
#define PLS_SINGLE_INPUT_DEVICE

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
#define APDS990x_ALS_INPUT	"apds990x-pls"
#ifndef PLS_SINGLE_INPUT_DEVICE
#define PLS_SINGLE_INPUT_DEVICE
#endif
#else
#define APDS990x_ALS_INPUT	"ALS"
#define APDS990x_PS_INPUT	"APS"
#endif

#if defined(CONFIG_ZYT_APDS990X_7660_SM_AV_0)
#define APDS990x_PS_DETECTION_THRESHOLD		900
#define APDS990x_PS_HSYTERESIS_THRESHOLD	800
#else
#define APDS990x_PS_DETECTION_THRESHOLD		600
#define APDS990x_PS_HSYTERESIS_THRESHOLD	500
#endif
#define APDS990x_ALS_THRESHOLD_HSYTERESIS	20	/* 20 = 20% */

#ifdef SPRD_ANDROID_235
extern int sprd_3rdparty_gpio_pls_irq;
#endif

/*
 * Defines
 */
#define APDS990x_ENABLE_REG		0x00
#define APDS990x_ATIME_REG		0x01
#define APDS990x_PTIME_REG		0x02
#define APDS990x_WTIME_REG		0x03
#define APDS990x_AILTL_REG		0x04
#define APDS990x_AILTH_REG		0x05
#define APDS990x_AIHTL_REG		0x06
#define APDS990x_AIHTH_REG		0x07
#define APDS990x_PILTL_REG		0x08
#define APDS990x_PILTH_REG		0x09
#define APDS990x_PIHTL_REG		0x0A
#define APDS990x_PIHTH_REG		0x0B
#define APDS990x_PERS_REG		0x0C
#define APDS990x_CONFIG_REG		0x0D
#define APDS990x_PPCOUNT_REG	0x0E
#define APDS990x_CONTROL_REG	0x0F
#define APDS990x_REV_REG		0x11
#define APDS990x_ID_REG			0x12
#define APDS990x_STATUS_REG		0x13
#define APDS990x_CDATAL_REG		0x14
#define APDS990x_CDATAH_REG		0x15
#define APDS990x_IRDATAL_REG	0x16
#define APDS990x_IRDATAH_REG	0x17
#define APDS990x_PDATAL_REG		0x18
#define APDS990x_PDATAH_REG		0x19

#define CMD_BYTE			0x80
#define CMD_WORD			0xA0
#define CMD_SPECIAL			0xE0

#define CMD_CLR_PS_INT			0xE5
#define CMD_CLR_ALS_INT			0xE6
#define CMD_CLR_PS_ALS_INT		0xE7

/*
 * Structs
 */

struct apds990x_data {
	struct i2c_client		*client;
	struct mutex			update_lock;
	struct delayed_work		dwork;	/* for PS interrupt */
	struct delayed_work		als_dwork; /* for ALS polling */
	struct input_dev		*input_dev_als;
	struct input_dev		*input_dev_ps;

	unsigned int	enable;
	unsigned int	atime;
	unsigned int	ptime;
	unsigned int	wtime;
	unsigned int	ailt;
	unsigned int	aiht;
	unsigned int	pilt;
	unsigned int	piht;
	unsigned int	pers;
	unsigned int	config;
	unsigned int	ppcount;
	unsigned int	control;

	/* control flag from HAL */
	unsigned int	enable_ps_sensor;
	unsigned int	enable_als_sensor;

	/* PS parameters */
	unsigned int	ps_threshold;
	unsigned int	ps_hysteresis_threshold; /* always lower than ps_threshold */
	unsigned int	ps_detection;		/* 0 = near-to-far; 1 = far-to-near */
	unsigned int	ps_data;			/* to store PS data */

	/* ALS parameters */
	unsigned int	als_threshold_l;	/* low threshold */
	unsigned int	als_threshold_h;	/* high threshold */
	unsigned int	als_data;			/* to store ALS data */

	unsigned int	als_gain;			/* needed for Lux calculation */
	unsigned int	als_poll_delay;	/* needed for light sensor polling : micro-second (us) */
	unsigned int	als_atime;			/* storage for als integratiion time */
};

#ifdef SPRD_ANDROID_235
/*
 * Global data
 */
static int apds990x_pls_irq;

/*
 * Management functions
 */
static int apds990x_pls_config_pins(void)
{
	gpio_direction_input(sprd_3rdparty_gpio_pls_irq);
	apds990x_pls_irq=sprd_alloc_gpio_irq(sprd_3rdparty_gpio_pls_irq);
	msleep(10); //wait for stable

	PLS_DBG("IRQ=%d , %s", apds990x_pls_irq,__func__);
	return apds990x_pls_irq;
}
#endif

static int apds990x_set_command(struct i2c_client *client, int command)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;
	int clearInt;

	if (command == 0)
		clearInt = CMD_CLR_PS_INT;
	else if (command == 1)
		clearInt = CMD_CLR_ALS_INT;
	else
		clearInt = CMD_CLR_PS_ALS_INT;

	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte(client, clearInt);
	mutex_unlock(&data->update_lock);

	return ret;
}

static int apds990x_set_enable(struct i2c_client *client, int enable)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;

	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_ENABLE_REG, enable);
	mutex_unlock(&data->update_lock);

	data->enable = enable;

	return ret;
}

static int apds990x_set_atime(struct i2c_client *client, int atime)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;

	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_ATIME_REG, atime);
	mutex_unlock(&data->update_lock);

	data->atime = atime;

	return ret;
}

static int apds990x_set_ptime(struct i2c_client *client, int ptime)
{
	struct apds990x_data	*data = i2c_get_clientdata(client);
	int ret;

	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_PTIME_REG, ptime);
	mutex_unlock(&data->update_lock);

	data->ptime = ptime;

	return ret;
}

static int apds990x_set_wtime(struct i2c_client *client, int wtime)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;

	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_WTIME_REG, wtime);
	mutex_unlock(&data->update_lock);

	data->wtime = wtime;

	return ret;
}

static int apds990x_set_ailt(struct i2c_client *client, int threshold)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;

	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_AILTL_REG, threshold);
	mutex_unlock(&data->update_lock);

	data->ailt = threshold;

	return ret;
}

static int apds990x_set_aiht(struct i2c_client *client, int threshold)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;

	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_AIHTL_REG, threshold);
	mutex_unlock(&data->update_lock);

	data->aiht = threshold;

	return ret;
}

static int apds990x_set_pilt(struct i2c_client *client, int threshold)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;
	
	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PILTL_REG, threshold);
	mutex_unlock(&data->update_lock);
	
	data->pilt = threshold;

	return ret;
}

static int apds990x_set_piht(struct i2c_client *client, int threshold)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;
	
	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PIHTL_REG, threshold);
	mutex_unlock(&data->update_lock);
	
	data->piht = threshold;

	return ret;
}

static int apds990x_set_pers(struct i2c_client *client, int pers)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;
	
	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_PERS_REG, pers);
	mutex_unlock(&data->update_lock);

	data->pers = pers;

	return ret;
}

static int apds990x_set_config(struct i2c_client *client, int config)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;
	
	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_CONFIG_REG, config);
	mutex_unlock(&data->update_lock);

	data->config = config;

	return ret;
}

static int apds990x_set_ppcount(struct i2c_client *client, int ppcount)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;
	
	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_PPCOUNT_REG, ppcount);
	mutex_unlock(&data->update_lock);

	data->ppcount = ppcount;

	return ret;
}

static int apds990x_set_control(struct i2c_client *client, int control)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int ret;
	
	mutex_lock(&data->update_lock);
	ret = i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_CONTROL_REG, control);
	mutex_unlock(&data->update_lock);

	data->control = control;

	/* obtain ALS gain value */
	if ((data->control&0x03) == 0x00) /* 1X Gain */
		data->als_gain = 1;
	else if ((data->control&0x03) == 0x01) /* 8X Gain */
		data->als_gain = 8;
	else if ((data->control&0x03) == 0x02) /* 16X Gain */
		data->als_gain = 16;
	else  /* 120X Gain */
		data->als_gain = 120;

	return ret;
}

static int LuxCalculation(struct i2c_client *client, int cdata, int irdata)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int luxValue=0;

	int IAC1=0;
	int IAC2=0;
	int IAC=0;
	int GA=48;			/* 0.48 without glass window */
	int COE_B=223;		/* 2.23 without glass window */
	int COE_C=70;		/* 0.70 without glass window */
	int COE_D=142;		/* 1.42 without glass window */
	int DF=52;

	IAC1 = (cdata - (COE_B*irdata)/100);	// re-adjust COE_B to avoid 2 decimal point
	IAC2 = ((COE_C*cdata)/100 - (COE_D*irdata)/100); // re-adjust COE_C and COE_D to void 2 decimal point

	if (IAC1 > IAC2)
		IAC = IAC1;
	else if (IAC1 <= IAC2)
		IAC = IAC2;
	else
		IAC = 0;

	luxValue = ((IAC*GA*DF)/100)/(((272*(256-data->atime))/100)*data->als_gain);
	// 部分 tp透光率不好，加强增益  
#if defined(CONFIG_ZYT_APDS990X_7660_SM_AV_0)
    luxValue *= 6;
#endif
	return luxValue;
}

static void apds990x_change_ps_threshold(struct i2c_client *client)
{
	struct apds990x_data *data = i2c_get_clientdata(client);

	data->ps_data =	i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_PDATAL_REG);
	PLS_DBG("%s:data->ps_data=======%d\n", __func__, data->ps_data);
	if ( (data->ps_data > data->pilt) && (data->ps_data >= data->piht) ) {
		/* far-to-near detected */
		data->ps_detection = 1;

		input_report_abs(data->input_dev_ps, ABS_DISTANCE, 0);/* FAR-to-NEAR detection */	
		input_sync(data->input_dev_ps);

		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PILTL_REG, data->ps_hysteresis_threshold);
		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PIHTL_REG, 1023);

		data->pilt = data->ps_hysteresis_threshold;
		data->piht = 1023;

		PLS_DBG("far-to-near detected\n");
	}
	else if ( (data->ps_data <= data->pilt) && (data->ps_data < data->piht) ) {
		/* near-to-far detected */
		data->ps_detection = 0;

		input_report_abs(data->input_dev_ps, ABS_DISTANCE, 1);/* NEAR-to-FAR detection */	
		input_sync(data->input_dev_ps);

		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PILTL_REG, 0);
		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PIHTL_REG, data->ps_threshold);

		data->pilt = 0;
		data->piht = data->ps_threshold;

		PLS_DBG("near-to-far detected\n");
	}
}

static void apds990x_change_als_threshold(struct i2c_client *client)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int cdata, irdata;
	int luxValue=0;

	cdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_CDATAL_REG);
	irdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_IRDATAL_REG);

	luxValue = LuxCalculation(client, cdata, irdata);

	luxValue = luxValue>0 ? luxValue : 0;
	luxValue = luxValue<10000 ? luxValue : 10000;
	
	// check PS under sunlight
	if ( (data->ps_detection == 1) && (cdata > (75*(1024*(256-data->atime)))/100))	// PS was previously in far-to-near condition
	{
		// need to inform input event as there will be no interrupt from the PS
		input_report_abs(data->input_dev_ps, ABS_DISTANCE, 1);/* NEAR-to-FAR detection */	
		input_sync(data->input_dev_ps);

		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PILTL_REG, 0);
		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PIHTL_REG, data->ps_threshold);

		data->pilt = 0;
		data->piht = data->ps_threshold;

		data->ps_detection = 0;	/* near-to-far detected */

		PLS_DBG("apds_990x_proximity_handler = FAR\n");	
	}
	
	input_report_abs(data->input_dev_als, ABS_MISC, luxValue); // report the lux level
	input_sync(data->input_dev_als);

	data->als_data = cdata;

	data->als_threshold_l = (data->als_data * (100-APDS990x_ALS_THRESHOLD_HSYTERESIS) ) /100;
	data->als_threshold_h = (data->als_data * (100+APDS990x_ALS_THRESHOLD_HSYTERESIS) ) /100;

	if (data->als_threshold_h >= 65535) data->als_threshold_h = 65535;

	i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_AILTL_REG, data->als_threshold_l);

	i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_AIHTL_REG, data->als_threshold_h);
}

static void apds990x_reschedule_work(struct apds990x_data *data,
					  unsigned long delay)
{
	unsigned long flags;

	spin_lock_irqsave(&data->update_lock.wait_lock, flags);

	/*
	* If work is already scheduled then subsequent schedules will not
	* change the scheduled time that's why we have to cancel it first.
	*/
	__cancel_delayed_work(&data->dwork);
	schedule_delayed_work(&data->dwork, delay);

	spin_unlock_irqrestore(&data->update_lock.wait_lock, flags);
}

/* ALS polling routine */
static void apds990x_als_polling_work_handler(struct work_struct *work)
{
	struct apds990x_data *data = container_of(work, struct apds990x_data, als_dwork.work);
	struct i2c_client *client=data->client;
	int cdata, irdata, pdata;
	int luxValue=0;
	
	cdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_CDATAL_REG);
	irdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_IRDATAL_REG);
	pdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_PDATAL_REG);
	
	luxValue = LuxCalculation(client, cdata, irdata);
	
	luxValue = luxValue>0 ? luxValue : 0;
	luxValue = luxValue<10000 ? luxValue : 10000;
	
	PLS_DBG("%s: lux = %d cdata = %x  irdata = %x pdata = %x \n", __func__, luxValue, cdata, irdata, pdata);

	// check PS under sunlight
	if ( (data->ps_detection == 1) && (cdata > (75*(1024*(256-data->atime)))/100))	// PS was previously in far-to-near condition
	{
		// need to inform input event as there will be no interrupt from the PS
		input_report_abs(data->input_dev_ps, ABS_DISTANCE, 1);/* NEAR-to-FAR detection */	
		input_sync(data->input_dev_ps);

		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PILTL_REG, 0);
		i2c_smbus_write_word_data(client, CMD_WORD|APDS990x_PIHTL_REG, data->ps_threshold);

		data->pilt = 0;
		data->piht = data->ps_threshold;

		data->ps_detection = 0;	/* near-to-far detected */

		PLS_DBG("apds_990x_proximity_handler = FAR\n");	
	}

	input_report_abs(data->input_dev_als, ABS_MISC, luxValue); // report the lux level
	input_sync(data->input_dev_als);
	
	schedule_delayed_work(&data->als_dwork, msecs_to_jiffies(data->als_poll_delay));	// restart timer
}

/* PS interrupt routine */
static void apds990x_work_handler(struct work_struct *work)
{
	struct apds990x_data *data = container_of(work, struct apds990x_data, dwork.work);
	struct i2c_client *client=data->client;
	int	status;
	int cdata;

	status = i2c_smbus_read_byte_data(client, CMD_BYTE|APDS990x_STATUS_REG);

	i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_ENABLE_REG, 1);	/* disable 990x's ADC first */

	PLS_DBG("status = %x\n", status);

	if ((status & data->enable & 0x30) == 0x30) {
		/* both PS and ALS are interrupted */
		apds990x_change_als_threshold(client);
		
		cdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_CDATAL_REG);
		if (cdata < (75*(1024*(256-data->atime)))/100)
			apds990x_change_ps_threshold(client);
		else {
			if (data->ps_detection == 1) {
				apds990x_change_ps_threshold(client);			
			}
			else {
				PLS_DBG("Triggered by background ambient noise\n");
			}
		}

		apds990x_set_command(client, 2);	/* 2 = CMD_CLR_PS_ALS_INT */
	}
	else if ((status & data->enable & 0x20) == 0x20) {
		/* only PS is interrupted */
		
		/* check if this is triggered by background ambient noise */
		cdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_CDATAL_REG);
		if (cdata < (75*(1024*(256-data->atime)))/100)
			apds990x_change_ps_threshold(client);
		else {
			if (data->ps_detection == 1) {
				apds990x_change_ps_threshold(client);			
			}
			else {
				PLS_DBG("Triggered by background ambient noise\n");
			}
		}

		apds990x_set_command(client, 0);	/* 0 = CMD_CLR_PS_INT */
	}
	else if ((status & data->enable & 0x10) == 0x10) {
		/* only ALS is interrupted */	
		apds990x_change_als_threshold(client);

		apds990x_set_command(client, 1);	/* 1 = CMD_CLR_ALS_INT */
	}

	i2c_smbus_write_byte_data(client, CMD_BYTE|APDS990x_ENABLE_REG, data->enable);	
}

/* assume this is ISR */
static irqreturn_t apds990x_interrupt(int vec, void *info)
{
	struct i2c_client *client=(struct i2c_client *)info;
	struct apds990x_data *data = i2c_get_clientdata(client);

	PLS_DBG("==> apds990x_interrupt\n");
	apds990x_reschedule_work(data, 0);	

	return IRQ_HANDLED;
}

/*
 * SysFS support
 */

static ssize_t apds990x_show_enable_ps_sensor(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct apds990x_data *data = i2c_get_clientdata(client);
	
	return sprintf(buf, "%d\n", data->enable_ps_sensor);
}

static ssize_t apds990x_store_enable_ps_sensor(struct device *dev,
				struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct apds990x_data *data = i2c_get_clientdata(client);
	unsigned long val = simple_strtoul(buf, NULL, 10);
 	unsigned long flags;
	
	PLS_DBG("%s: enable ps senosr ( %ld)\n", __func__, val);
	
	if ((val != 0) && (val != 1)) {
		PLS_DBG("%s:store unvalid value=%ld\n", __func__, val);
		return count;
	}
	
	if(val == 1) {
		//turn on p sensor
		if (data->enable_ps_sensor==0) {
			data->enable_ps_sensor= 1;
			apds990x_set_enable(client,0); /* Power Off */
			apds990x_set_atime(client, 0xf6); /* 27.2ms */
			apds990x_set_ptime(client, 0xff); /* 2.72ms */
		
			apds990x_set_ppcount(client, 8); /* 8-pulse */
#if defined(CONFIG_ZYT_APDS990X_7660_SM_AV_0)
			apds990x_set_control(client, 0x63); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#else
			apds990x_set_control(client, 0x20); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#endif
			apds990x_set_pilt(client, 0);		// init threshold for proximity
			apds990x_set_piht(client, APDS990x_PS_DETECTION_THRESHOLD);

			data->ps_threshold = APDS990x_PS_DETECTION_THRESHOLD;
			data->ps_hysteresis_threshold = APDS990x_PS_HSYTERESIS_THRESHOLD;
		
			apds990x_set_ailt( client, 0);
			apds990x_set_aiht( client, 0xffff);
		
			apds990x_set_pers(client, 0x33); /* 3 persistence */
		
			if (data->enable_als_sensor==0) {

				/* we need this polling timer routine for sunlight canellation */
				spin_lock_irqsave(&data->update_lock.wait_lock, flags); 
			
				/*
				 * If work is already scheduled then subsequent schedules will not
				 * change the scheduled time that's why we have to cancel it first.
				 */
				__cancel_delayed_work(&data->als_dwork);
				schedule_delayed_work(&data->als_dwork, msecs_to_jiffies(data->als_poll_delay));	// 100ms
			
				spin_unlock_irqrestore(&data->update_lock.wait_lock, flags);	
			}

			apds990x_set_enable(client, 0x27);	 /* only enable PS interrupt */
		}
	} 
	else {
		//turn off p sensor - kk 25 Apr 2011 we can't turn off the entire sensor, the light sensor may be needed by HAL
		data->enable_ps_sensor = 0;
		if (data->enable_als_sensor) {
			
			// reconfigute light sensor setting			
			apds990x_set_enable(client,0); /* Power Off */
			
			apds990x_set_atime(client, data->als_atime);  /* previous als poll delay */
			
			apds990x_set_ailt( client, 0);
			apds990x_set_aiht( client, 0xffff);
#if defined(CONFIG_ZYT_APDS990X_7660_SM_AV_0)
			apds990x_set_control(client, 0x63); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#else
			apds990x_set_control(client, 0x20); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#endif
			apds990x_set_pers(client, 0x33); /* 3 persistence */
			
			apds990x_set_enable(client, 0x3);	 /* only enable light sensor */
			
			spin_lock_irqsave(&data->update_lock.wait_lock, flags); 
			
			/*
			 * If work is already scheduled then subsequent schedules will not
			 * change the scheduled time that's why we have to cancel it first.
			 */
			__cancel_delayed_work(&data->als_dwork);
			schedule_delayed_work(&data->als_dwork, msecs_to_jiffies(data->als_poll_delay));	// 100ms
			
			spin_unlock_irqrestore(&data->update_lock.wait_lock, flags);	
			
		}
		else {
			apds990x_set_enable(client, 0);

			spin_lock_irqsave(&data->update_lock.wait_lock, flags); 
			
			/*
			 * If work is already scheduled then subsequent schedules will not
			 * change the scheduled time that's why we have to cancel it first.
			 */
			__cancel_delayed_work(&data->als_dwork);
		
			spin_unlock_irqrestore(&data->update_lock.wait_lock, flags); 
		}
	}
	
	
	return count;
}

static DEVICE_ATTR(enable_ps_sensor, S_IWUGO | S_IRUGO,
				   apds990x_show_enable_ps_sensor, apds990x_store_enable_ps_sensor);

static ssize_t apds990x_show_enable_als_sensor(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct apds990x_data *data = i2c_get_clientdata(client);
	
	return sprintf(buf, "%d\n", data->enable_als_sensor);
}

static ssize_t apds990x_store_enable_als_sensor(struct device *dev,
				struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct apds990x_data *data = i2c_get_clientdata(client);
	unsigned long val = simple_strtoul(buf, NULL, 10);
 	unsigned long flags;
	
	PLS_DBG("%s: enable als sensor ( %ld)\n", __func__, val);
	
	if ((val != 0) && (val != 1))
	{
		PLS_DBG("%s: enable als sensor=%ld\n", __func__, val);
		return count;
	}
	
	if(val == 1) {
		//turn on light  sensor
		if (data->enable_als_sensor==0) {

			data->enable_als_sensor = 1;
		
			apds990x_set_enable(client,0); /* Power Off */
		
			apds990x_set_atime(client, data->als_atime);  /* 100.64ms */
		
			apds990x_set_ailt( client, 0);
			apds990x_set_aiht( client, 0xffff);
		
#if defined(CONFIG_ZYT_APDS990X_7660_SM_AV_0)
			apds990x_set_control(client, 0x63); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#else
			apds990x_set_control(client, 0x20); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#endif
			apds990x_set_pers(client, 0x33); /* 3 persistence */
		
			if (data->enable_ps_sensor) {
				apds990x_set_ptime(client, 0xff); /* 2.72ms */
				apds990x_set_ppcount(client, 8); /* 8-pulse */			
				apds990x_set_enable(client, 0x27);	 /* if prox sensor was activated previously */
			}
			else {
				apds990x_set_enable(client, 0x3);	 /* only enable light sensor */
			}
		
			spin_lock_irqsave(&data->update_lock.wait_lock, flags); 
		
			/*
			 * If work is already scheduled then subsequent schedules will not
			 * change the scheduled time that's why we have to cancel it first.
			 */
			__cancel_delayed_work(&data->als_dwork);
			schedule_delayed_work(&data->als_dwork, msecs_to_jiffies(data->als_poll_delay));
		
			spin_unlock_irqrestore(&data->update_lock.wait_lock, flags);
		}
	}
	else {
		//turn off light sensor
		// what if the p sensor is active?
		data->enable_als_sensor = 0;
		if (data->enable_ps_sensor) {
			apds990x_set_enable(client,0); /* Power Off */
			apds990x_set_atime(client, 0xf6);  /* 27.2ms */
			apds990x_set_ptime(client, 0xff); /* 2.72ms */
			apds990x_set_ppcount(client, 8); /* 8-pulse */
#if defined(CONFIG_ZYT_APDS990X_7660_SM_AV_0)
			apds990x_set_control(client, 0x63); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#else
			apds990x_set_control(client, 0x20); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#endif
			
			apds990x_set_piht(client, 0);
			apds990x_set_piht(client, APDS990x_PS_DETECTION_THRESHOLD);
			
			apds990x_set_ailt( client, 0);
			apds990x_set_aiht( client, 0xffff);
			
			apds990x_set_pers(client, 0x33); /* 3 persistence */
			apds990x_set_enable(client, 0x27);	 /* only enable prox sensor with interrupt */			
		}
		else {
			apds990x_set_enable(client, 0);
		}
		
		
		spin_lock_irqsave(&data->update_lock.wait_lock, flags); 
		
		/*
		 * If work is already scheduled then subsequent schedules will not
		 * change the scheduled time that's why we have to cancel it first.
		 */
		__cancel_delayed_work(&data->als_dwork);
		
		spin_unlock_irqrestore(&data->update_lock.wait_lock, flags); 
	}
	
	return count;
}

static DEVICE_ATTR(enable_als_sensor, S_IWUGO | S_IRUGO,
				   apds990x_show_enable_als_sensor, apds990x_store_enable_als_sensor);

static ssize_t apds990x_show_als_poll_delay(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct apds990x_data *data = i2c_get_clientdata(client);
	
	return sprintf(buf, "%d\n", data->als_poll_delay*1000);	// return in micro-second
}

static ssize_t apds990x_store_als_poll_delay(struct device *dev,
					struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client		*client = to_i2c_client(dev);
	struct apds990x_data	*data = i2c_get_clientdata(client);
	unsigned long			val = simple_strtoul(buf, NULL, 10);
	int				ret;
	int				poll_delay=0;
 	unsigned long	flags;
	
	PLS_DBG("%s: set als delay \n", __func__);
	if (val<5000)
		val = 5000;	// minimum 5ms
	
	data->als_poll_delay = val/1000;	// convert us => ms
	
	poll_delay = 256 - (val/2720);	// the minimum is 2.72ms = 2720 us, maximum is 696.32ms
	if (poll_delay >= 256)
		data->als_atime = 255;
	else if (poll_delay < 0)
		data->als_atime = 0;
	else
		data->als_atime = poll_delay;
	
	ret = apds990x_set_atime(client, data->als_atime);
	
	if (ret < 0)
		return ret;

	/* we need this polling timer routine for sunlight canellation */
	spin_lock_irqsave(&data->update_lock.wait_lock, flags); 
		
	/*
	 * If work is already scheduled then subsequent schedules will not
	 * change the scheduled time that's why we have to cancel it first.
	 */
	__cancel_delayed_work(&data->als_dwork);
	
	schedule_delayed_work(&data->als_dwork, msecs_to_jiffies(data->als_poll_delay));	// 100ms
			
	spin_unlock_irqrestore(&data->update_lock.wait_lock, flags);	
	
	return count;
}

static DEVICE_ATTR(als_poll_delay, S_IWUSR | S_IRUGO,
				   apds990x_show_als_poll_delay, apds990x_store_als_poll_delay);
/*////////////////////////////////*/
static ssize_t apds990x_show_light_value(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
    //struct work_struct work;
	//struct apds990x_data *data = container_of(&work, struct apds990x_data, als_dwork.work);
	//struct i2c_client *client=data->client;

    struct i2c_client *client = to_i2c_client(cd);
	struct apds990x_data *data = i2c_get_clientdata(client);
	int cdata, irdata;
	int luxValue=0;
	
	cdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_CDATAL_REG);
	irdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_IRDATAL_REG);

	luxValue = LuxCalculation(client, cdata, irdata);
	
	luxValue = luxValue>0 ? luxValue : 0;
	luxValue = luxValue<10000 ? luxValue : 10000;
	return sprintf(buf, "luxValue = %d, cdata = %x,irdata = %x, als_gain=%d, als_atime=%d\n", luxValue, cdata, irdata, data->als_gain, data->als_atime);
}

static ssize_t apds990x_show_proximity_value(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
    //struct work_struct work;
	//struct apds990x_data *data = container_of(&work, struct apds990x_data, als_dwork.work);
	//struct i2c_client *client=data->client;

    struct i2c_client *client = to_i2c_client(cd);
	struct apds990x_data *data = i2c_get_clientdata(client);
	int pdata, status;
//	int luxValue=0;
	

	pdata = i2c_smbus_read_word_data(client, CMD_WORD|APDS990x_PDATAL_REG);

	status = i2c_smbus_read_byte_data(client, CMD_BYTE|APDS990x_STATUS_REG);

	return sprintf(buf, "PDATAL_REG=%d, STATUS_REG=%x,  data->piht=%d,  data->pilt=%d\n", pdata, status, data->piht, data->pilt);

}
static DEVICE_ATTR(get_proximity_value, S_IRUGO | S_IWUSR, apds990x_show_proximity_value, NULL);

static DEVICE_ATTR(get_light_value, S_IRUGO | S_IWUSR, apds990x_show_light_value, NULL);

/*/////////////////////////////////*/

static struct attribute *apds990x_attributes[] = {
	&dev_attr_enable_ps_sensor.attr,
	&dev_attr_enable_als_sensor.attr,
	&dev_attr_als_poll_delay.attr,
	&dev_attr_get_proximity_value.attr,
	&dev_attr_get_light_value.attr,
	NULL
};


static const struct attribute_group apds990x_attr_group = {
	.attrs = apds990x_attributes,
};

/*
 * Initialization function
 */

static int apds990x_init_client(struct i2c_client *client)
{
	struct apds990x_data *data = i2c_get_clientdata(client);
	int err;
	int id;

	err = apds990x_set_enable(client, 0);

	if (err < 0) {
		PLS_INFO("Failed to apds990x_set_enable(client, 0);\n");
		return err;
	}

	id = i2c_smbus_read_byte_data(client, CMD_BYTE|APDS990x_ID_REG);
	if (id == 0x20) {
		PLS_INFO("APDS-9901\n");
	}
	else if (id == 0x29) {
		PLS_INFO("APDS-990x\n");
	}
	else {
		PLS_INFO("Neither APDS-9901 nor APDS-990x\n");
		return -EIO;
	}
	apds990x_set_atime(client, 0xDB);	// 100.64ms ALS integration time
	apds990x_set_ptime(client, 0xFF);	// 2.72ms Prox integration time
	apds990x_set_wtime(client, 0xFF);	// 2.72ms Wait time

	apds990x_set_ppcount(client, 0x08);	// 8-Pulse for proximity
	apds990x_set_config(client, 0);		// no long wait
#if defined(CONFIG_ZYT_APDS990X_7660_SM_AV_0)
	apds990x_set_control(client, 0x63); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#else
	apds990x_set_control(client, 0x20); /* 100mA, IR-diode, 1X PGAIN, 1X AGAIN */
#endif

	apds990x_set_pilt(client, 0);		// init threshold for proximity
	apds990x_set_piht(client, APDS990x_PS_DETECTION_THRESHOLD);

	data->ps_threshold = APDS990x_PS_DETECTION_THRESHOLD;
	data->ps_hysteresis_threshold = APDS990x_PS_HSYTERESIS_THRESHOLD;

	apds990x_set_ailt(client, 0);		// init threshold for als
	apds990x_set_aiht(client, 0xFFFF);

	apds990x_set_pers(client, 0x22);	// 2 consecutive Interrupt persistence

	//sensor is in disabled mode but all the configurations are preset

	return 0;
}

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
static ssize_t cdc_als_enable_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct apds990x_data *data = dev_get_drvdata(dev);

	PLS_DBG("%s:als_enable = 0x%x\n", __FUNCTION__, data->enable_als_sensor);
	
	return sprintf(buf, "%d\n", data->enable_als_sensor);
}

static ssize_t cdc_als_enable_store(struct device *dev,
				struct device_attribute *attr, const char *buf, size_t count)
{
	struct apds990x_data *data = dev_get_drvdata(dev);
	struct i2c_client *client = data->client;

	PLS_DBG("%s:als_enable = 0x%x -> 0x%x\n", __FUNCTION__, data->enable_als_sensor, simple_strtoul(buf, NULL, 10));
	
	return apds990x_store_enable_als_sensor(&(client->dev), attr, buf, count);
}

static struct device_attribute cdc_als_enable_attribute = __ATTR(als_enable,0666,cdc_als_enable_show,cdc_als_enable_store);

static ssize_t cdc_als_delay_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct apds990x_data *data = dev_get_drvdata(dev);

	PLS_DBG("%s:als_delay(ns) = 0x%x\n", __FUNCTION__, data->als_poll_delay*1000);
	
	return sprintf(buf, "%d\n", data->als_poll_delay*1000);	// return in micro-second
}

static ssize_t cdc_als_delay_store(struct device *dev,
					struct device_attribute *attr, const char *buf, size_t count)
{
	struct apds990x_data *data = dev_get_drvdata(dev);
	struct i2c_client *client = data->client;
	
	PLS_DBG("%s:als_delay(ns) = 0x%x -> 0x%x\n", __FUNCTION__, data->als_poll_delay*1000, simple_strtoul(buf, NULL, 10)*1000);

	return apds990x_store_als_poll_delay(&(client->dev), attr, buf, count);
}

static struct device_attribute cdc_als_poll_delay_attribute = __ATTR(als_delay,0666,cdc_als_delay_show,cdc_als_delay_store);

static ssize_t cdc_ps_enable_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct apds990x_data *data = dev_get_drvdata(dev);

	PLS_DBG("%s:ps_enable = 0x%x\n", __FUNCTION__, data->enable_ps_sensor);
	
	return sprintf(buf, "%d\n", data->enable_ps_sensor);
}

static ssize_t cdc_ps_enable_store(struct device *dev,
				struct device_attribute *attr, const char *buf, size_t count)
{
	struct apds990x_data *data = dev_get_drvdata(dev);
	struct i2c_client *client = data->client;

	PLS_DBG("%s:ps_enable = 0x%x -> 0x%x\n", __FUNCTION__, data->enable_ps_sensor, simple_strtoul(buf, NULL, 10));

	return apds990x_store_enable_ps_sensor(&(client->dev), attr, buf, count);
}

static struct device_attribute cdc_ps_enable_attribute = __ATTR(ps_enable,0666,cdc_ps_enable_show,cdc_ps_enable_store);

static ssize_t chip_id_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", 0x9901);
}

static struct device_attribute cdc_chip_id_attribute = __ATTR(chip_id,0666,chip_id_show,NULL);


static struct attribute* sensors_pls_cdc_attrs [] =
{
	&cdc_als_enable_attribute.attr,
	&cdc_als_poll_delay_attribute.attr,
	&cdc_ps_enable_attribute.attr,
	&cdc_chip_id_attribute.attr,
	NULL,
};

static const struct attribute_group pls_cdc_attrs_group = {
	.attrs = sensors_pls_cdc_attrs,
};

static int check_plsensor_chip(void)
{
	CDC_Plsensor_Device_Id(0x9901);

	return 0;
}

static int remove_plsensor_chip(void)
{
	CDC_Plsensor_Device_Id(0xFFFF);

	return 0;
}
#endif

/*
 * I2C init/probing/exit functions
 */
static int apds990x_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2c_adapter		*adapter = to_i2c_adapter(client->dev.parent);
	struct apds990x_data	*data;
	struct apds990x_pls_platform_data	*pdata = client->dev.platform_data;
	int						err = 0;

	PLS_INFO("apds990x_probe \n");

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(CDC_Plsensor_Device_Id(0)!=0)
	{
		printk("PlSensor(0x%x)Exist!", CDC_Plsensor_Device_Id(0));
		err = -ENODEV;
		goto exit;
	}
#endif

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE)) {
		printk("%s: functionality check failed\n", __func__);
		err = -EIO;
		goto exit;
	}

	data = kzalloc(sizeof(struct apds990x_data), GFP_KERNEL);
	if (!data) {
		printk("%s: request memory failed\n", __func__);
		err = -ENOMEM;
		goto exit;
	}
	data->client = client;
	i2c_set_clientdata(client, data);

	data->enable = 0;			/* default mode is standard */
	data->ps_threshold = 0;
	data->ps_hysteresis_threshold = 0;
	data->ps_detection = 0;		/* default to no detection */
	data->enable_als_sensor = 0;	// default to 0
	data->enable_ps_sensor = 0;	// default to 0
	data->als_poll_delay = 600;	// default to 100ms
	data->als_atime	= 0xdb;	// work in conjuction with als_poll_delay

	PLS_DBG("enable = %x\n", data->enable);
#ifdef SPRD_ANDROID_235
	client->irq = apds990x_pls_config_pins();
#else
	gpio_request(pdata->irq_gpio_number, APDS990x_DRV_NAME);
	client->irq = gpio_to_irq(pdata->irq_gpio_number);
#endif
	mutex_init(&data->update_lock);
	PLS_DBG("I2C name=%s, addr=0x%x, gpio=%d, irq=%d", client->name, client->addr, \
		pdata->irq_gpio_number, client->irq);
	INIT_DELAYED_WORK(&data->dwork, apds990x_work_handler);
	INIT_DELAYED_WORK(&data->als_dwork, apds990x_als_polling_work_handler); 

	PLS_DBG("%s interrupt is hooked\n", __func__);

	/* Initialize the APDS990x chip */
	err = apds990x_init_client(client);
	if (err)
		goto exit_kfree;

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(check_plsensor_chip()<0)
	{
		err = -ENODEV;
		printk("Fail to find apds990x");
		goto exit_check_chip_failed;
	}
#endif

	if (request_irq(client->irq, apds990x_interrupt, 
		IRQF_DISABLED|IRQF_TRIGGER_FALLING,
		APDS990x_DRV_NAME, (void *)client)) 
	{
		PLS_INFO("%s Could not request APDS990x_INT !\n", __func__);
		goto exit_check_chip_failed;
	}

	/* Register to Input Device */
	data->input_dev_als = input_allocate_device();
	if (!data->input_dev_als) {
		err = -ENOMEM;
		PLS_DBG("Failed to allocate input device als\n");
		goto exit_free_irq;
	}

#ifdef PLS_SINGLE_INPUT_DEVICE
	data->input_dev_ps = data->input_dev_als;
#else
	data->input_dev_ps = input_allocate_device();
	if (!data->input_dev_ps) {
		err = -ENOMEM;
		PLS_DBG("Failed to allocate input device ps\n");
		goto exit_free_dev_als;
	}

	set_bit(EV_ABS, data->input_dev_ps->evbit);
	data->input_dev_ps->name = APDS990x_PS_INPUT;
#endif

	set_bit(EV_ABS, data->input_dev_als->evbit);

	input_set_abs_params(data->input_dev_als, ABS_MISC, 0, 10000, 0, 0);
	input_set_abs_params(data->input_dev_ps, ABS_DISTANCE, 0, 1, 0, 0);

	data->input_dev_als->name = APDS990x_ALS_INPUT;

	err = input_register_device(data->input_dev_als);
	if (err) {
		err = -ENOMEM;
		PLS_DBG("Unable to register input device als: %s\n",
			data->input_dev_als->name);
		goto exit_free_dev_ps;
	}

#ifndef PLS_SINGLE_INPUT_DEVICE
	err = input_register_device(data->input_dev_ps);
	if (err) {
		err = -ENOMEM;
		PLS_DBG("Unable to register input device ps: %s\n",
		data->input_dev_ps->name);
		goto exit_unregister_dev_als;
	}
#endif

	/* Register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &apds990x_attr_group);
	if (err)
		goto exit_unregister_dev_ps;

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	err = sysfs_create_group(&(data->input_dev_als->dev.kobj), &pls_cdc_attrs_group);
	if (err) {
		dev_err(&data->input_dev_als->dev, "create device file failed!\n");
		err = -EINVAL;
		goto err_create_sysfs;
	}

	input_set_drvdata(data->input_dev_als, data);
#endif

	PLS_DBG("%s support ver. %s enabled\n", __func__, DRIVER_VERSION);

	printk(KERN_INFO "apds990x_probe success! \n");

	return 0;

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
err_create_sysfs:
	sysfs_remove_group(&(data->input_dev_als->dev.kobj), &pls_cdc_attrs_group);
	sysfs_remove_group(&client->dev.kobj, &apds990x_attr_group);
#endif
exit_unregister_dev_ps:
#ifndef PLS_SINGLE_INPUT_DEVICE
	if (NULL == data->input_dev_ps) {
		input_unregister_device(data->input_dev_ps);
	}
exit_unregister_dev_als:
#endif
	if (NULL == data->input_dev_ps) {
		input_unregister_device(data->input_dev_als);
	}
exit_free_dev_ps:
#ifndef PLS_SINGLE_INPUT_DEVICE
	if (NULL == data->input_dev_ps) {
		input_free_device(data->input_dev_ps);
		data->input_dev_ps = NULL;
	}
exit_free_dev_als:
#endif
	if (NULL == data->input_dev_als) {
		input_free_device(data->input_dev_als);
		data->input_dev_als = NULL;
	}
exit_free_irq:
	free_irq(client->irq, client);
exit_check_chip_failed:
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	remove_plsensor_chip();
#endif
exit_kfree:
	cancel_delayed_work_sync(&data->als_dwork);
	cancel_delayed_work_sync(&data->dwork);
	gpio_free(pdata->irq_gpio_number);
	i2c_set_clientdata(client, NULL);
	kfree(data);
exit:
	return err;
}

static int apds990x_remove(struct i2c_client *client)
{
	struct apds990x_data				*data = i2c_get_clientdata(client);
	struct apds990x_pls_platform_data	*pdata = client->dev.platform_data;

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	input_set_drvdata(data->input_dev_als, NULL);
	sysfs_remove_group(&(data->input_dev_als->dev.kobj), &pls_cdc_attrs_group);
#endif
	sysfs_remove_group(&client->dev.kobj, &apds990x_attr_group);
#ifndef PLS_SINGLE_INPUT_DEVICE
	if (NULL == data->input_dev_ps) {
		input_unregister_device(data->input_dev_ps);
	}
#endif
	if (NULL == data->input_dev_ps) {
		input_unregister_device(data->input_dev_als);
	}
#ifndef PLS_SINGLE_INPUT_DEVICE
	if (NULL == data->input_dev_ps) {
		input_free_device(data->input_dev_ps);
		data->input_dev_ps = NULL;
	}
#endif
	if (NULL == data->input_dev_als) {
		input_free_device(data->input_dev_als);
		data->input_dev_als = NULL;
	}
	free_irq(client->irq, client);	
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	remove_plsensor_chip();
#endif
	cancel_delayed_work_sync(&data->dwork);
	cancel_delayed_work_sync(&data->dwork);
	gpio_free(pdata->irq_gpio_number);
	i2c_set_clientdata(client, NULL);
	kfree(data);

	/* Power down the device */
	apds990x_set_enable(client, 0);

	return 0;
}

#if 0
#ifdef CONFIG_PM
static int apds990x_suspend(struct i2c_client *client, pm_message_t mesg)
{
	return apds990x_set_enable(client, 0);
}

static int apds990x_resume(struct i2c_client *client)
{
	return apds990x_set_enable(client, 0);
}
#else
#define apds990x_suspend	NULL
#define apds990x_resume	NULL
#endif /* CONFIG_PM */
#endif

static const struct i2c_device_id apds990x_id[] = {
	{APDS990X_I2C_NAME, 0},
	{ }
};
MODULE_DEVICE_TABLE(i2c, apds990x_id);

static struct i2c_driver apds990x_driver = {
	.driver = {
		.name	= APDS990X_I2C_NAME,
		.owner	= THIS_MODULE,
	},
//	.suspend = apds990x_suspend,
//	.resume	= apds990x_resume,
	.probe	= apds990x_probe,
	.remove	= apds990x_remove,
	.id_table = apds990x_id,
};

static int __init apds990x_init(void)
{
	PLS_INFO("%s\n", __func__);
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(CDC_Plsensor_Device_Id(0)!=0)
	{
		printk("PlSensor(0x%x)Exist!", CDC_Plsensor_Device_Id(0));
		return -ENODEV;
	}
#endif
	return i2c_add_driver(&apds990x_driver);
}

static void __exit apds990x_exit(void)
{
	PLS_INFO("%s\n", __func__);
	i2c_del_driver(&apds990x_driver);
}

MODULE_AUTHOR("Lee Kai Koon <kai-koon.lee@avagotech.com>");
MODULE_DESCRIPTION("APDS990x ambient light + proximity sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);

module_init(apds990x_init);
module_exit(apds990x_exit);

