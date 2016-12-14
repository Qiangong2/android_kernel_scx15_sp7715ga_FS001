#ifndef __TP_PSENSOR_H__
#define __TP_PSENSOR_H__


#ifdef CONFIG_ZYT_SUPPORT
#define CONFIG_ZYT_CTP_PS_CDC
#endif

#ifdef CONFIG_ZYT_CTP_PS_CDC
#define TP_PS_DEVICE					"gtp_proximity"
//#define TP_PS_INPUT_DEV					"SPRD_WW_CTP"
/*
#define TP_PS_IOCTL_MAGIC				0X5D
#define TP_PS_IOCTL_PROX_ON				_IO(TP_PS_IOCTL_MAGIC, 7)
#define TP_PS_IOCTL_PROX_OFF			_IO(TP_PS_IOCTL_MAGIC, 8)
*/
#define GTP_IOCTL_MAGIC        	0X5D
#define GTP_IOCTL_PROX_ON		_IO(GTP_IOCTL_MAGIC, 7)
#define GTP_IOCTL_PROX_OFF		_IO(GTP_IOCTL_MAGIC, 8)

#else
#define TP_PS_DEVICE					"SPRD_WW_proximity"
#define TP_PS_INPUT_DEV					"proximity"

#define LTR_IOCTL_MAGIC 				0x1C
#define LTR_IOCTL_GET_PFLAG  			_IOR(LTR_IOCTL_MAGIC, 1, int)
#define LTR_IOCTL_GET_LFLAG  			_IOR(LTR_IOCTL_MAGIC, 2, int)
#define LTR_IOCTL_SET_PFLAG  			_IOW(LTR_IOCTL_MAGIC, 3, int)
#define LTR_IOCTL_SET_LFLAG  			_IOW(LTR_IOCTL_MAGIC, 4, int)
#define LTR_IOCTL_GET_DATA  			_IOW(LTR_IOCTL_MAGIC, 5, unsigned char)
#endif

typedef  struct _tp_ps_t{
	struct input_dev *input;
}tp_ps_t;


#endif //__TP_PSENSOR_H__
