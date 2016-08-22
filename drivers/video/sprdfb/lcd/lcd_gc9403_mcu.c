/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
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

static int32_t gc9403_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("gc9403_init\n");
	send_cmd(0xfe);
	send_cmd(0xef);
	send_cmd(0x3a);
	send_data(0x05); //16bit
	send_cmd(0x36);
	send_data(0x48);
	send_cmd(0x35);    // TE ON
	send_data(0x00);
	send_cmd(0xb4);
	send_data(0x00); //column inversion
	//---------------------------end display control setting--------------------//
	//--------------------------Power Control Registers Initial----------------//
	send_cmd(0xa8);
	send_data(0x04);
	send_cmd(0xa7);
	send_data(0x91);
	send_cmd(0xa4);
	send_data(0x14);
	send_cmd(0xa5);
	send_data(0x14);
	send_cmd(0xe5);
	send_data(0x02);
	send_cmd(0xed);
	send_data(0x33);
	send_data(0x33);
	send_cmd(0xE4);
	send_data(0x90);
	send_cmd(0xE2);
	send_data(0x00);


	send_cmd(0x2a);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x3F);
	send_cmd(0x2b);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0xDF);
	send_cmd(0x2c);
	//--------------------------end display window ------------------------------//
	LCD_DelayMS(10);
	//--------------------------------gamma setting--------------------------------//
	send_cmd(0xf0);
	send_data(0x0f);
	send_data(0x3f);
	send_data(0x3f);
	send_data(0x09);
	send_data(0x0a);

	send_data(0x05);
	send_data(0x3d);
	send_data(0xa9);
	send_data(0x2f);
	send_data(0x08);
	send_data(0x0f);
	send_data(0x01);
	send_data(0x20);
	send_data(0x1c);
	send_data(0x03);

	send_cmd(0xf1);
	send_data(0x0f);
	send_data(0x3f);
	send_data(0x3f);
	send_data(0x09);
	send_data(0x0a);
	send_data(0x05);
	send_data(0x3d);
	send_data(0xa9);
	send_data(0x2f);

	send_data(0x08);
	send_data(0x0f);
	send_data(0x01);
	send_data(0x20);
	send_data(0x1c);
	send_data(0x03);
	//-------------------------------end gamma setting---------------------------//
	send_cmd(0x11);
	LCD_DelayMS(120);
	send_cmd(0x29);
	send_cmd(0x2c);
	/*
	LCD_DelayMS(120);
	
	send_cmd(0x29);
	 
	LCD_DelayMS(120);
	
	send_cmd(0x2C);
*/
	return 0; 
}

static int32_t gc9403_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	if(is_sleep) 
	{	
    send_cmd(0xfe);
	send_cmd(0xef);

	send_cmd(0x28);
	LCD_DelayMS(120);
	send_cmd(0xe3); 
	send_data(0x43);
	send_cmd(0xe5); 
	send_data(0x05);
	send_cmd(0xe7); 
	send_data(0x60);

	LCD_DelayMS(20);
	send_cmd(0xec); 
	send_data(0x7c);
	LCD_DelayMS(20);
	send_cmd(0xec); 
	send_data(0x34);
	LCD_DelayMS(20);
	send_cmd(0xec); 
	send_data(0x04);
	LCD_DelayMS(20);

	send_cmd(0x10);
	}
	else 
	{	
		//Sleep Out
	#ifndef LCD_ADJUST_PARAM
    send_cmd(0xfe);
	send_cmd(0xef);

	send_cmd(0xe3); 
	send_data(0x03);
	send_cmd(0xe5); 
	send_data(0x01);
	send_cmd(0xe7); 
	send_data(0x00);

	LCD_DelayMS(20);
	send_cmd(0xec); 
	send_data(0x00);
	LCD_DelayMS(20);
	send_cmd(0x11);
	LCD_DelayMS(120);
	send_cmd(0x29);
	#else
		self->ops->lcd_reset(self);
		self->ops->lcd_init(self);
	#endif
	}

	return 0;
}

static int32_t gc9403_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("gc9403_set_window\n");

	send_cmd(0x2A); // col
	send_data((left >> 8));
	send_data((left & 0xFF));
	send_data((right >> 8));
	send_data((right & 0xFF));

	send_cmd(0x2B); // row
	send_data((top >> 8));
	send_data((top & 0xFF));
	send_data((bottom >> 8));
	send_data((bottom & 0xFF));

	send_cmd(0x2C); //Write data

	return 0;
}

static int32_t gc9403_invalidate(struct panel_spec *self)
{
	pr_debug("gc9403_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}

static int32_t gc9403_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	pr_debug("gc9403_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t gc9403_read_id(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	uint32_t  value=0,value1=0,value2=0,value3=0;

		return 0x9403;

}

#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t gc9403_esd_check(struct panel_spec *self)
{
	printk("\n gc9403_esd_check****************\n");

	return 1;	
}
#endif

static struct panel_operations lcd_gc9403_mcu_operations = {
	.panel_init = gc9403_init,
	.panel_set_window = gc9403_set_window,
	.panel_invalidate_rect= gc9403_invalidate_rect,
	.panel_invalidate = gc9403_invalidate,
	.panel_enter_sleep = gc9403_enter_sleep,
	.panel_readid          = gc9403_read_id,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check       = gc9403_esd_check,
#endif
};

static struct timing_mcu lcd_gc9403_mcu_timing[] = {
[0] = {                    // read/write register timing
		.rcss = 15,
		.rlpw = 60,
		.rhpw = 60,
		.wcss = 10,
		.wlpw = 35,
		.whpw = 35,
	},
[1] = {                    // read/write gram timing
		.rcss = 15,
		.rlpw = 60,
		.rhpw = 60,
		.wcss = 0,
		.wlpw = 16,
		.whpw = 16,
	},
};

static struct info_mcu lcd_gc9403_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing =lcd_gc9403_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_gc9403_mcu_spec = {
		.width = 320,
		.height = 480,
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_gc9403_mcu_info},
	.ops = &lcd_gc9403_mcu_operations,
};

struct panel_cfg lcd_gc9403_mcu = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x9403,
	.lcd_name = "lcd_gc9403_mcu",
	.panel = &lcd_gc9403_mcu_spec,
};

static int __init lcd_gc9403_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_gc9403_mcu);
}

subsys_initcall(lcd_gc9403_mcu_init);
