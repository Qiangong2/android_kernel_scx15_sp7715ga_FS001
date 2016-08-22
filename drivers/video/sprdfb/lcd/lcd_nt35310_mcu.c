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
#if (CONFIG_ZYT_PRJ == 7271) && (CONFIG_ZYT_CUST == 3)
#define LCD_35310_CFZZ_7271
#elif (CONFIG_ZYT_PRJ == 7675) && (CONFIG_ZYT_CUST == 75)
#define LCD_35310_GW_7675
#endif
static int32_t nt35310_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	printk("nt35310_init\n");

	send_cmd(0xED);  
	send_data(0x01); 
	send_data(0xFE); 
	
	send_cmd(0xEE);    
	send_data(0xDE);	
	send_data(0x21); 
	
	
	send_cmd(0xF3);    
	send_data(0x00);  
	
	send_cmd(0xB2);   
	send_data(0x90); 
	
	send_cmd(0x11);  
	
	LCD_DelayMS(120);
	
	send_cmd(0xc0); 
	send_data(0x4c); 
	send_data(0x4c); 
	send_data(0x10); 
	send_data(0x10); 
	
	send_cmd(0xC4);  
	send_data(0x78);   // 70
	
	send_cmd(0xBF);    
	send_data(0xAA); 
	
	send_cmd(0xB0); 	
	send_data(0x0D);	
	send_data(0x00);	 
	send_data(0x0D);	
	send_data(0x00);	 
	send_data(0x11);	
	send_data(0x00);	
	send_data(0x19);	
	send_data(0x00);	
	send_data(0x21);   
	send_data(0x00);	 
	send_data(0x2D);   
	send_data(0x00);	 
	send_data(0x3D);	
	send_data(0x00);	
	send_data(0x5D);	
	send_data(0x00);	 
	send_data(0x5D);   
	send_data(0x00); 
	
	send_cmd(0xB1); 	
	send_data(0x80);	
	send_data(0x00);	 
	send_data(0x8B);	
	send_data(0x00);	 
	send_data(0x96);	
	send_data(0x00); 
	
	send_cmd(0xB2); 	
	send_data(0x00);	
	send_data(0x00);	
	send_data(0x02);   
	send_data(0x00);	
	send_data(0x03);	
	send_data(0x00); 
	
	send_cmd(0xB3); 	
	send_data(0x00);	
	send_data(0x00);	
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);  
	send_data(0x00); 
	send_data(0x00);  
	send_data(0x00);  
	send_data(0x00); 
	send_data(0x00);  
	send_data(0x00);   
	send_data(0x00);	
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);  
	send_data(0x00);	
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);  
	send_data(0x00); 
	
	send_cmd(0xB4);    
	send_data(0x8B);	
	send_data(0x00);	
	send_data(0x96);   
	send_data(0x00);   
	send_data(0xA1);   
	send_data(0x00); 
	
	send_cmd(0xB5);    
	send_data(0x02);   
	send_data(0x00);	
	send_data(0x03);   
	send_data(0x00);   
	send_data(0x04);  
	send_data(0x00); 
	
	send_cmd(0xB6); 	
	send_data(0x00);   
	send_data(0x00); 
	
	send_cmd(0xB8);   
	send_data(0x00);  
	send_data(0x00);  
	send_data(0x00);  
	send_data(0x00);  
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00); 
	
	send_cmd(0xC2);   
	send_data(0x0A);   
	send_data(0x00);   
	send_data(0x04);   
	send_data(0x00);  
	
	send_cmd(0xC7);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00); 
	
	send_cmd(0xC9);   
	send_data(0x00);   
	send_data(0x00);  
	send_data(0x00);   
	send_data(0x00);	
	send_data(0x00);	
	send_data(0x00);   
	send_data(0x00);  
	send_data(0x00);  
	send_data(0x00);  
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00); 
	
	send_cmd(0xB7);   
	send_data(0x3F);   
	send_data(0x00);   
	send_data(0x5E);   
	send_data(0x00);   
	send_data(0x9E);   
	send_data(0x00);   
	send_data(0x74);   
	send_data(0x00);   
	send_data(0x8C);   
	send_data(0x00);   
	send_data(0xAC);   
	send_data(0x00);  
	send_data(0xDC);  
	send_data(0x00);  
	send_data(0x70);  
	send_data(0x00);  
	send_data(0xB9);  
	send_data(0x00);   
	send_data(0xEC);   
	send_data(0x00);  
	send_data(0xDC);  
	send_data(0x00);  
	
	send_cmd(0xE0);   
	send_data(0x01);   
	send_data(0x00);   
	send_data(0x05);   
	send_data(0x00);  
	send_data(0x0F);  
	send_data(0x00); 
	send_data(0x2F);  
	send_data(0x00); 
	send_data(0x45); 
	send_data(0x00); 
	send_data(0x54); 
	send_data(0x00);  
	send_data(0x67);   
	send_data(0x00);   
	send_data(0x7C);   
	send_data(0x00);  
	send_data(0x8C);   
	send_data(0x00);   
	send_data(0x99);   
	send_data(0x00);   
	send_data(0xA8);   
	send_data(0x00);   
	send_data(0xBA);   
	send_data(0x00);  
	send_data(0xC5);   
	send_data(0x00);   
	send_data(0xCB);   
	send_data(0x00);   
	send_data(0xD3);   
	send_data(0x00);   
	send_data(0xDA);   
	send_data(0x00);   
	send_data(0xE3);   
	send_data(0x00);  
	send_data(0xF3);  
	send_data(0x00); 
	
	send_cmd(0xE1);  
	send_data(0x00);  
	send_data(0x00);  
	send_data(0x04);  
	send_data(0x00);   
	send_data(0x0F);   
	send_data(0x00);   
	send_data(0x2F);  
	send_data(0x00);  
	send_data(0x45);   
	send_data(0x00);   
	send_data(0x54);   
	send_data(0x00);   
	send_data(0x66);   
	send_data(0x00);   
	send_data(0x7D);   
	send_data(0x00);   
	send_data(0x8B);  
	send_data(0x00);  
	send_data(0x99);  
	send_data(0x00);  
	send_data(0xA8);  
	send_data(0x00);  
	send_data(0xBA);  
	send_data(0x00);  
	send_data(0xC5);	
	send_data(0x00);	
	send_data(0xCB);	
	send_data(0x00);   
	send_data(0xD4);   
	send_data(0x00);  
	send_data(0xD9);  
	send_data(0x00);  
	send_data(0xE3);  
	send_data(0x00);  
	send_data(0xF3);   
	send_data(0x00); 
	
	send_cmd(0xE2);   
	send_data(0x01);   
	send_data(0x00);	
	send_data(0x05);	
	send_data(0x00);	
	send_data(0x0F);	
	send_data(0x00);   
	send_data(0x2F);   
	send_data(0x00);  
	send_data(0x45);  
	send_data(0x00);   
	send_data(0x54);   
	send_data(0x00);  
	send_data(0x67);   
	send_data(0x00);	
	send_data(0x7C);	
	send_data(0x00);	
	send_data(0x8C);   
	send_data(0x00);   
	send_data(0x99);   
	send_data(0x00);  
	send_data(0xA8); 
	send_data(0x00);  
	send_data(0xBA);  
	send_data(0x00); 
	send_data(0xC5);  
	send_data(0x00);  
	send_data(0xCB);  
	send_data(0x00);  
	send_data(0xD3);  
	send_data(0x00);  
	send_data(0xDA);   
	send_data(0x00);   
	send_data(0xE3);   
	send_data(0x00);  
	send_data(0xF3);   
	send_data(0x00); 
	
	send_cmd(0xE3);  
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x04);   
	send_data(0x00);   
	send_data(0x0F);  
	send_data(0x00);   
	send_data(0x2F);   
	send_data(0x00);  
	send_data(0x45);  
	send_data(0x00);  
	send_data(0x54);  
	send_data(0x00);  
	send_data(0x66);  
	send_data(0x00);  
	send_data(0x7D);   
	send_data(0x00);   
	send_data(0x8B);  
	send_data(0x00);  
	send_data(0x99);  
	send_data(0x00);  
	send_data(0xA8);  
	send_data(0x00);  
	send_data(0xBA);  
	send_data(0x00);  
	send_data(0xC5);  
	send_data(0x00);  
	send_data(0xCB);  
	send_data(0x00);  
	send_data(0xD4);   
	send_data(0x00);  
	send_data(0xD9);  
	send_data(0x00);  
	send_data(0xE3);   
	send_data(0x00);   
	send_data(0xF3);   
	send_data(0x00); 
	
	send_cmd(0xE4);   
	send_data(0x01);   
	send_data(0x00);   
	send_data(0x05);   
	send_data(0x00);   
	send_data(0x0F);   
	send_data(0x00);   
	send_data(0x2F);   
	send_data(0x00);   
	send_data(0x45);   
	send_data(0x00);  
	send_data(0x54); 
	send_data(0x00);  
	send_data(0x67);  
	send_data(0x00);  
	send_data(0x7C);  
	send_data(0x00);  
	send_data(0x8C); 
	send_data(0x00);   
	send_data(0x99);  
	send_data(0x00);   
	send_data(0xA8);   
	send_data(0x00);   
	send_data(0xBA);	
	send_data(0x00);	
	send_data(0xC5);	
	send_data(0x00);	
	send_data(0xCB);   
	send_data(0x00);  
	send_data(0xD3);   
	send_data(0x00);  
	send_data(0xDA); 
	send_data(0x00);  
	send_data(0xE3); 
	send_data(0x00); 
	send_data(0xF3);  
	send_data(0x00); 
	
	send_cmd(0xE5);   
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x04);   
	send_data(0x00);   
	send_data(0x0F);   
	send_data(0x00);  
	send_data(0x2F);  
	send_data(0x00);  
	send_data(0x45);  
	send_data(0x00);  
	send_data(0x54);  
	send_data(0x00);  
	send_data(0x66);   
	send_data(0x00);   
	send_data(0x7D);   
	send_data(0x00);   
	send_data(0x8B);   
	send_data(0x00);   
	send_data(0x99);  
	send_data(0x00);  
	send_data(0xA8);  
	send_data(0x00);  
	send_data(0xBA);  
	send_data(0x00);  
	send_data(0xC5);   
	send_data(0x00);   
	send_data(0xCB);   
	send_data(0x00);  
	send_data(0xD4);  
	send_data(0x00);	
	send_data(0xD9);	
	send_data(0x00);   
	send_data(0xE3);   
	send_data(0x00);   
	send_data(0xF3);   
	send_data(0x00); 
	
	send_cmd(0xE6);   
	send_data(0x21);   
	send_data(0x00);   
	send_data(0x55);   
	send_data(0x00);   
	send_data(0x99);   
	send_data(0x00);   
	send_data(0x77);   
	send_data(0x00);   
	send_data(0x77);   
	send_data(0x00);   
	send_data(0x76);   
	send_data(0x00);  
	send_data(0x78);   
	send_data(0x00); 
	send_data(0x98);  
	send_data(0x00);	
	send_data(0xBB);	
	send_data(0x00);	
	send_data(0x99);	
	send_data(0x00);	
	send_data(0x66);	
	send_data(0x00);   
	send_data(0x54);   
	send_data(0x00);   
	send_data(0x45);   
	send_data(0x00);  
	send_data(0x34);  
	send_data(0x00); 
	send_data(0x44); 
	send_data(0x00); 
	send_data(0x34);   
	send_data(0x00); 
	
	send_cmd(0xE7);    
	send_data(0x21);   
	send_data(0x00);   
	send_data(0x55);   
	send_data(0x00);   
	send_data(0x99);   
	send_data(0x00);   
	send_data(0x77);   
	send_data(0x00);   
	send_data(0x77);   
	send_data(0x00);	
	send_data(0x76);   
	send_data(0x00);   
	send_data(0x78);   
	send_data(0x00);   
	send_data(0x98);   
	send_data(0x00);   
	send_data(0xBB);   
	send_data(0x00);   
	send_data(0x99);   
	send_data(0x00);   
	send_data(0x66);   
	send_data(0x00);  
	send_data(0x54);  
	send_data(0x00);  
	send_data(0x45);  
	send_data(0x00);  
	send_data(0x34);  
	send_data(0x00);  
	send_data(0x44);   
	send_data(0x00);   
	send_data(0x34);  
	send_data(0x00); 
	
	send_cmd(0xE8);   
	send_data(0x21);  
	send_data(0x00);  
	send_data(0x55);   
	send_data(0x00);   
	send_data(0x99);   
	send_data(0x00);   
	send_data(0x77);   
	send_data(0x00);   
	send_data(0x77);   
	send_data(0x00);   
	send_data(0x76);  
	send_data(0x00);   
	send_data(0x78);   
	send_data(0x00);   
	send_data(0x98);  
	send_data(0x00);  
	send_data(0xBB);  
	send_data(0x00);   
	send_data(0x99);   
	send_data(0x00);   
	send_data(0x66);   
	send_data(0x00);   
	send_data(0x54);   
	send_data(0x00);   
	send_data(0x45);   
	send_data(0x00);   
	send_data(0x34);   
	send_data(0x00);   
	send_data(0x44);  
	send_data(0x00); 
	send_data(0x34);  
	send_data(0x00); 
	
	send_cmd(0xE9);   
	send_data(0xAA);  
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00); 
	
	
	send_cmd(0x00);   
	send_data(0xAA); 
	
	send_cmd(0xCF); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	
	send_cmd(0xF0); 
	send_data(0x00); 
	send_data(0x50); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	
	send_cmd(0xF1); 
	send_data(0x01); 
	
	
	send_cmd(0xF9); 
	send_data(0x06); 
	send_data(0x10); 
	send_data(0x29); 
	send_data(0x00); 
	
	send_cmd(0xDF);  
	send_data(0x10);  
	
	LCD_DelayMS(100);
	
	send_cmd(0x35);  
	send_data(0x00);  
	
	send_cmd(0x36); 
#if defined(LCD_35310_CFZZ_7271) || defined(LCD_35310_GW_7675)
	send_data(0x00); 
#else	
	send_data(0xC0); 
#endif	
	send_cmd(0x3A);  
	send_data(0x55);  
	
	send_cmd(0xB3);  
	send_data(0x00);  
	
	send_cmd(0xB4);   
	send_data(0x14);  //15
	
	send_cmd(0x51);   
	send_data(0xFF);  
	
	send_cmd(0x53); 
	send_data(0x2C); 
	
	send_cmd(0x55);  
	send_data(0x02); 
	
	send_cmd(0x11);
	
	LCD_DelayMS(120);
	
	send_cmd(0x29);
	 
	LCD_DelayMS(120);
	
	send_cmd(0x2C);

	return 0; 
}

static int32_t nt35310_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;

	if(is_sleep) 
	{	
		//Sleep In
		send_cmd(0x28);
		send_cmd(0x10);
		LCD_DelayMS(150);
	}
	else 
	{	
		//Sleep Out
	#ifndef LCD_ADJUST_PARAM
		send_cmd(0x29);
		LCD_DelayMS(60); 
		send_cmd(0x11);
		LCD_DelayMS(60); 
	#else
		self->ops->lcd_reset(self);
		self->ops->lcd_init(self);
	#endif
	}

	return 0;
}

static int32_t nt35310_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("nt35310_set_window\n");

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

static int32_t nt35310_invalidate(struct panel_spec *self)
{
	pr_debug("nt35310_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}

static int32_t nt35310_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	pr_debug("nt35310_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t nt35310_read_id(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	uint32_t  value=0,value1=0,value2=0,value3=0;
#if 0
	send_cmd(0xDB);
	value = read_data();
	LCD_PRINT("nt35310_read_id(0xDB):id=0x%x.\n", value);	 // 0x80

	if((0x80==value) || (0x81==value) || (0x82==value))
		return 0x5310;
	else
		return 0;
#else
	send_cmd(0xD4);
	value = read_data();  //¡°01¡± means Novatek
	value1 = read_data();
	value2 = read_data();  //Chip ID code. ¡°5310¡± means NT35310
	value3 = read_data();  //Chip version code
	pr_debug("nt35310_read_id(0xD4):value=0x%x.value1=0x%x.value2=0x%x.value3=0x%x.\n", value, value1, value2, value3);	 // 0x5310
	
	if((0x53 == value2) && (0x10 == value3))
	{
		return 0x5310;
	}
	else
	{
		return 0;
	}
#endif
}

#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t nt35310_esd_check(struct panel_spec *self)
{
	printk("\n nt35310_esd_check****************\n");

	return 1;	
}
#endif

static struct panel_operations lcd_nt35310_mcu_operations = {
	.panel_init = nt35310_init,
	.panel_set_window = nt35310_set_window,
	.panel_invalidate_rect= nt35310_invalidate_rect,
	.panel_invalidate = nt35310_invalidate,
	.panel_enter_sleep = nt35310_enter_sleep,
	.panel_readid          = nt35310_read_id,
	#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check       = nt35310_esd_check,
#endif
};

static struct timing_mcu lcd_nt35310_mcu_timing[] = {
[MCU_LCD_REGISTER_TIMING] = {                // read/write register timing
		.rcss = 15,
		.rlpw = 60,
		.rhpw = 60,
		.wcss = 10,
		.wlpw = 35,
		.whpw = 35,
	},
[MCU_LCD_GRAM_TIMING] = {                // read/write gram timing
		.rcss = 15,
		.rlpw = 60,
		.rhpw = 60,
		.wcss = 0,
		.wlpw = 16,
		.whpw = 16,
	},
};

static struct info_mcu lcd_nt35310_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing =lcd_nt35310_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_nt35310_mcu_spec = {
#ifdef CONFIG_LCD_HVGA
		.width = 320,
		.height = 480,
#elif defined (CONFIG_LCD_WVGA)
		.width = 480,
		.height = 800,
#else
		.width = 480,
		.height = 854,
#endif

   .is_clean_lcd = true,
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_nt35310_mcu_info},
	.ops = &lcd_nt35310_mcu_operations,
};

struct panel_cfg lcd_nt35310_mcu = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x5310,
	.lcd_name = "lcd_nt35310_mcu",
	.panel = &lcd_nt35310_mcu_spec,
};

static int __init lcd_nt35310_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_nt35310_mcu);
}

subsys_initcall(lcd_nt35310_mcu_init);


