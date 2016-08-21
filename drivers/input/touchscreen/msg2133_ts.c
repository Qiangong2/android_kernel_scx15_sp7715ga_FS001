/* 
 * drivers/input/touchscreen/msg2133_ts.c
 *
 * FocalTech msg2133 TouchScreen driver. 
 *
 * Copyright (c) 2010  Focal tech Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * VERSION      	DATE			AUTHOR
 *    1.0		  2010-01-05			WenFS
 *
 * note: only support mulititouch	Wenfs 2010-10-01
 */
#include <linux/firmware.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/earlysuspend.h>
#include <linux/regulator/consumer.h>
#include <linux/i2c/msg2133_ts.h>
#include <mach/regulator.h>
#include <mach/hardware.h>
#include <mach/globalregs.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/clk.h>
#ifdef CONFIG_I2C_SPRD
#include <mach/i2c-sprd.h>
#endif

#include <linux/cdc_com.h>



#ifdef CONFIG_UDC_TP
static  char file_buf[200];
#endif
#define I2C_BOARD_INFO_METHOD 1

#if TS_DEBUG_MSG
#define MSG2133_DBG(format, ...)	printk(KERN_INFO "MSG2133 " format "\n", ## __VA_ARGS__)
#else
#define MSG2133_DBG(format, ...)
#endif
#define MSG2133_INFO(format, ...)	printk(KERN_INFO "MSG2133 " format "\n", ## __VA_ARGS__)

//------------------------------proximity----------------------------------
/*
1、Base Band 发Command通知TP进入/退出通话模式
	0x26 0x52 0x00 0x62 0xa0  进入
	0x26 0x52 0x00 0x62 0xa1  退出

2、CTP发INT通知Base Band进入退出Palm Rejection.
	读取坐标第5个字节0x80为进入，0x40为退出

3、通话时，手机关屏不关TP；手机打开屏，Reset TP。

*/

#if (7675 == CONFIG_ZYT_PRJ) && (66 == CONFIG_ZYT_CUST)  //7675-鑫宇通
#define TP_PROXIMITY_SENSOR
#elif (7675 == CONFIG_ZYT_PRJ) && (70 == CONFIG_ZYT_CUST)
#define TP_PROXIMITY_SENSOR
#elif (3 == CONFIG_ZYT_CUST)
#define TP_PROXIMITY_SENSOR
#elif (8 == CONFIG_ZYT_CUST)
#define TP_PROXIMITY_SENSOR
#elif (73 == CONFIG_ZYT_CUST)
#define TP_PROXIMITY_SENSOR
#endif

#ifdef TP_PROXIMITY_SENSOR
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#ifdef CONFIG_UDC_GPIO
#include <linux/udc.h>
#endif

#ifdef CONFIG_ZYT_SUPPORT
#define PROXIMITY_DEVICE		"gtp_proximity"

//ioctl cmd
#define MSG_IOCTL_MAGIC			0X5D
#define MSG_IOCTL_PROX_ON		_IO(MSG_IOCTL_MAGIC, 7)
#define MSG_IOCTL_PROX_OFF		_IO(MSG_IOCTL_MAGIC, 8)
#else
#define PROXIMITY_DEVICE   "MSG2133A_proximity"

//ioctl cmd
#define MSG_IOCTL_MAGIC        MSG2133_TS_ADDR
#define MSG_IOCTL_PROX_ON		_IO(MSG_IOCTL_MAGIC, 1)
#define MSG_IOCTL_PROX_OFF		_IO(MSG_IOCTL_MAGIC, 2)
#endif

#define TPD_PROXIMITY_DEVICE            "MSG2133"
#define TPD_PROXIMITY_DMESG(a,arg...) printk(TPD_PROXIMITY_DEVICE ": " a,##arg)

#define TPD_PROXIMITY_DEBUG
#if defined(TPD_PROXIMITY_DEBUG)
#undef TPD_PROXIMITY_DEBUG
#define TPD_PROXIMITY_DEBUG(a,arg...) printk(TPD_PROXIMITY_DEVICE ": " a,##arg)
#else
#define TPD_PROXIMITY_DEBUG(arg...)
#endif

static u8 tpd_proximity_flag = 0;
static u8	PROXIMITY_STATE = 0;

static u8 tpd_proximity_dir_faraway = 1;//0-->close ; 1--> far away
static u8 tpd_proximity_need_report = 0;
#endif
//------------------------------proximity----------------------------------

//#define NEW_TP
#if (((7675 == CONFIG_ZYT_PRJ) && (66 == CONFIG_ZYT_CUST)) && defined(CONFIG_LCD_WVGA))
#define CTP_CONVERT_XY	0
#define CTP_NEGATIVE_X	1
#elif (CONFIG_ZYT_PRJ == 7675)  && (CONFIG_ZYT_CUST == 68)
#define CTP_CONVERT_XY	0
#define CTP_NEGATIVE_X	1
#elif ((CONFIG_ZYT_PRJ == 7675)  && (CONFIG_ZYT_CUST == 69)) && defined(CONFIG_LCD_WVGA)
#define CTP_CONVERT_XY	0
#define CTP_NEGATIVE_X	1
#elif  defined(CONFIG_LCD_WVGA)
#define CTP_CONVERT_XY	0
#define CTP_NEGATIVE_X	1
#else
#define CTP_CONVERT_XY	0
#define CTP_NEGATIVE_X	0
#endif

static struct i2c_client *this_client;
static struct i2c_board_info msg2133_i2c_boardinfo = {
	I2C_BOARD_INFO(MSG2133_TS_NAME, MSG2133_TS_ADDR),
};
struct msg2133_ts_data *g_msg2133_ts;

static void msg2133_device_power_on(void);


#ifdef MSG2133_UPDATE
struct class	*firmware_class;
struct device	*firmware_cmd_dev;
static int		update_switch = 0;
static int		FwDataCnt;
static  char	*fw_version;
static unsigned char	temp[94][1024];

struct msg2133_ts_data {
	struct input_dev		*input_dev;
	struct ts_event			event;
	struct work_struct		pen_event_work;
	struct workqueue_struct	*ts_workqueue;
	struct early_suspend	early_suspend;
	struct msg2133_ts_platform_data	*platform_data;
};
static void msg2133_reset(void)
{
		struct msg2133_ts_platform_data *pdata = g_msg2133_ts->platform_data;

	gpio_set_value(pdata->reset_gpio_number, 0);
	msg2133_device_power_on();
	msleep(20);
	gpio_set_value(pdata->reset_gpio_number,1);
	msleep(500);
}


static bool msg2133_i2c_read(char *pbt_buf, int dw_lenth)
{
	int ret;
	MSG2133_DBG("The msg_i2c_client->addr=0x%x\n",this_client->addr);
	ret = i2c_master_recv(this_client, pbt_buf, dw_lenth);

	if(ret <= 0){
		MSG2133_DBG("msg_i2c_read_interface error\n");
		return false;
	}

	return true;
}

static bool msg2133_i2c_write(char *pbt_buf, int dw_lenth)
{
	int ret;
	MSG2133_DBG("The msg_i2c_client->addr=0x%x\n",this_client->addr);
	ret = i2c_master_send(this_client, pbt_buf, dw_lenth);

	if(ret <= 0){
		MSG2133_DBG("msg_i2c_read_interface error\n");
		return false;
	}

	return true;
}


static void i2c_read_msg2133(unsigned char *pbt_buf, int dw_lenth)
{
	this_client->addr = MSG2133_FW_ADDR;
	i2c_master_recv(this_client, pbt_buf, dw_lenth);	//0xC5_8bit
	this_client->addr = MSG2133_TS_ADDR;
}

static void i2c_write_msg2133(unsigned char *pbt_buf, int dw_lenth)
{
	this_client->addr = MSG2133_FW_ADDR;
	i2c_master_send(this_client, pbt_buf, dw_lenth);		//0xC4_8bit
	this_client->addr = MSG2133_TS_ADDR;
}

static void i2c_read_update_msg2133(unsigned char *pbt_buf, int dw_lenth)
{	
	this_client->addr = MSG2133_FW_UPDATE_ADDR;
	i2c_master_recv(this_client, pbt_buf, dw_lenth);	//0x93_8bit
	this_client->addr = MSG2133_TS_ADDR;
}

static void i2c_write_update_msg2133(unsigned char *pbt_buf, int dw_lenth)
{	
	this_client->addr = MSG2133_FW_UPDATE_ADDR;
	i2c_master_send(this_client, pbt_buf, dw_lenth);	//0x92_8bit
	this_client->addr = MSG2133_TS_ADDR;
}



void dbbusDWIICEnterSerialDebugMode(void)
{
	unsigned char data[5];
	// Enter the Serial Debug Mode
	data[0] = 0x53;
	data[1] = 0x45;
	data[2] = 0x52;
	data[3] = 0x44;
	data[4] = 0x42;
	i2c_write_msg2133(data, 5);
}

void dbbusDWIICStopMCU(void)
{
	unsigned char data[1];
	// Stop the MCU
	data[0] = 0x37;
	i2c_write_msg2133(data, 1);
}

void dbbusDWIICIICUseBus(void)
{
	unsigned char data[1];
	// IIC Use Bus
	data[0] = 0x35;
	i2c_write_msg2133(data, 1);
}

void dbbusDWIICIICReshape(void)
{
	unsigned char data[1];
	// IIC Re-shape
	data[0] = 0x71;
	i2c_write_msg2133(data, 1);
}

void dbbusDWIICIICNotUseBus(void)
{
	unsigned char data[1];
	// IIC Not Use Bus
	data[0] = 0x34;
	i2c_write_msg2133(data, 1);
}

void dbbusDWIICNotStopMCU(void)
{
	unsigned char data[1];
	// Not Stop the MCU
	data[0] = 0x36;
	i2c_write_msg2133(data, 1);
}

void dbbusDWIICExitSerialDebugMode(void)
{
	unsigned char data[1];
	// Exit the Serial Debug Mode
	data[0] = 0x45;
	i2c_write_msg2133(data, 1);
	// Delay some interval to guard the next transaction
}

void drvISP_EntryIspMode(void)
{
	unsigned char bWriteData[5] =
	{
		0x4D, 0x53, 0x54, 0x41, 0x52
	};
	i2c_write_update_msg2133(bWriteData, 5);
	msleep(10);           // delay about 10ms
}

void drvISP_WriteEnable(void)
{
	unsigned char bWriteData[2] =
	{
		0x10, 0x06
	};
	unsigned char bWriteData1 = 0x12;
	i2c_write_update_msg2133(bWriteData, 2);
	i2c_write_update_msg2133(&bWriteData1, 1);
}

unsigned char drvISP_Read(unsigned char n, unsigned char *pDataToRead)    //First it needs send 0x11 to notify we want to get flash data back.
{
	unsigned char Read_cmd = 0x11;
	unsigned char i = 0;
	unsigned char dbbus_rx_data[16] = {0};
	i2c_write_update_msg2133(&Read_cmd, 1);
	//if (n == 1)
	{
		i2c_read_update_msg2133(&dbbus_rx_data[0], n + 1);

		for(i = 0; i < n; i++)
		{
			*(pDataToRead + i) = dbbus_rx_data[i + 1];
		}
	}
	//else
	{
		//i2c_read_update_msg2133(pDataToRead, n);
	}
	return 0;
}

unsigned char drvISP_ReadStatus(void)
{
	unsigned char bReadData = 0;
	unsigned char bWriteData[2] =
	{
		0x10, 0x05
	};
	unsigned char bWriteData1 = 0x12;
	//    msleep(1);           // delay about 100us
	i2c_write_update_msg2133(bWriteData, 2);
	//    msleep(1);           // delay about 100us
	drvISP_Read(1, &bReadData);
	//    msleep(10);           // delay about 10ms
	i2c_write_update_msg2133(&bWriteData1, 1);
	return bReadData;
}



void drvISP_BlockErase(unsigned int addr)
{
	unsigned char bWriteData[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char bWriteData1 = 0x12;
	unsigned int timeOutCount=0;

	drvISP_WriteEnable();
	//Enable write status register
	bWriteData[0] = 0x10;
	bWriteData[1] = 0x50;
	i2c_write_update_msg2133(bWriteData, 2);
	i2c_write_update_msg2133(&bWriteData1, 1);
	//Write Status
	bWriteData[0] = 0x10;
	bWriteData[1] = 0x01;
	bWriteData[2] = 0x00;
	i2c_write_update_msg2133(bWriteData, 3);
	i2c_write_update_msg2133(&bWriteData1, 1);
	//Write disable
	bWriteData[0] = 0x10;
	bWriteData[1] = 0x04;
	i2c_write_update_msg2133(bWriteData, 2);
	i2c_write_update_msg2133(&bWriteData1, 1);

	timeOutCount=0;
	msleep(1);           // delay about 100us
	while((drvISP_ReadStatus() & 0x01) == 0x01)
	{
		timeOutCount++;
		if ( timeOutCount > 10000 ) 
			break; /* around 1 sec timeout */
	}

	//pr_ch("The drvISP_ReadStatus3=%d\n", drvISP_ReadStatus());
	drvISP_WriteEnable();
	//pr_ch("The drvISP_ReadStatus4=%d\n", drvISP_ReadStatus());
	bWriteData[0] = 0x10;
	bWriteData[1] = 0xC7;        //Block Erase
	//bWriteData[2] = ((addr >> 16) & 0xFF) ;
	//bWriteData[3] = ((addr >> 8) & 0xFF) ;
	// bWriteData[4] = (addr & 0xFF) ;
	i2c_write_update_msg2133(bWriteData, 2);
	//i2c_write_update_msg2133( &bWriteData, 5);
	i2c_write_update_msg2133(&bWriteData1, 1);

	timeOutCount=0;
	msleep(1);           // delay about 100us
	while((drvISP_ReadStatus() & 0x01) == 0x01)
	{
		timeOutCount++;
		if ( timeOutCount > 10000 ) 
			break; /* around 1 sec timeout */
	}
}

void drvISP_Program(unsigned short k, unsigned char *pDataToWrite)
{
	unsigned short i = 0;
	unsigned short j = 0;
	//U16 n = 0;
	unsigned char TX_data[133];
	unsigned char bWriteData1 = 0x12;
	unsigned int addr = k * 1024;

#if 1
	for(j = 0; j < 8; j++)    //128*8 cycle
	{
		TX_data[0] = 0x10;
		TX_data[1] = 0x02;// Page Program CMD
		TX_data[2] = (addr + 128 * j) >> 16;
		TX_data[3] = (addr + 128 * j) >> 8;
		TX_data[4] = (addr + 128 * j);

		for(i = 0; i < 128; i++)
		{
			TX_data[5 + i] = pDataToWrite[j * 128 + i];
		}

		while((drvISP_ReadStatus() & 0x01) == 0x01)
		{
			;    //wait until not in write operation
		}

		drvISP_WriteEnable();
		i2c_write_update_msg2133( TX_data, 133);   //write 133 byte per cycle
		i2c_write_update_msg2133(&bWriteData1, 1);
	}

#else
	for(j = 0; j < 512; j++)    //128*8 cycle
	{
		TX_data[0] = 0x10;
		TX_data[1] = 0x02;// Page Program CMD
		TX_data[2] = (addr + 2 * j) >> 16;
		TX_data[3] = (addr + 2 * j) >> 8;
		TX_data[4] = (addr + 2 * j);

		for(i = 0; i < 2; i++)
		{
			TX_data[5 + i] = pDataToWrite[j * 2 + i];
		}

		while((drvISP_ReadStatus() & 0x01) == 0x01)
		{
			;    //wait until not in write operation
		}

		drvISP_WriteEnable();
		i2c_write_update_msg2133(TX_data, 7);    //write 7 byte per cycle
		i2c_write_update_msg2133(&bWriteData1, 1);
	}

#endif
}

void drvISP_ExitIspMode(void)
{
	unsigned char bWriteData = 0x24;
	i2c_write_update_msg2133(&bWriteData, 1);
}


static ssize_t firmware_version_show(struct device *dev,
                                     struct device_attribute *attr, char *buf)
{
	//printk("tyd-tp: firmware_version_show\n");
	MSG2133_DBG("*** firmware_version_show fw_version = %s***\n", fw_version);
	return sprintf(buf, "%s\n", fw_version);
}

static ssize_t firmware_version_store(struct device *dev,
                                      struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned char dbbus_tx_data[3];
	unsigned char dbbus_rx_data[4] ;
	unsigned short major = 0, minor = 0;
	dbbusDWIICEnterSerialDebugMode();
	dbbusDWIICStopMCU();
	dbbusDWIICIICUseBus();
	dbbusDWIICIICReshape();
	fw_version = kzalloc(sizeof(char), GFP_KERNEL);
	MSG2133_DBG("\n");
	printk("tyd-tp: firmware_version_store\n");
	//Get_Chip_Version();
	dbbus_tx_data[0] = 0x53;
	dbbus_tx_data[1] = 0x00;
	dbbus_tx_data[2] = 0x74;
	//i2c_write(TOUCH_ADDR_MSG20XX, &dbbus_tx_data[0], 3);
	//i2c_read(TOUCH_ADDR_MSG20XX, &dbbus_rx_data[0], 4);
	msg2133_i2c_write(&dbbus_tx_data[0], 3);
	msg2133_i2c_read(&dbbus_rx_data[0], 4);
	major = (dbbus_rx_data[1] << 8) + dbbus_rx_data[0];
	minor = (dbbus_rx_data[3] << 8) + dbbus_rx_data[2];
	MSG2133_DBG("***major = %d ***\n", major);
	MSG2133_DBG("***minor = %d ***\n", minor);
	sprintf(fw_version, "%03d%03d", major, minor);
	MSG2133_DBG("***fw_version = %s ***\n", fw_version);
	return size;
}

static ssize_t firmware_update_show(struct device *dev,
                                    struct device_attribute *attr, char *buf)
{
	printk("tyd-tp: firmware_update_show\n");
	return sprintf(buf, "%s\n", fw_version);
}

static ssize_t firmware_update_store(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned char i;
	unsigned char dbbus_tx_data[4];
	unsigned char dbbus_rx_data[2] = {0};
	update_switch = 1;
	//drvISP_EntryIspMode();
	//drvISP_BlockErase(0x00000);
	//M by cheehwa _HalTscrHWReset();

	//关闭中断
	disable_irq_nosync(this_client->irq);

	msg2133_reset();
	//msctpc_LoopDelay ( 100 );        // delay about 100ms*****
	// Enable slave's ISP ECO mode
	dbbusDWIICEnterSerialDebugMode();
	dbbusDWIICStopMCU();
	dbbusDWIICIICUseBus();
	dbbusDWIICIICReshape();
	//pr_ch("dbbusDWIICIICReshape\n");
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x08;
	dbbus_tx_data[2] = 0x0c;
	dbbus_tx_data[3] = 0x08;
	// Disable the Watchdog
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Get_Chip_Version();
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x11;
	dbbus_tx_data[2] = 0xE2;
	dbbus_tx_data[3] = 0x00;
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x60;
	dbbus_tx_data[3] = 0x55;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//pr_ch("update\n");
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x61;
	dbbus_tx_data[3] = 0xAA;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Stop MCU
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x0F;
	dbbus_tx_data[2] = 0xE6;
	dbbus_tx_data[3] = 0x01;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Enable SPI Pad
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x02;
	i2c_write_msg2133(dbbus_tx_data, 3);
	i2c_read_msg2133(&dbbus_rx_data[0], 2);
	//pr_tp("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
	dbbus_tx_data[3] = (dbbus_rx_data[0] | 0x20);  //Set Bit 5
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x25;
	i2c_write_msg2133(dbbus_tx_data, 3);
	dbbus_rx_data[0] = 0;
	dbbus_rx_data[1] = 0;
	i2c_read_msg2133(&dbbus_rx_data[0], 2);
	//pr_tp("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
	dbbus_tx_data[3] = dbbus_rx_data[0] & 0xFC;  //Clear Bit 1,0
	i2c_write_msg2133(dbbus_tx_data, 4);
	/*
	//------------
	// ISP Speed Change to 400K
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x11;
	dbbus_tx_data[2] = 0xE2;
	i2c_write_msg2133( dbbus_tx_data, 3);
	i2c_read_msg2133( &dbbus_rx_data[3], 1);
	//pr_tp("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
	dbbus_tx_data[3] = dbbus_tx_data[3]&0xf7;  //Clear Bit3
	i2c_write_msg2133( dbbus_tx_data, 4);
	*/
	//WP overwrite
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x0E;
	dbbus_tx_data[3] = 0x02;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//set pin high
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x10;
	dbbus_tx_data[3] = 0x08;
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbusDWIICIICNotUseBus();
	dbbusDWIICNotStopMCU();
	dbbusDWIICExitSerialDebugMode();
	///////////////////////////////////////
	// Start to load firmware
	///////////////////////////////////////
	drvISP_EntryIspMode();
	MSG2133_DBG("entryisp\n");
	drvISP_BlockErase(0x00000);
	//msleep(1000);
	MSG2133_DBG("FwVersion=2");

	for(i = 0; i < 94; i++)    // total  94 KB : 1 byte per R/W
	{
		//msleep(1);//delay_100us
		MSG2133_DBG("drvISP_Program\n");
		drvISP_Program(i, temp[i]);    // program to slave's flash
		//pr_ch("drvISP_Verify\n");
		//drvISP_Verify ( i, temp[i] ); //verify data
	}

	//MSG2133_DBG("update OK\n");
	drvISP_ExitIspMode();
	FwDataCnt = 0;
	msg2133_reset();
	MSG2133_DBG("update OK\n");
	update_switch = 0;
	//打开中断
	enable_irq(this_client->irq);
	return size;
}

static ssize_t firmware_data_show(struct device *dev,
                                  struct device_attribute *attr, char *buf)
{
	printk("tyd-tp: firmware_data_show\n");
	return FwDataCnt;
}

static ssize_t firmware_data_store(struct device *dev,
                                   struct device_attribute *attr, const char *buf, size_t size)
{
	int i;
	MSG2133_DBG("***FwDataCnt = %d ***\n", FwDataCnt);
	printk("tyd-tp: firmware_data_store\n");
	for(i = 0; i < 1024; i++)
	{
		memcpy(temp[FwDataCnt], buf, 1024);
	}

	FwDataCnt++;
	return size;
}

static ssize_t firmware_clear_show(struct device *dev,
                                   struct device_attribute *attr, char *buf)
{
	unsigned short k = 0, i = 0, j = 0;
	unsigned char bWriteData[5] =
		{
			0x10, 0x03, 0, 0, 0
		};
	unsigned char RX_data[256];
	unsigned char bWriteData1 = 0x12;
	unsigned int addr = 0;
	MSG2133_DBG("\n");
	printk("tyd-tp: firmware_clear_show\n");
	for(k = 0; k < 94; i++)    // total  94 KB : 1 byte per R/W
	{
		addr = k * 1024;

		for(j = 0; j < 8; j++)    //128*8 cycle
		{
			bWriteData[2] = (unsigned char)((addr + j * 128) >> 16);
			bWriteData[3] = (unsigned char)((addr + j * 128) >> 8);
			bWriteData[4] = (unsigned char)(addr + j * 128);

			while((drvISP_ReadStatus() & 0x01) == 0x01)
			{
				;    //wait until not in write operation
			}

			i2c_write_update_msg2133(bWriteData, 5);     //write read flash addr
			drvISP_Read(128, RX_data);
			i2c_write_update_msg2133(&bWriteData1, 1);    //cmd end

			for(i = 0; i < 128; i++)    //log out if verify error{
				if(RX_data[i] != 0xFF){
					MSG2133_DBG("k=%d,j=%d,i=%d===============erase not clean================", k, j, i);
				}
		}
	}
	MSG2133_DBG("read finish\n");
	return sprintf(buf, "%s\n", fw_version);
}

static ssize_t firmware_clear_store(struct device *dev,
                                    struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned char dbbus_tx_data[4];
	unsigned char dbbus_rx_data[2] = {0};
	//msctpc_LoopDelay ( 100 );        // delay about 100ms*****
	// Enable slave's ISP ECO mode
	/*
	dbbusDWIICEnterSerialDebugMode();
	dbbusDWIICStopMCU();
	dbbusDWIICIICUseBus();
	dbbusDWIICIICReshape();*/
	MSG2133_DBG("\n");
	printk("tyd-tp: firmware_clear_store\n");
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x08;
	dbbus_tx_data[2] = 0x0c;
	dbbus_tx_data[3] = 0x08;
	// Disable the Watchdog
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Get_Chip_Version();
	//FwVersion  = 2;
	//if (FwVersion  == 2)
	{
		dbbus_tx_data[0] = 0x10;
		dbbus_tx_data[1] = 0x11;
		dbbus_tx_data[2] = 0xE2;
		dbbus_tx_data[3] = 0x00;
		i2c_write_msg2133(dbbus_tx_data, 4);
	}
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x60;
	dbbus_tx_data[3] = 0x55;
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x61;
	dbbus_tx_data[3] = 0xAA;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Stop MCU
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x0F;
	dbbus_tx_data[2] = 0xE6;
	dbbus_tx_data[3] = 0x01;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Enable SPI Pad
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x02;
	i2c_write_msg2133(dbbus_tx_data, 3);
	i2c_read_msg2133(&dbbus_rx_data[0], 2);
	MSG2133_DBG("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
	dbbus_tx_data[3] = (dbbus_rx_data[0] | 0x20);  //Set Bit 5
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x25;
	i2c_write_msg2133(dbbus_tx_data, 3);
	dbbus_rx_data[0] = 0;
	dbbus_rx_data[1] = 0;
	i2c_read_msg2133(&dbbus_rx_data[0], 2);
	MSG2133_DBG("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
	dbbus_tx_data[3] = dbbus_rx_data[0] & 0xFC;  //Clear Bit 1,0
	i2c_write_msg2133(dbbus_tx_data, 4);
	//WP overwrite
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x0E;
	dbbus_tx_data[3] = 0x02;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//set pin high
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x10;
	dbbus_tx_data[3] = 0x08;
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbusDWIICIICNotUseBus();
	dbbusDWIICNotStopMCU();
	dbbusDWIICExitSerialDebugMode();
	///////////////////////////////////////
	// Start to load firmware
	///////////////////////////////////////
	drvISP_EntryIspMode();
	MSG2133_DBG("chip erase+\n");
	drvISP_BlockErase(0x00000);
	MSG2133_DBG("chip erase-\n");
	drvISP_ExitIspMode();
	return size;
}

static DEVICE_ATTR(version, 0777, firmware_version_show, firmware_version_store);
static DEVICE_ATTR(update, 0777, firmware_update_show, firmware_update_store);
static DEVICE_ATTR(data, 0777, firmware_data_show, firmware_data_store);
static DEVICE_ATTR(clear, 0777, firmware_clear_show, firmware_clear_store);

void msg2133_init_class(void)
{
	firmware_class = class_create(THIS_MODULE,"mtk-tpd" );//client->name

	if(IS_ERR(firmware_class)) {
		pr_err("Failed to create class(firmware)!\n");
		return;
	}

	firmware_cmd_dev = device_create(firmware_class,
			NULL, 0, NULL, "device");//device

	if(IS_ERR(firmware_cmd_dev))
		pr_err("Failed to create device(firmware_cmd_dev)!\n");

	// version /sys/class/mtk-tpd/device/version
	if(device_create_file(firmware_cmd_dev, &dev_attr_version) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_version.attr.name);

	// update /sys/class/mtk-tpd/device/update
	if(device_create_file(firmware_cmd_dev, &dev_attr_update) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_update.attr.name);

	// data /sys/class/mtk-tpd/device/data
	if(device_create_file(firmware_cmd_dev, &dev_attr_data) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_data.attr.name);

	// clear /sys/class/mtk-tpd/device/clear
	if(device_create_file(firmware_cmd_dev, &dev_attr_clear) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_clear.attr.name);
}

void msg2133_uninit_class(void)
{
	device_remove_file(firmware_cmd_dev, &dev_attr_version);
	device_remove_file(firmware_cmd_dev, &dev_attr_update);
	device_remove_file(firmware_cmd_dev, &dev_attr_data);
	device_remove_file(firmware_cmd_dev, &dev_attr_clear);
	device_destroy(firmware_class, 0);
	class_destroy(firmware_class);
}
#endif //endif MSG2133_UPDATE

#ifdef TP_PROXIMITY_SENSOR
void MSG2133_ps_i2c_write(u8 addr, u8* data, u16 size)
{
	#if 1
	int ret;
	
	TPD_PROXIMITY_DMESG("MSG2133_ps_i2c_write addr=0x%x tpd_proximity_flag=%d\n", this_client->addr, tpd_proximity_flag);

	
	ret = i2c_master_send(this_client, data, size);

	if(ret <= 0)
	{
		TPD_PROXIMITY_DMESG("MSG2133_ps_i2c_write error\n");
		return;
	}

	//test write/read
	{
		unsigned char reg_val[8] = {0};
		ret = i2c_master_recv(this_client, reg_val, 4);
		if(ret <= 0)
		{
			TPD_PROXIMITY_DMESG("MSG2133_ps_i2c_write recv error\n");
		}
		else
		{
			TPD_PROXIMITY_DMESG("MSG2133_ps_i2c_write recv 0x%x-0x%x-0x%x-0x%x\n", reg_val[0],reg_val[1],reg_val[2],reg_val[3]);
		}
	}
	#else
	u8 addr_bak;
	int ret;
	
	TPD_PROXIMITY_DMESG("MSG2133_ps_i2c_write addr=0x%x tpd_proximity_flag=%d\n", this_client->addr, tpd_proximity_flag);

	addr_bak = this_client->addr;
	this_client->addr = addr;
	this_client->addr |= I2C_ENEXT_FLAG;
	ret = i2c_master_send(this_client,data,size);
	this_client->addr = addr_bak;
	this_client->addr |= I2C_ENEXT_FLAG;

	if(ret <= 0)
	{
		TPD_PROXIMITY_DMESG("MSG2133_ps_i2c_write ret err %d at line %d\n", ret, __LINE__);
		return;
	}
	
	#endif

	return;
}
static int MSG2133_get_ps_value(void)
{
	TPD_PROXIMITY_DMESG("MSG2133_get_ps_value %d!\n", tpd_proximity_dir_faraway);
	return tpd_proximity_dir_faraway;
}

static int MSG2133_enable_ps(int enable)
{
    char bWriteData[4] =
    {
       // 0x52, 0x01, 0x24, 0xA5
       #if(7630 == CONFIG_ZYT_PRJ)&&(51 == CONFIG_ZYT_CUST)
	   0x52, 0x00, 0x4a, 0xA0 //msg2133a 天兆鑫 
       #else
	   0x52, 0x00, 0x62, 0xA0 
	#endif
    };
//if (PROXIMITY ==1)

    //_TRACE((TSCR_LEVEL_C_TYP,"CtpMsg20xxSetIntpDynamicReportRate: IntpInterval = %d", IntpInterval));
	if(enable==1)
	{
		tpd_proximity_flag =1;
		tpd_proximity_need_report = 1;		
		//bWriteData[3] = 0xA5;
		bWriteData[3] = 0xA0;
		PROXIMITY_STATE = 0;
	}
	else
	{	
		tpd_proximity_flag =0;
		tpd_proximity_need_report = 0;		
		
		//bWriteData[3] = 0x01;
		bWriteData[3] = 0xa1;
		PROXIMITY_STATE = 0;
	}

	//printk("CTP_Face_Mode_Switch  onoff_state %d, PROXIMITY %d\n",onoff_state, PROXIMITY);

   // _TRACE((TSCR_LEVEL_C_TYP,"CtpMsg20xxSetIntpDynamicReportRate: bWriteData[3] = %d", bWriteData[3]));
	msg2133_i2c_write(&bWriteData[0], 4);	
    //_msg20xx_packet_write(_ctp_dev_i2c_handle, bWriteData, sizeof(bWriteData));

   // setintpmsg20xx++;
   // setintpmsg20xxIntpInterval = IntpInterval;
   //_TRACE((TSCR_LEVEL_C_TYP,"setintpmsg20xx= %d, setintpmsg20xxIntpInterval = %d", setintpmsg20xx, setintpmsg20xxIntpInterval));
	  
    //return 1;
}

static int MSG2133_ioctl_operate(struct i2c_client *client, unsigned int cmd, void *arg)
{
	int err = 0;
	int value;

	TPD_PROXIMITY_DMESG("MSG2133_ioctl_operate %d!\n", cmd);

	switch(cmd)
	{
		case MSG_IOCTL_PROX_ON:
			MSG2133_enable_ps(1);   
			break;

		case MSG_IOCTL_PROX_OFF:
			TPD_PROXIMITY_DMESG("MSG2133_ioctl_operate not set debug!\n");
			MSG2133_enable_ps(0);
			break;

		default:
			pr_err("%s: invalid cmd %d\n", __func__, _IOC_NR(cmd));
			return -EINVAL;
	}
	return 0;

}
static struct file_operations msg2133_proximity_fops = {
	.owner = THIS_MODULE,
	.open = NULL,
	.release = NULL,
	.unlocked_ioctl = MSG2133_ioctl_operate
};

struct miscdevice msg2133_proximity_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = PROXIMITY_DEVICE,						//match the hal's name 
	.fops = &msg2133_proximity_fops,
	.mode = S_IRWXUGO
};

static ssize_t msg2133_enable_show(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t msg2133_enable_store(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);

static DEVICE_ATTR(enable, S_IRWXU | S_IRWXG | S_IRWXO,msg2133_enable_show, msg2133_enable_store);


static struct attribute *msg2133_attributes[] = {
	&dev_attr_enable.attr,
	NULL
};

static const struct attribute_group msg2133_attr_group = {
	.attrs = msg2133_attributes,
};


static ssize_t msg2133_enable_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	int						enable;
	//struct ft5x0x_ts_data	*acc = g_ft5x0x_ts;

	enable = tpd_proximity_flag;
		printk("%s\n", __func__);
	return sprintf(buf, "%d\n", enable);
}
//static int FT6206_enable_ps(int enable);

static ssize_t msg2133_enable_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	//struct ft5x0x_ts_data	*acc = g_ft5x0x_ts;
	int						ret;
	unsigned long			enable;
		printk("%s\n", __func__);
	enable = simple_strtoul(buf, NULL, 10);    

	enable = (enable > 0) ? 1 : 0;
	if(enable)
	{  
			MSG2133_enable_ps(1);   
	}
	else
	{
			MSG2133_enable_ps(0);   
	}

	return count;
}
#endif

static void msg2133_device_power_on(void)
{
#if 0
	//LDO_SetVoltLevel(LDO_LDO_SIM2, LDO_VOLT_LEVEL0);
	//LDO_TurnOnLDO(LDO_LDO_SIM2);

	MSG2133_DBG("msg2133: power on\n");
#endif
}

static void msg2133_device_power_off(void)
{
#if 0
	//LDO_TurnOffLDO(LDO_LDO_SIM2);
	MSG2133_DBG("msg2133: power off\n");
#endif
}


#ifdef VIRTUAL_KEYS
static ssize_t virtual_keys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
#if defined(CONFIG_UDC_TP)
{
	unsigned short udc_tp_addr;
	udc_tp_addr=udc_tp_get_virtual_key(MSG2133_TS_ADDR,file_buf);
	if (udc_tp_addr==MSG2133_TS_ADDR)
	return sprintf (buf, "%s", file_buf); 

}
#endif
//1back 2 home 4menu.

#if (((7675 == CONFIG_ZYT_PRJ) && (66 == CONFIG_ZYT_CUST)) && defined(CONFIG_LCD_WVGA))
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":64:840:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":128:840:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":192:840:64:60"
		"\n");
#elif (CONFIG_ZYT_PRJ == 7675)  && (CONFIG_ZYT_CUST == 68)
#if defined(CONFIG_LCD_WVGA)
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":64:840:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":128:840:64:60"//KEY_BACK
		":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":192:840:64:60"
		"\n");
#else
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":256:529:64:60"//KEY_HOMEPAGE
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":128:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":192:529:64:60"//KEY_BACK
		"\n"); 
#endif
#elif (CONFIG_ZYT_PRJ == 7675)  && (CONFIG_ZYT_CUST == 69)
#if defined(CONFIG_LCD_WVGA)
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":64:840:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":128:840:64:60"//KEY_BACK
		":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":192:840:64:60"
		"\n");
#else
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":128:529:64:60"//KEY_HOMEPAGE
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":192:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":64:529:64:60"//KEY_BACK
		"\n"); 
#endif
#elif (CONFIG_ZYT_PRJ == 7675)  && (CONFIG_ZYT_CUST == 8) && defined(CONFIG_LCD_HVGA)
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":64:529:64:60"//KEY_HOMEPAGE
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":128:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":192:529:64:60"//KEY_BACK
		"\n"); 
#elif (CONFIG_ZYT_PRJ == 7271)  && (CONFIG_ZYT_CUST == 73)
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":64:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":128:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":192:529:64:60"
		"\n");
#elif defined(CONFIG_LCD_WVGA)
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":64:840:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":128:840:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":192:840:64:60"
		"\n");
#else
	return sprintf(buf,
		__stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":64:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":128:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":192:529:64:60"
		":" __stringify(EV_KEY) ":" __stringify(KEY_SEARCH) ":256:529:64:60"
		"\n");
#endif
}

static struct kobj_attribute virtual_keys_attr = {
	.attr = {
		.name = "virtualkeys.SPRD_WW_CTP",
		.mode = S_IRUGO,
	},
	.show = &virtual_keys_show,
};

static struct attribute *properties_attrs[] = {
	&virtual_keys_attr.attr,
	NULL
};

static struct attribute_group properties_attr_group = {
	.attrs = properties_attrs,
};

static struct kobject *properties_kobj;

static void virtual_keys_init(void)
{
	int ret;

	MSG2133_DBG("%s\n",__func__);

	properties_kobj = kobject_create_and_add("board_properties", NULL);
	if (properties_kobj)
		ret = sysfs_create_group(properties_kobj, &properties_attr_group);
	if (!properties_kobj || ret)
		pr_err("failed to create board_properties\n");    
}

static void virtual_keys_destroy(void)
{
	if (properties_kobj)
	{
		sysfs_remove_group(properties_kobj, &properties_attr_group);
		kobject_put(properties_kobj);
		//kobject_del(properties_kobj);
	}
}
#endif

unsigned char msg2133_check_sum(unsigned char *pval)
{
	int i, sum = 0;

	for(i = 0; i < 7; i++)
	{
		sum += pval[i];
	}

	return (unsigned char)((-sum) & 0xFF);
}

static int msg2133_read_data(struct i2c_client *client)
{

	int ret, keycode;
	unsigned char reg_val[8] = {0};
	int dst_x=0,dst_y=0;	//,xysawp_temp=0;
	unsigned int temp_checksum;
	struct TouchScreenInfo_t touchData;
	struct msg2133_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;

	event->touch_point = 0;
	ret = i2c_master_recv(client, reg_val, 8);
	MSG2133_DBG("%s: ret = %d\n", __func__, ret);
	if(ret <= 0)
	{
		return ret;
	}
	
#if CTP_CONVERT_XY  
	event->y1 =  ((reg_val[1] & 0xF0) << 4) | reg_val[2];	
	event->x1  = ((reg_val[1] & 0x0F) << 8) | reg_val[3];
	dst_y = ((reg_val[4] & 0xF0) << 4) | reg_val[5];
	dst_x = ((reg_val[4] & 0x0F) << 8) | reg_val[6];
#else
	event->x1 =  ((reg_val[1] & 0xF0) << 4) | reg_val[2];	
	event->y1  = ((reg_val[1] & 0x0F) << 8) | reg_val[3];
	dst_x = ((reg_val[4] & 0xF0) << 4) | reg_val[5];
	dst_y = ((reg_val[4] & 0x0F) << 8) | reg_val[6];
#endif

	temp_checksum = msg2133_check_sum(reg_val);

	if((temp_checksum != reg_val[7]) || (reg_val[0] != 0x52) ){
		return 0;
	}else{
		if ((reg_val[1] == 0xFF) && (reg_val[2] == 0xFF) && (reg_val[3] == 0xFF) && (reg_val[4] == 0xFF) && (reg_val[6] == 0xFF)){
			event->x1 = 0; // final X coordinate
			event->y1 = 0; // final Y coordinate
			MSG2133_DBG("%s: reg_val[5] = 0x%x\n", __func__, reg_val[5]);

			if((reg_val[5]==0x0)||(reg_val[5]==0xFF)){
				event->touch_point  = 0; //touch end
				touchData.nTouchKeyCode = 0; //TouchKeyMode
				touchData.nTouchKeyMode = 0; //TouchKeyMode
				keycode=0;
			}
			#ifdef TP_PROXIMITY_SENSOR
			else if (reg_val[5] == 0x40)// leave
			{
				tpd_proximity_dir_faraway = 1;
				//tpd_proximity_need_report = 1;
			       MSG2133_DBG("11111111111111111111111111111111111111111\n");
			}
			else if (reg_val[5] == 0x80)// close to
			{
				tpd_proximity_dir_faraway = 0;
				//tpd_proximity_need_report = 1;
				MSG2133_DBG("2222222222222222222222222222222222222\n");
			}
			#endif
			else{
				touchData.nTouchKeyMode = 1; //TouchKeyMode
				touchData.nTouchKeyCode = reg_val[5]; //TouchKeyCode
				keycode= reg_val[5];
				MSG2133_DBG("%s: keycode = %d\n", __func__, keycode);
					if(keycode==1)								//MENU
					{
					#if defined(CONFIG_LCD_WVGA)
						event->x1 = 416; // final X coordinate
						event->y1 = 840; // final Y coordinate
					#else
						event->x1 = 192; // final X coordinate
						event->y1 = 529; // final Y coordinate
					#endif
					}
					else if(keycode==2)						//HOME
					{
					#if defined(CONFIG_LCD_WVGA)
						event->x1 = 352; // final X coordinate
						event->y1 = 840; // final Y coordinate
					#else
						event->x1 = 64; // final X coordinate
						event->y1 = 529; // final Y coordinate
					#endif
					}
					else if(keycode==4)						//BACK
					{
					#if defined(CONFIG_LCD_WVGA)
						event->x1 = 288; // final X coordinate
						event->y1 = 840; // final Y coordinate
					#else
						event->x1 = 128; // final X coordinate
						event->y1 = 529; // final Y coordinate
					#endif
					}
					else										//SEARCH
					{
						event->x1 = 64; // final X coordinate
						event->y1 = 529; // final Y coordinate
					}
				event->touch_point  = 1;
			}
		}else{
			touchData.nTouchKeyMode = 0; //Touch on screen...

			if ((dst_x == 0) && (dst_y == 0)){
				event->touch_point  = 1; //one touch
#if 0
				xysawp_temp=cinfo->x1;
				cinfo->x1=2047-cinfo->y1;
				cinfo->y1=xysawp_temp;
#endif
				event->x1 = (event->x1 * TS_WIDTH_MAX) / 2048;
				event->y1 = (event->y1 * TS_HEIGHT_MAX) / 2048;

				//printk("zxy y1 = %d",event->y1 * TS_HEIGHT_MAX);
				//printk("zxy x1 = %d",event->x1 * TS_WIDTH_MAX);
			}else{
				event->touch_point  = 2; //two touch
				if (dst_x > 2048) {    //transform the unsigh value to sign value
					dst_x -= 4096;
				}
				if (dst_y > 2048){
					dst_y -= 4096;
				}

				event->x2 = (event->x1 + dst_x);
				event->y2 = (event->y1 + dst_y);

#if 0
				xysawp_temp=cinfo->x1;
				cinfo->x1=2047-cinfo->y1;
				cinfo->y1=xysawp_temp;

				xysawp_temp=cinfo->x2;
				cinfo->x2=2047-cinfo->y2;
				cinfo->y2=xysawp_temp;
#endif

				event->x1 = (event->x1 * TS_WIDTH_MAX) / 2048;
				event->y1 = (event->y1 * TS_HEIGHT_MAX) / 2048;

				event->x2 = (event->x2 * TS_WIDTH_MAX) / 2048;
				event->y2 = (event->y2 * TS_HEIGHT_MAX) / 2048;

			}
		}
		return 1;
	}

	return 1;
}

static void msg2133_report_value(struct i2c_client *client)
{
	struct msg2133_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;

	MSG2133_DBG("%s, point = %d\n", __func__, event->touch_point);
	
	#ifdef TP_PROXIMITY_SENSOR
	TPD_PROXIMITY_DMESG("report tpd_proximity_need_report %d!\n", tpd_proximity_need_report);
	#endif
	
	if(event->touch_point){
		input_report_key(data->input_dev, BTN_TOUCH, 1);
		switch(event->touch_point) {
		case 2:
			MSG2133_DBG("%s, x2=%d, y2=%d\n", __func__, event->x2, event->y2);
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 15);
#if CTP_NEGATIVE_X
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, (TS_WIDTH_MAX - event->x2));
#else
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x2);
#endif
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y2);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);

		case 1:
			MSG2133_DBG("%s, x1=%d, y1=%d\n", __func__, event->x1, event->y1);
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 15);
#if CTP_NEGATIVE_X
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, (TS_WIDTH_MAX - event->x1));
#else
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x1);
#endif
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y1);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);

		default:
			//MSG2133_DBG("==touch_point default =\n");
			
			break;
		}
	}
	#ifdef TP_PROXIMITY_SENSOR
	else if (tpd_proximity_need_report == 1)
	{
		//tpd_proximity_need_report = 0;
		
		//get raw data
		TPD_PROXIMITY_DEBUG("tpd_proximity_dir_faraway=%d\n", tpd_proximity_dir_faraway);
		
		//let up layer to know
		input_report_abs(data->input_dev, ABS_DISTANCE, tpd_proximity_dir_faraway);
		input_mt_sync(data->input_dev);
		
		//return;
	}
	#endif
	else{
		//		input_report_abs(data->input_dev,ABS_PRESSURE,0);
		input_report_key(data->input_dev, BTN_TOUCH, 0);
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
	}

	input_sync(data->input_dev);
}	/*end msg2133_report_value*/

static void msg2133_ts_pen_irq_work(struct work_struct *work)
{
	int ret = -1;
	MSG2133_DBG("%s\n", __func__);
	ret = msg2133_read_data(this_client);	
	MSG2133_DBG("%s, ret = %d\n", __func__, ret);
	if (ret) {	
		msg2133_report_value(this_client);
	}
	enable_irq(this_client->irq);
}

static irqreturn_t msg2133_ts_interrupt(int irq, void *dev_id)
{
	struct msg2133_ts_data *msg2133_ts = (struct msg2133_ts_data *)dev_id;

	MSG2133_DBG("%s\n", __func__);

	disable_irq_nosync(this_client->irq);
	if (!work_pending(&msg2133_ts->pen_event_work)) {
		queue_work(msg2133_ts->ts_workqueue, &msg2133_ts->pen_event_work);
	}

	return IRQ_HANDLED;
}

static void msg2133_ts_suspend(struct early_suspend *handler)
{
		struct msg2133_ts_platform_data *pdata = g_msg2133_ts->platform_data;

	MSG2133_DBG("==%s==\n", __func__);
#ifdef TP_PROXIMITY_SENSOR
	TPD_PROXIMITY_DEBUG("tpd_proximity_flag=%d\n", tpd_proximity_flag);
	if (tpd_proximity_flag)
	{
		return;
	}
#endif

	disable_irq_nosync(this_client->irq);
	msleep(3);
	gpio_set_value(pdata->reset_gpio_number, 0);
	msleep(10);

	msg2133_device_power_off();
}

static void msg2133_ts_resume(struct early_suspend *handler)
{	
	MSG2133_DBG("==%s==start==\n", __func__);
		struct msg2133_ts_platform_data *pdata = g_msg2133_ts->platform_data;
#ifdef TP_PROXIMITY_SENSOR
	TPD_PROXIMITY_DEBUG("tpd_proximity_flag=%d, tpd_proximity_dir_faraway=%d\n", tpd_proximity_flag, tpd_proximity_dir_faraway);
	if((tpd_proximity_flag == 1)&&(tpd_proximity_dir_faraway == 0))
	{
		/*
		mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
		mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY,			NULL, 1);
		msleep(10);
		MSG2133_INT_Pin_EN(0);
		MSG2133_En_Pin_Out(0);
		//hwPowerDown(MT65XX_POWER_LDO_VGP2, "TP");
		msleep(10);
		//hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_2800, "TP");
		MSG2133_En_Pin_Out(1);
		msleep(150);
		MSG2133_INT_Pin_EN(1);
		mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_POLARITY_HIGH, tpd_eint_interrupt_handler,1);
		mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
		return 0;
		*/
	}
#endif

	gpio_set_value(pdata->reset_gpio_number, 0);
	msg2133_device_power_on();
	msleep(20);
	gpio_set_value(pdata->reset_gpio_number,1);
	msleep(100);
	enable_irq(this_client->irq);
	//added by johnford chen.
#ifdef TP_PROXIMITY_SENSOR
	if(tpd_proximity_flag == 1)
        {
        	MSG2133_enable_ps(1);   

        }
#endif
	
}

static int msg2133_ts_config_pins(void)
{ 
	int	msg2133_irq; 
	struct msg2133_ts_platform_data *pdata = g_msg2133_ts->platform_data;

    gpio_request(pdata->irq_gpio_number, "MSG_TS_INT");
    gpio_request(pdata->reset_gpio_number, "MSG_TS_RST");

	gpio_direction_output(pdata->reset_gpio_number, 1);	
	gpio_direction_input(pdata->irq_gpio_number);		
	gpio_set_value(pdata->reset_gpio_number, 1);

	msg2133_irq = gpio_to_irq(pdata->irq_gpio_number);

	msleep(10); //wait for stable	

	return msg2133_irq;
}

static int check_ctp_chip(void)
{
	unsigned char	dbbus_tx_data[3];
	unsigned char	dbbus_rx_data[4] ;
	unsigned short	major = 0, minor = 0;
	int				ret=-1;

	ctp_lock_mutex();
	msleep(400);	// Alex.shi:Afte reset, wait 400ms for stable
	//Get_Chip_Version();
	dbbus_tx_data[0] = 0x53;
	dbbus_tx_data[1] = 0x00;
	dbbus_tx_data[2] = 0x74;
	ret = msg2133_i2c_write(&dbbus_tx_data[0], 3);
	ret = msg2133_i2c_read(&dbbus_rx_data[0], 4);
	if (false == ret)
		ret = -1;
	else
	{
		major = (dbbus_rx_data[1] << 8) + dbbus_rx_data[0];
		minor = (dbbus_rx_data[3] << 8) + dbbus_rx_data[2];
		MSG2133_DBG("***major = %d ***\n", major);
		MSG2133_DBG("***minor = %d ***\n", minor);
		tp_device_id(0x2133);
		ret = 1;
	}
	ctp_unlock_mutex();
	return ret;
}

static int remove_ctp_chip(void)
{
	ctp_lock_mutex();
	tp_device_id(0xFFFF);
	ctp_unlock_mutex();
	return 0;
}

static int msg2133_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct msg2133_ts_data *msg2133_ts;
	struct regulator *reg_vdd;
	struct input_dev *input_dev;
	int err = 0;
	struct msg2133_ts_platform_data *pdata = client->dev.platform_data;

	MSG2133_INFO("%s\n", __func__);
	if(tp_device_id(0)!=0)
	{
		MSG2133_INFO("CTP(0x%x)Exist!", tp_device_id(0));
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	this_client = client;

	msg2133_device_power_on();

	reg_vdd = regulator_get(&client->dev, pdata->vdd_name);

	regulator_set_voltage(reg_vdd, 1800000, 1800000);

	regulator_enable(reg_vdd);

	//client->irq = msg2133_ts_config_pins();

    gpio_request(pdata->irq_gpio_number, "MSG_TS_INT");
    gpio_request(pdata->reset_gpio_number, "MSG_TS_RST");

	gpio_direction_output(pdata->reset_gpio_number, 1);	
	gpio_direction_input(pdata->irq_gpio_number);		
	gpio_set_value(pdata->reset_gpio_number, 1);

	client->irq = gpio_to_irq(pdata->irq_gpio_number);

	msleep(10); //wait for stable	



	if(check_ctp_chip()<0)
	{
		err = -ENODEV;
		MSG2133_INFO("Fail to find chip");
		goto exit_checkchip_failed;
	}


	msg2133_ts = kzalloc(sizeof(*msg2133_ts), GFP_KERNEL);
	if (!msg2133_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}
    g_msg2133_ts = msg2133_ts;
	g_msg2133_ts->platform_data = pdata;
	i2c_set_clientdata(client, msg2133_ts);
	

	MSG2133_DBG("I2C addr=%x", client->addr);

	INIT_WORK(&msg2133_ts->pen_event_work, msg2133_ts_pen_irq_work);

	msg2133_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!msg2133_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}

	//MSG2133_DBG("==request_irq=\n");
	MSG2133_DBG("%s: %s IRQ number is %d", __func__, client->name, client->irq);
	err = request_irq(client->irq, msg2133_ts_interrupt, IRQF_TRIGGER_RISING, client->name, msg2133_ts);
	if (err < 0) {
		MSG2133_DBG("%s: msg2133_probe: request irq failed\n", __func__);
		goto exit_irq_request_failed;
	}

	disable_irq(client->irq);

	//MSG2133_DBG("==input_allocate_device=\n");
	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		MSG2133_DBG("%s: failed to allocate input device\n", __func__);
		goto exit_input_dev_alloc_failed;
	}

	msg2133_ts->input_dev = input_dev;	

	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(EV_SYN, input_dev->evbit);


	__set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	__set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	__set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	__set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);


#ifdef VIRTUAL_KEYS
	__set_bit(KEY_MENU,  input_dev->keybit);
	__set_bit(KEY_BACK,  input_dev->keybit);
	__set_bit(KEY_HOMEPAGE,  input_dev->keybit);
	__set_bit(KEY_SEARCH,  input_dev->keybit);
#endif

#ifdef TP_PROXIMITY_SENSOR
	input_set_abs_params(input_dev, ABS_DISTANCE, 0, 1, 0, 0);
	//MSG2133_enable_ps(1);
#endif

	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, TS_WIDTH_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, TS_HEIGHT_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);



	input_dev->name		= "SPRD_WW_CTP";		//dev_name(&client->dev)
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
			"msg2133_ts_probe: failed to register input device: %s\n",
			dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	MSG2133_DBG("==register_early_suspend =");
	msg2133_ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	msg2133_ts->early_suspend.suspend = msg2133_ts_suspend;
	msg2133_ts->early_suspend.resume	= msg2133_ts_resume;
	register_early_suspend(&msg2133_ts->early_suspend);
#endif

	msleep(50);

#ifdef MSG2133_UPDATE
	msg2133_init_class();
#endif

#ifdef VIRTUAL_KEYS
	virtual_keys_init();
#endif
#ifdef TP_PROXIMITY_SENSOR

    err = sysfs_create_group(&(g_msg2133_ts->input_dev->dev.kobj), &msg2133_attr_group);
	if (err) {
		dev_err(&client->dev, "create device file failed!\n");
		err = -EINVAL;
		goto exit_create_sysfs_failed;
	}
#endif

	enable_irq(client->irq);

	return 0;
#ifdef TP_PROXIMITY_SENSOR
exit_create_sysfs_failed:
#endif
exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
	free_irq(client->irq, msg2133_ts);
exit_irq_request_failed:
	if (msg2133_ts->ts_workqueue)
		destroy_workqueue(msg2133_ts->ts_workqueue);
#ifdef TP_PROXIMITY_SENSOR
err_mis_reg:
	remove_ctp_chip();
#endif
exit_create_singlethread:
	MSG2133_DBG("==singlethread error =\n");
	cancel_work_sync(&msg2133_ts->pen_event_work);
	i2c_set_clientdata(client, NULL);
	kfree(msg2133_ts);
exit_alloc_data_failed:
	remove_ctp_chip();
exit_checkchip_failed:
	free_irq(client->irq,NULL);
	client->irq = 0;
		if (pdata->reset_gpio_number>0)
		gpio_free(pdata->reset_gpio_number);
	if (pdata->irq_gpio_number>0)
		gpio_free(pdata->irq_gpio_number);


    //gpio_free(sprd_3rdparty_gpio_tp_irq);
    //gpio_free(sprd_3rdparty_gpio_tp_rst);
exit_check_functionality_failed:
	printk("%s: Probe Fail!\n",__func__);

	return err;
}

static int  msg2133_ts_remove(struct i2c_client *client)
{
	struct msg2133_ts_data *msg2133_ts = i2c_get_clientdata(client);

	MSG2133_DBG("==msg2133_ts_remove=\n");


#ifdef VIRTUAL_KEYS
	virtual_keys_destroy();
#endif
#ifdef MSG2133_UPDATE
	msg2133_uninit_class();
#endif

#ifdef TP_PROXIMITY_SENSOR	
	sysfs_remove_group(&(msg2133_ts->input_dev->dev.kobj), &msg2133_attr_group);
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&msg2133_ts->early_suspend);
#endif
	input_unregister_device(msg2133_ts->input_dev);
	input_free_device(msg2133_ts->input_dev);
	free_irq(client->irq, msg2133_ts);
	if (msg2133_ts->ts_workqueue)
		destroy_workqueue(msg2133_ts->ts_workqueue);
	cancel_work_sync(&msg2133_ts->pen_event_work);
	i2c_set_clientdata(client, NULL);
	if (msg2133_ts->platform_data->reset_gpio_number>0)
		gpio_free(msg2133_ts->platform_data->reset_gpio_number);
	if (msg2133_ts->platform_data->irq_gpio_number>0)
		gpio_free(msg2133_ts->platform_data->irq_gpio_number);
	kfree(msg2133_ts);
	remove_ctp_chip();
	//free_irq(client->irq);
	client->irq = 0;
    //gpio_free(sprd_3rdparty_gpio_tp_irq);
    //gpio_free(sprd_3rdparty_gpio_tp_rst);
	msg2133_device_power_off();

	return 0;
}



static const struct i2c_device_id msg2133_ts_id[] = {
	{ MSG2133_TS_NAME, 0 },{ }
};

MODULE_DEVICE_TABLE(i2c, msg2133_ts_id);

static struct i2c_driver msg2133_ts_driver = {
	.probe		= msg2133_ts_probe,
	.remove		= msg2133_ts_remove,
	.id_table	= msg2133_ts_id,
	.driver	= {
		.name	= MSG2133_TS_NAME,
		.owner	= THIS_MODULE,
	},
};

#if I2C_BOARD_INFO_METHOD
static int __init msg2133_init_module(void)
{
	MSG2133_INFO("==msg2133_init_module==\n");
	
	if(tp_device_id(0)!=0)
	{
		printk("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	return i2c_add_driver(&msg2133_ts_driver);
}

static void __exit msg2133_exit_module(void)
{
	MSG2133_INFO("==msg2133_exit_module==\n");
	i2c_del_driver(&msg2133_ts_driver);
}
#else //register i2c device&driver dynamicly
int msg2133_add_i2c_device(struct i2c_board_info *info)
{
	int err;
	struct i2c_adapter *adapter;
	struct i2c_client *client;

	adapter = i2c_get_adapter(MSG2133_BUS_NUM);
	if (!adapter) {
		MSG2133_INFO("%s: can't get i2c adapter %d\n", __func__, MSG2133_BUS_NUM);
		err = -ENODEV;
		goto err_adapter;
	}

	client = i2c_new_device(adapter, info);
	if (!client) {
		MSG2133_INFO("%s:  can't add i2c device at 0x%x\n", __func__, (unsigned int)info->addr);
		err = -ENODEV;
		goto err_device;
	}

	i2c_put_adapter(adapter);
	this_client = client;

	return 0;

err_device:
	i2c_put_adapter(adapter);
err_adapter:
	return err;
}

static int __init msg2133_init_module(void)
{
	int err;

	printk("%s\n", __func__);

	if(tp_device_id(0)!=0)
	{
		MSG2133_INFO("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	err = msg2133_add_i2c_device(&msg2133_i2c_boardinfo);
	if (err)
		return err;
	err = i2c_add_driver(&msg2133_ts_driver);
	if (err != 0) {
		printk("%s: can't add i2c driver\n", __FUNCTION__);
		i2c_unregister_device(this_client);
		err = -ENODEV;
	}
	
	return err;
}

static void __exit msg2133_exit_module(void)
{
	printk("%s\n", __func__);
	i2c_unregister_device(this_client);
	i2c_del_driver(&msg2133_ts_driver);
}
#endif

module_init(msg2133_init_module);
module_exit(msg2133_exit_module);

MODULE_LICENSE("GPL");

