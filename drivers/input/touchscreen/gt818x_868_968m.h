/* drivers/input/touchscreen/gt818x.h
 * 
 * 2010 - 2012 Goodix Technology.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be a reference 
 * to you, when you are integrating the GOODiX's CTP IC into your system, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 * 
 * Version:1.2
 * Revision record:
 *      V1.0:2012/06/08,create file.
 *      V1.4:2012/09/20,G868 sensor ID & coor key suppoert
 */

#ifndef _LINUX_GOODIX_TOUCH_H
#define	_LINUX_GOODIX_TOUCH_H

#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <mach/gpio.h>
#include <mach/board.h>
#include <linux/earlysuspend.h>

// Alex.shi 支持的TP列表
// 格式:CONFIG_ZYT_IC_项目_客户_模组厂_序号
#if 0

#else
	// 默认参数 7629-比欧特 带接近功能  
	// #define CONFIG_ZYT_GT868_7629_ZYT_BOT_0
#endif

struct goodix_ts_data {
    spinlock_t irq_lock;
    struct i2c_client *client;
    struct input_dev  *input_dev;
    struct hrtimer timer;
    struct work_struct  work;
    struct early_suspend early_suspend;
    s32 irq_is_disable;
    s32 use_irq;
    u16 abs_x_max;
    u16 abs_y_max;
    u8  max_touch_num;
    u8  int_trigger_type;
    u8  green_wake_mode;
    u8  chip_type;
    u8  enter_update;
    u8  gtp_is_suspend;
    u8  gtp_rawdiff_mode;
    u8  driver_cnt;
    u8  sensor_cnt;
    u8  coor_div_2;
};

enum CHIP_TYPE
{
    GT818X,
    GT868
};

extern u16 show_len;
extern u16 total_len;

//***************************PART1:ON/OFF define*******************************
#define GTP_CUSTOM_CFG        1
#define GTP_DRIVER_SEND_CFG   1
#define GTP_HAVE_TOUCH_KEY    1
#define GTP_POWER_CTRL_SLEEP  0
#define GTP_AUTO_UPDATE       0
#define GTP_CHANGE_X2Y        0
#define GTP_ESD_PROTECT       0
#define GTP_CREATE_WR_NODE    1
#define GTP_ICS_SLOT_REPORT   0
#define GTP_USE_868_968M      1//0
#define GTP_PROXIMITY         1
#define GUP_USE_HEADER_FILE   0
#define I2C_BOARD_INFO_METHOD 0

#define GTP_DEBUG_ON          0
#define GTP_DEBUG_ARRAY_ON    0
#define GTP_DEBUG_FUNC_ON     0

#if 0
#define GTP_NEGATIVE_X			0 //X翻转
#define GTP_NEGATIVE_Y			1 //Y翻转
#else
#define GTP_NEGATIVE_X			0 //X翻转
#define GTP_NEGATIVE_Y			0 //Y翻转
#endif

// 采用坐标值上报CTP虚拟按键
#define TOUCH_VIRTUAL_KEYS

//***************************PART2:TODO define**********************************
//STEP_1(REQUIRED):Change config table.
/*TODO: puts the config info corresponded to your TP here, the following is just 
a sample config, send this config should cause the chip cannot work normally*/
//default or float

#if 1  //默认参数 7629-比欧特 带接近功能  
		#define CTP_CFG_GROUP1 {\
          		0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x70,0x00,0x60,0x00,0x50,0x00,0x40,0x00,0x30,0x00,0x20,0x00,0x10,0x00,0x00,0x00,0x80,0x00,0x90,0x00,0xA0,0x00,0xB0,0x00,0xC3,0x33,0xD3,0x33,0xE0,0x00,0xF0,0x00,0x07,0x13,0x40,0x40,0x40,0x22,0x22,0x22,0x0F,0x0E,0x0A,0x40,0x30,0x55,0x03,0x00,0x05,0xE0,0x01,0x20,0x03,0x00,0x00,0x62,0x52,0x5C,0x4D,0x00,0x00,0x2F,0x14,0x05,0x2F,0x80,0x00,0x00,0x00,0x00,0x14,0x10,0x00,0x00,0x00,0x40,0x07,0x40,0x17,0x00,0x10,0x30,0x40,0x00,0x0D,0x30,0x20,0x25,0x20,0x00,0x00,0x00,0x00,0x00,0x10,0x01}
		#define CTP_CFG_GROUP2 {\
	   	 }
		#define CTP_CFG_GROUP3 {\
	    	}
#else  //默认参数 7629-比欧特 带接近功能  
		#define CTP_CFG_GROUP1 {\
        	  0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x70,0x00,0x60,0x00,0x50,0x00,0x40,0x00,0x30,0x00,0x20,0x00,0x10,0x00,0x00,0x00,0x80,0x00,0x90,0x00,0xA0,0x00,0xB0,0x00,0xC3,0x33,0xD3,0x33,0xE0,0x00,0xF0,0x00,0x07,0x13,0x40,0x40,0x40,0x22,0x22,0x22,0x0F,0x0E,0x0A,0x40,0x30,0x55,0x03,0x00,0x05,0xE0,0x01,0x20,0x03,0x00,0x00,0x62,0x52,0x5C,0x4D,0x00,0x00,0x2F,0x14,0x05,0x2F,0x80,0x00,0x00,0x00,0x00,0x14,0x10,0x00,0x00,0x00,0x40,0x07,0x40,0x17,0x00,0x10,0x30,0x40,0x00,0x0D,0x30,0x20,0x25,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x01}
		#define CTP_CFG_GROUP2 {\
	 	   }
		#define CTP_CFG_GROUP3 {\
	 	   }
#endif

//STEP_2(REQUIRED):Change I/O define & I/O operation mode.
extern int sprd_3rdparty_gpio_tp_pwr;
extern int sprd_3rdparty_gpio_tp_rst;
extern int sprd_3rdparty_gpio_tp_irq;

#if 1
#define GTP_RST_PORT    GPIO_TOUCH_RESET
#define GTP_INT_PORT    GPIO_TOUCH_IRQ
#define GTP_INT_IRQ     gpio_to_irq(GPIO_TOUCH_IRQ)
#else
#define GTP_RST_PORT    sprd_3rdparty_gpio_tp_rst         //RESET port
#define GTP_INT_PORT    sprd_3rdparty_gpio_tp_irq
#define GTP_INT_IRQ     sprd_alloc_gpio_irq(sprd_3rdparty_gpio_tp_irq)
#endif
//#define GTP_INT_CFG     S3C_GPIO_SFN(0xF)

#define GTP_GPIO_AS_INPUT(pin)          do{\
                                            gpio_direction_input(pin);\
                                        }while(0)
#define GTP_GPIO_AS_INT(pin)            do{\
                                            GTP_GPIO_AS_INPUT(pin);\
                                        }while(0)
#define GTP_GPIO_GET_VALUE(pin)         gpio_get_value(pin)
#define GTP_GPIO_OUTPUT(pin,level)      gpio_direction_output(pin,level)
#define GTP_GPIO_REQUEST(pin, label)    gpio_request(pin, label)
#define GTP_GPIO_FREE(pin)              gpio_free(pin)
#define GTP_IRQ_TAB                     {IRQ_TYPE_EDGE_FALLING,IRQ_TYPE_EDGE_RISING}

//STEP_3(optional):Custom set some config by themself,if need.
#if GTP_CUSTOM_CFG
	#define GTP_MAX_HEIGHT   800			
	#define GTP_MAX_WIDTH    480
	#define GTP_INT_TRIGGER  1    //0:Falling 1:Rising
#else
	#if defined(CONFIG_LCD_QHD)
	#define GTP_MAX_WIDTH		539
	#define GTP_MAX_HEIGHT		959
	#elif defined(CONFIG_LCD_FWVGA)
	#define GTP_MAX_WIDTH		479
	#define GTP_MAX_HEIGHT		853
	#elif defined(CONFIG_LCD_WVGA)
	#define GTP_MAX_WIDTH		479
	#define GTP_MAX_HEIGHT		799
	#elif defined(CONFIG_LCD_HVGA)
	#define GTP_MAX_WIDTH		319
	#define GTP_MAX_HEIGHT		479
	#elif defined(CONFIG_LCD_QVGA)
	#define GTP_MAX_WIDTH		239
	#define GTP_MAX_HEIGHT		319
	#else
	#error "You do not set GT8XX's resolution."
	#endif
	#define GTP_INT_TRIGGER  1
#endif
#define GTP_MAX_TOUCH      5
#define GTP_ESD_CHECK_CIRCLE  2000

//STEP_4(optional):If this project have touch key,Set touch key config.
#if GTP_HAVE_TOUCH_KEY
#if 1  //默认参数 7629-比欧特  
	#define GTP_KEY_TAB  {KEY_MENU,KEY_HOMEPAGE,KEY_BACK}
    //define your key_center & key_width\key_height here
    #if GTP_USE_868_968M
        #define KEY_CNTR_PNT_X 	      {20, 160, 246}
        #define KEY_CNTR_PNT_Y 	      {856, 856, 856}
        #define KEY_AREA_HEIGHT_H     10
        #define KEY_AREA_WIDTH_H      20
    #endif	
#else  //默认参数 7629-比欧特  
	#define GTP_KEY_TAB  {KEY_MENU,KEY_HOMEPAGE,KEY_BACK}
    //define your key_center & key_width\key_height here
    #if GTP_USE_868_968M
        #define KEY_CNTR_PNT_X 	      {20, 160, 246}
        #define KEY_CNTR_PNT_Y 	      {856, 856, 856}
        #define KEY_AREA_HEIGHT_H     10
        #define KEY_AREA_WIDTH_H      20
    #endif	
#endif
#endif

//***************************PART3:OTHER define*********************************
#define GTP_DRIVER_VERSION    "V1.4<2012/09/20>"
#define GTP_I2C_NAME          "Goodix-TS"
#define GTP_NAME			  "Goodix-TS"
#define GTP_TS_DEVICE		  "Goodix-TS"
#define GTP_TS_NAME	   	      "Goodix-Ctp-8XX"
#define GTP_TS_ADDR			  0x5d
#define GTP_POLL_TIME         10
#define GTP_ADDR_LENGTH       2
#define GTP_CONFIG_LENGTH     106
#define FAIL                  0
#define SUCCESS               1

//Register define
#define GTP_REG_CONFIG_DATA   0x6A2
#define GTP_REG_INDEX         0x712
#define GTP_REG_SLEEP         0x692
#define GTP_REG_SENSOR_ID     0x710
#define GTP_REG_VERSION       0x715
#define GTP_READ_COOR_ADDR    0x721

#define DRVCNT_LOC            51
#define RESOLUTION_LOC        61
#define TRIGGER_LOC           57

//Log define
#define GTP_INFO(fmt,arg...)           printk("<<-GTP-INFO->>[%d]"fmt"\n", __LINE__, ##arg)
#define GTP_ERROR(fmt,arg...)          printk("<<-GTP-ERROR->>[%d]"fmt"\n",__LINE__, ##arg)
#define GTP_DEBUG(fmt,arg...)          do{\
                                         if(GTP_DEBUG_ON)\
                                         printk("<<-GTP-DEBUG->>[%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define GTP_DEBUG_ARRAY(array, num)    do{\
                                         s32 i;\
                                         u8* a = array;\
                                         if(GTP_DEBUG_ARRAY_ON)\
                                         {\
                                            printk("<<-GTP-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printk("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printk("\n");\
                                                }\
                                            }\
                                            printk("\n");\
                                        }\
                                       }while(0)
#define GTP_DEBUG_FUNC()               do{\
                                         if(GTP_DEBUG_FUNC_ON)\
                                         printk("<<-GTP-FUNC->>[%d]Func:%s\n", __LINE__, __func__);\
                                       }while(0)
#define GTP_SWAP(x, y)                 do{\
                                         typeof(x) z = x;\
                                         x = y;\
                                         y = z;\
                                       }while (0)
#define ABS_VAL(x)                     ((x < 0) ? -x : x)
//****************************PART4:UPDATE define*******************************
#define PACK_SIZE            64                    //update file package size
#define SEARCH_FILE_TIMES    50
#define UPDATE_FILE_PATH_2   "/data/goodix/_goodix_update_.bin"
#define UPDATE_FILE_PATH_1   "/sdcard/goodix/_goodix_update_.bin"

//Error no
#define ERROR_NO_FILE           2   //ENOENT
#define ERROR_FILE_READ         23  //ENFILE
#define ERROR_FILE_TYPE         21  //EISDIR
#define ERROR_GPIO_REQUEST      4   //EINTR
#define ERROR_I2C_TRANSFER      5   //EIO
#define ERROR_NO_RESPONSE       16  //EBUSY
#define ERROR_TIMEOUT           110 //ETIMEDOUT

//*****************************End of Part III********************************

#endif /* _LINUX_GOODIX_TOUCH_H */
