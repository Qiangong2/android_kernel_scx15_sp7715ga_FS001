/* drivers/video/sc8800g/sc8800g_lcd_ili9806h.c
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

#if (CONFIG_ZYT_PRJ == 5701) && (CONFIG_ZYT_CUST == 62)
	#define CONFIG_ZYT_ILI9806H_5701_XNKJ_YTQ   // 5701 小鸟科技 优泰奇屏
#elif (CONFIG_ZYT_PRJ == 7675)&& (CONFIG_ZYT_CUST == 67)
	#define CONFIG_ZYT_ILI9806H_7675_QIYANG_MINGZHE
#elif (CONFIG_ZYT_PRJ == 7271) && (CONFIG_ZYT_CUST == 67)
	#define CONFIG_ZYT_ILI9806H_7271_QIYANG_MINGZHE
#elif (CONFIG_ZYT_PRJ == 7005) && (CONFIG_ZYT_CUST == 70)
	#define CONFIG_ZYT_ILI9806H_FS005_JINGTAI
#endif

static int32_t ili9806h_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("ili9806h_init\n");

#if defined(CONFIG_ZYT_ILI9806H_5701_XNKJ_YTQ)
	LCD_DelayMS(120);
	//************* Start Initial Sequence **********// 

	send_cmd(0xFF); // EXTC Command Set enable register 
	send_data(0xFF); 
	send_data(0x98); 
	send_data(0x26); 

	send_cmd(0xBC); // GIP 1 
	send_data(0x21); 
	send_data(0x06); 
	send_data(0x20); 
	send_data(0x02); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0x80); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0xFF); 
	send_data(0xF0);
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0xC0); 
	send_data(0x08); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00);

	send_cmd(0xBD); // GIP 2 
	send_data(0x01); 
	send_data(0x23); 
	send_data(0x45); 
	send_data(0x67); 
	send_data(0x01); 
	send_data(0x23); 
	send_data(0x45); 
	send_data(0x67); 

	send_cmd(0xBE); // GIP 3 
	send_data(0x00); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x97); 
	send_data(0x86); 
	send_data(0xCA); 
	send_data(0xDB); 
	send_data(0xAC); 
	send_data(0xBD); 
	send_data(0x68); 
	send_data(0x79); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 

	send_cmd(0x3A);  
	send_data(0x55); 

	send_cmd(0xFA); 
	send_data(0x08); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x02); 
	send_data(0x08); 

	send_cmd(0xB1); //Frame rate 60.18Hz (TE on)
	send_data(0x00); 
	send_data(0x58);
	send_data(0x03);

	send_cmd(0xB4); //2 dot
	send_data(0x02); 
	send_data(0x02);
	send_data(0x02);

	send_cmd(0xC1); //AVDD=2xVCI=5.06, AVEE=-2xVCI=-4.892
	send_data(0x15); 
	send_data(0x78); 
	send_data(0x70);

	send_cmd(0xC7); //VCOM=-1.337	
	send_data(0x59); 

	send_cmd(0x35); //TE ON
	send_data(0x00); 

	send_cmd(0x36);
	send_data(0xC0);
	
	send_cmd(0x44); 
	send_data(0x00); 
	send_data(0x13); 

	send_cmd(0xF7); //480x800
	send_data(0x02); 

	send_cmd(0xF2); 
	send_data(0x05); 
	send_data(0x08); 
	send_data(0x08);
	send_data(0x8A); 
	send_data(0x07); 
	send_data(0x04);

	send_cmd(0XE0); // Positive Gamma Control 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x11); 
	send_data(0x12); 
	send_data(0x0D); 
	send_data(0x1F); 
	send_data(0XC9); 
	send_data(0x09); 
	send_data(0x02); 
	send_data(0x04); 
	send_data(0x03); 
	send_data(0x01); 
	send_data(0x0A); 
	send_data(0x36); 
	send_data(0x37); 
	send_data(0x00); 

	send_cmd(0XE1); // Negative Gamma Control 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x10); 
	send_data(0x10); 
	send_data(0x0A); 
	send_data(0x19); 
	send_data(0x77); 
	send_data(0x07); 
	send_data(0x04); 
	send_data(0x09); 
	send_data(0x06); 
	send_data(0x03); 
	send_data(0x0A); 
	send_data(0x1E); 
	send_data(0x1C); 
	send_data(0x00); 

	send_cmd(0x11);
	LCD_DelayMS(120);
	send_cmd(0x29); 
#elif defined(CONFIG_ZYT_ILI9806H_7675_QIYANG_MINGZHE)

	
send_cmd(0xFF); // EXTC Command Set enable register
send_data(0xFF);
send_data(0x98);
send_data(0x26);
send_cmd(0xDF);
send_data(0x06);
send_data(0x22);
send_data(0x20);
send_cmd(0xB6); //Bypass RAM
send_data(0x42);
send_cmd(0xBC); // GIP 1
send_data(0x21);
send_data(0x06);
send_data(0x20);
send_data(0x02);
send_data(0x01);
send_data(0x01);
send_data(0x98);
send_data(0x04);
send_data(0x05);
send_data(0x00);
send_data(0x00);
send_data(0x00);

send_data(0x01);
send_data(0x01);
send_data(0xFF);
send_data(0xF0);
send_data(0x00);
send_data(0x00);
send_data(0x00);
send_data(0xC0);
send_data(0x08);
send_data(0x00);
send_data(0x00);
send_data(0x00);
send_data(0x00);
send_data(0x00);
send_cmd(0xBD); // GIP 2
send_data(0x01);
send_data(0x23);
send_data(0x45);
send_data(0x67);
send_data(0x01);
send_data(0x23);
send_data(0x45);
send_data(0x67);
send_cmd(0xBE); // GIP 3
send_data(0x12);
send_data(0x22);
send_data(0x22);
send_data(0x22);
send_data(0x87);
send_data(0x96);
send_data(0xBA);
send_data(0xAB);
send_data(0xDC);
send_data(0xCD);
send_data(0x78);
send_data(0x69);
send_data(0x22);

send_data(0x22);
send_data(0x22);
send_data(0x22);
send_data(0x22);

send_cmd(0x3A); // Pixel Format
send_data(0x55);

send_cmd(0xFA); // P5_ bit 3 =1 Turn off VRGH
send_data(0x08);
send_data(0x00);
send_data(0x00);
send_data(0x02);
send_data(0x08);
send_cmd(0xB1); // FRAME RATE Setting
send_data(0x00);
send_data(0x58);
send_data(0x03);
send_cmd(0XB4); // Inversion Type
send_data(0x00);
send_cmd(0XC1); // Power Control 2
send_data(0x15);
send_data(0x78);
send_data(0x6A);
send_cmd(0xC7); // VCOM Control
send_data(0x51);
send_data(0x00);
send_data(0x65);
send_data(0x00);
send_cmd(0xED); // EN_volt_reg
send_data(0x7F);
send_data(0x0F);

send_cmd(0xF7); // Resolution Control
send_data(0x02);
send_cmd(0xE0);
send_data(0x00); //P1
send_data(0x06); //P2
send_data(0x15); //P3
send_data(0x11); //P4
send_data(0x12); //P5
send_data(0x1C); //P6
send_data(0xCA); //P7
send_data(0x08); //P8
send_data(0x02); //P9
send_data(0x08); //P10
send_data(0x02); //P11
send_data(0x0D); //P12
send_data(0x0B); //P13
send_data(0x36); //P14
send_data(0x31); //P15
send_data(0x00); //P16
send_cmd(0xE1);
send_data(0x00); //P1
send_data(0x05); //P2
send_data(0x0D); //P3
send_data(0x10); //P4
send_data(0x12); //P5
send_data(0x16); //P6
send_data(0x79); //P7
send_data(0x07); //P8
send_data(0x05); //P9
send_data(0x09); //P10
send_data(0x07); //P11
send_data(0x0C); //P12
send_data(0x0B); //P13
send_data(0x21); //P14
send_data(0x1B); //P15

send_data(0x00); //P16
	send_cmd(0x35); //Tearing Effect ON
	send_data(0x00);
	send_cmd(0x11); //Exit Sleep
	mdelay(120);
	send_cmd(0x29); // Display On
	mdelay(10);
#elif defined(CONFIG_ZYT_ILI9806H_7271_QIYANG_MINGZHE)
	send_cmd(0xFF); // EXTC Command Set enable register 
	send_data(0xFF); 
	send_data(0x98); 
	send_data(0x26); 

	send_cmd(0xBC); // GIP 1 
	send_data(0x21); 
	send_data(0x06); 
	send_data(0x20); 
	send_data(0x02); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0x80); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0xFF); 
	send_data(0xF0);
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0xC0); 
	send_data(0x08); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x00);

	send_cmd(0xBD); // GIP 2 
	send_data(0x01); 
	send_data(0x23); 
	send_data(0x45); 
	send_data(0x67); 
	send_data(0x01); 
	send_data(0x23); 
	send_data(0x45); 
	send_data(0x67); 

	send_cmd(0xBE); // GIP 3 
	send_data(0x00); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x97); 
	send_data(0x86); 
	send_data(0xCA); 
	send_data(0xDB); 
	send_data(0xAC); 
	send_data(0xBD); 
	send_data(0x68); 
	send_data(0x79); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 
	send_data(0x22); 

	send_cmd(0x3A);  
	send_data(0x55); 

	send_cmd(0xFA); 
	send_data(0x08); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x02); 
	send_data(0x08); 

	send_cmd(0xB1); //Frame rate 60.18Hz (TE on)
	send_data(0x00); 
	send_data(0x58);
	send_data(0x03);

	send_cmd(0xB4); //2 dot
	send_data(0x02); 
	send_data(0x02);
	send_data(0x02);

	send_cmd(0xC1); //AVDD=2xVCI=5.06, AVEE=-2xVCI=-4.892
	send_data(0x15); 
	send_data(0x78); 
	send_data(0x70);

	send_cmd(0xC7); //VCOM=-1.337	
	send_data(0x59); 

	send_cmd(0x35); //TE ON
	send_data(0x00); 

	send_cmd(0x44); 
	send_data(0x00); 
	send_data(0x13); 

	send_cmd(0xDF);  
	send_data(0x06); 
	send_data(0x22);  
	send_data(0x20); 
	 
	send_cmd(0xCE);  
	send_data(0xA0);  
	send_data(0x0E);  
	send_data(0x01); 
	send_data(0x02);

	send_cmd(0xF7); //480x800
	send_data(0x02); 

	send_cmd(0xF2); 
	send_data(0x05); 
	send_data(0x08); 
	send_data(0x08);
	send_data(0x8A); 
	send_data(0x07); 
	send_data(0x04);

	send_cmd(0XE0); // Positive Gamma Control 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x11); 
	send_data(0x12); 
	send_data(0x0D); 
	send_data(0x1F); 
	send_data(0XC9); 
	send_data(0x09); 
	send_data(0x02); 
	send_data(0x04); 
	send_data(0x03); 
	send_data(0x01); 
	send_data(0x0A); 
	send_data(0x36); 
	send_data(0x37); 
	send_data(0x00); 

	send_cmd(0XE1); // Negative Gamma Control 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x10); 
	send_data(0x10); 
	send_data(0x0A); 
	send_data(0x19); 
	send_data(0x77); 
	send_data(0x07); 
	send_data(0x04); 
	send_data(0x09); 
	send_data(0x06); 
	send_data(0x03); 
	send_data(0x0A); 
	send_data(0x1E); 
	send_data(0x1C); 
	send_data(0x00); 
	send_cmd(0x35); //Tearing Effect ON
	send_data(0x00);
	//send_cmd(0x40); //Tearing Effect ON
	//send_data(0x00);
	
	send_cmd(0x11); //Exit Sleep
	mdelay(120);
	send_cmd(0x29); // Display On
	mdelay(10);
#elif defined(CONFIG_ZYT_ILI9806H_FS005_JINGTAI)
	//************* Start Initial Sequence **********//
	send_cmd(0xFF); // EXTC Command Set enable register
	send_data(0xFF);
	send_data(0x98);
	send_data(0x26);

	send_cmd(0xB6); //Bypass RAM
	send_data(0x42);

	send_cmd(0xBC); // GIP 1
	send_data(0x21);
	send_data(0x06);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x01);
	send_data(0x80);
	send_data(0x02);
	send_data(0x05);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x01);
	send_data(0xF0);
	send_data(0xF4);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0xC0);
	send_data(0x08);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_cmd(0xBD); // GIP 2
	send_data(0x02);
	send_data(0x13);
	send_data(0x45);
	send_data(0x67);
	send_data(0x01);
	send_data(0x23);
	send_data(0x45);
	send_data(0x67);
	send_cmd(0xBE); // GIP 3
	send_data(0x13);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);
	send_data(0xBB);
	send_data(0xAA);
	send_data(0xDD);
	send_data(0xCC);
	send_data(0x66);
	send_data(0x77);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);

	send_cmd(0x3A); // Pixel Format
	send_data(0x55);

	send_cmd(0xFA); // P5_ bit 3 =1 Turn off VRGH
	send_data(0x08);
	send_data(0x00);
	send_data(0x00);
	send_data(0x02);
	send_data(0x08);
	send_cmd(0xB1); // FRAME RATE Setting
	send_data(0x00);
	send_data(0x58);
	send_data(0x03);
	send_cmd(0XB4); // Inversion Type
	send_data(0x02);
	send_cmd(0XC1); // Power Control 2
	send_data(0x10);
	send_data(0x78);
	send_data(0x6A);
	send_cmd(0xC7); // VCOM Control
	send_data(0x5D);
	send_data(0x00);
	send_cmd(0xED); // EN_volt_reg
	send_data(0x7F);
	send_data(0x0F);
	send_cmd(0xF2); // Engineering Setting
	send_data(0x02);
	send_data(0x07);
	send_data(0x03);
	send_data(0x92);
	send_data(0x07);
	send_data(0x04);
	send_cmd(0xF7); // Resolution Control
	send_data(0x02);

	send_cmd(0xE0);
	send_data(0x00); //P1
	send_data(0x05); //P2
	send_data(0x0E); //P3
	send_data(0x0E); //P4
	send_data(0x11); //P5
	send_data(0x19); //P6
	send_data(0xCB); //P7
	send_data(0x09); //P8
	send_data(0x02); //P9
	send_data(0x07); //P10
	send_data(0x04); //P11
	send_data(0x10); //P12
	send_data(0x0D); //P13
	send_data(0x32); //P14
	send_data(0x2C); //P15
	send_data(0x00); //P16

	send_cmd(0xE1);
	send_data(0x00); //P1
	send_data(0x04); //P2
	send_data(0x09); //P3
	send_data(0x0E); //P4
	send_data(0x10); //P5
	send_data(0x15); //P6
	send_data(0x79); //P7
	send_data(0x07); //P8
	send_data(0x05); //P9
	send_data(0x09); //P10
	send_data(0x06); //P11
	send_data(0x0E); //P12
	send_data(0x0C); //P13
	send_data(0x28); //P14
	send_data(0x22); //P15
	send_data(0x00); //P16

	send_cmd(0x35); //Tearing Effect ON
	send_data(0x00);

	send_cmd(0x11); //Exit Sleep
	LCD_DelayMS(150);
	send_cmd(0x29); // Display On
	LCD_DelayMS(10);
#else
	send_cmd(0xFF);
	send_data(0xFF);
	send_data(0x98);
	send_data(0x26);

	send_cmd(0xBC);
	send_data(0x3D);
	send_data(0x06);
	send_data(0x00);
	send_data(0x00);
	send_data(0x0D);
	send_data(0x0D);
	send_data(0x80);
	send_data(0x04);
	send_data(0x03);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x0D);
	send_data(0x0D);
	send_data(0x50);
	send_data(0xD0);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0xC0);
	send_data(0x08);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);

	send_cmd(0xBD);
	send_data(0x01);
	send_data(0x23);
	send_data(0x45);
	send_data(0x67);
	send_data(0x01);
	send_data(0x23);
	send_data(0x45);
	send_data(0x67);

	send_cmd(0xBE);
	send_data(0x13);
	send_data(0x22);
	send_data(0xDD);
	send_data(0xCC);
	send_data(0xBB);
	send_data(0xAA);
	send_data(0x22);
	send_data(0x26);
	send_data(0x72);
	send_data(0xFF);
	send_data(0x22);
	send_data(0xEE);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);
	send_data(0x22);

	send_cmd(0x3A);
	send_data(0x55);

	send_cmd(0xB1);
	send_data(0x00);
	send_data(0x58);
	send_data(0x03);

	send_cmd(0xB4);
	send_data(0x02);

	send_cmd(0xC0);
	send_data(0x0A);

	send_cmd(0xC1);
	send_data(0x10);
	send_data(0x78);
	send_data(0x78);

	send_cmd(0xC7);
	send_data(0x59);

	send_cmd(0xD7);
	send_data(0x10);
	send_data(0x84);
	send_data(0xB4);
	send_data(0x83);

	send_cmd(0xED);
	send_data(0x7F);
	send_data(0x0F);

	send_cmd(0xF7);
	send_data(0x02);

	send_cmd(0xF2);
	send_data(0x00);
	send_data(0x07);
	send_data(0x09);
	send_data(0x83);
	send_data(0x06);
	send_data(0x06);
		
	send_cmd(0xE0);
	send_data(0x00);
	send_data(0x07);
	send_data(0x0F);
	send_data(0x10);
	send_data(0x12);
	send_data(0x1B);
	send_data(0xC9);
	send_data(0x09);
	send_data(0x02);
	send_data(0x06);
	send_data(0x05);
	send_data(0x0D);
	send_data(0x0D);
	send_data(0x33);
	send_data(0x33);
	send_data(0x00);

	send_cmd(0xE1);
	send_data(0x00);
	send_data(0x05);
	send_data(0x0C);
	send_data(0x0E);
	send_data(0x10);
	send_data(0x17);
	send_data(0x78);
	send_data(0x08);
	send_data(0x04);
	send_data(0x07);
	send_data(0x07);
	send_data(0x0B);
	send_data(0x0E);
	send_data(0x2B);
	send_data(0x29);
	send_data(0x00);

	send_cmd(0x35);
	send_data(0x00);

	send_cmd(0x36);
	send_data(0xC0);
	
	send_cmd(0x11); //Exit Sleep
	mdelay(120);
	send_cmd(0x29); // Display On
#endif
	return 0;
}

static int32_t ili9806h_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	LCD_PRINT("ili9806h_enter_sleep\n");

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

static int32_t ili9806h_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9806h_set_window\n");

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

static int32_t ili9806h_invalidate(struct panel_spec *self)
{
	LCD_PRINT("ili9806h_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0, 
			self->width-1, self->height-1);
}

static uint32_t ili9806h_readid(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	uint32_t	lcm_dev_id = 0, data_read;

	send_cmd(0xD3);
	data_read = read_data();  //param1:dummy	
	LCD_PRINT("ili9806h_readid(0): 0x%x\n", data_read);
	data_read = read_data();  //param2:0
	LCD_PRINT("ili9806h_readid(1): 0x%x\n", data_read);

	data_read = read_data(); //param3:94h
	LCD_PRINT("ili9806h_readid(2): 0x%x\n", data_read);
	lcm_dev_id = data_read & 0xFF;

	data_read = read_data(); //param4:86h
	LCD_PRINT("ili9806h_readid(3): 0x%x\n", data_read);
	lcm_dev_id = (lcm_dev_id << 8) | (data_read & 0xFF);

	LCD_PRINT("ili9806h_readid: 0x%x\n", lcm_dev_id);   

	return lcm_dev_id;
}

static int32_t ili9806h_set_direction(struct panel_spec *self, uint16_t direction)
{
	LCD_PRINT("ili9806h_set_direction: direction = %d", direction);	 

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

static int32_t ili9806h_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9806h_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

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

static struct panel_operations lcd_ili9806h_mcu_operations = {
	.panel_init = ili9806h_init,
	.panel_set_window = ili9806h_set_window,
	.panel_invalidate_rect = ili9806h_invalidate_rect,
	.panel_invalidate = ili9806h_invalidate,
	.panel_set_direction = ili9806h_set_direction,
	.panel_enter_sleep = ili9806h_enter_sleep,
	.panel_readid = ili9806h_readid
};

static struct timing_mcu lcd_ili9806h_mcu_timing[] = {
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

static struct info_mcu lcd_ili9806h_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing = lcd_ili9806h_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_ili9806h_mcu_spec = {
	.width = 480,
#if defined(CONFIG_LCD_FWVGA)
	.height = 854,
#else
	.height = 800,
#endif
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.is_clean_lcd = true,
	.info = {.mcu = &lcd_ili9806h_mcu_info},
	.ops = &lcd_ili9806h_mcu_operations,
};

struct panel_cfg lcd_ili9806h_mcu = {
	/* this panel may on both CS0/1 */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x9826,
	.lcd_name = "lcd_ili9806h",
	.panel = &lcd_ili9806h_mcu_spec,
};

static int __init lcd_ili9806h_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_ili9806h_mcu);
}

subsys_initcall(lcd_ili9806h_mcu_init);
