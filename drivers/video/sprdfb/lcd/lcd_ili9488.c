/* drivers/video/sc8800g/sc8800g_lcd_ili9488.c
 *
 * Support for ILI9488 LCD device
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
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
#include <linux/io.h>
#include "lcdc_reg.h"
#include <mach/hardware.h>
#endif

//#define LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif

#if ((CONFIG_ZYT_PRJ==7675)/* && (CONFIG_ZYT_CUST==3)*/)
#if (CONFIG_ZYT_CUST==66)
//#define CONFIG_ZYT_ILI9488_7675_XYT_LIANGJINGJING   // 7675 鑫宇通 亮晶晶屏深超玻璃
#define CONFIG_ZYT_ILI9488_7675_XYT_SHENGDA   // 7675 鑫宇通 盛大屏翰彩玻璃
#elif (CONFIG_ZYT_CUST==37)
#define CONFIG_ZYT_ILI9488_7675_XYT_LIANGJINGJING
#elif (CONFIG_ZYT_CUST==68)
#define CONFIG_ZYT_ILI9488_7675_OT
#elif (CONFIG_ZYT_CUST==3)
#define ILI9488_7271_CFZZ
#elif (CONFIG_ZYT_CUST==67)
#ifdef CONFIG_MACH_SP7715EA_7675_HVGA
#define ILI9488_7271_QIYANG   //longteng glass
#else
#define CONFIG_ZYT_ILI9488_7675_QY_QIMEI  // 7675 启阳  IPS屏奇美玻璃
#endif
#elif (CONFIG_ZYT_CUST==8)
#define ILI9488_7675_JLD_SHENCHAO
#elif (CONFIG_ZYT_CUST==75)
#define ILI9488_7675_GW_LCD
#else
#define ILI9488_CAIFU
#endif
#elif ((CONFIG_ZYT_PRJ==7001) && (CONFIG_ZYT_CUST==63))
#define ILI9488_CAIFU
#elif defined(CONFIG_MACH_SP7715GA_FS003) || defined(CONFIG_MACH_SP7715EA_FS003_HVGA)
#define CONFIG_ZYT_ILI9488_FS003_LHTC_SHENGDA	// fs003 联合同创 盛大屏
#elif ((CONFIG_ZYT_PRJ==7676) && (CONFIG_ZYT_CUST==61))
#define CONFIG_ZYT_ILI9488_7676_TJ_TIANMA	// 7676 泰吉 天马玻璃
#elif (CONFIG_ZYT_PRJ==7271)
#if (CONFIG_ZYT_CUST==67)
#define ILI9488_7271_QIYANG
#elif (CONFIG_ZYT_CUST==3)
#define ILI9488_7271_CFZZ
#elif (CONFIG_ZYT_CUST==74)
#define CONFIG_ZYT_ILI9488_7271_YINGFEINIDI
#else
#define ILI9488_CAIFU
#endif
#elif (CONFIG_ZYT_PRJ==7007)
#define CONFIG_ZYT_ILI9488_FS007_QY_MINGZHE
#endif


static int32_t ili9488_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("ili9488_init\n");
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(4, LCM_CTRL);
#endif

	self->ops->panel_reset(self);

	LCD_DelayMS(120);

#if defined(CONFIG_ZYT_ILI9488_7675_XYT_LIANGJINGJING)
	//************* Start Initial Sequence **********// 
	send_cmd(0XF7); 
	send_data(0xA9);	
	send_data(0x51);	
	send_data(0x2C);	
	send_data(0x82);	
	//Power Control 1 
	send_cmd(0xC0); 
	send_data(0x11);	//Verg1out =4.52
	send_data(0x11);	//Vreg2out = -4.12
	//Power Control 2 
	send_cmd(0xC1); //VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5); 
	send_data(0x00);	
	send_data(0x17);	
	send_data(0x80);	

	send_cmd(0xB1); //Frame rate 
	send_data(0xB0);	//70Hz
	send_data(0x11);	

	send_cmd(0xB4); //Display Inversion Control 			
	send_data(0x02);	//2-dot 			

	send_cmd(0xB6); // Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control 		//RGB
	send_data(0x02);				


	send_cmd(0x55); 			
	send_data(0x00);			

	send_cmd(0xE9); 			
	send_data(0x01);			//00=18BIT,01=24BIT 

	send_cmd(0x36); 
	send_data(0x48);	
	
	send_cmd(0x3A); //Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35); // TE on
	send_data(0x00);
	
	send_cmd(0xE0); 
	send_data(0x00);	
	send_data(0x06);	
	send_data(0x06);	
	send_data(0x08);	
	send_data(0x18);	
	send_data(0x0c);	
	send_data(0x41);	
	send_data(0x9b);	
	send_data(0x4f);
	send_data(0x07);
	send_data(0x0e);
	send_data(0x0c);
	send_data(0x1c);
	send_data(0x1c);
	send_data(0x0F);

	send_cmd(0XE1);
	send_data(0x00);
	send_data(0x1b);
	send_data(0x1e);
	send_data(0x03);
	send_data(0x0e);
	send_data(0x04);
	send_data(0x35);
	send_data(0x24);	
	send_data(0x49);	
	send_data(0x04);	
	send_data(0x0f);	
	send_data(0x0e);	
	send_data(0x37);	
	send_data(0x3a);	
	send_data(0x0F);

	send_cmd(0x11); 
	LCD_DelayMS(120);	
	send_cmd(0x29);
#elif defined(CONFIG_ZYT_ILI9488_7675_XYT_SHENGDA)
	//************* Start Initial Sequence **********//
	send_cmd(0XF7);
	send_data(0xA9);
	send_data(0x51);
	send_data(0x2C);
	send_data(0x82);

	send_cmd(0xC0);
	send_data(0x11);
	send_data(0x09);

	send_cmd(0xC1);
	send_data(0x41);

	send_cmd(0XC5);
	send_data(0x00);
	send_data(0x0A);
	send_data(0x80);

	send_cmd(0xB1);
	send_data(0xB0);
	send_data(0x11);

	send_cmd(0xB4);
	send_data(0x02);

	send_cmd(0xB6);
	send_data(0x02);
	send_data(0x22);

	send_cmd(0xB7);
	send_data(0xc6);

	send_cmd(0xBE);
	send_data(0x00);
	send_data(0x04);

	send_cmd(0xE9);
	send_data(0x00);

	send_cmd(0x36);
	send_data(0x08);

	send_cmd(0x3A);
	send_data(0x55);

	send_cmd(0x35);	// TE on
	send_data(0x00);
	
	send_cmd(0xE0);
	send_data(0x00);
	send_data(0x07);
	send_data(0x10);
	send_data(0x09);
	send_data(0x17);
	send_data(0x0B);
	send_data(0x41);
	send_data(0x89);
	send_data(0x4B);
	send_data(0x0A);
	send_data(0x0C);
	send_data(0x0E);
	send_data(0x18);
	send_data(0x1B);
	send_data(0x0F);

	send_cmd(0XE1);
	send_data(0x00);
	send_data(0x17);
	send_data(0x1A);
	send_data(0x04);
	send_data(0x0E);
	send_data(0x06);
	send_data(0x2F);
	send_data(0x45);
	send_data(0x43);
	send_data(0x02);
	send_data(0x0A);
	send_data(0x09);
	send_data(0x32);
	send_data(0x36);
	send_data(0x0F);

	send_cmd(0x11);
	LCD_DelayMS(120);
	send_cmd(0x29);
#elif defined(CONFIG_ZYT_ILI9488_7676_TJ_TIANMA)
	//************* Start Initial Sequence **********//
	send_cmd(0xE0); //P-Gamma
	send_data(0x00);
	send_data(0x04);
	send_data(0x09);
	send_data(0x03);
	send_data(0x0E);
	send_data(0x05);
	send_data(0x2C);
	send_data(0x44);
	send_data(0x44);
	send_data(0x03);
	send_data(0x0A);
	send_data(0x08);
	send_data(0x2A);
	send_data(0x29);
	send_data(0x0F);

	send_cmd(0XE1); //N-Gamma
	send_data(0x00);
	send_data(0x03);
	send_data(0x00);
	send_data(0x00);
	send_data(0x0B);
	send_data(0x01);
	send_data(0x2B);
	send_data(0x24);
	send_data(0x46);
	send_data(0x06);
	send_data(0x0E);
	send_data(0x0C);
	send_data(0x2B);
	send_data(0x30);
	send_data(0x0F);

	send_cmd(0XC0); //Power Control 1
	send_data(0x13); //Vreg1out
	send_data(0x13); //Verg2out

	send_cmd(0xC1); //Power Control 2
	send_data(0x41); //VGH,VGL

	send_cmd(0xC5); //Power Control 3
	send_data(0x00);
	send_data(0x08); //Vcom
	send_data(0x80);

	send_cmd(0x36); //Memory Access
	send_data(0x88);

	send_cmd(0x3A); // Interface Pixel Format
	send_data(0x55);

	send_cmd(0x35);	// TE on
	send_data(0x00);
	
	send_cmd(0XB0); // Interface Mode Control
	send_data(0x00);

	send_cmd(0xB1); //Frame rate
	send_data(0xB0); //60Hz

	send_cmd(0xB4); //Display Inversion Control
	send_data(0x02); //2-dot

	send_cmd(0XB6); //RGB/MCU Interface Control
	send_data(0x02); //MCU
	send_data(0x02); //Source,Gate scan dieection

	send_cmd(0XE9); // Set Image Function
	send_data(0x00); // Disable 24 bit data input

	send_cmd(0xF7);
	send_data(0xA9);
	send_data(0x51);
	send_data(0x2C);
	send_data(0x82);

	send_cmd(0x21); 

	send_cmd(0x11); //Sleep out
	LCD_DelayMS(120);
	send_cmd(0x29); //Display on
#elif defined(CONFIG_ZYT_ILI9488_FS003_LHTC_SHENGDA)
	send_cmd(0xF7);
send_data(0xA9);
send_data(0x51);
send_data(0x2C);
send_data(0x82);

send_cmd(0xE9);
send_data(0x00);

send_cmd(0xC0);
send_data(0x0F);
send_data(0x0A);

send_cmd(0xC1);
send_data(0x41);

send_cmd(0xC5);
send_data(0x00);
send_data(0x50);   //flicker,如有轻微闪动，请调这个参数 0x4E 

send_data(0x80);

send_cmd(0xB1);
send_data(0xA0);
send_data(0x11);

send_cmd(0xB4);
send_data(0x02);

send_cmd(0xB6);
send_data(0x02);
send_data(0x22);

send_cmd(0x36);
send_data(0x08);

send_cmd(0x3A);
send_data(0x55);

send_cmd(0x35);	// TE on
send_data(0x00);

send_cmd(0xE0);
send_data(0x00);
send_data(0x05);
send_data(0x11);
send_data(0x09);
send_data(0x17);
send_data(0x0A);
send_data(0x40);
send_data(0x87);
send_data(0x4E);
send_data(0x08);
send_data(0x0F);
send_data(0x0B);
send_data(0x1C);
send_data(0x1F);
send_data(0x0F);

send_cmd(0xE1);
send_data(0x00);
send_data(0x1B);
send_data(0x1E);
send_data(0x04);
send_data(0x11);
send_data(0x06);
send_data(0x31);
send_data(0x25);
send_data(0x42);
send_data(0x02);
send_data(0x09);
send_data(0x08);
send_data(0x31);
send_data(0x37);
send_data(0x0F);

send_cmd(0x11);
LCD_DelayMS(120);
send_cmd(0x29);
#elif defined(CONFIG_ZYT_ILI9488_7675_QY_QIMEI)
	//************* Start Initial Sequence **********// 
	send_cmd(0xE0);      //P-Gamma 
	send_data(0x00); 
	send_data(0x10); 
	send_data(0x14); 
	send_data(0x01); 
	send_data(0x0E); 
	send_data(0x04); 
	send_data(0x33); 
	send_data(0x56); 
	send_data(0x48); 
	send_data(0x03); 
	send_data(0x0C); 
	send_data(0x0B); 
	send_data(0x2B); 
	send_data(0x34); 
	send_data(0x0F); 

	send_cmd(0XE1);      //N-Gamma 
	send_data(0x00); 
	send_data(0x12); 
	send_data(0x18); 
	send_data(0x05); 
	send_data(0x12); 
	send_data(0x06); 
	send_data(0x40); 
	send_data(0x34); 
	send_data(0x57); 
	send_data(0x06); 
	send_data(0x10); 
	send_data(0x0C); 
	send_data(0x3B); 
	send_data(0x3F); 
	send_data(0x0F); 

	send_cmd(0XC0);      //Power Control 1 
	send_data(0x0F);    //Vreg1out 
	send_data(0x0C);    //Verg2out 

	send_cmd(0xC1);      //Power Control 2 
	send_data(0x41);    //VGH,VGL 

	send_cmd(0xC5);      //Power Control 3 
	send_data(0x00); 
	send_data(0x21);    //Vcom 25
	send_data(0x80); 

	send_cmd(0x35);	    // TE on
	send_data(0x00);
	
	send_cmd(0x36);      //Memory Access 
	send_data(0x48); //48

	send_cmd(0x3A);      // Interface Pixel Format 
	send_data(0x55);    //18bit 

	send_cmd(0XB0);      // Interface Mode Control 
	send_data(0x00);     

	send_cmd(0xB1);      //Frame rate 
	send_data(0xA0);    //60Hz 

	send_cmd(0xB4);      //Display Inversion Control 
	send_data(0x02);    //2-dot 

	send_cmd(0XB6);      //RGB/MCU Interface Control 
	send_data(0x02);    //MCU 
	send_data(0x02);    //Source,Gate scan dieection 


	send_cmd(0XE9);      // Set Image Function   
	send_data(0x00);    //disable 24 bit data input 

	send_cmd(0xF7);     //   A d j u s t   C o n t r o l 
	send_data(0xA9);     
	send_data(0x51);     
	send_data(0x2C);     
	send_data(0x82);   //   D 7   s t r e a m ,   l o o s e  

	send_cmd(0x21);      //Normal Black 

	send_cmd(0x11);     //Sleep out 
	LCD_DelayMS(120); 
	send_cmd(0x29);    //Display on 
#elif defined(ILI9488_7271_QIYANG)
	//************* Start Initial Sequence **********//	
	send_cmd(0XF7);	
	send_data(0xA9);	
	send_data(0x51);	
	send_data(0x2C);	
	send_data(0x82);	
	//Power Control 1 
	send_cmd(0xC0);	
	send_data(0x10);	//11Verg1out =4.52
	send_data(0x12);	//11Vreg2out = -4.12
	//Power Control 2 
	send_cmd(0xC1);	//VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5);	
	send_data(0x00);	
	send_data(0x16);	//19
	send_data(0x80);	

	send_cmd(0xB1);	//Frame rate 
	send_data(0xB0);	//70Hz
	send_data(0x11);	

	send_cmd(0xB4);	//Display Inversion Control 			
	send_data(0x02);	//2-dot 			

	send_cmd(0xB6);	// Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control			//RGB
	send_data(0x02);				

	send_cmd(0x35);	// TE on
	send_data(0x00);

	send_cmd(0x55);				
	send_data(0x00);			

	send_cmd(0xE9);				
	send_data(0x00);				

	send_cmd(0x36);	
	send_data(0x48);	

	send_cmd(0x3A);	//Interface Pixel Format
	send_data(0x55);	

	send_cmd(0xE0);	
	send_data(0x00);	
	send_data(0x08);	
	send_data(0x0f);	
	send_data(0x09);	
	send_data(0x19);	
	send_data(0x0B);	
	send_data(0x44);	
	send_data(0x6c);	
	send_data(0x50);
	send_data(0x06);
	send_data(0x0b);
	send_data(0x07);
	send_data(0x1c);
	send_data(0x1c);
	send_data(0x0F);

	send_cmd(0XE1);
	send_data(0x00);
	send_data(0x1a);
	send_data(0x1d);
	send_data(0x01);
	send_data(0x0d);
	send_data(0x04);
	send_data(0x33);
	send_data(0x33);	
	send_data(0x46);	
	send_data(0x01);	
	send_data(0x0a);	
	send_data(0x09);	
	send_data(0x34);	
	send_data(0x39);	
	send_data(0x0F);

	send_cmd(0x11);	
	LCD_DelayMS(120);	
	send_cmd(0x29);	
#elif defined(CONFIG_ZYT_ILI9488_7675_OT)
//YTC_OUTUO 20140911
	send_cmd(0XF7);	
	send_data(0xA9);	
	send_data(0x51);	
	send_data(0x2C);	
	send_data(0x82);	
	//Power Control 1 
	send_cmd(0xC0);
	send_data(0x17);	//Verg1out =4.52
	send_data(0x15);	//Vreg2out = -4.12
	//Power Control 2 
	send_cmd(0xC1);	//VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5);	
	send_data(0x00);	
	send_data(0x12);	
	send_data(0x80);	

	send_cmd(0xB1);	//Frame rate 
	send_data(0xA0);	//70Hz
	send_data(0x11);	

	send_cmd(0xB4);	//Display Inversion Control 			
	send_data(0x02);	//2-dot 			

	send_cmd(0xB6);	// Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control			//RGB
	send_data(0x02);				


	send_cmd(0xB0);				
	send_data(0x00);			

	send_cmd(0xE9);				
	send_data(0x00);			//00=18BIT,01=24BIT	

	send_cmd(0x36);	
	send_data(0x48);	
	send_cmd(0x3A);	//Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35);	// TE on
	send_data(0x00);
	
	send_cmd(0xE0);	
	send_data(0x00);	
	send_data(0x03);	
	send_data(0x0C);	
	send_data(0x09);	
	send_data(0x17);	
	send_data(0x09);	
	send_data(0x3E);	
	send_data(0x89);	
	send_data(0x49);
	send_data(0x08);
	send_data(0x0D);
	send_data(0x0A);
	send_data(0x13);
	send_data(0x15);
	send_data(0x0F);

	send_cmd(0XE1);
	send_data(0x00);
	send_data(0x11);
	send_data(0x15);
	send_data(0x03);
	send_data(0x0F);
	send_data(0x05);
	send_data(0x2D);
	send_data(0x34);	
	send_data(0x41);	
	send_data(0x02);	
	send_data(0x0B);	
	send_data(0x0A);	
	send_data(0x33);	
	send_data(0x37);	
	send_data(0x0F);

	send_cmd(0x11);	
	LCD_DelayMS(120);	
	send_cmd(0x29);
#elif defined(ILI9488_7271_CFZZ)
	send_cmd(0XF7);
	send_data(0xA9);
	send_data(0x51);
	send_data(0x2C);
	send_data(0x82);

	send_cmd(0xC0);
	send_data(0x14);//11//0F
	send_data(0x14);//0F

	send_cmd(0xC1);
	send_data(0x41);

	send_cmd(0XC5);
	send_data(0x00);
	send_data(0x29);//09//17//20//2D//29
	send_data(0x80);

	send_cmd(0xB1);
	send_data(0xB0);
	send_data(0x11);

	send_cmd(0xB4);
	send_data(0x02);

	send_cmd(0xB6);
	send_data(0x02);
	send_data(0x02);//42

	send_cmd(0xE9);
	send_data(0x00);

	send_cmd(0x35);
	send_data(0x00);


send_cmd(0xF4);      //解决电荷残留问题
send_data(0x00);
send_data(0x00);
send_data(0x0f); 



	send_cmd(0x36);
	send_data(0x48);

	send_cmd(0x3A);
	send_data(0x55);

	send_cmd(0xE0);
	send_data(0x00);
	send_data(0x04);
	send_data(0x01);
	send_data(0x00);
	send_data(0x0F);
	send_data(0x08);
	send_data(0x3C);
	send_data(0x78);
	send_data(0x49);
	send_data(0x07);
	send_data(0x0E);
	send_data(0x0B);
	send_data(0x15);
	send_data(0x17);
	send_data(0x0F);

	send_cmd(0xE1);
	send_data(0x00);
	send_data(0x16);
	send_data(0x1C);
	send_data(0x03);
	send_data(0x0F);
	send_data(0x04);
	send_data(0x37);
	send_data(0x23);
	send_data(0x4E);
	send_data(0x04);
	send_data(0x0F);
	send_data(0x0E);
	send_data(0x3B);
	send_data(0x38);
	send_data(0x0F);

	send_cmd(0x11);
	LCD_DelayMS(120);
	send_cmd(0x29);
	LCD_DelayMS(50);
	send_cmd(0x2c);
#elif defined(CONFIG_ZYT_ILI9488_7271_YINGFEINIDI)
	send_cmd(0xE0); 
	send_data(0x00); 
	send_data(0x01); 
	send_data(0x02); 
	send_data(0x04); 
	send_data(0x14); 
	send_data(0x09); 
	send_data(0x3F); 
	send_data(0x57); 
	send_data(0x4D); 
	send_data(0x05); 
	send_data(0x0B); 
	send_data(0x09); 
	send_data(0x1A); 
	send_data(0x1D); 
	send_data(0x0F);  
	 
	send_cmd(0xE1); 
	send_data(0x00); 
	send_data(0x1D); 
	send_data(0x20); 
	send_data(0x02); 
	send_data(0x0E); 
	send_data(0x03); 
	send_data(0x35); 
	send_data(0x12); 
	send_data(0x47); 
	send_data(0x02); 
	send_data(0x0D); 
	send_data(0x0C); 
	send_data(0x38); 
	send_data(0x39); 
	send_data(0x0F); 

	send_cmd(0xC0); 
	send_data(0x18); 
	send_data(0x16); 
	 
	send_cmd(0xC1); 
	send_data(0x41); 

	send_cmd(0xC5); 
	send_data(0x00); 
	send_data(0x23); 
	send_data(0x80); 

	send_cmd(0x36); 
	send_data(0x08); 

	send_cmd(0x3A);// Interface Mode Control
	send_data(0x55);


	send_cmd(0XB0);  //Interface Mode Control  
	send_data(0x00); 
	send_cmd(0xB1);   //Frame rate 70HZ  
	send_data(0xB0); 

	send_cmd(0xB4); 
	send_data(0x02);   

	send_cmd(0xB6); //RGB/MCU Interface Control
	send_data(0x02); 
	send_data(0x22); 

	send_cmd(0xE9); 
	send_data(0x00);
	 
	send_cmd(0XF7);    
	send_data(0xA9); 
	send_data(0x51); 
	send_data(0x2C); 
	send_data(0x82);

	send_cmd(0x35);	// TE on
	send_data(0x00);

	send_cmd(0x11); 
	LCD_DelayMS(120); 
	send_cmd(0x29); 
#elif defined(ILI9488_7675_JLD_SHENCHAO)
    send_cmd(0XF7);	
	send_data(0xA9);	
	send_data(0x51);	
	send_data(0x2C);	
	send_data(0x82);	
	//Power Control 1 
	send_cmd(0xC0);	

 	send_data(0x11);	//Verg1out =4.52
	send_data(0x11);	//Vreg2out = -4.12

	//Power Control 2 
	send_cmd(0xC1);	//VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5);	
	send_data(0x00);	
	send_data(0x22);	
	send_data(0x80);	

	send_cmd(0xB1);	//Frame rate 
	send_data(0xB0);	//70Hz
	send_data(0x11);	

	send_cmd(0xB4);	//Display Inversion Control 			
	send_data(0x02);	//2-dot 			

	send_cmd(0xB6);	// Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control			//RGB
	send_data(0x02);				


	//send_cmd(0x55);				
	//send_data(0x00);			

	send_cmd(0xE9);				
	send_data(0x01);			//00=18BIT,01=24BIT	

	send_cmd(0x36);	

	send_data(0x48);	

	send_cmd(0x3A);	//Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35);	// TE on
	send_data(0x00);
	
	send_cmd(0xE0);	
	send_data(0x00);	
	send_data(0x04);	
	send_data(0x0e);	
	send_data(0x08);	
	send_data(0x17);	
	send_data(0x0a);	
	send_data(0x40);	
	send_data(0x79);	
	send_data(0x4d);
	send_data(0x07);
	send_data(0x0e);
	send_data(0x0a);
	send_data(0x1a);
	send_data(0x1d);
	send_data(0x0F);

	send_cmd(0XE1);
	send_data(0x00);
	send_data(0x1b);
	send_data(0x1f);
	send_data(0x02);
	send_data(0x10);
	send_data(0x05);
	send_data(0x32);
	send_data(0x34);	
	send_data(0x43);	
	send_data(0x02);	
	send_data(0x0a);	
	send_data(0x09);	
	send_data(0x33);	
	send_data(0x37);	
	send_data(0x0F);

	send_cmd(0x11);	
	LCD_DelayMS(120);	
	send_cmd(0x29);

#elif defined(ILI9488_7675_GW_LCD)
    send_cmd(0XF7);	
	send_data(0xA9);	
	send_data(0x51);	
	send_data(0x2C);	
	send_data(0x82);	
	//Power Control 1 
	send_cmd(0xC0);	

 	send_data(0x11);	//Verg1out =4.52
	send_data(0x11);	//Vreg2out = -4.12

	//Power Control 2 
	send_cmd(0xC1);	//VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5);	
	send_data(0x00);	
	send_data(0x21);	
	send_data(0x80);	

	send_cmd(0xB1);	//Frame rate 
	send_data(0xB0);	//70Hz
	send_data(0x11);	

	send_cmd(0xB4);	//Display Inversion Control 			
	send_data(0x02);	//2-dot 			

	send_cmd(0xB6);	// Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control			//RGB
	send_data(0x02);				


	send_cmd(0x55);				
	send_data(0x00);			

	send_cmd(0xE9);				
	send_data(0x01);			//00=18BIT,01=24BIT	

	send_cmd(0x36);	

	send_data(0x48);	

	send_cmd(0x3A);	//Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35);	// TE on
	send_data(0x00);
	
	send_cmd(0xE0);	
	send_data(0x00);	
	send_data(0x06);	
	send_data(0x06);	
	send_data(0x08);	
	send_data(0x18);	
	send_data(0x0c);	
	send_data(0x41);	
	send_data(0x9b);	
	send_data(0x4f);
	send_data(0x07);
	send_data(0x0e);
	send_data(0x0c);
	send_data(0x1c);
	send_data(0x1c);
	send_data(0x0F);

	send_cmd(0XE1);
	send_data(0x00);
	send_data(0x1b);
	send_data(0x1e);
	send_data(0x03);
	send_data(0x0e);
	send_data(0x04);
	send_data(0x35);
	send_data(0x24);	
	send_data(0x49);	
	send_data(0x04);	
	send_data(0x0f);	
	send_data(0x0e);	
	send_data(0x37);	
	send_data(0x3a);	
	send_data(0x0F);

	send_cmd(0x11);	
	LCD_DelayMS(120);	
	send_cmd(0x29);
#elif defined(CONFIG_ZYT_ILI9488_FS007_QY_MINGZHE)

	send_cmd(0XF7);	
	send_data(0xA9);	
	send_data(0x51);	
	send_data(0x2C);	
	send_data(0x82);	

    send_cmd(0xC0);
    send_data(0x17);
    send_data(0x15);
	//Power Control 2 
	send_cmd(0xC1);	//VGH=14.17,VGL =-9.42
	send_data(0x41);	


    send_cmd(0XC5);
    send_data(0x00);
    send_data(0x11);//0F 15 12 0D 17 11 13
    send_data(0x80);
	send_cmd(0xB1);	//Frame rate 
	send_data(0xB0);	//70Hz
	send_data(0x11);	

	send_cmd(0xB4);	//Display Inversion Control 			
	send_data(0x02);	//2-dot 			

	send_cmd(0xB6);	// Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control			//RGB
	send_data(0x02);				


	send_cmd(0x55);				
	send_data(0x00);			

	send_cmd(0xE9);				
	send_data(0x01);			//00=18BIT,01=24BIT	

	send_cmd(0x36);	

	send_data(0x48);	

	send_cmd(0x3A);	//Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35);	// TE on
	send_data(0x00);

	send_cmd(0xE0);
	send_data(0x00);
	send_data(0x07);
	send_data(0x10);
	send_data(0x09);
	send_data(0x17);
	send_data(0x0B);
	send_data(0x40);
	send_data(0x8A);
	send_data(0x4B);
	send_data(0x0A);
	send_data(0x0D);
	send_data(0x0F);
	send_data(0x15);
	send_data(0x16);
	send_data(0x0F);

	send_cmd(0xE1);
	send_data(0x00);
	send_data(0x1A);
	send_data(0x1B);
	send_data(0x02);
	send_data(0x0D);
	send_data(0x05);
	send_data(0x30);
	send_data(0x35);
	send_data(0x43);
	send_data(0x02);
	send_data(0x0A);
	send_data(0x09);
	send_data(0x32);
	send_data(0x36);
	send_data(0x0F);

	send_cmd(0x11);	
	LCD_DelayMS(120);	
	send_cmd(0x29);
#else

	send_cmd(0XF7);	
	send_data(0xA9);	
	send_data(0x51);	
	send_data(0x2C);	
	send_data(0x82);	
	//Power Control 1 
	send_cmd(0xC0);	
 #if defined(ILI9488_7271_CFZZ)
	send_data(0x15);	//Verg1out =4.52
	send_data(0x15);	//Vreg2out = -4.12
 #else
 	send_data(0x11);	//Verg1out =4.52
	send_data(0x11);	//Vreg2out = -4.12
 #endif
	//Power Control 2 
	send_cmd(0xC1);	//VGH=14.17,VGL =-9.42
	send_data(0x41);	

	send_cmd(0XC5);	
	send_data(0x00);	
	send_data(0x17);	
	send_data(0x80);	

	send_cmd(0xB1);	//Frame rate 
	send_data(0xB0);	//70Hz
	send_data(0x11);	

	send_cmd(0xB4);	//Display Inversion Control 			
	send_data(0x02);	//2-dot 			

	send_cmd(0xB6);	// Interface Mode Control			
	send_data(0x02);	//RGB/MCU Interface Control			//RGB
	send_data(0x02);				


	send_cmd(0x55);				
	send_data(0x00);			

	send_cmd(0xE9);				
	send_data(0x01);			//00=18BIT,01=24BIT	

	send_cmd(0x36);	
	#if defined(ILI9488_CAIFU) || defined(ILI9488_7271_CFZZ)
	send_data(0x48);	
	#else
	send_data(0x88);	
	#endif
	send_cmd(0x3A);	//Interface Pixel Format
	send_data(0x55);	

	send_cmd(0x35);	// TE on
	send_data(0x00);
	
	send_cmd(0xE0);	
	send_data(0x00);	
	send_data(0x06);	
	send_data(0x06);	
	send_data(0x08);	
	send_data(0x18);	
	send_data(0x0c);	
	send_data(0x41);	
	send_data(0x9b);	
	send_data(0x4f);
	send_data(0x07);
	send_data(0x0e);
	send_data(0x0c);
	send_data(0x1c);
	send_data(0x1c);
	send_data(0x0F);

	send_cmd(0XE1);
	send_data(0x00);
	send_data(0x1b);
	send_data(0x1e);
	send_data(0x03);
	send_data(0x0e);
	send_data(0x04);
	send_data(0x35);
	send_data(0x24);	
	send_data(0x49);	
	send_data(0x04);	
	send_data(0x0f);	
	send_data(0x0e);	
	send_data(0x37);	
	send_data(0x3a);	
	send_data(0x0F);

	send_cmd(0x11);	
	LCD_DelayMS(120);	
	send_cmd(0x29);
#endif

#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(0, LCM_CTRL);
#endif
	return 0;
}

static int32_t ili9488_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	LCD_PRINT("ili9488_enter_sleep\n");
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(4, LCM_CTRL);
#endif
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

#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(0, LCM_CTRL);
#endif
	return 0;
}

static int32_t ili9488_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9488_set_window\n");

#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(4, LCM_CTRL);
#endif    
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
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(0, LCM_CTRL);
#endif
	return 0;
}

static int32_t ili9488_invalidate(struct panel_spec *self)
{
	LCD_PRINT("ili9488_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0, 
			self->width-1, self->height-1);
}

static uint32_t ili9488_readid(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	uint32_t	lcm_dev_id = 0, data_read;
	uint32_t    value[6] ={0};
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(4, LCM_CTRL);
#endif

	send_cmd(0xD3);
	value[0] = read_data();  //param1:dummy	
	LCD_PRINT("ili9488_readid(0): 0x%x\n", value[0]);
	value[1] = read_data();  //param2:0
	LCD_PRINT("ili9488_readid(1): 0x%x\n", value[1]);

	value[2] = read_data(); //param3:94h
	LCD_PRINT("ili9488_readid(2): 0x%x\n", value[2]);
	
	value[3] = read_data(); //param4:88h
	LCD_PRINT("ili9488_readid(3): 0x%x\n", value[3]);

	if ((0x94 == value[2])&&(0x88 == value[3]))
	{
		lcm_dev_id = 0x9488;
	}
	LCD_PRINT("ili9488_readid: 0x%x\n", lcm_dev_id);   
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(0, LCM_CTRL);
#endif	

	return lcm_dev_id;
}

#ifdef CONFIG_FB_ESD_SUPPORT


#if defined(CONFIG_ZYT_ILI9488_FS007_QY_MINGZHE)
static uint32_t ili9488_esd_check(struct panel_spec *self)
{
	uint32_t    x0a[5] ={0};
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;
	read_data_t read_data = self->info.mcu->ops->read_data;
	send_cmd(0x0a);
	x0a[0] = read_data();
	x0a[1] = read_data();
	x0a[2] = read_data();
    printk("x0a[0]=0x%x,x0a[1]=0x%x,x0a[2]=0x%x", x0a[0], x0a[1],x0a[2]);


    if (x0a[1] != 0x9c)
    {
		printk("result=============1\n");
		return 0;
	}
	else
	{
		printk("result===================0\n");
		return 1;
	}
	return 0;	
}
#else
static uint32_t ili9488_esd_check(struct panel_spec *self)
{
	printk("\nili9488_esd_check****************\n");
	uint32_t    value[7] ={0};
	uint32_t    x09[5] ={0};
	uint32_t    xc0[3] ={0};
	uint32_t    xc1[2] ={0};
	uint32_t    xc5[4] ={0};
	uint32_t    xb6[3] ={0};
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;
	read_data_t read_data = self->info.mcu->ops->read_data;
	send_cmd(0x09);
	x09[0] = read_data();
	//printk("ili9488_esd_check----1-0X09--(0):0x%x\n",x09[0]);
	x09[1] = read_data();//
	//printk("ili9488_esd_check----2--0X09--(1):0x%x\n",x09[1]);//0xa4
	//send_cmd(0x0A);
	x09[2] = read_data();//
	//printk("ili9488_esd_check----1--0x09----(2):0x%x\n",x09[2]);//0x53
	x09[3] = read_data();//
	//printk("ili9488_esd_check----1--0x09----(3:0x%x\n",x09[3]);//0x6
	//send_cmd(0x0B);
	x09[4] = read_data();//
	//printk("ili9488_esd_check----1--0x09----(4):0x%x\n",x09[4]);//0x0
	//value[2] = read_data();//
	//printk("ili9488_esd_check----1--0x0B----(2):0x%x\n",value[2]);
	send_cmd(0xC0);
	xc0[0] = read_data();
	xc0[1] = read_data();//
	xc0[2] = read_data();//
	//printk("ili9488_esd_check----1--0xC0----(1):0x%x\n",xc0[1]);//0x11
	//printk("ili9488_esd_check----1--0xC0----(2):0x%x\n",xc0[2]);//0x11
	send_cmd(0xC1);
	xc1[0] = read_data();
	xc1[1] = read_data();//
	//printk("ili9488_esd_check----1--0xC1----(0):0x%x\n",xc1[0]);
	//printk("ili9488_esd_check----1--0xC1----(1):0x%x\n",xc1[1]);//0x41
	send_cmd(0xC5);
	xc5[0] = read_data();
	xc5[1] = read_data();
	xc5[2] = read_data();//
	xc5[3] = read_data();//
	//printk("ili9488_esd_check----1--0xC5----(0):0x%x\n",xc5[0]);
	//printk("ili9488_esd_check----1--0xC5----(1):0x%x\n",xc5[1]);//0x0
	//printk("ili9488_esd_check----1--0xC5----(2):0x%x\n",xc5[2]);//0x17
	//printk("ili9488_esd_check----1--0xC5----(3):0x%x\n",xc5[3]);//0x80
	send_cmd(0xB6);
	xb6[0] = read_data();
	xb6[1] = read_data();//
	xb6[2] = read_data();//
	//printk("ili9488_esd_check----1--0xB6----(0):0x%x\n",xb6[0]);
	//printk("ili9488_esd_check----1--0xB6----(1):0x%x\n",xb6[1]);//0x2
	//printk("ili9488_esd_check----1--0xB6----(2):0x%x\n",xb6[2]);//0x2
	
	//if(0x9C != value[1] | !=value[3] | 0x41!= value[4] | != value[5] | 0x02 != value[6])
	if(0xa4 != x09[1] | 0x53 != x09[2] | 0x6 != x09[3] | 0x0 != x09[4] | 0x11 != xc0[1] | 0x11 != xc0[2] | 0x41 != xc1[1] |0x0 !=xc5[1] |0x17 != xc5[2] | 0x80 !=xc5[3] |0x2 != xb6[1] | 0x2 != xb6[2])
	{
		printk("result=============1\n");
		return 0;
	}
	else
	{
		printk("result===================0\n");
		return 1;
	}
	return 0;	
}
#endif
#endif

static int32_t ili9488_set_direction(struct panel_spec *self, uint16_t direction)
{
	LCD_PRINT("ili9488_set_direction\n");
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(4, LCM_CTRL);
#endif	

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
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	lcdc_write(0, LCM_CTRL);
#endif
	return 0;
}

static int32_t ili9488_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("ili9488_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

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

static struct panel_operations lcd_ili9488_mcu_operations = {
	.panel_init = ili9488_init,
	.panel_set_window = ili9488_set_window,
	.panel_invalidate_rect = ili9488_invalidate_rect,
	.panel_invalidate = ili9488_invalidate,
	.panel_set_direction = ili9488_set_direction,
	.panel_enter_sleep = ili9488_enter_sleep,
	.panel_readid = ili9488_readid,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check = ili9488_esd_check,
#endif
};

static struct timing_mcu lcd_ili9488_mcu_timing[] = {
#if defined(CONFIG_ZYT_ILI9488_FS003_LHTC_SHENGDA)
[MCU_LCD_REGISTER_TIMING] = {                // read/write register timing
		.rcss = 50,  // 15ns
		.rlpw = 150,
		.rhpw = 150,
		.wcss = 20,
		.wlpw = 25,
		.whpw = 25,
	},
[MCU_LCD_GRAM_TIMING] = {                // read/write gram timing
		.rcss = 50,  // 15ns
		.rlpw = 150,
		.rhpw = 150,
		.wcss = 20,
		.wlpw = 25,//50
		.whpw = 25,//50
	},
#else
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
#endif
};

static struct info_mcu lcd_ili9488_mcu_info = {
	.bus_mode = LCD_BUS_8080,
#ifdef CONFIG_LCD_DATA_WIDTH_8BIT
	.bus_width = 8,
#else
	.bus_width = 16,
#endif
	.bpp = 16,
	.timing = lcd_ili9488_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_ili9488_mcu_spec = {
	.width = 320,
	.height = 480,
    .is_clean_lcd = true,
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_ili9488_mcu_info},
	.ops = &lcd_ili9488_mcu_operations,
};

struct panel_cfg lcd_ili9488_mcu = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x9488,
	.lcd_name = "lcd_ili9488_mcu",
	.panel = &lcd_ili9488_mcu_spec,
};

static int __init lcd_ili9488_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_ili9488_mcu);
}

subsys_initcall(lcd_ili9488_mcu_init);


