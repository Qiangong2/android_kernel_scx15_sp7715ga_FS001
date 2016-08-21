/* drivers/input/touchscreen/gt818x.c
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
 * Version:1.4
 * Author:scott@goodix.com
 * Release Date:2012/09/20
 * Revision record:
 *      V1.0:2012/06/08,create file
 *      V1.2:2012/08/06,modify to support GT868&GT968M
 *      V1.4:2012/09/20,G868 sensor ID & coor key suppoert
 */

#include <linux/irq.h>
#include <linux/cdc_com.h>
#include <linux/slab.h>
#include "gt818x_868_968m.h"
#if GTP_ICS_SLOT_REPORT
#include <linux/input/mt.h>
#endif
#define NOT_ONLY_LARGETOUCH 1

#include <linux/regulator/consumer.h>
#include <mach/regulator.h>
#if GTP_PROXIMITY
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>


#define PROXIMITY_DEVICE   "gtp_proximity"

/******************confirm the parameter that hal use**********************/
#define GTP_IOCTL_MAGIC        	0X5D
#define GTP_IOCTL_PROX_ON		_IO(GTP_IOCTL_MAGIC, 7)
#define GTP_IOCTL_PROX_OFF		_IO(GTP_IOCTL_MAGIC, 8)
/****************************************/

#define PS_DEBUG_ON 	1
#define PS_DBG(fmt, arg...)				do{\
							   if(PS_DEBUG_ON)\
							   printk("<<PS-DBG>>[%d]"fmt"\n", __LINE__, ##arg);\
							}while(0)

static int misc_opened;
static int gtp_proximity_start = 0;	/* 0 is stop, 1 is start */
#endif
struct sprd_i2c_setup_data {
	unsigned i2c_bus;  //the same number as i2c->adap.nr in adapter probe function
	unsigned short i2c_address;
	int irq;
	char type[I2C_NAME_SIZE];
};
static struct sprd_i2c_setup_data goodix_ts_setup={0, GTP_TS_ADDR, 0, GTP_I2C_NAME};
static struct workqueue_struct *goodix_wq;
static struct i2c_client * i2c_connect_client = NULL; 
static u8 config[GTP_CONFIG_LENGTH + GTP_ADDR_LENGTH]
                = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};

#if GTP_HAVE_TOUCH_KEY
    static const u16 touch_key_array[] = GTP_KEY_TAB;
    #define GTP_MAX_KEY_NUM	 (sizeof(touch_key_array)/sizeof(touch_key_array[0]))
    #ifdef TOUCH_VIRTUAL_KEYS
    static struct kobject *properties_kobj;
    #endif
#endif

static s8 gtp_i2c_test(struct i2c_client *client);	

static void goodix_ts_early_suspend(struct early_suspend *h);
static void goodix_ts_late_resume(struct early_suspend *h);
 
#if GTP_CREATE_WR_NODE
extern s32 init_wr_node(struct i2c_client*);
extern void uninit_wr_node(void);
#endif

#if GTP_AUTO_UPDATE
extern u8 gup_init_update_proc(struct goodix_ts_data *);
#endif

#if GTP_ESD_PROTECT
static struct delayed_work gtp_esd_check_work;
static struct workqueue_struct * gtp_esd_check_workqueue = NULL;
static void gtp_esd_check_func(struct work_struct *);
#endif

#ifdef TOUCH_VIRTUAL_KEYS
static ssize_t virtual_keys_show(struct kobject *kobj,
             struct kobj_attribute *attr, char *buf)
{
		return sprintf(buf,
				__stringify(EV_KEY) ":" __stringify(KEY_MENU) ":40:980:50:30"
				":" __stringify(EV_KEY) ":" __stringify(KEY_HOMEPAGE) ":120:980:50:30"
				":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":200:980:50:30"
				":" __stringify(EV_KEY) ":" __stringify(KEY_SEARCH) ":280:980:50:30"
				"\n");
}

static struct kobj_attribute virtual_keys_attr = {
	.attr = {
		.name = "virtualkeys.Goodix-TS",
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

static void virtual_keys_init(void)
{
	int ret;

	properties_kobj = kobject_create_and_add("board_properties", NULL);
	if (properties_kobj)
		ret = sysfs_create_group(properties_kobj,&properties_attr_group);
	if (!properties_kobj || ret)
		pr_err("failed to create board_properties\n");    
}

static void virtual_keys_destroy(void)
{
	kobject_del(properties_kobj);
}
#endif

/*******************************************************	
Function:
	Read data from the i2c slave device.

Input:
	client:	i2c device.
	buf[0]:operate address.
	buf[1]~buf[len]:read data buffer.
	len:operate length.
	
Output:
	numbers of i2c_msgs to transfer
*********************************************************/
s32 gtp_i2c_read(struct i2c_client *client, u8 *buf, s32 len)
{
    struct i2c_msg msgs[2];
    s32 ret=-1;
    s32 retries = 0;

    GTP_DEBUG_FUNC();

    msgs[0].flags = !I2C_M_RD;
    msgs[0].addr  = client->addr;
    msgs[0].len   = GTP_ADDR_LENGTH;
    msgs[0].buf   = &buf[0];

    msgs[1].flags = I2C_M_RD;
    msgs[1].addr  = client->addr;
    msgs[1].len   = len - GTP_ADDR_LENGTH;
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];

    while(retries < 5)
    {
        ret = i2c_transfer(client->adapter, msgs, 2);
        if (ret == 2)break;
        if (ret>0)
            ret = -EIO;	// i2c不通  
        retries++;
        GTP_DEBUG("%s:alex failed %d times", __FUNCTION__, retries);
    }
    return ret;
}

/*******************************************************	
Function:
	write data to the i2c slave device.

Input:
	client:	i2c device.
	buf[0]:operate address.
	buf[1]~buf[len]:write data buffer.
	len:operate length.
	
Output:
	numbers of i2c_msgs to transfer.
*********************************************************/
s32 gtp_i2c_write(struct i2c_client *client,u8 *buf,s32 len)
{
    struct i2c_msg msg;
    s32 ret=-1;
    s32 retries = 0;

    GTP_DEBUG_FUNC();

    msg.flags = !I2C_M_RD;
    msg.addr  = client->addr;
    msg.len   = len;
    msg.buf   = buf;

    while(retries < 5)
    {
        ret = i2c_transfer(client->adapter, &msg, 1);
        if (ret == 1)break;
        if (ret>0)
            ret = -EIO;	// i2c不通  
        retries++;
        GTP_DEBUG("%s:alex failed %d times", __FUNCTION__, retries);
    }
    return ret;
}

/*******************************************************	
Function:
	write i2c end cmd.

Input:
	client:	i2c device.
	
Output:
	numbers of i2c_msgs to transfer.
*********************************************************/
s32 gtp_i2c_end_cmd(struct i2c_client *client)
{
    s32 ret = -1;
    u8 end_cmd_data[2]={0x80, 0x00}; 
    
    GTP_DEBUG_FUNC();

    ret = gtp_i2c_write(client, end_cmd_data, 2);

    return ret;
}
#if GTP_PROXIMITY
static int gtp_proximity_open(void)
{
	int ret;
#if NOT_ONLY_LARGETOUCH
	u8 rbuffer[3] = {0x06, 0x92};
#else
	u8 wbuffer[3] = {0x06, 0xEA};
	u8 wbuffer_refresh[3] = {0x07, 0x0B, 0x01};

	wbuffer[2] = config[106];
#endif
	PS_DBG("%s\n", __func__);
	if (misc_opened)
		return -EBUSY;
	misc_opened = 1;
	gtp_proximity_start = 1;
#if NOT_ONLY_LARGETOUCH
	gtp_i2c_read(i2c_connect_client, rbuffer, 3);
	rbuffer[2] |= 0x80;
	ret = gtp_i2c_write(i2c_connect_client, rbuffer, 3);
#else
	ret = gtp_i2c_write(i2c_connect_client, wbuffer, 3);
	ret = gtp_i2c_write(i2c_connect_client, wbuffer_refresh, 3);
#endif
	gtp_i2c_end_cmd(i2c_connect_client);
	return ret;
}

static void gtp_proximity_release(void)
{
	int ret;
#if NOT_ONLY_LARGETOUCH
	u8 rbuffer[3] = {0x06, 0x92};
#else
	u8 wbuffer[3] = {0x06, 0xEA};
	u8 wbuffer_refresh[3] = {0x07, 0x0B, 0x01};

	wbuffer[2] = config[74];
#endif
	PS_DBG("%s\n", __func__);
	misc_opened = 0;
	gtp_proximity_start = 0;
#if NOT_ONLY_LARGETOUCH
	gtp_i2c_read(i2c_connect_client, rbuffer, 3);
	rbuffer[2] &= 0x7f;
	ret = gtp_i2c_write(i2c_connect_client, rbuffer, 3);
#else
	ret = gtp_i2c_write(i2c_connect_client, wbuffer, 3);
	ret = gtp_i2c_write(i2c_connect_client, wbuffer_refresh, 3);
#endif
	gtp_i2c_end_cmd(i2c_connect_client);
}

static long gtp_proximity_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	PS_DBG("%s cmd %d\n", __func__, _IOC_NR(cmd));

	switch (cmd) {
	case GTP_IOCTL_PROX_ON:
		gtp_proximity_open();
		break;
	case GTP_IOCTL_PROX_OFF:
		gtp_proximity_release();
		break;
	default:
		pr_err("%s: invalid cmd %d\n", __func__, _IOC_NR(cmd));
		return -EINVAL;
	}
	return 0;
}
static struct file_operations gtp_proximity_fops = {
	.owner = THIS_MODULE,
	.open = NULL,//gtp_proximity_open,
	.release = NULL,//gtp_proximity_release,
	.unlocked_ioctl = gtp_proximity_ioctl
};

struct miscdevice gtp_proximity_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = PROXIMITY_DEVICE,						//match the hal's name 
	.fops = &gtp_proximity_fops
};
#endif


/*******************************************************
Function:
	Send config Function.

Input:
	client:	i2c client.

Output:
	Executive outcomes.0--success,non-0--fail.
*******************************************************/
s32 gtp_send_cfg(struct i2c_client *client)
{
    s32 ret = -1;
#if GTP_DRIVER_SEND_CFG
    s32 retry = 0;

    for (retry = 0; retry < 2; retry++)	// Alex.shi 5->2
    {
        ret = gtp_i2c_write(client, config , GTP_CONFIG_LENGTH + GTP_ADDR_LENGTH);        
        gtp_i2c_end_cmd(client);

        if (ret > 0)
        {
            break;
        }
        GTP_DEBUG("%s:alex failed %d times", __FUNCTION__, retry);
    }
#endif

    return ret;
}

/*******************************************************
Function:
	Enable IRQ Function.

Input:
	ts:	i2c client private struct.
	
Output:
	None.
*******************************************************/
void gtp_irq_disable(struct goodix_ts_data *ts)
{	
    unsigned long irqflags;
	
    GTP_DEBUG_FUNC();
	
    spin_lock_irqsave(&ts->irq_lock, irqflags);
    if (!ts->irq_is_disable)
    {
        ts->irq_is_disable = 1; 
        disable_irq_nosync(ts->client->irq);
    }
    spin_unlock_irqrestore(&ts->irq_lock, irqflags);
}

/*******************************************************
Function:
	Disable IRQ Function.

Input:
	ts:	i2c client private struct.
	
Output:
	None.
*******************************************************/
void gtp_irq_enable(struct goodix_ts_data *ts)
{	
    unsigned long irqflags;
	
    GTP_DEBUG_FUNC();
		
    spin_lock_irqsave(&ts->irq_lock, irqflags);
    if (ts->irq_is_disable) 
    {
        enable_irq(ts->client->irq);
        ts->irq_is_disable = 0;	
    }
    spin_unlock_irqrestore(&ts->irq_lock, irqflags);
}

/*******************************************************
Function:
	Touch down report function.

Input:
	ts:private data.
	id:tracking id.
	x:input x.
	y:input y.
	w:input weight.
	
Output:
	None.
*******************************************************/
static void gtp_touch_down(struct goodix_ts_data* ts,s32 id,s32 x,s32 y,s32 w)
{
#if GTP_CHANGE_X2Y
    GTP_SWAP(x, y);
#endif

#if GTP_NEGATIVE_X
    GTP_DEBUG("in X:%d",x);
    x = abs((GTP_MAX_WIDTH-x));
    GTP_DEBUG("out X:%d",x);
#endif
#if GTP_NEGATIVE_Y
    GTP_DEBUG("in Y:%d",y);
    y = abs((GTP_MAX_HEIGHT-y));
    GTP_DEBUG("out Y:%d",y);
#endif

#if GTP_ICS_SLOT_REPORT
    input_mt_slot(ts->input_dev, id);
    input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, id);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_X, x);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, y);
    input_report_abs(ts->input_dev, ABS_MT_PRESSURE, w);
#else
    input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, w);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_X, x);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, y);
    input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, w);
    input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, id);
    input_mt_sync(ts->input_dev);
#endif

    GTP_DEBUG("ID:%d, X:%d, Y:%d, W:%d", id, x, y, w);
}

/*******************************************************
Function:
	Touch up report function.

Input:
	ts:private data.
	
Output:
	None.
*******************************************************/
static void gtp_touch_up(struct goodix_ts_data* ts, s32 id)
{
#if GTP_ICS_SLOT_REPORT
    input_mt_slot(ts->input_dev, id);
    input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, -1);
    GTP_DEBUG("Touch id[%2d] release!", id);
#else
    input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0);
    input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0);
    input_mt_sync(ts->input_dev);
#endif
}

/*******************************************************
Function:
	Goodix touchscreen work function.

Input:
	work:	work_struct of goodix_wq.
	
Output:
	None.
*******************************************************/
static void goodix_ts_work_func(struct work_struct *work)
{
    u8  index_data[3] = {(u8)(GTP_REG_INDEX>>8),(u8)GTP_REG_INDEX,0};
    u8  point_data[2 + 1 + 8 * GTP_MAX_TOUCH] = {GTP_READ_COOR_ADDR>>8, GTP_READ_COOR_ADDR & 0xFF};
    u8  touch_num = 0;
    static u8 pre_touch = 0;
    static u8 pre_key = 0;
    u8  key_value = 0;
    u8* coor_data = NULL;
    s32 input_x = 0;
    s32 input_y = 0;
    s32 input_w = 0;
    s32 input_id = 0;
    s32 idx = 0;
    s32 ret = -1;
    
#if GTP_USE_868_968M
    s32 key_pnt_x[GTP_MAX_KEY_NUM] = KEY_CNTR_PNT_X;
    s32 key_pnt_y[GTP_MAX_KEY_NUM] = KEY_CNTR_PNT_Y; 
#endif

    struct goodix_ts_data *ts = NULL;
#if GTP_PROXIMITY
	u8 proximity_data[3] = {0x07, 0x1D};
#endif
    GTP_DEBUG_FUNC();
    
    ts = container_of(work, struct goodix_ts_data, work);
    if (ts->enter_update)
    {
        goto exit_work_func;
    }

    ret = gtp_i2c_read(ts->client, index_data, 3);
    gtp_i2c_end_cmd(ts->client);
    if (ret < 0)
    {
        GTP_ERROR("I2C transfer error. errno:%d\n ", ret);
        goto exit_work_func;
    }

    if ((index_data[GTP_ADDR_LENGTH] & 0x0f) == 0x0f)
    {
        ret = gtp_send_cfg(ts->client);
        if (ret < 0)
        {
            GTP_DEBUG("Reload config failed!\n");
        }
        goto exit_work_func;
    }
    
    if ((index_data[GTP_ADDR_LENGTH] & 0x30) != 0x20)
    {
        GTP_INFO("Data not ready!");
        goto exit_work_func;
    }
    
    touch_num = index_data[GTP_ADDR_LENGTH] & 0x0f;
    if(touch_num > 5)
    {
        touch_num = 5;
    }
#if GTP_PROXIMITY
    if (gtp_proximity_start == 1)
    {
		ret = gtp_i2c_read(ts->client, proximity_data, sizeof(proximity_data)/sizeof(proximity_data[0]));
	#if NOT_ONLY_LARGETOUCH
		if (((proximity_data[2]&0x10) == 0x10) || ((proximity_data[2]&0x0f) == 0x0f) || (touch_num >= 2))
		{
			input_report_abs(ts->input_dev, ABS_DISTANCE, 0);
		}
		else
		{
			input_report_abs(ts->input_dev, ABS_DISTANCE, 1);
		}
	#else
		if ((touch_num >= 2) || ((proximity_data[2]&0x0f) == 0x0f))
		{
			input_report_abs(ts->input_dev, ABS_DISTANCE, 0);
		}
		else
		{
			input_report_abs(ts->input_dev, ABS_DISTANCE, 1);
		}
	#endif
   		PS_DBG("proximity_data[2] = %d, touch_num = %d, \n", proximity_data[2], touch_num);
   }   
#endif	    
    ret = gtp_i2c_read(ts->client, point_data, 2 + 8 * touch_num + 1);
    if(ret < 0)
    {
        GTP_ERROR("I2C transfer error. Number:%d\n ", ret);
        goto exit_work_func;
    }
    gtp_i2c_end_cmd(ts->client);

    GTP_DEBUG_ARRAY(index_data, 3);
    GTP_DEBUG("touch num:%x", touch_num);
    GTP_DEBUG_ARRAY(point_data, 2 + 8 * touch_num + 1);

    coor_data = &point_data[3];

#if GTP_ICS_SLOT_REPORT
    if (pre_touch || touch_num)
    {
        s32 pos = 0;

        for (idx = 0; idx < GTP_MAX_TOUCH; idx++)
        {
            input_id = coor_data[pos] - 1;
            if (input_id == idx)
            {
                input_x = (coor_data[pos + 2] << 8) | coor_data[pos + 1];
                input_y = (coor_data[pos + 4] << 8) | coor_data[pos + 3];
                input_w = 20;

                pos += 8;

                if (ts->coor_div_2)
                {
                    input_x /= 2;
                    input_y /= 2;
                }
                gtp_touch_down(ts, idx, input_x, input_y, input_w);
                pre_touch |= 0x01 << idx;
            }
            else if (pre_touch & (0x01 << idx))
            {
                gtp_touch_up(ts, idx);
                pre_touch &= ~(0x01 << idx);
            }
        }
    }
    
#else
    if (touch_num)
    {
        s32 pos = 0;
        
       input_report_key(ts->input_dev, BTN_TOUCH, 1);
       for (idx = 0; idx < touch_num; idx++)
       {
            input_id = coor_data[pos] - 1;
            input_x = (coor_data[pos + 2] << 8) | coor_data[pos + 1];
            input_y = (coor_data[pos + 4] << 8) | coor_data[pos + 3];
            input_w = 20;

            pos += 8;
            
            if (ts->coor_div_2)
            {
                input_x /= 2;
                input_y /= 2;
            }
            if ((input_x > ts->abs_x_max)||(input_y > ts->abs_y_max))
            {
                continue;
            }
            gtp_touch_down(ts, input_id, input_x, input_y, input_w);
        }
    }
    else if (pre_touch)
    {
        GTP_DEBUG("Touch Release!");
        input_report_key(ts->input_dev, BTN_TOUCH, 0);
        gtp_touch_up(ts, 0);
    }
#if GTP_HAVE_TOUCH_KEY
#if GTP_USE_868_968M
	GTP_DEBUG("ts->coor_div_2 = %d, touch_num = %d", ts->coor_div_2, touch_num);
	if (ts->coor_div_2)
	{
		if (touch_num == 1)
		{
			GTP_DEBUG("input_y = %d, ts->abs_y_max = %d", input_y, ts->abs_y_max);
			// key report as coordinates,judge key or ts point
			if (input_y > ts->abs_y_max)
			{
				for (idx = 0; idx < GTP_MAX_KEY_NUM; idx++)
				{
					if ((ABS_VAL(input_x - key_pnt_x[idx]) <= KEY_AREA_WIDTH_H) &&
						(ABS_VAL(input_y - key_pnt_y[idx]) <= KEY_AREA_HEIGHT_H))
					{
						GTP_DEBUG("key_x = %d, key_y = %d", input_x, input_y);
						key_value |= (0x01<<idx);
					}
					else
					{
						key_value &= (~(0x01<<idx));
					}
				}
			}
		}
	}
    else
	{
        key_value = point_data[GTP_ADDR_LENGTH] & 0x0f;
	}
#else
	key_value = point_data[GTP_ADDR_LENGTH] & 0x0f;
#endif
	GTP_DEBUG("button key index =%d", key_value);
#ifdef TOUCH_VIRTUAL_KEYS
	if ((1==key_value) || (2==key_value) || (4==key_value) || (8==key_value))
	{
		int index_value=0;
		int	x=40;

		if (1==key_value)
			index_value = 0;
		else if (2==key_value)
			index_value = 1;
		else if (4==key_value)
			index_value = 2;
		else if (8==key_value)
			index_value = 3;

		if (KEY_MENU == touch_key_array[index_value])
		{
			x = 40;
		}
		else if (KEY_HOMEPAGE == touch_key_array[index_value])
		{
			x = 120;
		}
		else if (KEY_BACK == touch_key_array[index_value])
		{
			x = 200;
		}
		else if (KEY_SEARCH == touch_key_array[index_value])
		{
			x = 280;
		}
		input_report_key(ts->input_dev, BTN_TOUCH, 1);
		input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, 20);
		input_report_abs(ts->input_dev, ABS_MT_POSITION_X, x);
		input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, 980);
		input_mt_sync(ts->input_dev);
		input_sync(ts->input_dev);

		pre_touch = 1;	// alex.shi 这样才会发送弹起消息
        goto exit_work_func;
	}
#else
    if(key_value || pre_key)
    {
        for (idx = 0; idx < GTP_MAX_KEY_NUM; idx++)
        {
            input_report_key(ts->input_dev, touch_key_array[idx], key_value & (0x01<<idx));   
        }
    }
#endif
    pre_key = key_value;
#endif

    pre_touch = touch_num;
    
#endif
    input_sync(ts->input_dev);

exit_work_func:
    if (ts->use_irq)
    {
        gtp_irq_enable(ts);
    }
}

/*******************************************************
Function:
	Timer interrupt service routine.

Input:
	timer:	timer struct pointer.
	
Output:
	Timer work mode. HRTIMER_NORESTART---not restart mode
*******************************************************/
static enum hrtimer_restart goodix_ts_timer_handler(struct hrtimer *timer)
{
    struct goodix_ts_data *ts = container_of(timer, struct goodix_ts_data, timer);
	
    GTP_DEBUG_FUNC();

    queue_work(goodix_wq, &ts->work);
    hrtimer_start(&ts->timer, ktime_set(0, (GTP_POLL_TIME+6)*1000000), HRTIMER_MODE_REL);
    return HRTIMER_NORESTART;
}

/*******************************************************
Function:
	External interrupt service routine.

Input:
	irq:	interrupt number.
	dev_id: private data pointer.
	
Output:
	irq execute status.
*******************************************************/
static irqreturn_t goodix_ts_irq_handler(int irq, void *dev_id)
{
    struct goodix_ts_data *ts = dev_id;

    GTP_DEBUG_FUNC();

    gtp_irq_disable(ts);
    queue_work(goodix_wq, &ts->work);

    return IRQ_HANDLED;
}

/*******************************************************
Function:
	Reset chip Function.

Input:
	ms:reset time.
	
Output:
	None.
*******************************************************/
void gtp_reset_guitar(s32 ms)
{
    GTP_DEBUG_FUNC();
    GTP_GPIO_OUTPUT(GTP_RST_PORT, 0);
    msleep(ms);

    GTP_GPIO_AS_INPUT(GTP_RST_PORT);
    msleep(50);

    return;
}

/*******************************************************
Function:
	Eter sleep function.

Input:
	ts:private data.
	
Output:
	Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gtp_enter_sleep(struct goodix_ts_data * ts)
{
    s8 ret = -1;
    s8 retry = 0;
    u8 i2c_control_buf[3] = {(u8)(GTP_REG_SLEEP >> 8), (u8)GTP_REG_SLEEP, 0x01};

    GTP_DEBUG_FUNC();

    while(retry++ < 2)	// Alex.shi 5->2
    {
        ret = gtp_i2c_write(ts->client, i2c_control_buf, 3);
        gtp_i2c_end_cmd(ts->client);
        if (ret > 0)
        {
            GTP_DEBUG("GTP enter sleep!");
            return ret;
        }
        msleep(10);
        GTP_DEBUG("%s:alex failed %d times", __FUNCTION__, retry);
    }
    GTP_ERROR("GTP send sleep cmd failed.");
    return ret;
}

/*******************************************************
Function:
	Wakeup from sleep mode Function.

Input:
	ts:	private data.
	
Output:
	Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gtp_wakeup_sleep(struct goodix_ts_data * ts)
{
    u8 retry = 0;
    s8 ret = -1;

    GTP_DEBUG_FUNC();

#if GTP_POWER_CTRL_SLEEP
    while(retry++ < 2)	// Alex.shi 5->2
    {
        gtp_reset_guitar(20);
        ret = gtp_send_cfg(ts->client);
        if (ret > 0)
        {
            GTP_DEBUG("Wakeup sleep send config success.");
            return ret;
        }
        GTP_DEBUG("%s:alex failed %d times", __FUNCTION__, retry);
    }
#else
    GTP_GPIO_OUTPUT(GTP_INT_PORT, 0);
    msleep(2);
    GTP_GPIO_AS_INPUT(GTP_INT_PORT);
    msleep(2);
    GTP_GPIO_OUTPUT(GTP_INT_PORT, 0);
    msleep(2);
    GTP_GPIO_AS_INT(GTP_INT_PORT);
    msleep(50);
    while(retry++ < 2)	// Alex.shi 10->2
    {
        ret = gtp_i2c_test(ts->client);
        if (ret > 0)
        {
            GTP_DEBUG("GTP wakeup sleep.");
            return ret;
        }
        gtp_reset_guitar(20);
    }
#endif

    GTP_ERROR("GTP wakeup sleep failed.");
    return ret;
}

/*******************************************************
Function:
	GTP read sensor id function.

Input:
	client:	i2c client private struct.
	
Output:
	sensor ID.
*******************************************************/
u8 gtp_get_sensor_id(struct i2c_client *client)
{
    u8 buf[8] = {0};
    u8 sensor_id = 0;
    u8 i = 0;
    u8 count = 0;
    
    // step 1: setup sensorID port as input
    buf[0] = 0x16;
    buf[1] = 0x00;
    gtp_i2c_read(client, buf, 3);
    buf[2] &= 0xfd;
    gtp_i2c_write(client, buf, 3);

    // step2: setup SensorID as pullup, shutdown SensorID pulldown
    buf[0] = 0x16;
    buf[1] = 0x06;
    gtp_i2c_read(client, buf, 4);
    buf[2] |= 0x02;
    buf[3] &= 0xfd;
    gtp_i2c_write(client, buf, 4);
    
    msleep(1);
    // step3: read 0x1602, result and 0x02, test equal 0, repeat 200 times
    count = 0;
    for (i = 0; i < 200; i++)
    {
        buf[0] = 0x16;
        buf[1] = 0x02;
        gtp_i2c_read(client, buf, 3);
        buf[2] &= 0x02;
        if (buf[2] == 0)
        {
            ++count;
        }
    }
    // if count greater than 100, then assign sensorid as 2
    if (count >= 100)
    {
        GTP_DEBUG("count = %d", count);
        sensor_id = 2;
        goto SENSOR_ID_NONC;
    }
    
    // step4: setup SensorID as pulldown, shutdown SensorID pullup
    buf[0] = 0x16;
    buf[1] = 0x06;
    gtp_i2c_read(client, buf, 4);
    buf[2] &= 0xfd;
    buf[3] |= 0x02;
    gtp_i2c_write(client, buf, 4);
    
    msleep(1);
    count = 0;
    // step 5: do the same as step 3
    for (i = 0; i < 200; ++i)
    {
        buf[0] = 0x16;
        buf[1] = 0x02;
        gtp_i2c_read(client, buf, 3);
        buf[2] &= 0x02;
        if (buf[2] != 0)
        {
            ++count;
        }	
    }
    if (count >= 100)
    {
        GTP_DEBUG("count = %d", count);
        sensor_id = 1;
        goto SENSOR_ID_NONC;
    }
    
    sensor_id = 0;
    goto SENSOR_ID_NC;
    
SENSOR_ID_NONC:
    buf[0] = 0x16;
    buf[1] = 0x06;
    gtp_i2c_read(client, buf, 4);
    buf[2] &= 0xfd;
    buf[3] &= 0xfd;
    gtp_i2c_write(client, buf, 4);

SENSOR_ID_NC:
    return sensor_id;
}

/*******************************************************
Function:
	GTP initialize function.

Input:
	ts:	i2c client private struct.
	
Output:
	Executive outcomes.0---succeed.
*******************************************************/
static s32 gtp_init_panel(struct goodix_ts_data *ts)
{
    s32 ret = -1;
  
#if GTP_DRIVER_SEND_CFG
    u8 rd_cfg_buf[16];

    u8 cfg_info_group1[] = CTP_CFG_GROUP1;
    u8 cfg_info_group2[] = CTP_CFG_GROUP2;
    u8 cfg_info_group3[] = CTP_CFG_GROUP3;
    u8 *send_cfg_buf[3] = {cfg_info_group1, cfg_info_group2, cfg_info_group3};
    u8 cfg_info_len[3] = {sizeof(cfg_info_group1)/sizeof(cfg_info_group1[0]), 
                          sizeof(cfg_info_group2)/sizeof(cfg_info_group2[0]),
                          sizeof(cfg_info_group3)/sizeof(cfg_info_group3[0])};
    GTP_DEBUG("len1=%d,len2=%d,len3=%d",cfg_info_len[0],cfg_info_len[1],cfg_info_len[2]);
    if ((!cfg_info_len[1]) && (!cfg_info_len[2]))
    {
        rd_cfg_buf[GTP_ADDR_LENGTH] = 0; 
    }
    else
    {
    #if GTP_USE_868_968M
        GTP_GPIO_OUTPUT(GTP_INT_PORT, 0);
        gtp_reset_guitar(20);
        rd_cfg_buf[GTP_ADDR_LENGTH] = gtp_get_sensor_id(ts->client);
        GTP_GPIO_OUTPUT(GTP_INT_PORT, 1);
        gtp_reset_guitar(20);
        GTP_GPIO_AS_INT(GTP_INT_PORT);
    #else
        rd_cfg_buf[0] = GTP_REG_SENSOR_ID >> 8;
        rd_cfg_buf[1] = GTP_REG_SENSOR_ID & 0xff;
        ret = gtp_i2c_read(ts->client, rd_cfg_buf, 3);
        gtp_i2c_end_cmd(ts->client);
        if (ret < 0)
        {
            GTP_ERROR("Read SENSOR ID failed,default use group1 config!");
            rd_cfg_buf[GTP_ADDR_LENGTH] = 0;
        }
        rd_cfg_buf[GTP_ADDR_LENGTH] &= 0x03;
    #endif
    }
    GTP_DEBUG("SENSOR ID:%d", rd_cfg_buf[GTP_ADDR_LENGTH]);
    if (rd_cfg_buf[GTP_ADDR_LENGTH] > 2)
    {
        GTP_ERROR("Invalid Sensor ID.");
        rd_cfg_buf[GTP_ADDR_LENGTH] = 0;
    }
    memcpy(&config[GTP_ADDR_LENGTH], send_cfg_buf[rd_cfg_buf[GTP_ADDR_LENGTH]], GTP_CONFIG_LENGTH);

	#if GTP_USE_868_968M
    ts->coor_div_2 = 0;
    if (config[GTP_ADDR_LENGTH + DRVCNT_LOC] == 0x0f)
    {
        ts->coor_div_2 = 1;
    }
    GTP_DEBUG("ts drv = 0x%x, coor_div_2 = %d", config[GTP_ADDR_LENGTH + DRVCNT_LOC], ts->coor_div_2);
	#endif

	#if (GTP_CUSTOM_CFG || GTP_USE_868_968M)
    config[RESOLUTION_LOC]     = (u8)(GTP_MAX_WIDTH);
    config[RESOLUTION_LOC + 1] = (u8)(GTP_MAX_WIDTH>>8);
    config[RESOLUTION_LOC + 2] = (u8)GTP_MAX_HEIGHT;
    config[RESOLUTION_LOC + 3] = (u8)(GTP_MAX_HEIGHT>>8);
    if (GTP_INT_TRIGGER == 0)  //FALLING
    {
        config[TRIGGER_LOC] &= 0xf7; 
    }
    else if (GTP_INT_TRIGGER == 1)  //RISING
    {
        config[TRIGGER_LOC] |= 0x08;
    }
	#endif  //endif GTP_CUSTOM_CFG

#else //else DRIVER NEED NOT SEND CONFIG

    ret = gtp_i2c_read(ts->client, config, GTP_CONFIG_LENGTH + GTP_ADDR_LENGTH);
    gtp_i2c_end_cmd(ts->client);
    if (ret < 0)
    {
        GTP_ERROR("GTP read resolution & max_touch_num failed, use default value!");
        ts->abs_x_max = GTP_MAX_WIDTH;
        ts->abs_y_max = GTP_MAX_HEIGHT;
        ts->int_trigger_type = GTP_INT_TRIGGER;
    }
#endif //endif GTP_DRIVER_SEND_CFG

    GTP_DEBUG_FUNC();

    if (!ts->coor_div_2)
    {
        ts->abs_x_max = (config[RESOLUTION_LOC + 1] << 8) + config[RESOLUTION_LOC];
        ts->abs_y_max = (config[RESOLUTION_LOC + 3] << 8) + config[RESOLUTION_LOC + 2];
    }
    else // enlarged height, assign touchscreen height the correct one
    {
        ts->abs_x_max = GTP_MAX_WIDTH;
        ts->abs_y_max = GTP_MAX_HEIGHT;
    }
    
    ts->int_trigger_type = (config[TRIGGER_LOC] >> 3) & 0x01;
    GTP_DEBUG("config_x = %d, config_y = %d",  (config[RESOLUTION_LOC + 1] << 8) + config[RESOLUTION_LOC], (config[RESOLUTION_LOC + 3] << 8) + config[RESOLUTION_LOC + 2]);
    GTP_DEBUG("abs_x_max = %d, abs_y_max = %d", ts->abs_x_max, ts->abs_y_max);   

    if ((!ts->abs_x_max) || (!ts->abs_y_max))
    {
        GTP_ERROR("GTP resolution & max_touch_num invalid, use default value!");
        ts->abs_x_max = GTP_MAX_WIDTH;
        ts->abs_y_max = GTP_MAX_HEIGHT;
    }

    ret = gtp_send_cfg(ts->client);
    if (ret < 0)
    {
        GTP_ERROR("Send config error. ret = %d", ret);
    }

    GTP_DEBUG("X_MAX = %d,Y_MAX = %d,TRIGGER = 0x%02x",ts->abs_x_max,ts->abs_y_max,ts->int_trigger_type);
    msleep(10);

    return 0;
}

/*******************************************************
Function:
	Read goodix touchscreen version function.

Input:
	client:	i2c client struct.
	version:address to store version info
	
Output:
	Executive outcomes.0---succeed.
*******************************************************/
static s32 gtp_read_version(struct i2c_client *client, u16* version)
{
    s32 ret = -1;
    u8 buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};
    struct goodix_ts_data* ts = i2c_get_clientdata(client);

    GTP_DEBUG_FUNC();

    ret = gtp_i2c_read(client, buf, 6);
    gtp_i2c_end_cmd(client);
    if (ret < 0)
    {
        GTP_ERROR("GTP read version failed"); 
        return ret;
    }

    if (version)
    {
        *version = (buf[5] << 8) | buf[4];
    }

    GTP_INFO("IC VERSION:%02x%02x_%02x%02x", buf[3], buf[2], buf[5], buf[4]);
    if(0x68 == buf[3])
    {
        ts->chip_type = GT868;
    }
    else
    {
        ts->chip_type = GT818X;
    }
	if ((0x68 != buf[3]) && (0x18 != buf[3]))
	{
		GTP_ERROR("GTP is not GT868 or GT818:0x%x\n", buf[3]);
		ret = -ENODEV;	// maybe GT8XX
	}

    return ret;
}

/*******************************************************
Function:
	I2c test Function.

Input:
	client:i2c client.
	
Output:
	Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gtp_i2c_test(struct i2c_client *client)
{
    u8 retry = 0;
    s8 ret = -1;
  
    GTP_DEBUG_FUNC();
  
    while(retry++ < 2)
    {
        ret = gtp_i2c_end_cmd(client);
        if (ret > 0)
        {
            return ret;
        }
        GTP_ERROR("GTP i2c test failed time %d.",retry);
        msleep(10);
    }
    return ret;
}

/*******************************************************
Function:
	Request gpio Function.

Input:
	ts:private data.
	
Output:
	Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gtp_request_io_port(struct goodix_ts_data *ts)
{
    s32 ret = 0;

    ret = GTP_GPIO_REQUEST(GTP_INT_PORT, "GTP_INT_IRQ");
    if (ret < 0) 
    {
        GTP_ERROR("Failed to request GPIO:%d, ERRNO:%d", (s32)GTP_INT_PORT, ret);
        ret = -ENODEV;
    }
    else
    {
        GTP_GPIO_AS_INT(GTP_INT_PORT);	
        ts->client->irq = GTP_INT_IRQ;
    }

    ret = GTP_GPIO_REQUEST(GTP_RST_PORT, "GTP_RST_PORT");
    if (ret < 0) 
    {
        GTP_ERROR("Failed to request GPIO:%d, ERRNO:%d",(s32)GTP_RST_PORT,ret);
        ret = -ENODEV;
    }

    GTP_GPIO_AS_INPUT(GTP_RST_PORT);
    gtp_reset_guitar(20);
    
    if(ret < 0)
    {
        GTP_GPIO_FREE(GTP_RST_PORT);
        GTP_GPIO_FREE(GTP_INT_PORT);
    }

    return ret;
}

/*******************************************************
Function:
	Request irq Function.

Input:
	ts:private data.
	
Output:
	Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gtp_request_irq(struct goodix_ts_data *ts)
{
    s32 ret = -1;
    const u8 irq_table[2] = GTP_IRQ_TAB;

    GTP_DEBUG("INT trigger type:%x", ts->int_trigger_type);

    ret  = request_irq(ts->client->irq, 
                       goodix_ts_irq_handler,
                       irq_table[ts->int_trigger_type],
                       ts->client->name,
                       ts);
    if (ret)
    {
        GTP_ERROR("Request IRQ failed!ERRNO:%d.", ret);
        GTP_GPIO_AS_INPUT(GTP_INT_PORT);
        //GTP_GPIO_FREE(GTP_INT_PORT);

        hrtimer_init(&ts->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        ts->timer.function = goodix_ts_timer_handler;
        hrtimer_start(&ts->timer, ktime_set(1, 0), HRTIMER_MODE_REL);
        return -1;
    }
    else 
    {
        gtp_irq_disable(ts);
        ts->use_irq = 1;
        return 0;
    }
}

/*******************************************************
Function:
	Request input device Function.

Input:
	ts:private data.
	
Output:
	Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gtp_request_input_dev(struct goodix_ts_data *ts)
{
    s8 ret = -1;
    s8 phys[32];
    u16 report_max_x = 0;
    u16 report_max_y = 0;
#if GTP_HAVE_TOUCH_KEY
    u8 index = 0;
#endif
  
    GTP_DEBUG_FUNC();
  
    ts->input_dev = input_allocate_device();
    if (ts->input_dev == NULL)
    {
        GTP_ERROR("Failed to allocate input device.");
        return -ENOMEM;
    }

    ts->input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;
    ts->input_dev->absbit[0] = BIT(ABS_X) | BIT(ABS_Y) | BIT(ABS_PRESSURE);
#if GTP_ICS_SLOT_REPORT
    __set_bit(INPUT_PROP_DIRECT, ts->input_dev->propbit);
    input_mt_init_slots(ts->input_dev, 255);
#else
    ts->input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
#endif

#if GTP_HAVE_TOUCH_KEY
    for (index = 0; index < GTP_MAX_KEY_NUM; index++)
    {
        input_set_capability(ts->input_dev,EV_KEY,touch_key_array[index]);	
    }
#endif

    report_max_x = ts->abs_x_max;
    report_max_y = ts->abs_y_max;
#if GTP_CHANGE_X2Y
    GTP_SWAP(report_max_x, report_max_y);
#endif

#if GTP_PROXIMITY
   input_set_abs_params(ts->input_dev, ABS_DISTANCE, 0, 1, 0, 0);
#endif
    input_set_abs_params(ts->input_dev, ABS_X, 0, report_max_x, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_Y, 0, report_max_y, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_PRESSURE, 0, 255, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_POSITION_X, 0, report_max_x, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_POSITION_Y, 0, report_max_y, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_TRACKING_ID, 0, 255, 0, 0);
#if GTP_ICS_SLOT_REPORT
    input_set_abs_params(ts->input_dev, ABS_MT_PRESSURE, 0, 255, 0, 0);	
#endif
    sprintf(phys, "input/ts");
    ts->input_dev->name = GTP_TS_NAME;
    ts->input_dev->phys = phys;
    ts->input_dev->id.bustype = BUS_I2C;
    ts->input_dev->id.vendor = 0xDEAD;
    ts->input_dev->id.product = 0xBEEF;
    ts->input_dev->id.version = 10427;
	
    ret = input_register_device(ts->input_dev);
    if (ret)
    {
        GTP_ERROR("Register %s input device failed", ts->input_dev->name);
        return -ENODEV;
    }

#ifdef CONFIG_HAS_EARLYSUSPEND
    ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
    ts->early_suspend.suspend = goodix_ts_early_suspend;
    ts->early_suspend.resume = goodix_ts_late_resume;
    register_early_suspend(&ts->early_suspend);
#endif

    return 0;
}
static void goodix_ts_pwron(void)
{
#if 0
	printk(KERN_INFO "==%s==\n", __func__);
	LDO_SetVoltLevel(LDO_LDO_SIM2, LDO_VOLT_LEVEL0);
	//	LDO_SetVoltLevel(LDO_LDO_SIM2, LDO_VOLT_LEVEL2); 				//modified by bernard
	LDO_TurnOnLDO(LDO_LDO_SIM2);
	msleep(50);
#endif
}

static int check_gt818_chip(void)
{
#if 1	// Alex.shi 本例中能走到这步已经说明是这款IC
	ctp_lock_mutex();
	tp_device_id(0x0818);
	ctp_unlock_mutex();
	return 1;
#else
	int	ret=-1;

	ctp_lock_mutex();
	ret=gtp_i2c_test(i2c_connect_client);
	if(ret>0)
	{
		tp_device_id(0x0818);
	}
	ctp_unlock_mutex();
	return ret;
#endif
}

static int remove_gt818_chip(void)
{
	ctp_lock_mutex();
	tp_device_id(0xFFFF);
	ctp_unlock_mutex();
	return 0;
}

/*******************************************************
Function:
	Goodix touchscreen probe function.

Input:
	client:	i2c device struct.
	id:device id.
	
Output:
	Executive outcomes. 0---succeed.
Comment:
*******************************************************/
static int goodix_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	s32						ret = -1;
	struct goodix_ts_data	*ts;
	u16						version_info;

	const char *vdd_name = "vddsim2";
		struct regulator *reg_vdd;
	printk("GT868:==goodix_ts_probe=\n");
	if(tp_device_id(0)!=0)
	{
		GTP_INFO("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) 
	{
		GTP_ERROR("I2C check functionality failed.");
		return -ENODEV;
	}
	client->addr = GTP_TS_ADDR;

	//do NOT remove these output log
	goodix_ts_pwron();



	reg_vdd = regulator_get(&client->dev, vdd_name);
	regulator_set_voltage(reg_vdd, 1800000, 1800000);
	regulator_enable(reg_vdd);

	
	GTP_INFO("GTP Driver Version:%s",GTP_DRIVER_VERSION);
	GTP_INFO("GTP Driver build@%s,%s", __TIME__,__DATE__);
	GTP_INFO("GTP I2C address:0x%02x", client->addr);

	i2c_connect_client = client;
	GTP_DEBUG("Before create_singlethread_workqueue");
	goodix_wq = create_singlethread_workqueue("goodix_wq");
	if (!goodix_wq)
	{
		GTP_ERROR("Creat workqueue failed.");
		return -ENOMEM;
	}

	GTP_DEBUG("Before kzalloc");
	ts = kzalloc(sizeof(*ts), GFP_KERNEL);
	if (ts == NULL)
	{
		GTP_ERROR("Alloc GFP_KERNEL memory failed.");
		ret = -ENOMEM;
		goto err_kzalloc;
	}

	memset(ts, 0, sizeof(*ts));
	INIT_WORK(&ts->work, goodix_ts_work_func);
	ts->client = client;
	i2c_set_clientdata(client, ts);
	spin_lock_init(&ts->irq_lock);

	GTP_DEBUG("Before gtp_request_io_port");
	ret = gtp_request_io_port(ts);
	if (ret < 0)
	{
		GTP_ERROR("GTP request IO port failed.");
		goto err_irq;
	}

	GTP_DEBUG("Before gtp_i2c_test");
	ret = gtp_i2c_test(client);
	if (ret < 0)
	{
		GTP_ERROR("I2C communication ERROR!");
		goto err_testi2c;
	}

#if GTP_AUTO_UPDATE
	GTP_DEBUG("Before gup_init_update_proc");
	ret = gup_init_update_proc(ts);
	if (ret < 0)
	{
		GTP_ERROR("Create update thread error.");
		goto err_testi2c;
	}
#endif

	GTP_DEBUG("Before gtp_init_panel");
	ret = gtp_init_panel(ts);
	if (ret < 0)
	{
		GTP_ERROR("GTP init panel failed.");
		goto err_testi2c;
	}

	if(check_gt818_chip()<0)
	{
		GTP_ERROR("Fail to find chip");
		goto err_testi2c;
	}
	GTP_DEBUG("After check_gt818_chip");

#if GTP_PROXIMITY	
	GTP_DEBUG("Before misc_register");
	ret = misc_register(&gtp_proximity_misc);
	if (ret < 0)
	{
		pr_err("%s: could not register misc device\n", __func__);
		goto err_mis_reg;
	}
#endif

	GTP_DEBUG("Before gtp_request_input_dev");
	ret = gtp_request_input_dev(ts);
	if (ret < 0)
	{
		GTP_ERROR("GTP request input dev failed");
		goto err_request_input;
	}

	GTP_DEBUG("Before gtp_request_irq");
	ret = gtp_request_irq(ts); 
	if (ret < 0)
	{
		GTP_INFO("GTP works in polling mode.");
	}
	else
	{
		GTP_INFO("GTP works in interrupt mode.");
	}

	GTP_DEBUG("Before gtp_read_version");
	ret = gtp_read_version(client, &version_info);
	if (ret < 0)
	{
		GTP_ERROR("Read version failed.");
		goto err_version;
	}
	GTP_INFO("Chip type:%s.", ts->chip_type==GT818X ? "GT818X" : "GT868");

#ifdef TOUCH_VIRTUAL_KEYS
	virtual_keys_init();
#endif
	gtp_irq_enable(ts);

#if GTP_CREATE_WR_NODE
	init_wr_node(client);
#endif

#if GTP_ESD_PROTECT
	INIT_DELAYED_WORK(&gtp_esd_check_work, gtp_esd_check_func);
	gtp_esd_check_workqueue = create_workqueue("gtp_esd_check");
	queue_delayed_work(gtp_esd_check_workqueue, &gtp_esd_check_work, GTP_ESD_CHECK_CIRCLE); 
#endif

	return 0;

err_version:
	if (ts) 
	{
		if (ts->use_irq)
		{
			GTP_GPIO_AS_INPUT(GTP_INT_PORT);
			//GTP_GPIO_FREE(GTP_INT_PORT);
			free_irq(client->irq, ts);
		}
		else
		{
			hrtimer_cancel(&ts->timer);
		}
	}
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ts->early_suspend);
#endif
	input_unregister_device(ts->input_dev);
	input_free_device(ts->input_dev);
err_request_input:
#if GTP_PROXIMITY	
	misc_deregister(&gtp_proximity_misc);
#endif
err_mis_reg:
	remove_gt818_chip();
err_testi2c:
	if(ts->client->irq>0)
	{
		//sprd_free_gpio_irq(ts->client->irq);
		ts->client->irq=-1;
		GTP_GPIO_FREE(GTP_RST_PORT);
		GTP_GPIO_FREE(GTP_INT_PORT);
	}
err_irq:
	i2c_set_clientdata(client, NULL);
	kfree(ts);
err_kzalloc:
	if (goodix_wq)
	{
		destroy_workqueue(goodix_wq);
	}
	return ret;
}

/*******************************************************
Function:
	Goodix touchscreen driver release function.

Input:
	client:	i2c device struct.
	
Output:
	Executive outcomes. 0---succeed.
*******************************************************/
static int goodix_ts_remove(struct i2c_client *client)
{
	struct goodix_ts_data *ts = i2c_get_clientdata(client);

	printk("==goodix_ts_remove=\n");

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ts->early_suspend);
#endif

#if GTP_CREATE_WR_NODE
	uninit_wr_node();
#endif

#if GTP_ESD_PROTECT
	destroy_workqueue(gtp_esd_check_workqueue);
#endif
#ifdef TOUCH_VIRTUAL_KEYS
	virtual_keys_destroy();
#endif

	if (ts) 
	{
		if (ts->use_irq)
		{
			GTP_GPIO_AS_INPUT(GTP_INT_PORT);
			//GTP_GPIO_FREE(GTP_INT_PORT);
			free_irq(client->irq, ts);
		}
		else
		{
			hrtimer_cancel(&ts->timer);
		}
	}

	GTP_INFO("GTP driver is removing...");
	input_unregister_device(ts->input_dev);
	input_free_device(ts->input_dev);
#if GTP_PROXIMITY	
	misc_deregister(&gtp_proximity_misc);
#endif
	remove_gt818_chip();
	if(ts->client->irq>0)
	{
		//sprd_free_gpio_irq(ts->client->irq);
		ts->client->irq=-1;
		GTP_GPIO_FREE(GTP_RST_PORT);
		GTP_GPIO_FREE(GTP_INT_PORT);
	}
	i2c_set_clientdata(client, NULL);
	kfree(ts);
	if (goodix_wq)
	{
		destroy_workqueue(goodix_wq);
	}

	return 0;
}

/*******************************************************
Function:
	Early suspend function.

Input:
	h:early_suspend struct.
	
Output:
	None.
*******************************************************/
static void goodix_ts_early_suspend(struct early_suspend *h)
{
	struct goodix_ts_data *ts;
	s8 ret = -1;	
	ts = container_of(h, struct goodix_ts_data, early_suspend);

	GTP_DEBUG_FUNC();

#if GTP_PROXIMITY
	if (gtp_proximity_start == 1)
		return;
#endif	
#if GTP_ESD_PROTECT
	ts->gtp_is_suspend = 1;
	cancel_delayed_work_sync(&gtp_esd_check_work);
#endif

	if (ts->use_irq)
	{
		gtp_irq_disable(ts);
	}
	else
	{
		hrtimer_cancel(&ts->timer);
	}
	ret = gtp_enter_sleep(ts);
	if (ret < 0)
	{
		GTP_ERROR("GTP early suspend failed.");
	}
}

/*******************************************************
Function:
	Late resume function.

Input:
	h:early_suspend struct.
	
Output:
	None.
*******************************************************/
static void goodix_ts_late_resume(struct early_suspend *h)
{
	struct goodix_ts_data *ts;
	s8 ret = -1;
	ts = container_of(h, struct goodix_ts_data, early_suspend);

	GTP_DEBUG_FUNC();

#if GTP_PROXIMITY
	if (gtp_proximity_start == 1)
		return;
#endif
	ret = gtp_wakeup_sleep(ts);
	if (ret < 0)
	{
		GTP_ERROR("GTP later resume failed.");
	}

	if (ts->use_irq)
	{
		gtp_irq_enable(ts);
	}
	else
	{
		hrtimer_start(&ts->timer, ktime_set(1, 0), HRTIMER_MODE_REL);
	}

#if GTP_ESD_PROTECT
	ts->gtp_is_suspend = 0;
	queue_delayed_work(gtp_esd_check_workqueue, &gtp_esd_check_work, GTP_ESD_CHECK_CIRCLE);
#endif
}

#if GTP_ESD_PROTECT
static void gtp_esd_check_func(struct work_struct *work)
{
	int i;
	int ret = -1;
	struct goodix_ts_data *ts = NULL;

	GTP_DEBUG_FUNC();

	ts = i2c_get_clientdata(i2c_connect_client);

	if (ts->gtp_is_suspend)
	{
		return;
	}

	for (i = 0; i < 3; i++)
	{
		ret = gtp_i2c_end_cmd(i2c_connect_client);
		if (ret >= 0)
		{
			break;
		}
	}

	if (i >= 3)
	{
		gtp_reset_guitar(50);
	}

	if(!ts->gtp_is_suspend)
	{
		queue_delayed_work(gtp_esd_check_workqueue, &gtp_esd_check_work, GTP_ESD_CHECK_CIRCLE);
	}

	return;
}
#endif

static const struct i2c_device_id goodix_ts_id[] = {
	{ GTP_I2C_NAME, 0 },
	{ }
};

static struct i2c_driver goodix_ts_driver = {
	.probe      = goodix_ts_probe,
	.remove     = goodix_ts_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend    = goodix_ts_early_suspend,
	.resume     = goodix_ts_late_resume,
#endif
	.id_table   = goodix_ts_id,
	.driver = {
		.name     = GTP_I2C_NAME,
		.owner    = THIS_MODULE,
	},
};

#if I2C_BOARD_INFO_METHOD
/*******************************************************	
Function:
	Driver Install function.
Input:
  None.
Output:
	Executive Outcomes. 0---succeed.
********************************************************/
static int __devinit goodix_ts_init(void)
{
	GTP_INFO("GT868 driver install.");

	if(tp_device_id(0)!=0)
	{
		printk("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}

	return i2c_add_driver(&goodix_ts_driver);;
}

/*******************************************************	
Function:
	Driver uninstall function.
Input:
  None.
Output:
	Executive Outcomes. 0---succeed.
********************************************************/
static void __exit goodix_ts_exit(void)
{
	GTP_INFO("GTP driver exited.");
	i2c_del_driver(&goodix_ts_driver);
}
#else
static int sprd_add_i2c_device(struct sprd_i2c_setup_data *i2c_set_data, struct i2c_driver *driver)
{
	struct i2c_board_info	info;
	struct i2c_adapter		*adapter;
	struct i2c_client		*client;
	int						ret,err;

	GTP_DEBUG("%s : i2c_bus=%d; slave_address=0x%x; i2c_name=%s",__func__,i2c_set_data->i2c_bus, \
		i2c_set_data->i2c_address, i2c_set_data->type);

	memset(&info, 0, sizeof(struct i2c_board_info));
	info.addr = i2c_set_data->i2c_address;
	strlcpy(info.type, i2c_set_data->type, I2C_NAME_SIZE);
	if(i2c_set_data->irq > 0)
		info.irq = i2c_set_data->irq;

	adapter = i2c_get_adapter( i2c_set_data->i2c_bus);
	if (!adapter) {
		printk("%s: can't get i2c adapter %d\n",
			__func__,  i2c_set_data->i2c_bus);
		err = -ENODEV;
		goto err_adapter;
	}

	client = i2c_new_device(adapter, &info);
	if (!client) {
		printk("%s:  can't add i2c device at 0x%x\n",
			__func__, (unsigned int)info.addr);
		err = -ENODEV;
		goto err_device;
	}

	i2c_put_adapter(adapter);

	ret = i2c_add_driver(driver);
	if (ret != 0) {
		printk("%s: can't add i2c driver\n", __func__);
		err = -ENODEV;
		goto err_driver;
	}	

	return 0;

err_driver:
	i2c_unregister_device(client);
	return err;
err_device:
	i2c_put_adapter(adapter);
err_adapter:
	return err;
}

static void sprd_del_i2c_device(struct i2c_client *client, struct i2c_driver *driver)
{
	GTP_DEBUG("%s : slave_address=0x%x; i2c_name=%s",__func__, client->addr, client->name);
	if (client!=NULL)
		i2c_unregister_device(client);
	i2c_del_driver(driver);
}

static int __init goodix_ts_init(void)
{
	GTP_INFO("GT868 driver install.");
	if(tp_device_id(0)!=0)
	{
		GTP_INFO("CTP(0x%x)Exist!", tp_device_id(0));
		return -ENODEV;
	}
	// Alex.shi 将GT系列的device信息移到board.c中还要考虑两者兼容等问题,麻烦了点,先放这里吧  

	goodix_ts_setup.i2c_bus =1;

	goodix_ts_setup.i2c_address = GTP_TS_ADDR;
	strcpy (goodix_ts_setup.type,GTP_I2C_NAME);
	return sprd_add_i2c_device(&goodix_ts_setup, &goodix_ts_driver);
}

static void __exit goodix_ts_exit(void)
{
	GTP_INFO("GTP driver exited.");
	sprd_del_i2c_device(i2c_connect_client, &goodix_ts_driver);	
}
#endif
late_initcall(goodix_ts_init);
module_exit(goodix_ts_exit);

MODULE_DESCRIPTION("GTP Series Driver");
MODULE_LICENSE("GPL");
