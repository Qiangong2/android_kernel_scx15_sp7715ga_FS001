#ifndef __IT7260_TS_H__
#define __IT7260_TS_H__

#define IT7260_TS_DEVICE		"IT7260"
#define IT7260_TS_NAME			"IT7260"
#define IT7260_TS_ADDR			0x46

// Tiger平台默认将device信息放在board.c中
#define I2C_BOARD_INFO_METHOD

#define MAX_BUFFER_SIZE			144

struct ioctl_cmd168 {
	unsigned short bufferIndex;
	unsigned short length;
	unsigned short buffer[MAX_BUFFER_SIZE];
};

#define IOC_MAGIC		'd'
#define IOCTL_SET		_IOW(IOC_MAGIC, 1, struct ioctl_cmd168)
#define IOCTL_GET		_IOR(IOC_MAGIC, 2, struct ioctl_cmd168)

//register
#define TPD_REG_BASE            0x00

struct tpd_operation_data_t
{    
	u8 hst_mode;    
	u8 tt_mode;    
	u8 tt_stat;    
	u8 x1_M,x1_L;    
	u8 y1_M,y1_L;    
	u8 z1;    
	u8 touch12_id;    
	u8 x2_M,x2_L;    
	u8 y2_M,y2_L;    
	u8 z2;    
	/*
	u8 gest_cnt;    
	u8 gest_id;    
	u8 gest_set;     
	u8 x3_M,x3_L;    
	u8 y3_M,y3_L;    
	u8 z3;    
	u8 touch34_id;    
	u8 x4_M,x4_L;    
	u8 y4_M,y4_L;    
	u8 z4;
	*/
};


struct touch_info 
{    
	int x1, y1;    
	int x2, y2;    
	int p1, p2;  
	//int id1,id2;  
	int count;
};

struct it7260_ts_platform_data{
	int irq_gpio_number;
	int reset_gpio_number;
	const char *vdd_name;
};

//static struct tpd_operation_data_t g_operation_data;
#endif

