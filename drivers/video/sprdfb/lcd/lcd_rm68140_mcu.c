/* drivers/video/sc8810/lcd_rm68140.c
 *
 * Support for rm68140 LCD device
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

#undef LCD_ADJUST_PARAM
#ifdef LCD_ADJUST_PARAM
extern  long  com1;
extern  long  com2;
#endif

#if defined(CONFIG_MACH_SP7715GA_7676)
	#define CONFIG_ZYT_RM68140_7676_TRX
#endif
#if defined(CONFIG_MACH_SP7715GA_7675_HVGA) || defined(CONFIG_MACH_SP7715EA_7675_HVGA)
	#define CONFIG_ZYT_RM68140_7675_CAIFU
#endif
static int32_t rm68140_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("rm68140_init\n");

	LCD_DelayMS(120); // Delay 100 ms
	//************* Start Initial Sequence **********// 
#ifdef CONFIG_ZYT_RM68140_7675_CAIFU
	send_cmd(0xc0); 	//set ver address	
	send_data(0x0D);
	send_data(0x0D);

	send_cmd(0xC1); 	//set ver address	
	send_data(0x45);
	send_data(0x07);

	send_cmd(0xC5); 	//set ver address	
	send_data(0x00);
	send_data(0x47);	   //55
	send_data(0x80);

	send_cmd(0xC2); 	//set ver address	
	send_data(0x33);


	send_cmd(0xB1); 	//set ver address	
	send_data(0xa0);
	send_data(0x11);

	send_cmd(0xB4);
	send_data(0x02);

	send_cmd(0xB6); 	//set ver address	
	send_data(0x00);
	send_data(0x22);//02
	send_data(0x3B);

	send_cmd(0xB7); 	//set ver address	
	send_data(0x07);

	send_cmd(0xF0); 	//set ver address	
	send_data(0x36);
	send_data(0xA5);
	send_data(0xD3);

	send_cmd(0xE5); 	//set ver address	
	send_data(0x80);

	send_cmd(0xF0); 	//set ver address	
	send_data(0x36);
	send_data(0xA5);
	send_data(0x53);

	send_cmd(0xE0); 	//set ver address	
	send_data(0x13);
	send_data(0x57);
	send_data(0x23);
	send_data(0x23);
	send_data(0x00);
	send_data(0x00);
	send_data(0x14);
	send_data(0x57);
	send_data(0x24);
	send_data(0x23);
	send_data(0x00);
	send_data(0x00);


	send_cmd(0x36); 	//set ver address	
	send_data(0x08);//


	send_cmd(0x3a); 	//set ver address	
	send_data(0x55);//

	send_cmd(0x35);		  // TE ON
	send_data(0x00); 
	
	send_cmd(0x11); 	//set ver address
	LCD_DelayMS(120);	
		
	send_cmd(0x29); 	//set ver address	
	LCD_DelayMS(100); 
	
	send_cmd(0X2A);
	send_data(0X00);
	send_data(0X00);
	send_data(0X01);
	send_data(0X3F);

	send_cmd(0X2B);
	send_data(0X00);
	send_data(0X00);
	send_data(0X01);
	send_data(0XDF);

	send_cmd(0x2C); 
#else
		send_cmd(0xc0); 	//set ver address	
	send_data(0x0D);
	send_data(0x0D);

	send_cmd(0xC1); 	//set ver address	
	send_data(0x45);
	send_data(0x07);

	send_cmd(0xC5); 	//set ver address	
	send_data(0x00);
	send_data(0x3a);	   //55
	send_data(0x80);

	send_cmd(0xC2); 	//set ver address	
	send_data(0x33);


	send_cmd(0xB1); 	//set ver address	
	send_data(0xb0);
	send_data(0x11);

	send_cmd(0xB4);
	send_data(0x02);

	send_cmd(0xB6); 	//set ver address	
	send_data(0x00);
	send_data(0x22);//02
	send_data(0x3B);

	send_cmd(0xB7); 	//set ver address	
	send_data(0x07);

	send_cmd(0xF0); 	//set ver address	
	send_data(0x36);
	send_data(0xA5);
	send_data(0xD3);

	send_cmd(0xE5); 	//set ver address	
	send_data(0x80);

	send_cmd(0xF0); 	//set ver address	
	send_data(0x36);
	send_data(0xA5);
	send_data(0x53);

	send_cmd(0xE0); 	//set ver address	
	send_data(0x13);
	send_data(0x57);
	send_data(0x23);
	send_data(0x23);
	send_data(0x00);
	send_data(0x00);
	send_data(0x14);
	send_data(0x57);
	send_data(0x24);
	send_data(0x23);
	send_data(0x00);
	send_data(0x00);

#if defined(CONFIG_ZYT_RM68140_7676_TRX)
	send_cmd(0x36); 	//set ver address	
	send_data(0x48);//
#else
	send_cmd(0x36); 	//set ver address	
	send_data(0x08);//
#endif

	send_cmd(0x3a); 	//set ver address	
	send_data(0x55);//

	send_cmd(0x35);		  // TE ON
	send_data(0x00); 
	
	send_cmd(0x11); 	//set ver address
	LCD_DelayMS(120);	
		
	send_cmd(0x29); 	//set ver address	
	LCD_DelayMS(100); 
	
	send_cmd(0X2A);
	send_data(0X00);
	send_data(0X00);
	send_data(0X01);
	send_data(0X3F);

	send_cmd(0X2B);
	send_data(0X00);
	send_data(0X00);
	send_data(0X01);
	send_data(0XDF);

	send_cmd(0x2C); 
#endif
	LCD_PRINT("rm68140_init_____end\n");
	return 0;
}

static int32_t rm68140_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

    /* set window size  */
	send_cmd(0x002A);
	send_data(left>>8);
	send_data(left&0xff);
	send_data(right>>8);
	send_data(right&0xff);

	send_cmd(0x002B);
	send_data(top>>8);
	send_data(top&0xff);
	send_data(bottom>>8);
	send_data(bottom&0xff);

	send_cmd(0x002c);
	return 0;
}

static int32_t rm68140_invalidate(struct panel_spec *self)
{

	return self->ops->panel_set_window(self, 0, 0, 
			self->width-1, self->height-1);

}

static int32_t rm68140_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{

	return self->ops->panel_set_window(self, left, top, 
			right, bottom);
}

static int32_t rm68140_set_direction(struct panel_spec *self, uint16_t direction)
{
	self->direction = direction;

	return 0;
}

static int32_t rm68140_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	if ( is_sleep ){
		self->info.mcu->ops->send_cmd(0x28);
        LCD_DelayMS(20);
		self->info.mcu->ops->send_cmd(0x10);
        LCD_DelayMS(150);
	}
	else
	{
		#ifndef LCD_ADJUST_PARAM
		self->info.mcu->ops->send_cmd(0x11);
		LCD_DelayMS(120);
		self->info.mcu->ops->send_cmd(0x29);
		LCD_DelayMS(20);
		#else
		self->ops->lcd_reset(self);
		self->ops->lcd_init(self);
		#endif
	}
	return 0;
}

static int32_t rm68140_read_id(struct panel_spec * self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	
	uint32_t id;
	uint32_t id3;
	uint32_t id4;
	
	send_cmd(0xbf);
	read_data();
	read_data();
	id3=read_data();
	id4=read_data();
	id=(id3<<8)+id4;
	return id;
}

#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t rm68140_esd_check(struct panel_spec *self)
{
	printk("\n rm68140_esd_check****************\n");

	return 1;	
}
#endif

static struct panel_operations lcd_rm68140_mcu_operations = {
	.panel_init            = rm68140_init,
	.panel_set_window      = rm68140_set_window,
	.panel_invalidate      = rm68140_invalidate,
	.panel_invalidate_rect = rm68140_invalidate_rect,
	.panel_set_direction   = rm68140_set_direction,
	.panel_enter_sleep     = rm68140_enter_sleep,
	.panel_readid          = rm68140_read_id,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check       = rm68140_esd_check,
#endif
};

static struct timing_mcu lcd_rm68140_mcu_timing[] = {
[MCU_LCD_REGISTER_TIMING] = {                // read/write register timing
		.rcss = 450,
		.rlpw = 170,
		.rhpw = 250,
		.wcss = 20,
#ifdef CONFIG_ZYT_RM68140_7675_CAIFU
		.wlpw = 50,
		.whpw = 50,
#else
		.wlpw = 50,
		.whpw = 50,
#endif
	},
[MCU_LCD_GRAM_TIMING] = {                // read/write gram timing
		.rcss = 450,
		.rlpw = 170,
		.rhpw = 250,
		.wcss = 20,//60
#ifdef CONFIG_ZYT_RM68140_7675_CAIFU
		.wlpw = 20,
		.whpw = 20,
#else
		.wlpw = 20,
		.whpw = 20,
#endif
	}
};

static struct info_mcu lcd_rm68140_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing = lcd_rm68140_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_rm68140_mcu_spec = {
	.width = 320,
	.height = 480,
   .is_clean_lcd = true,
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_rm68140_mcu_info},
	.ops = &lcd_rm68140_mcu_operations,
};

struct panel_cfg lcd_rm68140_mcu = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x6814,
	.lcd_name = "lcd_rm68140_mcu",
	.panel = &lcd_rm68140_mcu_spec,
};

static int __init lcd_rm68140_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_rm68140_mcu);
}

subsys_initcall(lcd_rm68140_mcu_init);


