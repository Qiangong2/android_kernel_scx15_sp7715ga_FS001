#ifndef __LINUX_MSG2133_TS_H__
#define __LINUX_MSG2133_TS_H__

#define TS_DEBUG_MSG				0
#define VIRTUAL_KEYS				1
#define MSG2133_UPDATE				1

#if defined(CONFIG_LCD_FWVGA)
#define TS_WIDTH_MAX	480
#define TS_HEIGHT_MAX	854
#elif defined(CONFIG_LCD_WVGA)
#define TS_WIDTH_MAX	480
#define TS_HEIGHT_MAX	800
#elif defined(CONFIG_LCD_HVGA)
#define TS_WIDTH_MAX	320
#define TS_HEIGHT_MAX	480
#elif defined(CONFIG_LCD_QVGA)
#define TS_WIDTH_MAX	240
#define TS_HEIGHT_MAX	320
#else
#error "You do not set MSG2133's resolution."
#endif

#define MSG2133_TS_NAME				"msg2133"
#define MSG2133_TS_ADDR				0x26
#define MSG2133_FW_ADDR				0x62
#define MSG2133_FW_UPDATE_ADDR		0x49	


#define TPD_OK						0
#define TPD_REG_BASE				0x00
#define TPD_SOFT_RESET_MODE			0x01
#define TPD_OP_MODE					0x00
#define TPD_LOW_PWR_MODE			0x04
#define TPD_SYSINFO_MODE			0x10

#define GET_HSTMODE(reg)		((reg & 0x70) >> 4)		// in op mode or not 
#define GET_BOOTLOADERMODE(reg)	((reg & 0x10) >> 4)		// in bl mode 


struct ts_event {
	u16	x1;
	u16	y1;
	u16	x2;
	u16	y2;
	u8  touch_point;
};
struct msg2133_ts_platform_data{
	int irq_gpio_number;
	int reset_gpio_number;
	const char *vdd_name;
};


struct TouchScreenInfo_t{
	unsigned char	nTouchKeyMode;
	unsigned char	nTouchKeyCode;
	//unsigned char	nFingerNum;
};

#endif

