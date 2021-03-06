#ifndef __AP3212C_PLS_H__
#define __AP3212C_PLS_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/earlysuspend.h>

#define AP3212C_PLS_DEVICE 		"ap3212c_pls"
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
#define AP3212C_PLS_INPUT_DEV	AP3212C_PLS_DEVICE
#else
#define AP3212C_PLS_INPUT_DEV	"proximity"
#endif
#define AP3212C_PLS_IRQ_PIN		"ap3212c_irq_pin"
//#define AP3212C_PLS_ADDRESS		0x1C
// mickyliang ++ 
// I2C address for AP3216C
#define AP3212C_PLS_ADDRESS		0x1E
// mickyliang --
#define AP3212C_PLS_RETRY_COUNT		3

#include <linux/types.h>
#include <linux/ioctl.h>

#define LTR_IOCTL_MAGIC 				0x1C
#define LTR_IOCTL_GET_PFLAG  			_IOR(LTR_IOCTL_MAGIC, 1, int)
#define LTR_IOCTL_GET_LFLAG  			_IOR(LTR_IOCTL_MAGIC, 2, int)
#define LTR_IOCTL_SET_PFLAG  			_IOW(LTR_IOCTL_MAGIC, 3, int)
#define LTR_IOCTL_SET_LFLAG  			_IOW(LTR_IOCTL_MAGIC, 4, int)
#define LTR_IOCTL_GET_DATA  			_IOW(LTR_IOCTL_MAGIC, 5, unsigned char)


//REG old
/*
#define AP3212C_PLS_REG_CONFIG		0x00
#define AP3212C_PLS_REG_TIME_CTRL	0x01
#define AP3212C_PLS_REG_DLS_CTRL		0x02
#define AP3212C_PLS_REG_INT_STATUS	0x03
#define AP3212C_PLS_REG_DPS_CTRL		0x04
#define AP3212C_PLS_REG_DATA			0x05
#define AP3212C_PLS_REG_DLS_WIN		0x08
*/
//REG new
#define AP3212C_RAN_COMMAND		0x10
#define AP3212C_RAN_MASK		0x30
#define AP3212C_RAN_SHIFT		(4)

//ALS interrupt filter
#define AP3212C_AIF_COMMAND		0x10
#define AP3212C_AIF_MASK		0x0F
#define AP3212C_AIF_SHIFT		(0)

//PS interrupt filter
#define AP3212C_PIF_COMMAND		0x20
#define AP3212C_PIF_MASK		0x03
#define AP3212C_PIF_SHIFT		(0)

#define AP3212C_MODE_COMMAND	0x00
#define AP3212C_MODE_SHIFT		(0)
#define AP3212C_MODE_MASK		0x07

#define	AP3212C_ADC_LSB			0x0c
#define	AP3212C_ADC_MSB			0x0d

#define	AP3212C_PX_LSB			0x0e
#define	AP3212C_PX_MSB			0x0f
#define	AP3212C_PX_LSB_MASK		0x0f
#define	AP3212C_PX_MSB_MASK		0x3f

#define AP3212C_OBJ_COMMAND		0x0f
#define AP3212C_OBJ_MASK		0x80
#define AP3212C_OBJ_SHIFT		(7)

#define AP3212C_INT_COMMAND		0x01
#define AP3212C_INT_SHIFT		(0)
#define AP3212C_INT_MASK		0x03
#define AP3212C_INT_PMASK		0x02
#define AP3212C_INT_AMASK		0x01

#define AP3212C_ALS_LTHL		0x1a
#define AP3212C_ALS_LTHL_SHIFT	(0)
#define AP3212C_ALS_LTHL_MASK	0xff

#define AP3212C_ALS_LTHH		0x1b
#define AP3212C_ALS_LTHH_SHIFT	(0)
#define AP3212C_ALS_LTHH_MASK	0xff

#define AP3212C_ALS_HTHL		0x1c
#define AP3212C_ALS_HTHL_SHIFT	(0)
#define AP3212C_ALS_HTHL_MASK	0xff

#define AP3212C_ALS_HTHH		0x1d
#define AP3212C_ALS_HTHH_SHIFT	(0)
#define AP3212C_ALS_HTHH_MASK	0xff

#define AP3212C_ALS_CONFIGURE	0x10
#define AP3212C_ALS_CALI		0x19

#define AP3212C_PX_LTHL			0x2a
#define AP3212C_PX_LTHL_SHIFT	(0)
#define AP3212C_PX_LTHL_MASK	0x03

#define AP3212C_PX_LTHH			0x2b
#define AP3212C_PX_LTHH_SHIFT	(0)
#define AP3212C_PX_LTHH_MASK	0xff

#define AP3212C_PX_HTHL			0x2c
#define AP3212C_PX_HTHL_SHIFT	(0)
#define AP3212C_PX_HTHL_MASK	0x03

#define AP3212C_PX_HTHH			0x2d
#define AP3212C_PX_HTHH_SHIFT	(0)
#define AP3212C_PX_HTHH_MASK	0xff

#define AP3212C_PX_CONFIGURE	0x20
#define AP3212C_PX_LED			0x21

//CMD old
/*
#define AP3212C_PLS_BOTH_ACTIVE		0x02
#define AP3212C_PLS_ALPS_ACTIVE		0x00
#define AP3212C_PLS_PXY_ACTIVE		0x01
#define AP3212C_PLS_BOTH_DEACTIVE	0x0B
#define	AP3212C_PLS_INT_MASK			0x03
#define AP3212C_PLS_DPS_INT			0x02
#define AP3212C_PLS_DLS_INT			0x01
*/
#define AP3212C_PLS_BOTH_ACTIVE		0x03
#define AP3212C_PLS_ALPS_ACTIVE		0x01
#define AP3212C_PLS_PXY_ACTIVE		0x02
#define AP3212C_PLS_BOTH_DEACTIVE	0x00
//#define	AP3212C_PLS_INT_MASK			0x03
//#define AP3212C_PLS_DPS_INT			0x02
//#define AP3212C_PLS_DLS_INT			0x01

#define AP3212B_NUM_CACHABLE_REGS	23
#define AP3216C_NUM_CACHABLE_REGS	26

typedef enum _SENSOR_TYPE{
	AP3212C_PLS_ALPS=0,
	AP3212C_PLS_PXY,
	AP3212C_PLS_BOTH,
}SENSOR_TYPE;

struct ap3212c_pls_platform_data {
	int irq_gpio_number;
};

typedef  struct _ap3212c_pls_t{
	int    irq;
	struct input_dev *input;
	struct i2c_client *client;
	struct work_struct	work;
	struct workqueue_struct *ltr_work_queue;
	struct early_suspend ltr_early_suspend;
}ap3212c_pls_t;

#if 0
#define ADD_TO_IDX(addr,idx)	{														\
	int i;												\
	for(i = 0; i < reg_num; i++)						\
	{													\
		if (addr == reg_array[i])						\
		{												\
			idx = i;									\
			break;										\
		}												\
	}													\

#endif

#endif

