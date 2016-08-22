/* drivers/video/sc8800g/sc8800g_lcd_ili9486.c
 *
 * Support for ili9486 LCD device
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

//#define LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif



static int32_t ili9486_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("ili9486_init\n");

	self->ops->panel_reset(self);
#if (CONFIG_ZYT_PRJ == 7001)
   //龙腾玻璃 财富一部
    send_cmd(0XF1);
	send_data(0x36);	
	send_data(0x04);	
	send_data(0x00);	
	send_data(0x3c);
	send_data(0x0F);	
	send_data(0x8F);	
	
	send_cmd(0XF2);
	send_data(0x18);	
	send_data(0xa3);	
	send_data(0x12);	
	send_data(0x02);
	send_data(0xb2);	
	send_data(0x12);	
	send_data(0xFF);
	send_data(0x10);
	send_data(0x00);
	
	send_cmd(0XF8);
	send_data(0x21);	
	send_data(0x04);	
	
	send_cmd(0XF9);
	send_data(0x00);	
	send_data(0x08);
	
	send_cmd(0xB7);
	send_data(0xC6);
	
	send_cmd(0XB1);
	send_data(0xB0);	
	send_data(0x11);	
	
	send_cmd(0xB4); //Display Inversion Control 			
	send_data(0x02);	//2-dot
	
	send_cmd(0xB6);// Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control 		#RGB
	send_data(0x22);				
	
	send_cmd(0xC0);
	send_data(0x0A);	//Verg1out =4.52
	send_data(0x0A);	//Vreg2out = -4.12
 
	send_cmd(0xC1);//VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5);
	send_data(0x00);	
	send_data(0x12);	
	send_data(0x80);		

	send_cmd(0xC2);			
	send_data(0x22);			

  send_cmd(0xE0);
	send_data(0x0F);	
	send_data(0x23);	
	send_data(0x20);	
	send_data(0x0D);	
	send_data(0x0F);	
	send_data(0x08);	
	send_data(0x48);	
	send_data(0x9b);	
	send_data(0x38);
	send_data(0x0C);
	send_data(0x14);
	send_data(0x05);
	send_data(0x0B);
	send_data(0x06);
	send_data(0x00);

	send_cmd(0XE1);
	send_data(0x0F);
	send_data(0x39);
	send_data(0x34);
	send_data(0x0A);
	send_data(0x0B);
	send_data(0x05);
	send_data(0x48);
	send_data(0x64);	
	send_data(0x35);	
	send_data(0x08);	
	send_data(0x10);	
	send_data(0x04);	
	send_data(0x23);	
	send_data(0x1E);	
	send_data(0x00);

	send_cmd(0x36);
	send_data(0x08);
	
	send_cmd(0x3A);//#Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35);//# TE on
	send_data(0x00);	

	send_cmd(0x11);
	LCD_DelayMS(120);	
	send_cmd(0x29);
  LCD_DelayMS(50);			
#else
	LCD_DelayMS(120);

	//************* Start Initial Sequence **********// 
	send_cmd(0XF1); 
	send_data(0x36);	
	send_data(0x04);	
	send_data(0x00);	
	send_data(0x3c);
	send_data(0x0F);	
	send_data(0x8F);	
	
	send_cmd(0XF2); 
	send_data(0x18);	
	send_data(0xa3);	
	send_data(0x12);	
	send_data(0x02);
	send_data(0xb2);	
	send_data(0x12);	
	send_data(0xFF);
	send_data(0x10);
	send_data(0x00);
	
	send_cmd(0XF8); 
	send_data(0x21);	
	send_data(0x04);	
	
	send_cmd(0XF9); 
	send_data(0x00);	
	send_data(0x08);
	
	send_cmd(0xB7); 
	send_data(0xC6);
	
	send_cmd(0XB1); 
	send_data(0xB0);	
	send_data(0x11);	
	
	send_cmd(0xB4); //Display Inversion Control 			
	send_data(0x02);	//2-dot
	
	send_cmd(0xB6); // Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control 		//RGB
	send_data(0x02);				
	
	send_cmd(0xC0); 
	send_data(0x0A);	//Verg1out =4.52
	send_data(0x0A);	//Vreg2out = -4.12
 
	send_cmd(0xC1); //VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5); 
	send_data(0x00);	
	send_data(0x0D);	
	send_data(0x80);		

	send_cmd(0xC2); 			
	send_data(0x22);			

  send_cmd(0xE0); 
	send_data(0x0F);	
	send_data(0x23);	
	send_data(0x20);	
	send_data(0x0D);	
	send_data(0x0F);	
	send_data(0x08);	
	send_data(0x48);	
	send_data(0x9b);	
	send_data(0x38);
	send_data(0x0C);
	send_data(0x14);
	send_data(0x05);
	send_data(0x0B);
	send_data(0x06);
	send_data(0x00);

	send_cmd(0XE1);
	send_data(0x0F);
	send_data(0x39);
	send_data(0x34);
	send_data(0x0A);
	send_data(0x0B);
	send_data(0x05);
	send_data(0x48);
	send_data(0x64);	
	send_data(0x35);	
	send_data(0x08);	
	send_data(0x10);	
	send_data(0x04);	
	send_data(0x23);	
	send_data(0x1E);	
	send_data(0x00);

	send_cmd(0x36); 
	send_data(0x48);	
	
	send_cmd(0x3A); //Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35); // TE on
	send_data(0x00);	

	send_cmd(0x11); 
	LCD_DelayMS(120);	
	send_cmd(0x29);
  LCD_DelayMS(50);	
#endif
	return 0;
}

static int32_t ili9486_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	LCD_PRINT("ili9486_enter_sleep\n");
	if ( is_sleep ){
		self->info.mcu->ops->send_cmd(0x0028);
		LCD_DelayMS(10);
		self->info.mcu->ops->send_cmd(0x0010);
		LCD_DelayMS(120);
	}
	else{
#ifndef LCD_ADJUST_PARAM
		self->info.mcu->ops->send_cmd(0x0011);
		LCD_DelayMS(120);
		self->info.mcu->ops->send_cmd(0x0029);
		LCD_DelayMS(50);
		self->info.mcu->ops->send_cmd(0x2C);
#else
		self->ops->lcd_reset(self);
		self->ops->lcd_init(self);
#endif

	}

	return 0;
}

static int32_t ili9486_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9486_set_window\n");

	self->info.mcu->ops->send_cmd(0x002A);
	self->info.mcu->ops->send_data((left >> 8));
	self->info.mcu->ops->send_data((left & 0xFF));
	self->info.mcu->ops->send_data((right >> 8));
	self->info.mcu->ops->send_data((right & 0xFF));

	self->info.mcu->ops->send_cmd(0x2B); //power setting 
	self->info.mcu->ops->send_data((top >> 8)); 
	self->info.mcu->ops->send_data((top & 0xFF)); 
	self->info.mcu->ops->send_data((bottom >> 8)); 
	self->info.mcu->ops->send_data((bottom & 0xFF)); 
	
	self->info.mcu->ops->send_cmd(0x002c);
	return 0;
}

static int32_t ili9486_invalidate(struct panel_spec *self)
{
	LCD_PRINT("ili9486_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0, 
			self->width-1, self->height-1);
}

static uint32_t ili9486_readid(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	uint32_t	lcm_dev_id = 0, data_read;
	uint32_t    value[6] ={0};

	send_cmd(0xD3);
	value[0] = read_data();  //param1:dummy	
	LCD_PRINT("ili9486_readid(0): 0x%x\n", value[0]);
	value[1] = read_data();  //param2:0
	LCD_PRINT("ili9486_readid(1): 0x%x\n", value[1]);

	value[2] = read_data(); //param3:94h
	LCD_PRINT("ili9486_readid(2): 0x%x\n", value[2]);
	
	value[3] = read_data(); //param4:88h
	LCD_PRINT("ili9486_readid(3): 0x%x\n", value[3]);

	if ((0x94 == value[2])&&(0x86 == value[3]))
	{
		lcm_dev_id = 0x9486;
	}
	LCD_PRINT("ili9486_readid: 0x%x\n", lcm_dev_id);   

	return lcm_dev_id;
}


static int32_t ili9486_set_direction(struct panel_spec *self, uint16_t direction)
{
	LCD_PRINT("ili9486_set_direction\n");

	switch (direction) {
	case LCD_DIRECT_NORMAL:
		self->info.mcu->ops->send_cmd_data(0x0036, 0x00);
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

static int32_t ili9486_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9486_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

	/*left 	= (left >= self->width)    ? (self->width-1) : left;
	right 	= (right >= self->width)   ? (self->width-1) : right;
	top 	= (top >= self->height)    ? (self->height-1) : top;
	bottom 	= (bottom >= self->height) ? (self->height-1) : bottom;

	
	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_OPERATE_FAIL;
	}
	*/

	return self->ops->panel_set_window(self, left, top, 
			right, bottom);
}

static struct panel_operations lcd_ili9486_mcu_operations = {
	.panel_init = ili9486_init,
	.panel_set_window = ili9486_set_window,
	.panel_invalidate_rect = ili9486_invalidate_rect,
	.panel_invalidate = ili9486_invalidate,
	.panel_set_direction = ili9486_set_direction,
	.panel_enter_sleep = ili9486_enter_sleep,
	.panel_readid = ili9486_readid,
};

static struct timing_mcu lcd_ili9486_mcu_timing[] = {
[MCU_LCD_REGISTER_TIMING] = {                // read/write register timing
		.rcss = 5,  // 15ns
		.rlpw = 150,
		.rhpw = 150,
		.wcss = 10,
		.wlpw = 50,
		.whpw = 50,
	},
[MCU_LCD_GRAM_TIMING] = {                // read/write gram timing
		.rcss = 5,  // 15ns
		.rlpw = 150,
		.rhpw = 150,
		.wcss = 10,
		.wlpw = 40,//50
		.whpw = 40,//50
	},
};

static struct info_mcu lcd_ili9486_mcu_info = {
	.bus_mode = LCD_BUS_8080,
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	.bus_width = 8,
#else
	.bus_width = 16,
#endif
	.bpp = 16,
	.timing = lcd_ili9486_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_ili9486_mcu_spec = {
	.width = 320,
	.height = 480,
    .is_clean_lcd = true,
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_ili9486_mcu_info},
	.ops = &lcd_ili9486_mcu_operations,
};

struct panel_cfg lcd_ili9486_mcu = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x9486,
	.lcd_name = "lcd_ili9486_mcu",
	.panel = &lcd_ili9486_mcu_spec,
};

static int __init lcd_ili9486_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_ili9486_mcu);
}

subsys_initcall(lcd_ili9486_mcu_init);


