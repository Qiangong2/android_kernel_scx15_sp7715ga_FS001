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

#if(CONFIG_ZYT_PRJ == 7271) && (CONFIG_ZYT_CUST == 37)
#define R61581B_7271_YINGMAI
#endif

#if(CONFIG_ZYT_PRJ == 7271) && (CONFIG_ZYT_CUST == 67)
#define R61581B_7271_QIYANG
#endif

#if (CONFIG_ZYT_CUST==18)
#define R61581B_WJT_7675
#endif

#if(CONFIG_ZYT_PRJ == 7007) && (CONFIG_ZYT_CUST == 67)
#define R61581B_FS007_QIYANG
#endif
static int32_t r61581b_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	printk("r61581b_init\n");
#ifdef R61581B_7271_YINGMAI
	send_cmd( 0xFF);
	send_cmd( 0xFF);
	LCD_DelayMS( 5); 
	send_cmd( 0xFF);
	send_cmd( 0xFF);
	send_cmd( 0xFF);
	send_cmd( 0xFF);
	LCD_DelayMS( 10);

	send_cmd( 0x11); 
	LCD_DelayMS( 150);

	send_cmd( 0xB0);
	send_data( 0x00);

	send_cmd( 0xB3);
	send_data( 0x02);
	send_data( 0x00);
	send_data( 0x00);
	send_data( 0x00);

	send_cmd( 0xC0);
	send_data( 0x12);//17
	send_data( 0x3B);//480
	send_data( 0x00);
	send_data( 0x02);
	send_data( 0x00);
	send_data( 0x01);
	send_data( 0x00);//NW
	send_data( 0x43);

	send_cmd( 0xC1);
	send_data( 0x00);
	send_data( 0x10);//CLOCK
	send_data( 0x08);
	send_data( 0x08);

	send_cmd( 0xC4);
	send_data( 0x11);
	send_data( 0x01);
	send_data( 0x73);
	send_data( 0x03);

	send_cmd( 0xC6);
	send_data( 0x02);//risingedge

	send_cmd( 0xC8);//GAMMA
	send_data( 0x02);
	send_data( 0x02);
	send_data( 0x1B);
	send_data( 0x6C);
	send_data( 0x04);
	send_data( 0x08);
	send_data( 0x12);
	send_data( 0x0A);
	send_data( 0x00);
	send_data( 0x32);

	send_data( 0x0A);
	send_data( 0x12);
	send_data( 0x08);
	send_data( 0x64);
	send_data( 0x0C);
	send_data( 0x1B);
	send_data( 0x02);
	send_data( 0x02);
	send_data( 0x32);
	send_data( 0x00);

	send_cmd( 0xD0);
	send_data( 0x07);
	send_data( 0x07);
	send_data( 0x1D);
	send_data( 0x06);

	send_cmd( 0xD1);
	send_data( 0x03);
	send_data( 0x41);//VCM
	send_data( 0x10);//VDV

	send_cmd( 0xD2);
	send_data( 0x03);
	send_data( 0x24);
	send_data( 0x04);

	send_cmd( 0x35);
	send_data( 0x00);

	send_cmd( 0x44);
	send_data( 0x00);
	send_data( 0x01);

	send_cmd( 0x36);
	send_data( 0x40);

	send_cmd( 0x3A);
	send_data( 0x05);

	send_cmd( 0x29);
	LCD_DelayMS( 30);

	send_cmd( 0x2A);
	send_data( 0x00);
	send_data( 0x00);
	send_data( 0x01);
	send_data( 0x3F);//320

	send_cmd( 0x2B);
	send_data( 0x00); 
	send_data( 0x00);
	send_data( 0x01);
	send_data( 0xDF);//480

	send_cmd( 0x2C);
#elif defined(R61581B_WJT_7675)
// 7682 ÌìÈðÏé
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(5); 
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(10);
	
	send_cmd(0x11); 
	LCD_DelayMS(150);

	send_cmd(0xB0);
	send_data(0x00);

	send_cmd(0xB3);
	send_data(0x02);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);

	send_cmd(0xC0);
	send_data(0x12);//
	send_data(0x3B);//480
	send_data(0x00);
	send_data(0x01);
	send_data(0x00);
	send_data(0x01);
	send_data(0x00);//NW
	send_data(0x43);

	send_cmd(0xC1);
	send_data(0x08);
	send_data(0x11);//CLOCK
	send_data(0x08);
	send_data(0x08);

	send_cmd(0xC4);
	send_data(0x11);
	send_data(0x01);
	send_data(0x73);
	send_data(0x03);

	send_cmd(0xC6);
	send_data(0x02);//risingedge

	send_cmd(0xC8);//GAMMA
	send_data(0x01);
	send_data(0x01);
	send_data(0x1B);
	send_data(0x7d);
	send_data(0x05);
	send_data(0x09);
	send_data(0x11);
	send_data(0x09);
	send_data(0x00);
	send_data(0x32);

	send_data(0x09);
	send_data(0x11);
	send_data(0x09);
	send_data(0x75);
	send_data(0x0d);
	send_data(0x1B);
	send_data(0x01);
	send_data(0x01);
	send_data(0x32);
	send_data(0x00);

	send_cmd(0xD0);
	send_data(0x07);
	send_data(0x07);
	send_data(0x1D);
	send_data(0x06);

	send_cmd(0xD1);
	send_data(0x03);
	send_data(0x3c);//VCM
	send_data(0x10);//VDV

	send_cmd(0xD2);
	send_data(0x03);
	send_data(0x24);
	send_data(0x04);

	send_cmd(0x35);
	send_data(0x00);

	send_cmd(0x44);
	send_data(0x00);
	send_data(0x01);

	send_cmd(0x36);
	send_data(0x40);   //  0x00

	send_cmd(0x3A);
	send_data(0x05);

	send_cmd(0x29);
	LCD_DelayMS(30);

	send_cmd(0x2A);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x3F);//320

	send_cmd(0x2B);
	send_data(0x00); 
	send_data(0x00);
	send_data(0x01);
	send_data(0xDF);//480

	send_cmd(0x2C);

#elif defined(R61581B_7271_QIYANG)
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(5);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(10);

	send_cmd(0xB0);
	send_data(0x00);

	send_cmd(0xB3);
	send_data(0x02);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);

	send_cmd(0xC0);
	send_data(0x13);
	send_data(0x3B);//480
	send_data(0x00);
	send_data(0x02);
	send_data(0x00);
	send_data(0x01);
	send_data(0x00);//NW
	send_data(0x43);

	send_cmd(0xC1);
	send_data(0x08);
	send_data(0x12);//CLOCK
	send_data(0x08);
	send_data(0x08);

	send_cmd(0xC4);
	send_data(0x11);
	send_data(0x07);
	send_data(0x03);
	send_data(0x03);

	send_cmd(0xC6);//dataenable,VSYNC,HSYNC,PCKL??
	send_data(0x02);

	send_cmd(0xC8);//GAMMA
	send_data(0x03);
	send_data(0x03);
	send_data(0x13);
	send_data(0x5C);
	send_data(0x03);
	send_data(0x07);
	send_data(0x14);
	send_data(0x08);
	send_data(0x00);
	send_data(0x21);

	send_data(0x08);
	send_data(0x14);
	send_data(0x07);
	send_data(0x53);
	send_data(0x0C);
	send_data(0x13);
	send_data(0x03);
	send_data(0x03);
	send_data(0x21);
	send_data(0x00);

	send_cmd(0x35);
	send_data(0x00);

	send_cmd(0x36);
	send_data(0x00);

	send_cmd(0x3A);
	send_data(0x55);//24bit=77,18bit=66,16bit=55

	send_cmd(0x44);
	send_data(0x00);
	send_data(0x01);

	send_cmd(0xD0);
	send_data(0x07);
	send_data(0x07);//VCI1
	send_data(0x1D);//VRH
	send_data(0x02);//BT

	send_cmd(0xD1);
	send_data(0x03);
	send_data(0x1A);//VCM
	send_data(0x10);//VDV

	send_cmd(0xD2);
	send_data(0x03);
	send_data(0x14);
	send_data(0x04);

	send_cmd(0x29);
	LCD_DelayMS(30);

	send_cmd(0x2A);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x3F);//320

	send_cmd(0x2B);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0xDF);//480

	send_cmd(0xB4);
	send_data(0x00);//RGB???10,MCU?00
	LCD_DelayMS(100);

	send_cmd(0xB0);
	send_data(0x03);

	send_cmd(0x11);
	LCD_DelayMS(150);

	send_cmd(0x2C);
#elif defined(R61581B_FS007_QIYANG)
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(5);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(10);

	send_cmd(0xB0);
	send_data(0x00);

	send_cmd(0xB3);
	send_data(0x02);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);

	send_cmd(0xC0);
	send_data(0x13);
	send_data(0x3B);//480
	send_data(0x00);
	send_data(0x02);
	send_data(0x00);
	send_data(0x01);
	send_data(0x00);//NW
	send_data(0x43);

	send_cmd(0xC1);
	send_data(0x08);
	send_data(0x12);//CLOCK
	send_data(0x08);
	send_data(0x08);

	send_cmd(0xC4);
	send_data(0x11);
	send_data(0x07);
	send_data(0x03);
	send_data(0x03);

	send_cmd(0xC6);//dataenable,VSYNC,HSYNC,PCKL??
	send_data(0x02);

	send_cmd(0xC8);//GAMMA
	send_data(0x03);
	send_data(0x03);
	send_data(0x13);
	send_data(0x5C);
	send_data(0x03);
	send_data(0x07);
	send_data(0x14);
	send_data(0x08);
	send_data(0x00);
	send_data(0x21);

	send_data(0x08);
	send_data(0x14);
	send_data(0x07);
	send_data(0x53);
	send_data(0x0C);
	send_data(0x13);
	send_data(0x03);
	send_data(0x03);
	send_data(0x21);
	send_data(0x00);

	send_cmd(0x35);
	send_data(0x00);

	send_cmd(0x36);
	send_data(0x00);

	send_cmd(0x3A);
	send_data(0x55);//24bit=77,18bit=66,16bit=55

	send_cmd(0x44);
	send_data(0x00);
	send_data(0x01);

	send_cmd(0xD0);
	send_data(0x07);
	send_data(0x07);//VCI1
	send_data(0x1D);//VRH
	send_data(0x02);//BT

	send_cmd(0xD1);
	send_data(0x03);
	send_data(0x1A);//VCM
	send_data(0x10);//VDV

	send_cmd(0xD2);
	send_data(0x03);
	send_data(0x14);
	send_data(0x04);

	send_cmd(0x29);
	LCD_DelayMS(30);

	send_cmd(0x2A);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x3F);//320

	send_cmd(0x2B);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0xDF);//480

	send_cmd(0xB4);
	send_data(0x00);//RGB???10,MCU?00
	LCD_DelayMS(100);

	send_cmd(0xB0);
	send_data(0x03);

	send_cmd(0x11);
	LCD_DelayMS(150);

	send_cmd(0x2C);
#else
// 7682 ÌìÈðÏé
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(5); 
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	LCD_DelayMS(10);

	send_cmd(0x11); 
	LCD_DelayMS(150);

	send_cmd(0xB0);
	send_data(0x00);

	send_cmd(0xB3);
	send_data(0x02);
	send_data(0x01);
	send_data(0x00);
	send_data(0x00);

	send_cmd(0xC0);
	send_data(0x17);//0x13
	send_data(0x3B);//480
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x00);//NW
	send_data(0x43);

	send_cmd(0xC1);
	send_data(0x00);
	send_data(0x10);//CLOCK
	send_data(0x08);
	send_data(0x08);

	send_cmd(0xC4);
	send_data(0x11);
	send_data(0x01);
	send_data(0x73);
	send_data(0x03);

	send_cmd(0xC6);
	send_data(0x02);//risingedge

	send_cmd(0xC8);//GAMMA
	send_data(0x02);
	send_data(0x02);
	send_data(0x1B);
	send_data(0x6C);
	send_data(0x04);
	send_data(0x08);
	send_data(0x12);
	send_data(0x0A);
	send_data(0x00);
	send_data(0x32);

	send_data(0x0A);
	send_data(0x12);
	send_data(0x08);
	send_data(0x64);
	send_data(0x0C);
	send_data(0x1B);
	send_data(0x02);
	send_data(0x02);
	send_data(0x32);
	send_data(0x00);

	send_cmd(0xD0);
	send_data(0x07);
	send_data(0x07);
	send_data(0x1D);
	send_data(0x06);

	send_cmd(0xD1);
	send_data(0x03);
	send_data(0x48);//VCM
	send_data(0x11);//VDV

	send_cmd(0xD2);
	send_data(0x03);
	send_data(0x24);
	send_data(0x04);

	send_cmd(0x35);
	send_data(0x00);

	send_cmd(0x44);
	send_data(0x00);
	send_data(0x01);

	send_cmd(0x36);
	send_data(0x40);   //  0x00

	send_cmd(0x3A);
	send_data(0x05);

	send_cmd(0x29);
	LCD_DelayMS(30);

	send_cmd(0x2A);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x3F);//320

	send_cmd(0x2B);
	send_data(0x00); 
	send_data(0x00);
	send_data(0x01);
	send_data(0xDF);//480

	send_cmd(0x2C);

#endif
	

	return 0; 
}



static int32_t r61581b_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	if(is_sleep==1){
		//Sleep In
		send_cmd(0x28);
		LCD_DelayMS(10); 
		send_cmd(0x10);
		LCD_DelayMS(120); 
	}else{
		//Sleep Out
		r61581b_init(self);
	}

	return 0;
}




static int32_t r61581b_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("r61581b_set_window\n");

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
static int32_t r61581b_invalidate(struct panel_spec *self)
{
	pr_debug("r61581b_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t r61581b_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	pr_debug("r61581b_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t r61581b_read_id(struct panel_spec *self)
{
	int32_t 	id1  = 0, id2 = 0;
	int32_t 	id3  = 0, id4 = 0;
	int32_t	id;
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	send_data_t send_data = self->info.mcu->ops->send_data;

	send_cmd(0xB0);
    send_data(0x00);
    send_cmd(0xBF);
	
	id1 = read_data();
	pr_debug("r61581b_read_id id1:0x%x", id1);	// dummy
	id2 = read_data();
	pr_debug("r61581b_read_id id2:0x%x", id2);		// dummy
	id2 = read_data();
	pr_debug("r61581b_read_id id2-2:0x%x\n", id2);		// dummy

	id3 = read_data();
	pr_debug("r61581b_read_id id3:0x%x", id3);	// 0x15
	id4 = read_data();
	pr_debug("r61581b_read_id id4:0x%x", id4);		// 0x81

	id = id3<<8|id4;
	pr_debug("r61581b_read_id id:0x%x", id);		// 0x1581

	return id;
}

#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t r61581b_esd_check(struct panel_spec *self)
{
	printk("\n r61581b_esd_check****************\n");

	return 1;	
}
#endif

static struct panel_operations lcd_r61581b_mcu_operations = {
	.panel_init = r61581b_init,
	.panel_set_window = r61581b_set_window,
	.panel_invalidate_rect= r61581b_invalidate_rect,
	.panel_invalidate = r61581b_invalidate,
	.panel_enter_sleep = r61581b_enter_sleep,
	.panel_readid          = r61581b_read_id,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check       = r61581b_esd_check,
#endif
};

static struct timing_mcu lcd_r61581b_mcu_timing[] = {
[MCU_LCD_REGISTER_TIMING] = {                // read/write register timing
		.rcss = 50,
		.rlpw = 70,
		.rhpw = 70,
		
		.wcss = 15,//60,
		.wlpw = 20,//30,
		.whpw = 20,//30,
	},
[MCU_LCD_GRAM_TIMING] = {                // read/write gram timing
		.rcss = 50,
		.rlpw = 70,
		.rhpw = 70,

		.wcss = 15,//60,
		.wlpw = 20,//30,
		.whpw = 20,//30,
	},
};

static struct info_mcu lcd_r61581b_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing =lcd_r61581b_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_r61581b_mcu_spec = {
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
	.info = {.mcu = &lcd_r61581b_mcu_info},
	.ops = &lcd_r61581b_mcu_operations,
};

struct panel_cfg lcd_r61581b_mcu = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x1581,
	.lcd_name = "lcd_r61581b_mcu",
	.panel = &lcd_r61581b_mcu_spec,
};

static int __init lcd_r61581b_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_r61581b_mcu);
}

subsys_initcall(lcd_r61581b_mcu_init);


