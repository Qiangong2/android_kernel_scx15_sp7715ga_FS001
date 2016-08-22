/* drivers/video/sc8800g/sc8800g_lcd_ili9806x.c
 *
 * Support for ILI9806 LCD device
 *
 * Copyright (C) 2010 Spreadtrum
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include "../sprdfb_panel.h"

#undef  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif
// alex 注意:以下宏只在调试lcd效果时起作用,提交到服务器时一定不能打开
#undef LCD_ADJUST_PARAM

#ifdef LCD_ADJUST_PARAM
extern  long  com1;
extern  long  com2;
#endif


static int32_t ili9806x_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("ili9806x_init\n");
	send_cmd(0XFF);	//EXTC command Password
	send_data(0xFF);	
	send_data(0x98);	
	send_data(0x06);

	send_cmd(0XBA);
	send_data(0xE0);

	send_cmd(0xBC);      // GIP 1 
	send_data(0x03); 
	send_data(0x0F); 
	send_data(0x63); 
	send_data(0x33); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0x1B); 
	send_data(0x11); 
	send_data(0x38); 
	send_data(0x73); 
	send_data(0xFF); 
	send_data(0xFF); 
	send_data(0x0A); 
	send_data(0x0A); 
	send_data(0x05); 
	send_data(0x00); 
	send_data(0xFF); 
	send_data(0xE2); 
	send_data(0x01); 
	send_data(0x00); 
	send_data(0xC1); 


	send_cmd(0XBD);	
	send_data(0x01);	
	send_data(0x23);	
	send_data(0x45);
	send_data(0x67);	
	send_data(0x01);	
	send_data(0x23);
	send_data(0x45);	
	send_data(0x67);

	send_cmd(0XBE);	
	send_data(0x00);	
	send_data(0x22);	
	send_data(0x27);
	send_data(0x6A);	
	send_data(0xBC);	
	send_data(0xD8);
	send_data(0x92);	
	send_data(0x22);
	send_data(0x22);

	send_cmd(0XC7);	
	send_data(0x6a);

	send_cmd(0XED);	
	send_data(0x7F);
	send_data(0x0F);
	send_data(0x00);

	send_cmd(0XC0);	
	send_data(0x63);
	send_data(0x0B);
	send_data(0x02);

	send_cmd(0XFC);	
	send_data(0x08);

	send_cmd(0XDF);	
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x20);

	send_cmd(0XF3);	
	send_data(0x74);

	send_cmd(0XB4);	
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);

	send_cmd(0XF7);	
	send_data(0x82);

	send_cmd(0XB1);	
	send_data(0x00);
	send_data(0x12);
	send_data(0x15);

	send_cmd(0xF1);       // Panel Timing Control 
	send_data(0x29); 
	send_data(0x8A); 
	send_data(0x07); 

	send_cmd(0XF2);	
	send_data(0x40);
	send_data(0xd0);
	send_data(0x50);
	send_data(0x28);

	send_cmd(0XC1);	
	send_data(0x17);
	send_data(0x60);
	send_data(0x60);
	send_data(0x20);


	send_cmd(0XE0);	
	send_data(0x00);
	send_data(0x16);
	send_data(0x21);
	send_data(0x0F);
	send_data(0x10);
	send_data(0x15);
	send_data(0x07);
	send_data(0x06);
	send_data(0x05);
	send_data(0x09);
	send_data(0x07);
	send_data(0x0F);
	send_data(0x0F);
	send_data(0x35);
	send_data(0x31);
	send_data(0x00);

	send_cmd(0XE1);	
	send_data(0x00);
	send_data(0x16);
	send_data(0x19);
	send_data(0x0C);
	send_data(0x0F);
	send_data(0x10);
	send_data(0x06);
	send_data(0x07);
	send_data(0x04);
	send_data(0x08);
	send_data(0x08);
	send_data(0x0A);
	send_data(0x0B);
	send_data(0x24);
	send_data(0x20);
	send_data(0x00);

	send_cmd(0X36);	
	send_data(0x00);


	send_cmd(0X3A);	
	send_data(0x55);

	send_cmd(0X35);	
	send_data(0x00);

	send_cmd(0X11);	
	LCD_DelayMS(120);
	send_cmd(0X29);	

	return 0;
}

static int32_t ili9806x_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	LCD_PRINT("ili9806x_enter_sleep\n");

	if ( is_sleep ){
		self->info.mcu->ops->send_cmd(0x0028);
		mdelay(10);
		self->info.mcu->ops->send_cmd(0x0010);
		mdelay(120);
	}
	else{
#ifndef LCD_ADJUST_PARAM//modified by chenjf.
		//Sleep Out
		self->info.mcu->ops->send_cmd(0x0011);
		mdelay(120);
		self->info.mcu->ops->send_cmd(0x0029);
		self->info.mcu->ops->send_cmd(0x2C);
#else
		self->ops->reset(self);
		self->ops->lcd_init(self);
#endif
	}

	return 0;
}

static int32_t ili9806x_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9806x_set_window\n");

	self->info.mcu->ops->send_cmd(0x002A);
	self->info.mcu->ops->send_data((left>>8) & 0xff);
	self->info.mcu->ops->send_data(left&0xff);
	self->info.mcu->ops->send_data((right>>8) & 0xff);
	self->info.mcu->ops->send_data(right&0xff);

	self->info.mcu->ops->send_cmd(0x2B); //power setting 
	self->info.mcu->ops->send_data((top>>8) & 0xff); 
	self->info.mcu->ops->send_data(top&0xff); 
	self->info.mcu->ops->send_data((bottom>>8) & 0xff); 
	self->info.mcu->ops->send_data(bottom&0xff); 

	self->info.mcu->ops->send_cmd(0x002c);

	return 0;
}

static int32_t ili9806x_invalidate(struct panel_spec *self)
{
	LCD_PRINT("ili9806x_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0, 
			self->width-1, self->height-1);
}

static uint32_t ili9806x_readid(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	uint32_t	lcm_dev_id = 0, data_read;

	send_cmd(0xD3);
	data_read = read_data();  //param1:dummy	
	LCD_PRINT("ili9806x_readid(0): 0x%x\n", data_read);
	data_read = read_data();  //param2:0
	LCD_PRINT("ili9806x_readid(1): 0x%x\n", data_read);

	data_read = read_data(); //param3:94h
	LCD_PRINT("ili9806x_readid(2): 0x%x\n", data_read);
	lcm_dev_id = data_read & 0xFF;

	data_read = read_data(); //param4:86h
	LCD_PRINT("ili9806x_readid(3): 0x%x\n", data_read);
	lcm_dev_id = (lcm_dev_id << 8) | (data_read & 0xFF);

	LCD_PRINT("ili9806x_readid: 0x%x\n", lcm_dev_id);   

	return lcm_dev_id;
}

static uint32_t ili9806x_esd_check(struct panel_spec *self)
{
	uint32_t    x0a[5] ={0};
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;
	read_data_t read_data = self->info.mcu->ops->read_data;
	send_cmd(0x0a);
	x0a[0] = read_data();
	x0a[1] = read_data();
	x0a[2] = read_data();
   // printk("x0a[0]=0x%x,x0a[1]=0x%x,x0a[2]=0x%x", x0a[0], x0a[1],x0a[2]);

    if (x0a[1] != 0x9c)
    {
	//	printk("result=============1\n");
		return 0;
	}
	else
	{
	//	printk("result===================0\n");
		return 1;
	}
	return 0;	
}
static int32_t ili9806x_set_direction(struct panel_spec *self, uint16_t direction)
{
	LCD_PRINT("ili9806x_set_direction: direction = %d", direction);	 

	switch (direction) {
	case LCD_DIRECT_NORMAL:
		self->info.mcu->ops->send_cmd_data(0x0036, 0x09);
		break;
	case LCD_DIRECT_ROT_90:
		self->info.mcu->ops->send_cmd_data(0x0036, 0x00E8);
		break;
	case LCD_DIRECT_ROT_180:
	case LCD_DIRECT_MIR_HV:
		self->info.mcu->ops->send_cmd_data(0x0036, 0x0008);
		break;
	case LCD_DIRECT_ROT_270:
		self->info.mcu->ops->send_cmd_data(0x0036, 0x0028);
		break;
	case LCD_DIRECT_MIR_H:
		self->info.mcu->ops->send_cmd_data(0x0036, 0x0088);
		break;
	case LCD_DIRECT_MIR_V:
		self->info.mcu->ops->send_cmd_data(0x0036, 0x0048);
		break;
	default:
		LCD_PRINT("unknown lcd direction!\n");
		self->info.mcu->ops->send_cmd_data(0x0036, 0x00c8);
		direction = LCD_DIRECT_NORMAL;
		break;
	}

	self->direction = direction;

	return 0;
}

static int32_t ili9806x_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9806x_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

	left 	= (left >= self->width)    ? (self->width-1) : left;
	right 	= (right >= self->width)   ? (self->width-1) : right;
	top 	= (top >= self->height)    ? (self->height-1) : top;
	bottom 	= (bottom >= self->height) ? (self->height-1) : bottom;

	/*
	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_OPERATE_FAIL;
	}
	*/

	return self->ops->panel_set_window(self, left, top, 
			right, bottom);
}

static struct panel_operations lcd_ili9806x_mcu_operations = {
	.panel_init = ili9806x_init,
	.panel_set_window = ili9806x_set_window,
	.panel_invalidate_rect = ili9806x_invalidate_rect,
	.panel_invalidate = ili9806x_invalidate,
	.panel_set_direction = ili9806x_set_direction,
	.panel_enter_sleep = ili9806x_enter_sleep,
	.panel_readid = ili9806x_readid,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check = ili9806x_esd_check,
#endif
};

static struct timing_mcu lcd_ili9806x_mcu_timing[] = {
[MCU_LCD_REGISTER_TIMING] = {                // read/write register timing
		.rcss = 10,
		.rlpw = 50,
		.rhpw = 50,
		.wcss = 30,
		.wlpw = 70,
		.whpw = 70,
	},
[MCU_LCD_GRAM_TIMING] = {                // read/write gram timing
		.rcss = 10,
		.rlpw = 50,
		.rhpw = 50,
		.wcss = 10,//0
		.wlpw = 20,//15
		.whpw = 20,//24
	}
};

static struct info_mcu lcd_ili9806x_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing = lcd_ili9806x_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_ili9806x_mcu_spec = {
	.width = 480,
#if defined(CONFIG_LCD_FWVGA)
	.height = 854,
#else
	.height = 800,
#endif
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.is_clean_lcd = true,
	.info = {.mcu = &lcd_ili9806x_mcu_info},
	.ops = &lcd_ili9806x_mcu_operations,
};

struct panel_cfg lcd_ili9806x_mcu = {
	/* this panel may on both CS0/1 */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x9808,
	.lcd_name = "lcd_ili9806x",
	.panel = &lcd_ili9806x_mcu_spec,
};

static int __init lcd_ili9806x_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_ili9806x_mcu);
}

subsys_initcall(lcd_ili9806x_mcu_init);
