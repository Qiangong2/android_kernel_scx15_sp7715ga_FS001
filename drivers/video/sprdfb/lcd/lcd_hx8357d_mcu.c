/* drivers/video/sc8810/lcd_hx8357.c
 *
 * Support for hx8357 LCD device
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

#if (defined(CONFIG_MACH_SP7715EA_7676) || defined(CONFIG_MACH_SP7715GA_7676)) && (CONFIG_ZYT_CUST==24)
	#define CONFIG_ZYT_HX8357D_7676_BL_XCTY   // 7676 百利 信创天源屏
#elif (defined(CONFIG_MACH_SP7715EA_7676) || defined(CONFIG_MACH_SP7715GA_7676)) && (CONFIG_ZYT_CUST==56)
	#define CONFIG_ZYT_HX8357D_7676_XZQ_HY   // 7676 新中桥 华宇屏
#elif (CONFIG_ZYT_PRJ==7271)
#if (CONFIG_ZYT_CUST==37)
	#define CONFIG_ZYT_HX8357D_7271_YINGMAI
#elif (CONFIG_ZYT_CUST==3)
    #define CONFIG_ZYT_HX8357D_7271_CFZZ_HELITAI_B766
#elif (CONFIG_ZYT_CUST==18)
	#define CONFIG_ZYT_HX8357D_7271_WJT_TYF   // 7271 万金塔 天亿富屏
#elif (CONFIG_ZYT_CUST==70)
    #define CONFIG_ZYT_HX8357D_7271_CFZZ_LIUBU
#endif
#elif ((CONFIG_ZYT_PRJ==7675) && (CONFIG_ZYT_CUST==37))
	#define CONFIG_ZYT_HX8357D_7675_YM_ZGD   // 7675 O?Ao O??a礳?A
#elif ((CONFIG_ZYT_PRJ==7675) && (CONFIG_ZYT_CUST==3))
    #define CONFIG_ZYT_HX8357D_7271_CFZZ_HELITAI_B766
#endif

static int32_t hx8357d_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	printk("hx8357d_init_uboot\n");
//
	//LCD_DelayMS(120); //120ms
/////////////////////////////////////
//CMI3518    2013-01-07

//************* Reset LCD Driver ****************//
//LCD_nRESET = 1;
//LCD_DelayMS(10); // Delay 10ms
//LCD_nRESET = 0;
//Delayms(10); // Delay 10ms // This delay time is necessary
//LCD_nRESET = 1;
//    LCD_DelayMS(120); // Delay 100 ms
    //************* Start Initial Sequence **********//
//    send_cmd(0x11); //Sleep Out
//    LCD_DelayMS(150);
     
#if defined(CONFIG_ZYT_HX8357D_7676_BL_XCTY)
	send_cmd(0xB9);    //SET password 
	send_data(0xFF);   
	send_data(0x83);   
	send_data(0x57);  


	send_cmd(0xB0);
	send_data(0x68); //70Hz

	send_cmd(0x3A);    //COLMOD 
	send_data(0x55);     //RGB666  

	send_cmd(0x35);
    send_data(0x00); 
	
	send_cmd(0x36); 
	send_data(0xC0); 
	
	send_cmd(0xB1);     //SETPower 
	send_data(0x00);   //STB 
	send_data(0x14);   //VGH = 13V, VGL = -10V 
	send_data(0x18);   //VSPR = 4.5V 
	send_data(0x18);   //VSNR = -4.5V
	send_data(0xC3);   //AP 
	send_data(0x77);   //FS 

	send_cmd(0xB4);    //SETCYC 
	send_data(0x22);     //2-dot 
	send_data(0x40);   //RTN 
	send_data(0x00);   //DIV 
	send_data(0x2A);   //N_DUM 
	send_data(0x2A);    //I_DUM 
	send_data(0x20);   //GDON 
	send_data(0x91);   //GDOFF 

	send_cmd(0xB6);   //VCOMDC 
	send_data(0x32);  

	send_cmd(0xC0);   //SETSTBA 
	send_data(0x50);   //N_OPON 
	send_data(0x50);   //I_OPON 
	send_data(0x00);    //STBA 
	send_data(0x3C);   //STBA 
	send_data(0xC4);   //STBA 
	send_data(0x08);   //GENON 

	send_cmd(0xC2);    // Set Gate EQ 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x04); 

	send_cmd(0xCC);   //Set Panel 
	send_data(0x09);   //SS_Panel = 1, BGR_Panel = 1 

	//GAMMA 2.5" 
	send_cmd(0xE0);       
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x10); 
	send_data(0x18); 
	send_data(0x3B); 
	send_data(0x47); 
	send_data(0x50); 
	send_data(0x48); 
	send_data(0x42); 
	send_data(0x3A); 
	send_data(0x32); 
	send_data(0x2E); 
	send_data(0x29); 
	send_data(0x24); 
	send_data(0x19); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x10); 
	send_data(0x18); 
	send_data(0x3B); 
	send_data(0x47); 
	send_data(0x50); 
	send_data(0x48); 
	send_data(0x42); 
	send_data(0x3A); 
	send_data(0x32); 
	send_data(0x2E); 
	send_data(0x29); 
	send_data(0x24); 
	send_data(0x19); 
	send_data(0x00); 
	send_data(0x01);


	send_cmd(0x11);    //Sleep Out 
	LCD_DelayMS(120);  

	send_cmd(0x29);    //Display On 
	LCD_DelayMS(10); 
	send_cmd(0x2C);   //Write SRAM Data 
#elif defined(CONFIG_ZYT_HX8357D_7676_XZQ_HY)
	send_cmd(0xB9);      //SET password 
	send_data(0xFF);     
	send_data(0x83);     
	send_data(0x57);     
	LCD_DelayMS(1); 

	send_cmd(0xB1);        //SETPower 
	send_data(0x00);     //STB 
	send_data(0x14);     //VGH = 13V, VGL = -10V 
	send_data(0x18);     //VSPR = 4.5V 
	send_data(0x18);     //VSNR = -4.5V 
	send_data(0xC3);    //AP 
	send_data(0x77);     //FS 
	LCD_DelayMS(1); 

	send_cmd(0xB4);      //SETCYC 
	send_data(0x22);         //2-dot 
	send_data(0x40);     //RTN 
	send_data(0x00);     //DIV 
	send_data(0x2A);     //N_DUM 
	send_data(0x2A);     //I_DUM 
	send_data(0x20);     //GDON 
	send_data(0x91);     //GDOFF 
	LCD_DelayMS(1); 

	send_cmd(0xB6);    //VCOMDC 
	send_data(0x32);   
	LCD_DelayMS(1); 

	send_cmd(0xC0);    //SETSTBA 
	send_data(0x50);     //N_OPON 
	send_data(0x50);     //I_OPON 
	send_data(0x00);     //STBA 
	send_data(0x3C);    //STBA 
	send_data(0xC4);    //STBA 
	send_data(0x08);     //GENON 
	LCD_DelayMS(1); 

	send_cmd(0xC2);      // Set Gate EQ 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x04);  
	LCD_DelayMS(1); 

	send_cmd(0xCC);     //Set Panel 
	send_data(0x05);     //SS_Panel = 1, BGR_Panel = 1 
	LCD_DelayMS(1); 
	
	//send_cmd(0xb0);     //Set Panel 
	//send_data(0xc0);
	//send_data(0x00);   //

	send_cmd(0xE0);      //Set Gamma 
	send_data(0x03);     //VRP0 
	send_data(0x07);      //VRP1 
	send_data(0x13);      //VRP2 
	send_data(0x23);      //VRP3 
	send_data(0x2B);      //VRP4 
	send_data(0x3C);      //VRP5 
	send_data(0x48);      //VRP6 
	send_data(0x50);      //VRP7 
	send_data(0x49);      //VRP8 
	send_data(0x42);      //VRP9 
	send_data(0x3C);      //VRP10 
	send_data(0x31);      //VRP11 
	send_data(0x2F);      //VRP12 
	send_data(0x29);      //VRP13 
	send_data(0x25);      //VRP14 
	send_data(0x1C);      //VRP15 
	send_data(0x03);      //VRN0 
	send_data(0x07);      //VRN1 
	send_data(0x13);      //VRN2 
	send_data(0x2B);      //VRN3 
	send_data(0x2B);      //VRN4 
	send_data(0x3C);      //VRN5 
	send_data(0x48);      //VRN6 
	send_data(0x50);      //VRN7 
	send_data(0x49);      //VRN8 
	send_data(0x42);      //VRN9 
	send_data(0x3C);      //VRN10 
	send_data(0x31);      //VRN11 
	send_data(0x2F);      //VRN12 
	send_data(0x29);      //VRN13 
	send_data(0x25);      //VRN14 
	send_data(0x1C);      //VRN15 
	send_data(0x00); 
	send_data(0x01);     //GMA_Reload 
	LCD_DelayMS(1); 

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

	send_cmd(0x3A);      //COLMOD 
	send_data(0x55);        //RGB666   
	
    //send_cmd(0x36); 
	//send_data(0x04); 
	
	send_cmd(0x35);
    send_data(0x00); 
		
	//send_cmd(0x44); 
	//send_data(0x55);
	//send_data(0x90); 
	
	send_cmd(0x11);    //Sleep Out 
	LCD_DelayMS(120);  

	send_cmd(0x29);    //Display On 
	LCD_DelayMS(10); 
	send_cmd(0x2C);   //Write SRAM Data 
#elif defined(CONFIG_ZYT_HX8357D_7271_YINGMAI) 
	send_cmd(0xB9); //EXTC
	send_data(0xFF); //EXTC
	send_data(0x83); //EXTC
	send_data(0x57); //EXTC
	LCD_DelayMS(15);
	send_cmd(0x11); // SLPOUT
	LCD_DelayMS(50);
	LCD_DelayMS(50);
	LCD_DelayMS(50);
	LCD_DelayMS(50);
	send_cmd(0x35); // TE ON
	send_data(0x00);
	send_cmd(0xB6); //
	send_data(0x30); //22
	send_cmd(0xB0);
	send_data(0x66); 
	send_data(0x01); 

	send_cmd(0xB1); //
	send_data(0x00); //
	send_data(0x12); //BT
	send_data(0x30); //1c VSPR
	send_data(0x30); //1c VSNR
	send_data(0xc3); //83 AP  83
	send_data(0x44); //FS
	send_cmd(0xB4); //
	send_data(0x22); //NW
	send_data(0x40); //RTN
	send_data(0x00); //DIV
	send_data(0x2A); //DUM
	send_data(0x2A); //DUM
	send_data(0x0F); //0D GDON
	send_data(0x7B); //GDOFF
	send_cmd(0xB5); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0x67); 
	send_cmd(0xC2); 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x04); 
	send_data(0x00); 
	send_data(0x00); 
	send_cmd(0xE3); 
	send_data(0x17); 
	send_data(0x0F);
	send_cmd(0xC0); //STBA
	send_data(0x63); //OPON
	send_data(0x50); //OPON
	send_data(0x01); //
	send_data(0x3C); //
	send_data(0xC8); //
	send_data(0x08); //GEN
	LCD_DelayMS(1);
	send_cmd(0xE0);
	send_data(0x03); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x09); //    07
	send_data(0x10); //13  1A   10
	send_data(0x23); //20  33   23
	send_data(0x3A); //    41       3A
	send_data(0x46); //    49
	send_data(0x50); 
	send_data(0x49); 
	send_data(0x42); 
	send_data(0x39); 
	send_data(0x36); 
	send_data(0x31); 
	send_data(0x2D); 
	send_data(0x21); 
	send_data(0x03); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x09); 
	send_data(0x10); //13
	send_data(0x23); //20
	send_data(0x3A); //
	send_data(0x46); 
	send_data(0x50); 
	send_data(0x49); 
	send_data(0x42); 
	send_data(0x39); 
	send_data(0x36); 
	send_data(0x31); 
	send_data(0x2D); 
	send_data(0x21); 
	send_data(0x00); 
	send_data(0x01); 
	LCD_DelayMS(1);
	
	send_cmd(0x3A); //COLMOD
	send_data(0x55); //RGB565; RGB666 66   RGB888  77
	
	send_cmd(0xCC); // Set Panel
	send_data(0x09); // 配合36修改，尽量保留36为00，否则摄像头会切屏
	send_cmd(0x29); //Display On
	LCD_DelayMS(10);
	send_cmd(0x2C); //Write SRAM send_dataa
#elif defined(CONFIG_ZYT_HX8357D_7271_WJT_TYF)
	/////////////////HX8357D+CT3.5 CHAOSHENG///////////////////

	send_cmd(0xB9); //EXTC
	send_data(0xFF); //EXTC
	send_data(0x83); //EXTC
	send_data(0x57); //EXTC
	LCD_DelayMS(15);
	send_cmd(0x11); // SLPOUT
	LCD_DelayMS(120);
	send_cmd(0x35); // TE ON
	send_data(0x00);
	send_cmd(0xB6); //
	send_data(0x30); //22
	send_cmd(0xB0);
	send_data(0x66); 
	send_data(0x01); 
	send_cmd(0xCC); // Set Panel
	send_data(0x09); // 配合36修改，尽量保留36为00，否则摄像头会切屏
	send_cmd(0xB1); //
	send_data(0x00); //
	send_data(0x12); //BT
	send_data(0x30); //1c VSPR
	send_data(0x30); //1c VSNR
	send_data(0xc3); //83 AP  83
	send_data(0x44); //FS
	send_cmd(0xB4); //
	send_data(0x22); //NW
	send_data(0x40); //RTN
	send_data(0x00); //DIV
	send_data(0x2A); //DUM
	send_data(0x2A); //DUM
	send_data(0x0F); //0D GDON
	send_data(0x7B); //GDOFF
	send_cmd(0xB5); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0x67); 
	send_cmd(0xC2); 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x04); 
	send_data(0x00); 
	send_data(0x00); 
	send_cmd(0xE3); 
	send_data(0x17); 
	send_data(0x0F);
	send_cmd(0xC0); //STBA
	send_data(0x63); //OPON
	send_data(0x50); //OPON
	send_data(0x01); //
	send_data(0x3C); //
	send_data(0xC8); //
	send_data(0x08); //GEN
	LCD_DelayMS(1);
	send_cmd(0xE0);
	send_data(0x03); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x07); 
	send_data(0x1A); 
	send_data(0x33); 
	send_data(0x41); 
	send_data(0x49); 
	send_data(0x50); 
	send_data(0x49); 
	send_data(0x42); 
	send_data(0x39); 
	send_data(0x36); 
	send_data(0x31); 
	send_data(0x2D); 
	send_data(0x21); 
	send_data(0x03); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x07); 
	send_data(0x1A); 
	send_data(0x33); 
	send_data(0x41); 
	send_data(0x49); 
	send_data(0x50); 
	send_data(0x49); 
	send_data(0x42); 
	send_data(0x39); 
	send_data(0x36); 
	send_data(0x31); 
	send_data(0x2D); 
	send_data(0x21); 
	send_data(0x00); 
	send_data(0x01); 

	LCD_DelayMS(1);
	send_cmd(0x3A); //COLMOD
	send_data(0x55); //RGB565; RGB666 66   RGB888  77

	send_cmd(0x29); //Display On
	LCD_DelayMS(10);
	send_cmd(0x2C); //Write SRAM send_dataa
#elif defined(CONFIG_ZYT_HX8357D_7675_YM_ZGD)
	send_cmd(0xB9); //EXTC
	send_data(0xFF); //EXTC
	send_data(0x83); //EXTC
	send_data(0x57); //EXTC
	LCD_DelayMS(15);
	send_cmd(0x11); // SLPOUT
	LCD_DelayMS(120);
	send_cmd(0x35); // TE ON
	send_data(0x00);
	send_cmd(0xB6); //
	send_data(0x30); //22
	send_cmd(0xB0);
	send_data(0x66); 
	send_data(0x01); 
	send_cmd(0xCC); // Set Panel
	send_data(0x09); // 配合36修改，尽量保留36为00，否则摄像头会切屏
	send_cmd(0xB1); //
	send_data(0x00); //
	send_data(0x12); //BT
	send_data(0x30); //1c VSPR
	send_data(0x30); //1c VSNR
	send_data(0xc3); //83 AP  83
	send_data(0x44); //FS
	send_cmd(0xB4); //
	send_data(0x22); //NW
	send_data(0x40); //RTN
	send_data(0x00); //DIV
	send_data(0x2A); //DUM
	send_data(0x2A); //DUM
	send_data(0x0F); //0D GDON
	send_data(0x7B); //GDOFF
	send_cmd(0xB5); 
	send_data(0x01); 
	send_data(0x01); 
	send_data(0x67); 
	send_cmd(0xC2); 
	send_data(0x00); 
	send_data(0x08); 
	send_data(0x04); 
	send_data(0x00); 
	send_data(0x00); 
	send_cmd(0xE3); 
	send_data(0x17); 
	send_data(0x0F);
	send_cmd(0xC0); //STBA
	send_data(0x63); //OPON
	send_data(0x50); //OPON
	send_data(0x01); //
	send_data(0x3C); //
	send_data(0xC8); //
	send_data(0x08); //GEN
	LCD_DelayMS(1);
	send_cmd(0xE0);
	send_data(0x03); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x07); 
	send_data(0x1A); 
	send_data(0x33); 
	send_data(0x41); 
	send_data(0x49); 
	send_data(0x50); 
	send_data(0x49); 
	send_data(0x42); 
	send_data(0x39); 
	send_data(0x36); 
	send_data(0x31); 
	send_data(0x2D); 
	send_data(0x21); 
	send_data(0x03); 
	send_data(0x04); 
	send_data(0x05); 
	send_data(0x07); 
	send_data(0x1A); 
	send_data(0x33); 
	send_data(0x41); 
	send_data(0x49); 
	send_data(0x50); 
	send_data(0x49); 
	send_data(0x42); 
	send_data(0x39); 
	send_data(0x36); 
	send_data(0x31); 
	send_data(0x2D); 
	send_data(0x21); 
	send_data(0x00); 
	send_data(0x01); 

	LCD_DelayMS(1);
	send_cmd(0x3A); //COLMOD
	send_data(0x55); //RGB565; RGB666 66   RGB888  77

	send_cmd(0x29); //Display On
	LCD_DelayMS(10);
	send_cmd(0x2C); //Write SRAM send_data
#elif defined(CONFIG_ZYT_HX8357D_7271_CFZZ_HELITAI_B766)
  send_cmd(0xB9);      //SET password 
  send_data(0xFF);     
  send_data(0x83);     
  send_data(0x57);  
   
   
  send_cmd(0xB1);        //SETPower 
  send_data(0x00);     //STB 
  send_data(0x14);     //VGH = 13V, VGL = -10V   
  send_data(0x1E);     //VSPR = 4.5V   18
  send_data(0x1E);     //VSNR = -4.5V  18
  send_data(0xC3);    //AP 
  send_data(0x77);     //FS 
   
  send_cmd(0xB4);      //SETCYC 
  send_data(0x22);         //2-dot  22
  send_data(0x40);     //RTN 
  send_data(0x00);     //DIV 
  send_data(0x2A);     //N_DUM 
  send_data(0x2A);     //I_DUM 
  send_data(0x20);     //GDON 
  send_data(0x91);     //GDOFF 
   
  send_cmd(0xB6);    //VCOMDC 
  send_data(0x3C);     //0X3C  2E
   
  send_cmd(0xC0);    //SETSTBA 
  send_data(0x50);     //N_OPON 
  send_data(0x50);     //I_OPON 
  send_data(0x00);     //STBA 
  send_data(0x3C);    //STBA 
  send_data(0xC4);    //STBA 
  send_data(0x08);     //GENON 
 
 
  send_cmd(0xC2);      // Set Gate EQ 
  send_data(0x00); 
  send_data(0x08); 
  send_data(0x04);  
   
  send_cmd(0xCC);     //Set Panel 
  send_data(0x09);     //SS_Panel = 1, BGR_Panel = 1 
   
  send_cmd(0xE0);      //Set Gamma 
  send_data(0x03);     //VRP0 
  send_data(0x07);      //VRP1 
  send_data(0x13);      //VRP2 
  send_data(0x23);      //VRP3 
  send_data(0x2B);      //VRP4 
  send_data(0x3C);      //VRP5 
  send_data(0x48);      //VRP6 
  send_data(0x50);      //VRP7 
  send_data(0x49);      //VRP8 
  send_data(0x42);      //VRP9 
  send_data(0x3C);      //VRP10 
  send_data(0x31);      //VRP11 
  send_data(0x2F);      //VRP12 
  send_data(0x29);      //VRP13 
  send_data(0x25);      //VRP14 
  send_data(0x1C);      //VRP15 
  send_data(0x03);      //VRN0 
  send_data(0x07);      //VRN1 
  send_data(0x13);      //VRN2 
  send_data(0x2B);      //VRN3 
  send_data(0x2B);      //VRN4 
  send_data(0x3C);      //VRN5 
  send_data(0x48);      //VRN6 
  send_data(0x50);      //VRN7 
  send_data(0x49);      //VRN8 
  send_data(0x42);      //VRN9 
  send_data(0x3C);      //VRN10 
  send_data(0x31);      //VRN11 
  send_data(0x2F);      //VRN12 
  send_data(0x29);      //VRN13 
  send_data(0x25);      //VRN14 
  send_data(0x1C);      //VRN15 
  send_data(0x00); 
  send_data(0x01);     //GMA_Reload 


  send_cmd(0x35);
  send_data(0x00);    //TE ON
		
  send_cmd(0x3A);      //COLMOD 
  send_data(0x05);        //RGB666     

  send_cmd(0x11);      //Sleep Out 
  LCD_DelayMS(250); 

send_cmd(0x29);
LCD_DelayMS(10);
send_cmd(0x2C); 

#elif defined(CONFIG_ZYT_HX8357D_7271_CFZZ_LIUBU)
   send_cmd(0xB9);      //SET password 
  send_data(0xFF);     
  send_data(0x83);     
  send_data(0x57);  
   
   
  send_cmd(0xB1);        //SETPower 
  send_data(0x00);     //STB 
  send_data(0x14);     //VGH = 13V, VGL = -10V 
  send_data(0x18);     //VSPR = 4.5V 
  send_data(0x18);     //VSNR = -4.5V 
  send_data(0xC3);    //AP 
  send_data(0x77);     //FS 
   
  send_cmd(0xB4);      //SETCYC 
  send_data(0x22);         //2-dot  22
  send_data(0x40);     //RTN 
  send_data(0x00);     //DIV 
  send_data(0x2A);     //N_DUM 
  send_data(0x2A);     //I_DUM 
  send_data(0x20);     //GDON 
  send_data(0x91);     //GDOFF 
   
  send_cmd(0xB6);    //VCOMDC 
  send_data(0x27);     //0X3C
   
  send_cmd(0xC0);    //SETSTBA 
  send_data(0x50);     //N_OPON 
  send_data(0x50);     //I_OPON 
  send_data(0x00);     //STBA 
  send_data(0x3C);    //STBA 
  send_data(0xC4);    //STBA 
  send_data(0x08);     //GENON 
 
 
  send_cmd(0xC2);      // Set Gate EQ 
  send_data(0x00); 
  send_data(0x08); 
  send_data(0x04);  
   
  send_cmd(0xCC);     //Set Panel 
  send_data(0x09);     //SS_Panel = 1, BGR_Panel = 1 
   
  send_cmd(0xE0);      //Set Gamma 
  send_data(0x03);     //VRP0 
  send_data(0x07);      //VRP1 
  send_data(0x13);      //VRP2 
  send_data(0x23);      //VRP3 
  send_data(0x2B);      //VRP4 
  send_data(0x3C);      //VRP5 
  send_data(0x48);      //VRP6 
  send_data(0x50);      //VRP7 
  send_data(0x49);      //VRP8 
  send_data(0x42);      //VRP9 
  send_data(0x3C);      //VRP10 
  send_data(0x31);      //VRP11 
  send_data(0x2F);      //VRP12 
  send_data(0x29);      //VRP13 
  send_data(0x25);      //VRP14 
  send_data(0x1C);      //VRP15 
  send_data(0x03);      //VRN0 
  send_data(0x07);      //VRN1 
  send_data(0x13);      //VRN2 
  send_data(0x2B);      //VRN3 
  send_data(0x2B);      //VRN4 
  send_data(0x3C);      //VRN5 
  send_data(0x48);      //VRN6 
  send_data(0x50);      //VRN7 
  send_data(0x49);      //VRN8 
  send_data(0x42);      //VRN9 
  send_data(0x3C);      //VRN10 
  send_data(0x31);      //VRN11 
  send_data(0x2F);      //VRN12 
  send_data(0x29);      //VRN13 
  send_data(0x25);      //VRN14 
  send_data(0x1C);      //VRN15 
  send_data(0x00); 
  send_data(0x01);     //GMA_Reload 

    send_cmd(0x35);
    send_data(0x00);    //TE ON
		
  send_cmd(0x3A);      //COLMOD 
  send_data(0x05);        //RGB666     

  send_cmd(0x11);      //Sleep Out 
  LCD_DelayMS(250); 

//LCD_DelayMS(200); 
 
send_cmd(0x29);      //Display On 
LCD_DelayMS(10); 
send_cmd(0x2C);    //Write SRAM Data
#else
  send_cmd(0xB9);      //SET password 
  send_data(0xFF);     
  send_data(0x83);     
  send_data(0x57);  
   
   
  send_cmd(0xB1);        //SETPower 
  send_data(0x00);     //STB 
  send_data(0x14);     //VGH = 13V, VGL = -10V 
  send_data(0x18);     //VSPR = 4.5V 
  send_data(0x18);     //VSNR = -4.5V 
  send_data(0xC3);    //AP 
  send_data(0x77);     //FS 
   
  send_cmd(0xB4);      //SETCYC 
  send_data(0x22);         //2-dot  22
  send_data(0x40);     //RTN 
  send_data(0x00);     //DIV 
  send_data(0x2A);     //N_DUM 
  send_data(0x2A);     //I_DUM 
  send_data(0x20);     //GDON 
  send_data(0x91);     //GDOFF 
   
  send_cmd(0xB6);    //VCOMDC 
  send_data(0x2E);     //0X3C
   
  send_cmd(0xC0);    //SETSTBA 
  send_data(0x50);     //N_OPON 
  send_data(0x50);     //I_OPON 
  send_data(0x00);     //STBA 
  send_data(0x3C);    //STBA 
  send_data(0xC4);    //STBA 
  send_data(0x08);     //GENON 
 
 
  send_cmd(0xC2);      // Set Gate EQ 
  send_data(0x00); 
  send_data(0x08); 
  send_data(0x04);  
   
  send_cmd(0xCC);     //Set Panel 
  send_data(0x09);     //SS_Panel = 1, BGR_Panel = 1 
   
  send_cmd(0xE0);      //Set Gamma 
  send_data(0x03);     //VRP0 
  send_data(0x07);      //VRP1 
  send_data(0x13);      //VRP2 
  send_data(0x23);      //VRP3 
  send_data(0x2B);      //VRP4 
  send_data(0x3C);      //VRP5 
  send_data(0x48);      //VRP6 
  send_data(0x50);      //VRP7 
  send_data(0x49);      //VRP8 
  send_data(0x42);      //VRP9 
  send_data(0x3C);      //VRP10 
  send_data(0x31);      //VRP11 
  send_data(0x2F);      //VRP12 
  send_data(0x29);      //VRP13 
  send_data(0x25);      //VRP14 
  send_data(0x1C);      //VRP15 
  send_data(0x03);      //VRN0 
  send_data(0x07);      //VRN1 
  send_data(0x13);      //VRN2 
  send_data(0x2B);      //VRN3 
  send_data(0x2B);      //VRN4 
  send_data(0x3C);      //VRN5 
  send_data(0x48);      //VRN6 
  send_data(0x50);      //VRN7 
  send_data(0x49);      //VRN8 
  send_data(0x42);      //VRN9 
  send_data(0x3C);      //VRN10 
  send_data(0x31);      //VRN11 
  send_data(0x2F);      //VRN12 
  send_data(0x29);      //VRN13 
  send_data(0x25);      //VRN14 
  send_data(0x1C);      //VRN15 
  send_data(0x00); 
  send_data(0x01);     //GMA_Reload 

    send_cmd(0x35);
    send_data(0x00);    //TE ON
		
  send_cmd(0x3A);      //COLMOD 
  send_data(0x05);        //RGB666     

  send_cmd(0x11);      //Sleep Out 
  LCD_DelayMS(250); 

//LCD_DelayMS(200); 
 
send_cmd(0x29);      //Display On 
LCD_DelayMS(10); 
send_cmd(0x2C);    //Write SRAM Data
#endif

	printk("hx8357d_init: end\n");

	return 0;
}

static int32_t hx8357d_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

//	printk("hx8357d_set_window\n");

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


static int32_t hx8357d_invalidate(struct panel_spec *self)
{
//	printk("hx8357d_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
			self->width-1, self->height-1);
}

static int32_t hx8357d_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
//	printk("hx8357d_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t hx8357d_set_direction(struct panel_spec *self, uint16_t direction)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

//	printk("hx8357d_set_direction\n");
	send_cmd(0x36);

	switch (direction) {
	case LCD_DIRECT_NORMAL:
#if defined(CONFIG_ZYT_PROJECT_7616) || defined(CONFIG_ZYT_PROJECT_7619)
		send_data(0x00);
#else
		send_data(0xC0);
#endif
		break;
	case LCD_DIRECT_ROT_90:
		send_data(0xA0);
		break;
	case LCD_DIRECT_ROT_180:
		send_data(0x60);
		break;
	case LCD_DIRECT_ROT_270:
		send_data(0xB0);
		break;
	case LCD_DIRECT_MIR_H:
		send_data(0x40);
		break;
	case LCD_DIRECT_MIR_V:
		send_data(0x10);
		break;
	case LCD_DIRECT_MIR_HV:
		send_data(0xE0);
		break;
	default:
		printk("unknown lcd direction!\n");
		send_data(0x0);
		direction = LCD_DIRECT_NORMAL;
		break;
	}

	self->direction = direction;

	return 0;
}

static int32_t hx8357d_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_cmd_data_t send_cmd_data = self->info.mcu->ops->send_cmd_data;
	send_data_t send_data = self->info.mcu->ops->send_data;

	if(is_sleep) {
          send_cmd(0x28);
          LCD_DelayMS(10);
          send_cmd(0x10); // 
          LCD_DelayMS(120);
	}
	else {
#if 1
		//Sleep Out
          send_cmd(0x11); // 
          LCD_DelayMS(120);
          send_cmd(0x29);
          LCD_DelayMS(10);
#else
		// re init	
		self->ops->lcd_reset(self);
		self->ops->lcd_init(self);
#endif
	}
	return 0;
}

static int32_t hx8357d_read_id(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	send_data_t send_data = self->info.mcu->ops->send_data;
    uint32_t ID0 =0,ID1=0;
	uint32_t    i = 0, result = 0;
	
    send_cmd(0xB9); //SET password
    send_data(0xFF);
    send_data(0x83);
    send_data(0x57);
    LCD_DelayMS(1);

    send_cmd(0xD0);     
    LCD_DelayMS(10); 
    ID0 = read_data(); //空读 
#if 0//(CONFIG_ZYT_CUST == 31)
    while(i < 6)
	{
		//ID0 = read_data(); //空读 
		ID1 = read_data(); //0X99    
		printk("kernel hx8357d_read_id(%d): id=0x%x\n", i, ID1);
		if (ID1 == 0X99)
		{
			result = 1;
		    break;
	    }
	    i++;
	}
#else
	//ID0 = read_data(); //空读 
	ID1 = read_data(); //0X99    
#endif   

	printk("kernel hx8357d_read_id=%x\n", ID1);
	return ID1;
}

#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t hx8357d_esd_check(struct panel_spec *self)
{
	printk("\n hx8357d_esd_check****************\n");

	return 1;	
}
#endif

static struct panel_operations lcd_hx8357d_operations = {
	.panel_init            = hx8357d_init,
	.panel_set_window      = hx8357d_set_window,
	.panel_invalidate      = hx8357d_invalidate,
	.panel_invalidate_rect = hx8357d_invalidate_rect,
	.panel_set_direction   = hx8357d_set_direction,
	.panel_enter_sleep     = hx8357d_enter_sleep,
	.panel_readid          = hx8357d_read_id,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check       = hx8357d_esd_check,
#endif
};
#if 0
static struct timing_mcu lcd_hx8357d_timing[] = {
[0] = {                // read/write register timing
		.rcss = 25,  // 25 ns
		.rlpw = 70,
		.rhpw = 70,
		.wcss = 10,
		.wlpw = 50,
		.whpw = 50,
	},
[1] = {                // read/write gram timing
		.rcss = 25,  // 25 ns
		.rlpw = 70,
		.rhpw = 70,
		.wcss = 20,
		.wlpw = 20,
		.whpw = 20,
	}
};
#else
static struct timing_mcu lcd_hx8357d_timing[] = {
[0] = {                // read/write register timing
		.rcss = 100,  // 25 ns
		.rlpw = 200,
		.rhpw = 200,
		.wcss = 50,
		.wlpw = 100,
		.whpw = 100,
	},
[1] = {                // read/write gram timing
		.rcss = 100,  // 25 ns
		.rlpw = 200,
		.rhpw = 200,
		.wcss = 40, //4
		.wlpw = 30, //20
		.whpw = 30,
	}
};

#endif
static struct info_mcu lcd_hx8357d_info = {
	.bus_mode = LCD_BUS_8080,
#if defined(LCD_DATA_WIDTH_18BIT)
	.bus_width = 18,
#else
	.bus_width = 16,
#endif
	.bpp = 16,
	.timing = lcd_hx8357d_timing,
	.ops = NULL,
};

struct panel_spec lcd_panel_hx8357d_mcu_spec = {
	.width = 320,
	.height = 480,
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_hx8357d_info},
	.ops = &lcd_hx8357d_operations,
};

struct panel_cfg lcd_hx8357d_mcu = {
	/* this panel may on both CS0/1 */
	.dev_id = SPRDFB_UNDEFINELCD_ID,
	.lcd_id = 0x99,
	.lcd_name = "lcd_hx8357d_mcu",
	.panel = &lcd_panel_hx8357d_mcu_spec,
};
static int __init lcd_hxhx8357d_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_hx8357d_mcu);
}

subsys_initcall(lcd_hxhx8357d_mcu_init);
