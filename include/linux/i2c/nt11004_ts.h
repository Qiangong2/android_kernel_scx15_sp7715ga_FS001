#ifndef __LINUX_NT11004_TS_H__
#define __LINUX_NT11004_TS_H__

#include <linux/i2c.h>
#include <linux/i2c/nt11004_ts.h>
#include <linux/earlysuspend.h>
#include <mach/board.h>

#define NOVATEK_TS_ADDR		(0x34)
#define NOVATEK_HW_ADDR		(0x7F)


#define NOVATEK_TS_NAME		"nt11004_ts"


//define default resolution of LCM
#define LCD_MAX_WIDTH		320
#define LCD_MAX_HEIGHT 		480

//define default resolution of the touchscreen
#define TPD_MAX_WIDTH 		320
#define TPD_MAX_HEIGHT 		480


struct i2c_dev
{
	struct list_head list;
	struct i2c_adapter *adap;
	struct device *dev;
};

struct novatek_platform_data{
	int irq_gpio_number;
	int reset_gpio_number;
	const char *vdd_name;
};


struct ntp_i2c_data 
{
	struct i2c_client *client;
	struct input_dev *input;
	struct workqueue_struct *ts_workqueue;
	struct work_struct 	pen_event_work;
};


#define MAX_FINGER_NUM 2
#define NTP_WITH_BUTTON
#define TOUCH_VIRTUAL_KEYS


#define NTP_APK_DRIVER_FUNC_SUPPORT
//#define NTP_BOOTLOADER_FUNC_SUPPORT
//#define NTP_WAKEUP_SYSTEM_SUPPORT



#endif
