#ifndef __LINUX_FT5X0X_TS_H__
#define __LINUX_FT5X0X_TS_H__

#if defined(CONFIG_MACH_SP6825GA) || defined(CONFIG_MACH_SP6825GB) || defined(CONFIG_MACH_SP8825GB) || defined(CONFIG_MACH_SP8825GA)
#define SCREEN_MAX_X    479
#define SCREEN_MAX_Y    853
#elif defined(CONFIG_LCD_QHD)&&defined(CONFIG_MACH_SP7730EC_7685)	// alex.shi add for test SP7730EA on our project
#define SCREEN_MAX_X	719
#define SCREEN_MAX_Y	1279
#elif defined(CONFIG_LCD_QHD)
#define SCREEN_MAX_X	539
#define SCREEN_MAX_Y	959
#elif defined(CONFIG_LCD_FWVGA)
#define SCREEN_MAX_X	479
#define SCREEN_MAX_Y	853
#elif defined(CONFIG_LCD_WVGA)
#define SCREEN_MAX_X	479
#define SCREEN_MAX_Y	799
#elif defined(CONFIG_LCD_HVGA)
#define SCREEN_MAX_X	319
#define SCREEN_MAX_Y	479
#elif defined(CONFIG_LCD_QVGA)
#define SCREEN_MAX_X	239
#define SCREEN_MAX_Y	319
#else
#error "You do not set FT5306's resolution."
#endif
#define PRESS_MAX       255

#if 0	//defined(CONFIG_MACH_SP6825EB_7663) || defined(CONFIG_MACH_SP8825EB_7663)
//  Alex.shi 用7616洲际通4寸屏整机装机时要走这边,实际tiger项目后续还是让客户按我们标准做,故非能这里  
#define CTP_NEGATIVE_Y	1
#else
#define CTP_NEGATIVE_Y	0
#endif


#define FTS_CTL_IIC
#if defined(FTS_CTL_IIC)

int ft5x0x_i2c_Read(struct i2c_client *client, char *writebuf, int writelen,
		    char *readbuf, int readlen);
int ft5x0x_i2c_Write(struct i2c_client *client, char *writebuf, int writelen);
void ft5x0x_reset_tp(int HighOrLow);
#endif

#define FT5X0X_NAME			"ft5x0x_ts"//"synaptics_i2c_rmi"//"synaptics-rmi-ts"// 
#define FT5206_TS_DEVICE		"ft5x0x_ts"
#define FT5206_TS_NAME			"ft5x0x_ts"
#define FT5206_TS_ADDR			0x38

struct ft5x0x_ts_platform_data{
	int irq_gpio_number;
	int reset_gpio_number;
	const char *vdd_name;
};

enum ft5x0x_ts_regs {
	FT5X0X_REG_THGROUP					= 0x80,
	FT5X0X_REG_THPEAK						= 0x81,
	FT5X0X_REG_THCAL						= 0x82,
	FT5X0X_REG_THWATER					= 0x83,
	FT5X0X_REG_THTEMP					= 0x84,
	FT5X0X_REG_THDIFF						= 0x85,
	FT5X0X_REG_CTRL						= 0x86,
	FT5X0X_REG_TIMEENTERMONITOR			= 0x87,
	FT5X0X_REG_PERIODACTIVE				= 0x88,
	FT5X0X_REG_PERIODMONITOR			= 0x89,
	FT5X0X_REG_HEIGHT_B					= 0x8a,
	FT5X0X_REG_MAX_FRAME					= 0x8b,
	FT5X0X_REG_DIST_MOVE					= 0x8c,
	FT5X0X_REG_DIST_POINT				= 0x8d,
	FT5X0X_REG_FEG_FRAME					= 0x8e,
	FT5X0X_REG_SINGLE_CLICK_OFFSET		= 0x8f,
	FT5X0X_REG_DOUBLE_CLICK_TIME_MIN	= 0x90,
	FT5X0X_REG_SINGLE_CLICK_TIME			= 0x91,
	FT5X0X_REG_LEFT_RIGHT_OFFSET		= 0x92,
	FT5X0X_REG_UP_DOWN_OFFSET			= 0x93,
	FT5X0X_REG_DISTANCE_LEFT_RIGHT		= 0x94,
	FT5X0X_REG_DISTANCE_UP_DOWN		= 0x95,
	FT5X0X_REG_ZOOM_DIS_SQR				= 0x96,
	FT5X0X_REG_RADIAN_VALUE				=0x97,
	FT5X0X_REG_MAX_X_HIGH                       	= 0x98,
	FT5X0X_REG_MAX_X_LOW             			= 0x99,
	FT5X0X_REG_MAX_Y_HIGH            			= 0x9a,
	FT5X0X_REG_MAX_Y_LOW             			= 0x9b,
	FT5X0X_REG_K_X_HIGH            			= 0x9c,
	FT5X0X_REG_K_X_LOW             			= 0x9d,
	FT5X0X_REG_K_Y_HIGH            			= 0x9e,
	FT5X0X_REG_K_Y_LOW             			= 0x9f,
	FT5X0X_REG_AUTO_CLB_MODE			= 0xa0,
	FT5X0X_REG_LIB_VERSION_H 				= 0xa1,
	FT5X0X_REG_LIB_VERSION_L 				= 0xa2,
	FT5X0X_REG_CIPHER						= 0xa3,
	FT5X0X_REG_MODE						= 0xa4,
	FT5X0X_REG_PMODE						= 0xa5,	/* Power Consume Mode */
	FT5X0X_REG_FIRMID						= 0xa6,
	FT5X0X_REG_STATE						= 0xa7,
	FT5X0X_REG_FT5201ID					= 0xa8,
	FT5X0X_REG_ERR						= 0xa9,
	FT5X0X_REG_CLB						= 0xaa,
};

//FT5X0X_REG_PMODE
#define PMODE_ACTIVE        0x00
#define PMODE_MONITOR       0x01
#define PMODE_STANDBY       0x02
#define PMODE_HIBERNATE     0x03


	#ifndef ABS_MT_TOUCH_MAJOR
	#define ABS_MT_TOUCH_MAJOR	0x30	/* touching ellipse */
	#define ABS_MT_TOUCH_MINOR	0x31	/* (omit if circular) */
	#define ABS_MT_WIDTH_MAJOR	0x32	/* approaching ellipse */
	#define ABS_MT_WIDTH_MINOR	0x33	/* (omit if circular) */
	#define ABS_MT_ORIENTATION	0x34	/* Ellipse orientation */
	#define ABS_MT_POSITION_X	0x35	/* Center X ellipse position */
	#define ABS_MT_POSITION_Y	0x36	/* Center Y ellipse position */
	#define ABS_MT_TOOL_TYPE	0x37	/* Type of touching device */
	#define ABS_MT_BLOB_ID		0x38	/* Group set of pkts as blob */
	#endif /* ABS_MT_TOUCH_MAJOR */


#endif
