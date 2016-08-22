/* drivers/video/sprdfb/lcd_st7796s.c
 *
 * Support for st7796s LCD device
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

//#define ST7796s_BUS_WIDTH_8BIT  // bus_width = 8

#if (defined(CONFIG_MACH_SP7715EA_FS001) || defined(CONFIG_MACH_SP7715GA_FS001) || defined(CONFIG_MACH_SP6815GA_FS010))
	#define CONFIG_ZYT_ST7796S_FS001_CFZZ_XC   // fs001 财富之舟 硒创屏
#elif (CONFIG_ZYT_PRJ==7007)
	#define CONFIG_ZYT_ST7796S_FS001_CFZZ_XC   // fs001 财富之舟 硒创屏
#elif defined(CONFIG_MACH_SP7715GA_FS003) || defined(CONFIG_MACH_SP7715EA_FS003_HVGA)
	#define CONFIG_ZYT_ST7796S_FS003_LHTC_TYF   // fs003 联合同创 天亿富 屏  
#elif (CONFIG_ZYT_PRJ==7675)
#if (CONFIG_ZYT_CUST == 8)
    #define CONFIG_ZYT_ST7796S_7675_JLD_XICHUANG
#elif (CONFIG_ZYT_CUST == 67)
    #define CONFIG_ZYT_ST7796S_7675_QY
#endif
#endif

static int32_t st7796s_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("st7796s_init\n");
	//Power setting sequence
	LCD_DelayMS(120);
	send_cmd(0x11);
	LCD_DelayMS(120);

#if defined(ST7796s_BUS_WIDTH_8BIT)
    send_cmd(0xDF);
    send_data(0x5A69);
    send_data(0x0201);

    send_cmd(0x36);
    send_data(0x0000);

    send_cmd(0x3a);
    send_data(0x0500);

    send_cmd(0x35);
    send_data(0x0000);

    send_cmd(0xd2);
    send_data(0x4c00);

    send_cmd(0xb2);
    send_data(0x0404);
    send_data(0x0033);
    send_data(0x3300);

    send_cmd(0xC6);
    send_data(0x0A00);

    send_cmd(0xb8);
    send_data(0x2A2B);
    send_data(0x0575);

    send_cmd(0xb7);
    send_data(0x7500);

    send_cmd(0xbb);
    send_data(0x1B00);

    send_cmd(0xC2);
    send_data(0x0100);

    send_cmd(0xC3);
    send_data(0x0b00);

    send_cmd(0xC4);
    send_data(0x2000);

    send_cmd(0xd0);
    send_data(0xa4a1);

    send_cmd(0xE8);
    send_data(0x8300);

    send_cmd(0xE9);
    send_data(0x0909);
    send_data(0x0500);

    send_cmd(0xC0);
    send_data(0x2500);

    send_cmd(0xb0);
    send_data(0x00d0);

    send_cmd(0xE0);
    send_data(0xd00E);
    send_data(0x1810);
    send_data(0x0E2A);
    send_data(0x3E54);
    send_data(0x4C2B);
    send_data(0x1716);
    send_data(0x1D21);

    send_cmd(0XE1);
    send_data(0xd00E);
    send_data(0x1710);
    send_data(0x0E29);
    send_data(0x3C44);
    send_data(0x4D2C);
    send_data(0x1615);
    send_data(0x1E22);

    send_cmd(0x29);
#else // 16/18/24bit
#if defined(CONFIG_ZYT_ST7796S_FS001_CFZZ_XC)
	send_cmd(0xf0);
	send_data(0xc3);
	send_cmd(0xf0);
	send_data(0x96);

	send_cmd(0x35);    // TE ON
	send_data(0x00);

	send_cmd(0x36);
	send_data(0x48);	//48
	
	send_cmd(0x3a);
	send_data(0x55);
	
	send_cmd(0xb4);
	send_data(0x01);
	send_cmd(0xe8);
	send_data(0x40);
	send_data(0x8a);
	send_data(0x00);
	send_data(0x00);
	send_data(0x29);
	send_data(0x19);
	send_data(0xa5);
	send_data(0x33);

	send_cmd(0xc1);
	send_data(0x0f);
	send_cmd(0xc2);
	send_data(0xa7);
	send_cmd(0xc5);
	send_data(0x27);
	send_cmd(0xe0);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x15);
	send_data(0x2f);
	send_data(0x54);
	send_data(0x42);
	send_data(0x3c);
	send_data(0x17);
	send_data(0x14);
	send_data(0x18);
	send_data(0x1b);

	send_cmd(0xe1);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x03);
	send_data(0x2d);
	send_data(0x43);
	send_data(0x42);
	send_data(0x3b);
	send_data(0x16);
	send_data(0x14);
	send_data(0x17);
	send_data(0x1b);

	send_cmd(0xf0);
	send_data(0x3c);
	send_cmd(0xf0);
	send_data(0x69);
	LCD_DelayMS(120);
	send_cmd(0x29);  //Display On
	LCD_DelayMS(25);

	send_cmd(0x2C);
#elif defined(CONFIG_ZYT_ST7796S_FS003_LHTC_TYF)

       //海菲ST7796+CTC3.5 20150515

	send_cmd(0xf0);
	send_data(0xc3);

	send_cmd(0xf0);
	send_data(0x96);

	send_cmd(0x3A);
	send_data(0x55);

	send_cmd(0x36);
	send_data(0x48);

	send_cmd(0x35);
	send_data(0x00);

	send_cmd(0x44);
	send_data(0x01);
	send_data(0x22);

	send_cmd(0xB4);
	send_data(0x01);

	send_cmd(0xB6);    
	send_data(0x00);    
	send_data(0x02); //0x22不旋转,//0x42旋转180度；  
	send_data(0x3B); 

	send_cmd(0xe8);
	send_data(0x40);
	send_data(0x8a);
	send_data(0x00);
	send_data(0x00);
	send_data(0x29);
	send_data(0x19);
	send_data(0xa5);
	send_data(0x33);

	send_cmd(0xc1);
	send_data(0x06);

	send_cmd(0xc2);
	send_data(0xa7);

	send_cmd(0xc5);
	send_data(0x24);//18

	send_cmd(0xe0);//PositiveVoltageGammaControl
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x15);
	send_data(0x2f);
	send_data(0x54);
	send_data(0x42);
	send_data(0x3c);
	send_data(0x17);
	send_data(0x14);
	send_data(0x18);
	send_data(0x1b);

	send_cmd(0xe1);//NegativeVoltageGammaControl
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x03);
	send_data(0x2d);
	send_data(0x43);
	send_data(0x42);
	send_data(0x3b);
	send_data(0x16);
	send_data(0x14);
	send_data(0x17);
	send_data(0x1b);

send_cmd(0xf0);
send_data(0x3c);

send_cmd(0xf0);
send_data(0x69);
LCD_DelayMS(120);//LCD_DelayMS120ms
send_cmd(0x29);//DisplayON
#elif defined(CONFIG_ZYT_ST7796S_7675_JLD_XICHUANG)
    send_cmd(0xf0); 
	send_data(0xc3);
	send_cmd(0xf0); 
	send_data(0x96);
	send_cmd(0x36); 
	send_data(0x48);
	send_cmd(0x3a); 
	send_data(0x55);
	send_cmd(0x35);    // TE ON
	send_data(0x00);
	send_cmd(0xb4); 
	send_data(0x01);
	send_cmd(0xe8); 
	send_data(0x40);
	send_data(0x8a);
	send_data(0x00);
	send_data(0x00);
	send_data(0x29);
	send_data(0x19);
	send_data(0xa5);
	send_data(0x33);

	send_cmd(0xc1); 
	send_data(0x0f);
	send_cmd(0xc2); 
	send_data(0xa7);
	send_cmd(0xc5); 
	send_data(0x27);
	send_cmd(0xe0);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x15);
	send_data(0x2f);
	send_data(0x54);
	send_data(0x42);
	send_data(0x3c);
	send_data(0x17);
	send_data(0x14);
	send_data(0x18);
	send_data(0x1b);

	send_cmd(0xe1);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x03);
	send_data(0x2d);
	send_data(0x43);
	send_data(0x42);
	send_data(0x3b);
	send_data(0x16);
	send_data(0x14);
	send_data(0x17);
	send_data(0x1b);

	send_cmd(0xf0); 
	send_data(0x3c);
	send_cmd(0xf0); 
	send_data(0x69);
	LCD_DelayMS(120);
	send_cmd(0x29); //Display On
#elif defined(CONFIG_ZYT_ST7796S_7675_QY)
    send_cmd(0xf0); 
	send_data(0xc3);
	send_cmd(0xf0); 
	send_data(0x96);
	send_cmd(0x36); 
	send_data(0x48);
	send_cmd(0x3a); 
	send_data(0x55);
	send_cmd(0x35);    // TE ON
	send_data(0x00);
	send_cmd(0xb4); 
	send_data(0x01);
	send_cmd(0xe8); 
	send_data(0x40);
	send_data(0x8a);
	send_data(0x00);
	send_data(0x00);
	send_data(0x29);
	send_data(0x19);
	send_data(0xa5);
	send_data(0x33);

	send_cmd(0xc1); 
	send_data(0x0f);
	send_cmd(0xc2); 
	send_data(0xa7);
	send_cmd(0xc5); 
	send_data(0x27);
	send_cmd(0xe0);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x15);
	send_data(0x2f);
	send_data(0x54);
	send_data(0x42);
	send_data(0x3c);
	send_data(0x17);
	send_data(0x14);
	send_data(0x18);
	send_data(0x1b);

	send_cmd(0xe1);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x03);
	send_data(0x2d);
	send_data(0x43);
	send_data(0x42);
	send_data(0x3b);
	send_data(0x16);
	send_data(0x14);
	send_data(0x17);
	send_data(0x1b);

	send_cmd(0xf0); 
	send_data(0x3c);
	send_cmd(0xf0); 
	send_data(0x69);
	LCD_DelayMS(120);
	send_cmd(0x29); //Display On
#else
	send_cmd(0xf0); 
	send_data(0xc3);
	send_cmd(0xf0); 
	send_data(0x96);
	send_cmd(0x36); 
	send_data(0x88);
	send_cmd(0x3a); 
	send_data(0x55);
	send_cmd(0x35);    // TE ON
	send_data(0x00);
	send_cmd(0xb4); 
	send_data(0x01);
	send_cmd(0xe8); 
	send_data(0x40);
	send_data(0x8a);
	send_data(0x00);
	send_data(0x00);
	send_data(0x29);
	send_data(0x19);
	send_data(0xa5);
	send_data(0x33);

	send_cmd(0xc1); 
	send_data(0x0f);
	send_cmd(0xc2); 
	send_data(0xa7);
	send_cmd(0xc5); 
	send_data(0x27);
	send_cmd(0xe0);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x15);
	send_data(0x2f);
	send_data(0x54);
	send_data(0x42);
	send_data(0x3c);
	send_data(0x17);
	send_data(0x14);
	send_data(0x18);
	send_data(0x1b);

	send_cmd(0xe1);
	send_data(0xf0);
	send_data(0x09);
	send_data(0x0b);
	send_data(0x06);
	send_data(0x04);
	send_data(0x03);
	send_data(0x2d);
	send_data(0x43);
	send_data(0x42);
	send_data(0x3b);
	send_data(0x16);
	send_data(0x14);
	send_data(0x17);
	send_data(0x1b);

	send_cmd(0xf0); 
	send_data(0x3c);
	send_cmd(0xf0); 
	send_data(0x69);
	LCD_DelayMS(120);
	send_cmd(0x29); //Display On
#endif
#endif
	return 0;
}


static int32_t st7796s_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("st7796s_set_window\n");

#if defined(ST7796s_BUS_WIDTH_8BIT)
	send_cmd(0x002A); // col
	send_data(left);
	send_data(right);

	send_cmd(0x002B); // row
	send_data(top);
	send_data(bottom);
	send_cmd(0x002C);
#else // 16/18/24bit
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
	send_cmd(0x2C);
#endif

	return 0;
}


static int32_t st7796s_invalidate(struct panel_spec *self)
{
	pr_debug("st7796s_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
			self->width-1, self->height-1);
}

static int32_t st7796s_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{

	pr_debug("st7796s_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t st7796s_set_direction(struct panel_spec *self, uint16_t direction)
{

	pr_debug("st7796s_set_direction\n");
	return 0;
}

static int32_t st7796s_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	//send_data_t send_cmd = self->info.mcu->ops->send_cmd;

	if(is_sleep) {
		//send_cmd(0x10);
		LCD_DelayMS(120);
	}
	else {
	
		LCD_DelayMS(120);
		
	}
	return 0;
}

static uint32_t st7796s_read_id(struct panel_spec *self)
{
	int32_t read_value = 0;
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	uint32_t    value[6] ={0};

	send_cmd(0xD3);

#if defined(ST7796s_BUS_WIDTH_8BIT)
	read_value += (read_data() &0xff)<<16;
	read_value += read_data();
#else
	value[0] = read_data();
	pr_debug("st7796s_read_id value[0] = 0x%x\n",value[0]);
	value[1] = read_data();
	pr_debug("st7796s_read_id value[1] = 0x%x\n",value[1]);
	value[2] = read_data();
	pr_debug("st7796s_read_id value[2] = 0x%x\n",value[2]);
	value[3] = read_data();
	pr_debug("st7796s_read_id value[3] = 0x%x\n",value[3]);
#endif
	if ((0x77 == value[2])&&(0x96 == value[3]))
	{
		read_value = 0x7796;
	}
	pr_debug("st7796s_read_id read_value = 0x%x\n",read_value);

	return read_value;
}

#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t st7796s_esd_check(struct panel_spec *self)
{
	printk("\n st7796s_esd_check****************\n");

	return 1;	
}
#endif

static struct panel_operations lcd_st7796s_operations = {
	.panel_init            = st7796s_init,
	.panel_set_window      = st7796s_set_window,
	.panel_invalidate      = st7796s_invalidate,
	//.panel_invalidate_rect = st7796s_invalidate_rect,
	.panel_set_direction   = st7796s_set_direction,
	.panel_enter_sleep     = st7796s_enter_sleep,
	.panel_readid         = st7796s_read_id,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check       = st7796s_esd_check,
#endif
};

static struct timing_mcu lcd_st7796s_timing[] = {
	[MCU_LCD_REGISTER_TIMING] = {
		.rcss = 45,
		.rlpw = 60,
		.rhpw = 100,
		.wcss = 10,
		.wlpw = 28,
		.whpw = 28,
	},
	[MCU_LCD_GRAM_TIMING] = {
		.rcss = 25,  /* 25 ns */
		.rlpw = 70,
		.rhpw = 70,
		.wcss = 10,
		.wlpw = 28,
		.whpw = 28,
	}
};


static struct info_mcu lcd_st7796s_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16, //16,
	.bpp = 16, /*RGB565*/
	.timing = &lcd_st7796s_timing,
	.ops = NULL,
};

struct panel_spec lcd_panel_st7796s = {
	.width = 320,
	.height = 480,
	.fps = 60,
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {
                 .mcu = &lcd_st7796s_info
                },
	.ops = &lcd_st7796s_operations,
};

struct panel_cfg lcd_st7796s = {
	/* this panel may on both CS0/1 */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x7796,
	.lcd_name = "lcd_st7796s",
	.panel = &lcd_panel_st7796s,
};

static int __init lcd_st7796s_init(void)
{
	return sprdfb_panel_register(&lcd_st7796s);
}

subsys_initcall(lcd_st7796s_init);
