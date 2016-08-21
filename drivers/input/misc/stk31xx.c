/*
 *  stk_i2c_ps31xx_switch.c - Linux kernel modules for proximity/ambient light sensor
 *  (Polling + Switch Mode)
 *
 *  Copyright (C) 2011 Patrick Chang / SenseTek <patrick_chang@sitronix.com.tw>
 *  Copyright (C) 2012 Lex Hsieh     / SenseTek <lex_hsieh@sitronix.com.tw>
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
 */
/*
 * 思路概述  
 *	为als和ps各开一个线程来循环执行采样.als是als_polling_function,ps是ps_polling_function;
 * 	采样间隔分别为 pStkPsData->als_delay 和 pStkPsData->ps_delay.这两个时间均在驱动中写死,
 *  上层修改不了
 *
 *	调试手段  
 *		/sys/devices/platform/stk-oss/下提供了几个必要的调试方式:  
 *			DBG:
 *				help: help infomation
 *				als_code: show the als code
 *				lux: show the lux value
 *				ps_code: show the ps code
 *				distance: show the distance value
 *				driver_version: this driver's version
 *				lux_range: the maxium lux
 *				als_enable: enable or disable the als sensor
 *				ps_enable: enable or disable the ps sensor
 *				dist_mode: show the distance mode
 *				ps_code_thd_l: set the low ps_code_thread
 *				ps_code_thd_h: set the high ps_code_thread
 */

#include <linux/module.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/completion.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/wakelock.h>
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
#include <linux/cdc_com.h>
#endif

// Alex.shi 接近传感器的调试与整机装机和TP有关系  
// 格式:CONFIG_ZYT_IC_项目_客户_模组厂_序号
#if defined(CONFIG_MACH_SP6825EB_7668) || defined(CONFIG_MACH_SP8825EB_7668)
	// 7668财富之州一部的TP采用唯一墩泰的TP  
	#define CONFIG_ZYT_STK3101_7668_CFZZ_WEIYI_0
#endif

//CONFIG
#define CONFIG_STK_ALS_SAMPLING_RATE			2
#define CONFIG_STK_PS_SAMPLING_RATE				50
#define CONFIG_STK_PS_ENGINEER_TUNING
#define CONFIG_STK_ALS_TRANSMITTANCE_TUNING
#define CONFIG_STK_PS_GAIN_SETTING				0x09
#if defined(CONFIG_ZYT_STK3101_7668_CFZZ_WEIYI_0)
#define CONFIG_STK_ALS_TRANSMITTANCE			125
#define CONFIG_STK_PS_CODE_HIGH_THRESHOLD		60
#define CONFIG_STK_PS_CODE_LOW_THRESHOLD		50
#else
#define CONFIG_STK_ALS_TRANSMITTANCE			500
#define CONFIG_STK_PS_CODE_HIGH_THRESHOLD		120
#define CONFIG_STK_PS_CODE_LOW_THRESHOLD		110
#endif
#define CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD
#define CONFIG_STK_ALS_CHANGE_THRESHOLD			20
#define CONFIG_STK_SYSFS_DBG
// Alex.shi 测试时打开以下宏,正式版本不开
#undef CONFIG_STK_SHOW_INFO

#define STK_PS_SLEEP_TIME						0x01
#define STK_PS_INTEGRAL_TIME					0x00
#define STK_PS_IRLED_DRIVING_CURRENT			0
//CONFIG END


#include <linux/i2c/stk_i2c_ps31xx.h>
#include <linux/i2c/stk_defines.h>
#include <linux/i2c/stk_lk_defs.h>

#define ALS_ODR_DELAY			(1000/CONFIG_STK_ALS_SAMPLING_RATE)
#define PS_ODR_DELAY			(1000/CONFIG_STK_PS_SAMPLING_RATE)

#define stk_writew	i2c_smbus_write_word_data
#define stk_readw	i2c_smbus_read_word_data

#define STK_LOCK0	mutex_unlock(&stkps_io_lock)
#define STK_LOCK1	mutex_lock(&stkps_io_lock)

static int32_t init_all_setting(void);
static int32_t enable_ps(uint8_t enable);
static int32_t enable_als(uint8_t enable);
static int32_t software_reset(void);

static int32_t set_als_it(uint8_t it);
static int32_t set_als_gain(uint8_t gain);
static int32_t set_ps_it(uint8_t it);
static int32_t set_ps_slp(uint8_t slp);
static int32_t set_ps_led_driving_current(uint8_t irdr);
static int32_t set_ps_gc(uint8_t gc);

static int32_t set_ps_thd_l(uint8_t thd_l);
static int32_t set_ps_thd_h(uint8_t thd_h);

static int ps_polling_function(void* arg);
static int als_polling_function(void* arg);
static struct task_struct *ps_polling_tsk=NULL;
static struct task_struct *als_polling_tsk=NULL;

static struct mutex stkps_io_lock;
static struct completion ps_thread_completion;
static struct completion als_thread_completion;
static struct stkps31xx_data* pStkPsData = NULL;
static struct wake_lock proximity_sensor_wakelock;
static struct wake_lock ps_poll_wakelock;

#ifdef CONFIG_STK_PS_ENGINEER_TUNING
static uint8_t ps_code_low_thd;
static uint8_t ps_code_high_thd;
#endif

#ifdef CONFIG_STK_ALS_TRANSMITTANCE_TUNING
static int32_t als_transmittance = CONFIG_STK_ALS_TRANSMITTANCE;
#endif //CONFIG_STK_ALS_TRANSMITTANCE_TUNING

inline uint32_t alscode2lux(uint32_t alscode)
{
	alscode += ((alscode<<7)+(alscode<<3)+(alscode>>1));     // 137.5
	//x1       //   x128         x8            x0.5
	alscode<<=3; // x 8 (software extend to 19 bits)
	// Gain & IT setting ==> x8
	// ==> i.e. code x 8800
	// Org : 1 code = 0.88 Lux
	// 8800 code = 0.88 lux --> this means it must be * 1/10000


#ifdef CONFIG_STK_ALS_TRANSMITTANCE_TUNING
	alscode/=als_transmittance;
#else
	alscode/=CONFIG_STK_ALS_TRANSMITTANCE;
#endif //CONFIG_STK_ALS_TRANSMITTANCE_TUNING
	return alscode;
}

static int32_t init_all_setting(void)
{
	if (software_reset()<0)
	{
		ERR("STK PS : error --> device not found\n");
		return 0;
	}
	enable_ps(0);
	enable_als(0);
	set_ps_slp(STK_PS_SLEEP_TIME);
	set_ps_gc(CONFIG_STK_PS_GAIN_SETTING);
	set_ps_it(STK_PS_INTEGRAL_TIME);
	set_ps_led_driving_current(STK_PS_IRLED_DRIVING_CURRENT);
	set_als_gain(0x01); // x2
	set_als_it(0x02); // x4
	set_ps_thd_h(CONFIG_STK_PS_CODE_HIGH_THRESHOLD);
	set_ps_thd_l(CONFIG_STK_PS_CODE_LOW_THRESHOLD);

	return 1;
}

static int32_t software_reset(void)
{
	// software reset and check stk 83xx is valid
	int32_t	r;
	uint8_t	w_reg;
	uint8_t	org_reg;

	INFO("STK PS : i2c_addr=0x%x\n", pStkPsData->client->addr);

	r = i2c_smbus_read_byte_data(pStkPsData->client,STK_PS_STATUS_REG);
	INFO("STK PS read: [0x%x]=0x%x\n", STK_PS_STATUS_REG, r);
	if (r<0)
	{
		ERR("STK PS software reset: read i2c error\n");
		return r;
	}
	if ((r&STK_PS_STATUS_ID_MASK)!=STK_PS31xx_ID)
	{
		ERR("STK PS : invalid ID number");
		return -EINVAL;
	}
	r = i2c_smbus_read_byte_data(pStkPsData->client,STK_PS_GC_REG);
	INFO("STK PS read: [0x%x]=0x%x\n", STK_PS_GC_REG, r);
	if (r<0)
	{
		ERR("STK PS software reset: read i2c error\n");
		return r;
	}
	org_reg = (uint8_t)(r&0xf0);
	w_reg = ~((uint8_t)(r&0xff));
	r = i2c_smbus_write_byte_data(pStkPsData->client,STK_PS_GC_REG,w_reg);
	INFO("STK PS write: [0x%x]=0x%x\n", STK_PS_GC_REG, w_reg);
	if (r<0)
	{
		ERR("STK PS software reset: write i2c error\n");
		return r;
	}
	r = i2c_smbus_read_byte_data(pStkPsData->client,STK_PS_GC_REG);
	INFO("STK PS read: [0x%x]=0x%x\n", STK_PS_GC_REG, r);
	if (w_reg!=(uint8_t)(r&0xff))
	{
		ERR("STK PS software reset: read-back value is not  the same\n");
		return -1;
	}
	r = i2c_smbus_write_byte_data(pStkPsData->client,STK_PS_SOFTWARE_RESET_REG,0);
	INFO("STK PS write: [0x%x]=0x%x\n", STK_PS_SOFTWARE_RESET_REG, 0);
	msleep(5);
	if (r<0)
	{
		ERR("STK PS software reset: read error after reset\n");
		return r;
	}
	return 0;
}



static int32_t set_als_it(uint8_t it)
{
	pStkPsData->als_cmd_reg &= (~STK_ALS_CMD_IT_MASK);
	pStkPsData->als_cmd_reg |= (STK_ALS_CMD_IT_MASK & STK_ALS_CMD_IT(it));
	return i2c_smbus_write_byte_data(pStkPsData->client,STK_ALS_CMD_REG,pStkPsData->als_cmd_reg);
}
static int32_t set_als_gain(uint8_t gain)
{
	if(gain >= 2)
	{
		INFO("STK PS : als_gain >= 2\n");
	}
	pStkPsData->als_cmd_reg &= (~STK_ALS_CMD_GAIN_MASK);
	pStkPsData->als_cmd_reg |= (STK_ALS_CMD_GAIN_MASK & STK_ALS_CMD_GAIN(gain));
	return i2c_smbus_write_byte_data(pStkPsData->client,STK_ALS_CMD_REG,pStkPsData->als_cmd_reg);
}
static int32_t set_ps_it(uint8_t it)
{
	pStkPsData->ps_cmd_reg &= (~STK_PS_CMD_IT_MASK);
	pStkPsData->ps_cmd_reg |= (STK_PS_CMD_IT_MASK & STK_PS_CMD_IT(it));
	return i2c_smbus_write_byte_data(pStkPsData->client,STK_PS_CMD_REG,pStkPsData->ps_cmd_reg);
}
static int32_t set_ps_slp(uint8_t slp)
{
	pStkPsData->ps_cmd_reg &= (~STK_PS_CMD_SLP_MASK);
	pStkPsData->ps_cmd_reg |= (STK_PS_CMD_SLP_MASK & STK_PS_CMD_SLP(slp));
	return i2c_smbus_write_byte_data(pStkPsData->client,STK_PS_CMD_REG,pStkPsData->ps_cmd_reg);
}
static int32_t set_ps_led_driving_current(uint8_t irdr)
{
	pStkPsData->ps_cmd_reg &= (~STK_PS_CMD_DR_MASK);
	pStkPsData->ps_cmd_reg |= (STK_PS_CMD_DR_MASK & STK_PS_CMD_DR(irdr));
	return i2c_smbus_write_byte_data(pStkPsData->client,STK_PS_CMD_REG,pStkPsData->ps_cmd_reg);
}
static int32_t set_ps_gc(uint8_t gc)
{
	int32_t retval;

	retval = i2c_smbus_read_byte_data(pStkPsData->client,STK_PS_GC_REG);
		if (retval<0)
	return retval;
	pStkPsData->ps_gc_reg = (uint8_t)retval;
	pStkPsData->ps_gc_reg &= (~STK_PS_GC_GAIN_MASK);
	pStkPsData->ps_gc_reg |= (STK_PS_GC_GAIN(gc)&STK_PS_GC_GAIN_MASK);

	return i2c_smbus_write_byte_data(pStkPsData->client,STK_PS_GC_REG,pStkPsData->ps_gc_reg);
}


static int32_t set_ps_thd_l(uint8_t thd_l)
{
#ifdef CONFIG_STK_PS_ENGINEER_TUNING
	ps_code_low_thd = thd_l;
#endif
	return 0;
}
static int32_t set_ps_thd_h(uint8_t thd_h)
{
#ifdef CONFIG_STK_PS_ENGINEER_TUNING
	ps_code_high_thd = thd_h;
#endif
	return 0;
}

inline static int32_t get_als_reading(void)
{
	int32_t word_data, tmp_word_data;
	/*
	int32_t lsb,msb;
	msb = i2c_smbus_read_byte_data(pStkPsData->client,STK_ALS_DT1_REG);
	lsb = i2c_smbus_read_byte_data(pStkPsData->client,STK_ALS_DT2_REG);
	word_data = (msb<<8) | lsb;
	*/
	// make sure MSB and LSB are the same data set
	tmp_word_data = i2c_smbus_read_word_data(pStkPsData->client, STK_ALS_DT1_REG);
	if(tmp_word_data < 0)
	{
		ERR("STK PS :%s fail, err=0x%x", __func__, tmp_word_data);
		return tmp_word_data;	   
	}
	else
	{
		word_data = ((tmp_word_data & 0xFF00) >> 8) | ((tmp_word_data & 0x00FF) << 8) ;
		//INFO("%s: word_data=0x%4x\n", __func__, word_data);
		return word_data;
	}
}

inline int32_t get_ps_reading(void)
{
	int32_t ps;

	ps = i2c_smbus_read_byte_data(pStkPsData->client,STK_PS_READING_REG);
	//INFO("STK PS :%s read ps, value=0x%x", __func__, ps);
	if(ps < 0)
	{
		ERR("STK PS :%s fail, err=0x%x", __func__, ps);		
		return -EINVAL;
	}
	return ps;	
}

inline void als_report_event(struct input_dev* dev,int32_t report_value)
{
	input_report_abs(dev, ABS_MISC, report_value);
	input_sync(dev);
	INFO("STK PS : als input event %d lux\n",report_value);
}

inline void update_and_check_report_als(int32_t als_code)
{
	int32_t lux_last,lux;
	lux_last = pStkPsData->als_lux_last;
	lux = alscode2lux(als_code);
	if (unlikely(abs(lux - lux_last)>=CONFIG_STK_ALS_CHANGE_THRESHOLD))
	{
		pStkPsData->als_lux_last = lux;
		als_report_event(pStkPsData->ps_input_dev,lux);
	}
}

static int als_polling_function(void* arg)
{
	uint32_t delay;
	init_completion(&als_thread_completion);
	while (1)
	{
		STK_LOCK(1);
		delay = pStkPsData->als_delay;
		STK_LOCK(0);
		msleep(delay);		/* wait for IT */

		STK_LOCK(1);        
		pStkPsData->als_reading = get_als_reading();
		update_and_check_report_als(pStkPsData->als_reading);
		if (pStkPsData->bALSThreadRunning == 0)
			break;
		STK_LOCK(0);        
	};

	STK_LOCK(0);
	complete(&als_thread_completion);

	return 0;
}


inline void ps_report_event(struct input_dev* dev,int32_t report_value)
{
	pStkPsData->ps_distance_last = report_value;
	input_report_abs(dev, ABS_DISTANCE, report_value);
	input_sync(dev);
	INFO("STK PS : ps input event %d\n",report_value);
	wake_lock_timeout(&proximity_sensor_wakelock, 2*HZ);
}


inline void update_and_check_report_ps(int32_t ps_code)
{
	int32_t ps_code_last;

	ps_code_last = pStkPsData->ps_code_last;
#ifdef CONFIG_STK_PS_ENGINEER_TUNING
	//INFO("STK PS : ps_code=%d, ps_code_high_thd=%d\n",ps_code, ps_code_high_thd);
	if (ps_code > ps_code_high_thd)
	{
		if (unlikely(ps_code_last < ps_code_high_thd))
		{
			pStkPsData->ps_code_last = ps_code;
			ps_report_event(pStkPsData->ps_input_dev,0);
		}
	}
#else
	//INFO("STK PS : ps_code=%d, ps_code_high_thd=%d\n",ps_code, CONFIG_STK_PS_CODE_HIGH_THRESHOLD);
	if (ps_code > CONFIG_STK_PS_CODE_HIGH_THRESHOLD)
	{
		if (unlikely(ps_code_last < CONFIG_STK_PS_CODE_HIGH_THRESHOLD))
		{
			pStkPsData->ps_code_last = ps_code;
			ps_report_event(pStkPsData->ps_input_dev,0);
		}
	}
#endif

#ifdef CONFIG_STK_PS_ENGINEER_TUNING
	else if (ps_code < ps_code_low_thd)
	{
		if (ps_code_last > ps_code_low_thd)
		{
			pStkPsData->ps_code_last = ps_code;
			ps_report_event(pStkPsData->ps_input_dev,1);
		}
	}
#else
	else  if (ps_code < CONFIG_STK_PS_CODE_LOW_THRESHOLD)
	{
		if (ps_code_last > CONFIG_STK_PS_CODE_LOW_THRESHOLD)
		{
			pStkPsData->ps_code_last = ps_code;
			ps_report_event(pStkPsData->ps_input_dev,1);
		}
	}
#endif

	/*
	ps_distance_last = pStkPsData->ps_distance_last;
	distance_i = ps31xx_lookup_table[ps_code];
	if (distance_i>CONFIG_STK_PS_DISTANCE_HIGH_THRESHOLD)
	{
		// Long Range
		if (unlikely(ps_distance_last<CONFIG_STK_PS_DISTANCE_HIGH_THRESHOLD))
		{// From Mid/Short to Long
			pStkPsData->ps_distance_last = distance_i;
			ps_report_event(pStkPsData->ps_input_dev,1);
		}
	}
	else if (distance_i<CONFIG_STK_PS_DISTANCE_LOW_THRESHOLD)
	{ // Short Range
		if (ps_distance_last>CONFIG_STK_PS_DISTANCE_LOW_THRESHOLD)
		{
			// Form Mid/Long to Short
			pStkPsData->ps_distance_last = distance_i;
			ps_report_event(pStkPsData->ps_input_dev,0);
		}
	}
	*/
}

static int ps_polling_function(void* arg)
{
	uint32_t delay;

	init_completion(&ps_thread_completion);
	while (1)
	{		
		STK_LOCK(1);
		delay = pStkPsData->ps_delay;		
		STK_LOCK(0);
		msleep(delay);	/* wait for IT */

		STK_LOCK(1);        
		pStkPsData->ps_reading = get_ps_reading();
		update_and_check_report_ps(pStkPsData->ps_reading);
		if (pStkPsData->bPSThreadRunning == 0)
			break;
		STK_LOCK(0);
	};
	STK_LOCK(0);
	complete(&ps_thread_completion);

	return 0;
}

static int32_t enable_ps(uint8_t enable)
{
	int32_t ret;

	if (enable)
	{
		if (pStkPsData->bPSThreadRunning == 0)
		{
			// Dummy Report
			/*ps_report_event(pStkPsData->ps_input_dev,-1);*/

			//reset ps reading, and let it generate an input event while enabling
			//pStkPsData->ps_distance_last =
			//(CONFIG_STK_PS_DISTANCE_HIGH_THRESHOLD+CONFIG_STK_PS_DISTANCE_LOW_THRESHOLD)/2;
			wake_lock(&ps_poll_wakelock);
			pStkPsData->ps_code_last = 0;
			pStkPsData->ps_reading = 0;
			pStkPsData->bPSThreadRunning = 1;
			ps_report_event(pStkPsData->ps_input_dev,1);
			ps_polling_tsk = kthread_run(ps_polling_function,NULL,"ps_polling");
		}
		else
		{
			WARNING("STK PS : thread has running\n");
		}
	}
	else
	{
		if (pStkPsData->bPSThreadRunning)
		{
			pStkPsData->bPSThreadRunning = 0;
			STK_LOCK(0);
			wait_for_completion(&ps_thread_completion);
			STK_LOCK(1);
			ps_polling_tsk = NULL;
			wake_unlock(&ps_poll_wakelock);
		}
	}

	pStkPsData->ps_cmd_reg &= (~STK_PS_CMD_SD_MASK);
	pStkPsData->ps_cmd_reg |= STK_PS_CMD_SD(enable?0:1);
	ret = i2c_smbus_write_byte_data(pStkPsData->client,STK_PS_CMD_REG,pStkPsData->ps_cmd_reg);

	if(ret < 0)
	{
		ERR("STK PS: %s: fail, err=0x%x", __func__, ret);	
		return ret;			
	}

	return 0;
}

static int32_t enable_als(uint8_t enable)
{
	int32_t	ret;

	if (enable)
	{
		if (pStkPsData->bALSThreadRunning == 0)
		{
			pStkPsData->als_reading = 0;
			pStkPsData->bALSThreadRunning = 1;
			als_polling_tsk = kthread_run(als_polling_function,NULL,"als_polling");
		}
		else
		{
			WARNING("STK ALS : thread has running\n");
		}
	}
	else
	{
		if (pStkPsData->bALSThreadRunning)
		{
			pStkPsData->bALSThreadRunning = 0;
			STK_LOCK(0);
			wait_for_completion(&als_thread_completion);
			STK_LOCK(1);
			als_polling_tsk = NULL;
		}
	}
	pStkPsData->als_cmd_reg &= (~STK_ALS_CMD_SD_MASK);
	pStkPsData->als_cmd_reg |= STK_ALS_CMD_SD(enable?0:1);
	ret = i2c_smbus_write_byte_data(pStkPsData->client,STK_ALS_CMD_REG,pStkPsData->als_cmd_reg);
	if(ret < 0)
	{
		ERR("STK PS: %s: fail, err=0x%x", __func__, ret);	
		return ret;			
	}

	return 0;
}


#ifdef CONFIG_STK_SYSFS_DBG
// For Debug
static ssize_t help_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "Usage : cat xxxx\nor echo val > xxxx\
		\nWhere xxxx = ps_code : RO (0~255)\nals_code : RO (0~65535)\nlux : RW (0~by your setting)\ndistance : RW(by your setting)\
		\nals_enable : RW (0~1)\nps_enable : RW(0~1)\nals_transmittance : RW (1~10000)\
		\nps_sleep_time : RW (0~3)\nps_led_driving_current : RW(0~1)\nps_integral_time(0~3)\nps_gain_setting : RW(0~3)\n");
}

static ssize_t driver_version_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf,"%s\n",STK_DRIVER_VER);
}

static ssize_t als_code_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t reading;
	STK_LOCK(1);
	reading = pStkPsData->als_reading;
	STK_LOCK(0);
	return sprintf(buf, "%d\n", reading);
}


static ssize_t ps_code_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t reading;
	STK_LOCK(1);
	reading = pStkPsData->ps_reading;
	STK_LOCK(0);
	return sprintf(buf, "%d\n", reading);
}

#endif //CONFIG_STK_SYSFS_DBG

static ssize_t lux_range_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "%d\n", alscode2lux((1<<16) -1));//full code

}

static ssize_t dist_mode_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf,"%d\n",(int32_t)STK_PS_DISTANCE_MODE);
}
static ssize_t dist_res_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "1\n"); // means 0.001 cm in Android
}
static ssize_t lux_res_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "1\n"); // means 1 lux in Android
}
static ssize_t distance_range_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "%d\n",1);
}

static ssize_t ps_enable_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t enable;
	STK_LOCK(1);
	enable = pStkPsData->bPSThreadRunning;
	STK_LOCK(0);
	return sprintf(buf, "%d\n", enable);
}

static ssize_t ps_enable_store(struct kobject *kobj,
                               struct kobj_attribute *attr,
                               const char *buf, size_t len)
{
	uint32_t value = simple_strtoul(buf, NULL, 10);
	INFO("STK PS31xx Driver : Enable PS : %d\n",value);
	STK_LOCK(1);
	enable_ps(value);
	STK_LOCK(0);
	return len;
}

static ssize_t als_enable_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t enable;
	STK_LOCK(1);
	enable = pStkPsData->bALSThreadRunning;
	STK_LOCK(0);
	return sprintf(buf, "%d\n", enable);
}


static ssize_t als_enable_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	uint32_t value = simple_strtoul(buf, NULL, 10);
	INFO("STK PS31xx Driver : Enable ALS : %d\n",value);
	STK_LOCK(1);
	enable_als(value);
	STK_LOCK(0);
	return len;
}


static ssize_t lux_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t als_reading;
	STK_LOCK(1);
	als_reading = pStkPsData->als_reading;
	STK_LOCK(0);
	return sprintf(buf, "%d lux\n", alscode2lux(als_reading));
}

static ssize_t lux_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 16);
	STK_LOCK(1);
	als_report_event(pStkPsData->ps_input_dev,value);
	STK_LOCK(0);
	return len;
}

static ssize_t distance_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t dist=1;
	STK_LOCK(1);
#ifdef CONFIG_STK_PS_ENGINEER_TUNING
	if (get_ps_reading()>=ps_code_high_thd)
#else
	if (get_ps_reading()>=CONFIG_STK_PS_CODE_HIGH_THRESHOLD)
#endif
	{
		ps_report_event(pStkPsData->ps_input_dev,0);
		dist=0;
	}
	else
	{
		ps_report_event(pStkPsData->ps_input_dev,1);
		dist=1;
	}
	STK_LOCK(0);

	return sprintf(buf, "%d\n", dist);
}

static ssize_t distance_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 16);
	STK_LOCK(1);
	ps_report_event(pStkPsData->ps_input_dev,value);
	STK_LOCK(0);
	return len;
}

#ifdef CONFIG_STK_SYSFS_DBG
/* Only for debug */
static struct kobj_attribute help_attribute = (struct kobj_attribute)__ATTR_RO(help);
static struct kobj_attribute driver_version_attribute = (struct kobj_attribute)__ATTR_RO(driver_version);
static struct kobj_attribute als_code_attribute = (struct kobj_attribute)__ATTR_RO(als_code);
static struct kobj_attribute ps_code_attribute = (struct kobj_attribute)__ATTR_RO(ps_code);
#endif //CONFIG_STK_SYSFS_DBG

static struct kobj_attribute lux_range_attribute = (struct kobj_attribute)__ATTR_RO(lux_range);
static struct kobj_attribute lux_attribute = (struct kobj_attribute)__ATTR_RW(lux);
static struct kobj_attribute distance_attribute = (struct kobj_attribute)__ATTR_RW(distance);
static struct kobj_attribute ps_enable_attribute = (struct kobj_attribute)__ATTR_RW(ps_enable);
static struct kobj_attribute als_enable_attribute = (struct kobj_attribute)__ATTR_RW(als_enable);
static struct kobj_attribute ps_dist_mode_attribute = (struct kobj_attribute)__ATTR_RO(dist_mode);
static struct kobj_attribute ps_dist_res_attribute = (struct kobj_attribute)__ATTR_RO(dist_res);
static struct kobj_attribute lux_res_attribute = (struct kobj_attribute)__ATTR_RO(lux_res);
static struct kobj_attribute ps_distance_range_attribute = (struct kobj_attribute)__ATTR_RO(distance_range);

#ifdef CONFIG_STK_SYSFS_DBG
#ifdef CONFIG_STK_ALS_TRANSMITTANCE_TUNING
static ssize_t als_transmittance_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t transmittance;
	STK_LOCK(1);
	transmittance = als_transmittance;
	STK_LOCK(0);
	return sprintf(buf, "%d\n", transmittance);
}


static ssize_t als_transmittance_store(struct kobject *kobj,
                                       struct kobj_attribute *attr,
                                       const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	STK_LOCK(1);
	als_transmittance = value;
	STK_LOCK(0);
	return len;
}


static struct kobj_attribute als_transmittance_attribute = (struct kobj_attribute)__ATTR_RW(als_transmittance);
#endif // CONFIG_STK_ALS_TRANSMITTANCE_TUNING



#ifdef CONFIG_STK_PS_ENGINEER_TUNING
static ssize_t ps_sleep_time_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t value;
	STK_LOCK(1);
	value = pStkPsData->ps_cmd_reg;
	STK_LOCK(0);
	value&=STK_PS_CMD_SLP_MASK;
	value>>=STK_PS_CMD_SLP_SHIFT;
	return sprintf(buf, "0x%x\n", value);
}


static ssize_t ps_sleep_time_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	STK_LOCK(1);
	set_ps_slp(value);
	STK_LOCK(0);
	return len;
}


static ssize_t ps_led_driving_current_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t value;
	STK_LOCK(1);
	value = pStkPsData->ps_cmd_reg;
	STK_LOCK(0);
	value&=STK_PS_CMD_DR_MASK;
	value>>=STK_PS_CMD_DR_SHIFT;
	return sprintf(buf, "0x%x\n", value);
}


static ssize_t ps_led_driving_current_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	STK_LOCK(1);
	set_ps_led_driving_current(value);
	STK_LOCK(0);
	return len;
}
static ssize_t ps_integral_time_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t value;
	STK_LOCK(1);
	value = pStkPsData->ps_cmd_reg;
	STK_LOCK(0);
	value&=STK_PS_CMD_IT_MASK;
	value>>=STK_PS_CMD_IT_SHIFT;
	return sprintf(buf, "0x%x\n", value);
}


static ssize_t ps_integral_time_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	STK_LOCK(1);
	set_ps_it((uint8_t)value);
	STK_LOCK(0);
	return len;
}
static ssize_t ps_gain_setting_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t gc_reg;
	STK_LOCK(1);
	gc_reg = pStkPsData->ps_gc_reg;
	STK_LOCK(0);
	return sprintf(buf, "0x%x\n", gc_reg);
}


static ssize_t ps_gain_setting_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	STK_LOCK(1);
	set_ps_gc((uint8_t)value);
	STK_LOCK(0);
	return len;
}


static ssize_t ps_code_thd_l_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "%d\n", ps_code_low_thd);
}


static ssize_t ps_code_thd_l_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	STK_LOCK(1);
	set_ps_thd_l(value);
	STK_LOCK(0);
	return len;
}

static ssize_t ps_code_thd_h_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "%d\n", ps_code_high_thd);
}


static ssize_t ps_code_thd_h_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf, size_t len)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	STK_LOCK(1);
	set_ps_thd_h(value);
	STK_LOCK(0);
	return len;
}

static ssize_t all_reg_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	int32_t ps_reg[13];
	uint8_t cnt;
	STK_LOCK(1);
	for(cnt=0;cnt<12;cnt++)
	{
		ps_reg[cnt] = i2c_smbus_read_byte_data(pStkPsData->client, (cnt+1));
		if(ps_reg[cnt] < 0)
		{
			STK_LOCK(0);
			ERR("all_reg_show:i2c_smbus_read_byte_data fail, ret=%d", ps_reg[cnt]);	
			return -EINVAL;
		}
		else
		{
			INFO("reg[%2X]=0x%2X\n", cnt+1, ps_reg[cnt]);
		}
	}
	ps_reg[12] = i2c_smbus_read_byte_data(pStkPsData->client, STK_PS_GC_REG);
	if(ps_reg[12] < 0)
	{
		STK_LOCK(0);
		ERR("all_reg_show:i2c_smbus_read_byte_data fail, ret=%d", ps_reg[12]);	
		return -EINVAL;
	}
	INFO("reg[0x82]=0x%2X\n", ps_reg[12]);	
	STK_LOCK(0);

	return sprintf(buf, "%2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2x\n", ps_reg[0], ps_reg[1], ps_reg[2], ps_reg[3], 
		ps_reg[4], ps_reg[5], ps_reg[6], ps_reg[7], ps_reg[8], ps_reg[9], ps_reg[10], ps_reg[11], ps_reg[12]);
}

static ssize_t recv_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return 0;
}

static ssize_t recv_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len)
{
	unsigned long value;
	int ret;
	int32_t recv_data;	
	if((ret = strict_strtoul(buf, 16, &value)) < 0)
	{
		ERR("STK PS %s:strict_strtoul failed, ret=0x%x", __func__, ret);
		return ret;	
	}

	recv_data = i2c_smbus_read_byte_data(pStkPsData->client,value);
	printk("STK PS: reg 0x%x=0x%x\n", (int)value, recv_data);
	return len;
}

static ssize_t send_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return 0;
}

static ssize_t send_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len)
{
	int addr, cmd;
	u8 addr_u8, cmd_u8;
	s32 ret, i;
	char *token[10];

	for (i = 0; i < 2; i++)
		token[i] = strsep((char **)&buf, " ");
	if((ret = strict_strtoul(token[0], 16, (unsigned long *)&(addr))) < 0)
	{
		ERR("STK PS %s:strict_strtoul failed, ret=0x%x", __func__, ret);
		return ret;	
	}
	if((ret = strict_strtoul(token[1], 16, (unsigned long *)&(cmd))) < 0)
	{
		ERR("STK PS %s:strict_strtoul failed, ret=0x%x", __func__, ret);
		return ret;	
	}
	INFO("STK PS: write reg 0x%x=0x%x\n", addr, cmd);		
	/*
	if(2 != sscanf(buf, "%2x %2x", &addr, &cmd))
	{
	ERR("STK PS %s: unknown format\n", __func__);
	return 0;
	}
	*/
	addr_u8 = (u8) addr;
	cmd_u8 = (u8) cmd;
	STK_LOCK(1);
	ret = i2c_smbus_write_byte_data(pStkPsData->client,addr_u8,cmd_u8);
	STK_LOCK(0);
	if (0 != ret)
	{	
		ERR("STK PS %s: i2c_smbus_write_byte_data fail\n", __func__);
		return ret;
	}

	return len;	
}

static struct kobj_attribute ps_sleep_time_attribute = (struct kobj_attribute)__ATTR_RW(ps_sleep_time);
static struct kobj_attribute ps_led_driving_current_attribute = (struct kobj_attribute)__ATTR_RW(ps_led_driving_current);
static struct kobj_attribute ps_integral_time_attribute = (struct kobj_attribute)__ATTR_RW(ps_integral_time);
static struct kobj_attribute ps_gain_setting_attribute = (struct kobj_attribute)__ATTR_RW(ps_gain_setting);
static struct kobj_attribute ps_code_thd_l_attribute = (struct kobj_attribute)__ATTR_RW(ps_code_thd_l);
static struct kobj_attribute ps_code_thd_h_attribute = (struct kobj_attribute)__ATTR_RW(ps_code_thd_h);
static struct kobj_attribute all_reg_attribute = (struct kobj_attribute)__ATTR_RO(all_reg);
static struct kobj_attribute dev_recv_attribute = (struct kobj_attribute)__ATTR_RW(recv);
static struct kobj_attribute dev_send_attribute = (struct kobj_attribute)__ATTR_RW(send);
#endif //CONFIG_STK_PS_ENGINEER_TUNING

#endif //CONFIG_STK_SYSFS_DBG

static struct attribute* sensetek_optical_sensors_attrs [] =
{
	&lux_range_attribute.attr,
	&lux_attribute.attr,
	&distance_attribute.attr,
	&ps_enable_attribute.attr,
	&als_enable_attribute.attr,
	&ps_dist_mode_attribute.attr,
	&ps_dist_res_attribute.attr,
	&lux_res_attribute.attr,
	&ps_distance_range_attribute.attr,
	NULL,
};

#ifdef CONFIG_STK_SYSFS_DBG
static struct attribute* sensetek_optical_sensors_dbg_attrs [] =
{
	&help_attribute.attr,
	&driver_version_attribute.attr,
	&lux_range_attribute.attr,
	&ps_code_attribute.attr,
	&als_code_attribute.attr,
	&lux_attribute.attr,
	&distance_attribute.attr,
	&ps_enable_attribute.attr,
	&als_enable_attribute.attr,
	&ps_dist_mode_attribute.attr,
	&ps_dist_res_attribute.attr,
	&lux_res_attribute.attr,
	&ps_distance_range_attribute.attr,
	#ifdef CONFIG_STK_ALS_TRANSMITTANCE_TUNING
	&als_transmittance_attribute.attr,
	#endif
	#ifdef CONFIG_STK_PS_ENGINEER_TUNING
	&ps_sleep_time_attribute.attr,
	&ps_led_driving_current_attribute.attr,
	&ps_integral_time_attribute.attr,
	&ps_gain_setting_attribute.attr,
	&ps_code_thd_l_attribute.attr,
	&ps_code_thd_h_attribute.attr,
	&all_reg_attribute.attr,		
	&dev_recv_attribute.attr,
	&dev_send_attribute.attr,
	#endif //CONFIG_STK_PS_ENGINEER_TUNING
	NULL,
};

// those attributes are only for engineer test/debug
static struct attribute_group sensetek_optics_sensors_attrs_group =
{
	.name = "DBG",
	.attrs = sensetek_optical_sensors_dbg_attrs,
};
#endif //CONFIG_STK_SYSFS_DBG

static struct platform_device *stk_oss_dev = NULL; /* Device structure */

static int stk_sysfs_create_files(struct kobject *kobj,struct attribute** attrs)
{
	int err;
	while(*attrs!=NULL)
	{
		err = sysfs_create_file(kobj,*attrs);
		if (err)
			return err;
		attrs++;
	}
	return 0;
}

static int stk_sysfs_remove_files(struct kobject *kobj,struct attribute** attrs)
{
	while(*attrs!=NULL)
	{
		sysfs_remove_file(kobj,*attrs);
		attrs++;
	}
	return 0;
}

#if 0	//def CONFIG_HAS_EARLYSUSPEND
static void stk31xx_early_suspend(struct early_suspend *h)
{
	int32_t enable;
	INFO("%s", __func__);
	STK_LOCK(1);
	enable = (pStkPsData->als_cmd_reg & STK_ALS_CMD_SD_MASK)?0:1;    		
	if(enable)
	{
		enable_als(0);
		pStkPsData->als_cmd_reg &= (~STK_ALS_CMD_SD_MASK);	
	}
	STK_LOCK(0);	
	return;
}

static void stk31xx_late_resume(struct early_suspend *h)
{
	int32_t enable;
	INFO("%s", __func__);	
	STK_LOCK(1);
	enable = (pStkPsData->als_cmd_reg & STK_ALS_CMD_SD_MASK)?0:1;    		
	if(enable)
	{
		enable_als(1);		
	}
	STK_LOCK(0);
	return;
}
#endif	//#ifdef CONFIG_HAS_EARLYSUSPEND


#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
static ssize_t chip_id_show(struct kobject * kobj, struct kobj_attribute * attr, char * buf)
{
	return sprintf(buf, "%d\n", 0x3101);
}

static struct kobj_attribute chip_id_attribute = (struct kobj_attribute)__ATTR_RO(chip_id);

static struct attribute* sensors_pls_cdc_attrs [] =
{
	&als_enable_attribute.attr,
	&ps_enable_attribute.attr,
	&chip_id_attribute.attr,
	NULL,
};

static const struct attribute_group pls_cdc_attrs_group = {
	.attrs = sensors_pls_cdc_attrs,
};

static int check_plsensor_chip(void)
{
	CDC_Plsensor_Device_Id(0x3101);

	return 0;
}

static int remove_plsensor_chip(void)
{
	CDC_Plsensor_Device_Id(0xFFFF);

	return 0;
}
#endif

static int stk_ps_probe(struct i2c_client *client,
                        const struct i2c_device_id *id)
{
	int									err=0;
	struct stkps31xx_data				*ps_data;
	//struct stk31xx_pls_platform_data	*pdata = client->dev.platform_data;

	INFO("stk31xx PS : I2C Probing");
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(CDC_Plsensor_Device_Id(0)!=0)
	{
		printk("PlSensor(0x%x)Exist!", CDC_Plsensor_Device_Id(0));
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
#endif

	/*
	printk("STKPS -- %s: I2C is probing (%s)%d\n nDetect = %d\n", __func__,id->name,id->driver_data);
	*/
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
	{
		printk("STKPS -- No Support for I2C_FUNC_SMBUS_BYTE_DATA\n");
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_WORD_DATA))
	{
		printk("STKPS -- No Support for I2C_FUNC_SMBUS_WORD_DATA\n");
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	stk_oss_dev = platform_device_alloc(DEVICE_NAME, -1);
	if (!stk_oss_dev)
	{
		printk("%s: platform_device_alloc failed\n", __func__);
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
	if (platform_device_add(stk_oss_dev))
	{
		printk("%s: platform_device_add failed\n", __func__);
		err = -ENOMEM;
		goto exit_device_add;
	}
	err = stk_sysfs_create_files(&(stk_oss_dev->dev.kobj),sensetek_optical_sensors_attrs);
	if (err)
	{
		printk("%s: stk_sysfs_create_files failed\n", __func__);
		err = -ENOMEM;
		goto exit_sysfs_create_files;
	}
#ifdef CONFIG_STK_SYSFS_DBG
	err = sysfs_create_group(&(stk_oss_dev->dev.kobj), &sensetek_optics_sensors_attrs_group);
	if (err)
	{
		printk("%s: sysfs_create_group failed\n", __func__);
		err = -ENOMEM;
		goto exit_sysfs_create_group;
	}
#endif //CONFIG_STK_SYSFS_DBG


	//if (id->driver_data == 0)
	//{
	ps_data = kzalloc(sizeof(struct stkps31xx_data),GFP_KERNEL);
	if (!ps_data)
	{
		printk("%s: request memory failed\n", __func__);
		err= -ENOMEM;
		goto exit_request_memory_failed;
	}
	ps_data->client = client;
	i2c_set_clientdata(client,ps_data);
	mutex_init(&stkps_io_lock);

	ps_data->ps_input_dev = input_allocate_device();
	if (ps_data->ps_input_dev==NULL)
	{
		printk("%s: could not allocate input device\n", __func__);
		err= -ENOMEM;
		goto exit_input_alloc_device;
	}
	ps_data->ps_input_dev->name = PS_NAME;
	set_bit(EV_ABS, ps_data->ps_input_dev->evbit);
	set_bit(EV_ABS, ps_data->ps_input_dev->evbit);
	input_set_abs_params(ps_data->ps_input_dev, ABS_MISC, 0, alscode2lux((1<<16)-1), 0, 0);
	input_set_abs_params(ps_data->ps_input_dev, ABS_DISTANCE, 0,1, 0, 0);
	err = input_register_device(ps_data->ps_input_dev);
	if (err<0)
	{
		ERR("STK PS : can not register als input device\n");
		goto exit_input_alloc_device;
	}
	INFO("STK PS : register als/ps input device OK\n");
	pStkPsData = ps_data;
	ps_data->ps_delay = PS_ODR_DELAY;
	ps_data->als_delay = ALS_ODR_DELAY;
	wake_lock_init(&proximity_sensor_wakelock,WAKE_LOCK_IDLE,"stk_ps_wakelock");
	wake_lock_init(&ps_poll_wakelock,WAKE_LOCK_SUSPEND,"stkps_poll_wakelock");
	if (!init_all_setting())
	{
		err = EINVAL;
		goto exit_init_all_setting;
	}

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(check_plsensor_chip()<0)
	{
		err = -ENODEV;
		printk("Fail to find STK31xx");
		goto exit_check_chip_failed;
	}

	err = sysfs_create_group(&(ps_data->ps_input_dev->dev.kobj), &pls_cdc_attrs_group);
	if (err) {
		dev_err(&ps_data->ps_input_dev->dev, "create device file failed!\n");
		err = -EINVAL;
		goto err_create_sysfs;
	}
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
	ps_data->stk_early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ps_data->stk_early_suspend.suspend = NULL;	// stk31xx_early_suspend;
	ps_data->stk_early_suspend.resume = NULL;	// stk31xx_late_resume;
	register_early_suspend(&ps_data->stk_early_suspend);
#endif
	return 0;
	//}
	//return -EINVAL;

#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
err_create_sysfs:
	sysfs_remove_group(&(ps_data->ps_input_dev->dev.kobj), &pls_cdc_attrs_group);
exit_check_chip_failed:
	remove_plsensor_chip();
#endif
exit_init_all_setting:
	wake_lock_destroy(&proximity_sensor_wakelock);
	wake_lock_destroy(&ps_poll_wakelock);
	pStkPsData = NULL;
	input_unregister_device(ps_data->ps_input_dev);
exit_input_alloc_device:
	if (ps_data->ps_input_dev!=NULL)
		input_free_device(ps_data->ps_input_dev);
	mutex_destroy(&stkps_io_lock);
	i2c_set_clientdata(client,NULL);
	kfree(ps_data);
exit_request_memory_failed:
#ifdef CONFIG_STK_SYSFS_DBG
	sysfs_remove_group(&(stk_oss_dev->dev.kobj), &sensetek_optics_sensors_attrs_group);
exit_sysfs_create_group:
#endif
	stk_sysfs_remove_files(&(stk_oss_dev->dev.kobj), sensetek_optical_sensors_attrs);
exit_sysfs_create_files:
	platform_device_del(stk_oss_dev);
exit_device_add:
	platform_device_put(stk_oss_dev);
exit_check_functionality_failed:
	return err;
}


static int stk_ps_remove(struct i2c_client *client)
{
#ifdef CONFIG_HAS_EARLYSUSPEND
	if (pStkPsData)
	{
		unregister_early_suspend(&pStkPsData->stk_early_suspend);
	}
#endif
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	sysfs_remove_group(&(pStkPsData->ps_input_dev->dev.kobj), &pls_cdc_attrs_group);
	remove_plsensor_chip();
#endif
	wake_lock_destroy(&proximity_sensor_wakelock);
	wake_lock_destroy(&ps_poll_wakelock);
	if (pStkPsData)
	{
		if (pStkPsData->ps_input_dev!=NULL)
		{
			input_unregister_device(pStkPsData->ps_input_dev);
			input_free_device(pStkPsData->ps_input_dev);
		}
		kfree(pStkPsData);
		pStkPsData = NULL;
	}
	mutex_destroy(&stkps_io_lock);
	i2c_set_clientdata(client,NULL);
	if (stk_oss_dev);
	{
#ifdef CONFIG_STK_SYSFS_DBG
		sysfs_remove_group(&(stk_oss_dev->dev.kobj), &sensetek_optics_sensors_attrs_group);
#endif
		stk_sysfs_remove_files(&(stk_oss_dev->dev.kobj), sensetek_optical_sensors_attrs);
		platform_device_del(stk_oss_dev);
		platform_device_put(stk_oss_dev);
	}

	return 0;
}

static const struct i2c_device_id stk_ps_id[] =
{
	{STKPS_DRV_NAME, 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, stk_ps_id);

static struct i2c_driver stk_ps_driver =
{
	.driver = {
		.name = STKPS_DRV_NAME,
	},
	.probe = stk_ps_probe,
	.remove = stk_ps_remove,
	.id_table = stk_ps_id,
};


static int __init stk_i2c_ps31xx_init(void)
{
	printk("%s\n", __func__);
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
	if(CDC_Plsensor_Device_Id(0)!=0)
	{
		printk("PlSensor(0x%x)Exist!", CDC_Plsensor_Device_Id(0));
		return -ENODEV;
	}
#endif
	return i2c_add_driver(&stk_ps_driver);
}

static void __exit stk_i2c_ps31xx_exit(void)
{
	printk("%s\n", __func__);
	i2c_del_driver(&stk_ps_driver);
}

MODULE_AUTHOR("Alex Shi <sikinzen@126.com>");
MODULE_DESCRIPTION("SenseTek Proximity Sensor driver");
MODULE_LICENSE("GPL");
module_init(stk_i2c_ps31xx_init);
module_exit(stk_i2c_ps31xx_exit);
