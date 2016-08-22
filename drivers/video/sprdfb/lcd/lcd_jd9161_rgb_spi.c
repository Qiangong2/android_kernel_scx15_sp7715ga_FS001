/* drivers/video/sc8810/lcd_JD9161_rgb_spi.c
 *
 *
 *
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

#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
#if (CONFIG_ZYT_CUST == 78) //hongkai lcd
#define JD9161_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(8,(cmd & 0xFF));\
}

#define  JD9161_SpiWriteData(data)\
{ \
	spi_send_data(8,(data & 0xFF));\
}
#else
#define JD9161_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(16,(cmd & 0xFFFF));\
}

#define  JD9161_SpiWriteData(data)\
{ \
	spi_send_data(16,(data & 0xFFFF));\
}
#endif
#else
#define JD9161_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd((cmd & 0xFF));\
}

#define  JD9161_SpiWriteData(data)\
{ \
	spi_send_data((data & 0xFF));\
}
#endif
    //  #define   mdelay(x/1000) udelay(x)  



static int32_t JD9161_init(struct panel_spec *self)
{	
	uint32_t data = 0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read; 

	printk("JD9161_initttttttttttttttttttttttttttttttt\n");

#if (CONFIG_ZYT_CUST == 78)

JD9161_SpiWriteCmd(0xBF); 
JD9161_SpiWriteData(0x91);
JD9161_SpiWriteData(0x61);
JD9161_SpiWriteData(0xF2);

//Set VCOM
JD9161_SpiWriteCmd(0xB3); 
JD9161_SpiWriteData(0x00);
JD9161_SpiWriteData(0xA9);//0x75

//Set VCOM_R
JD9161_SpiWriteCmd(0xB4); 
JD9161_SpiWriteData(0x00);
JD9161_SpiWriteData(0xA9);//0x75

//Set VGMP, VGSP, VGMN, VGSNM_R
JD9161_SpiWriteCmd(0xB8); 
JD9161_SpiWriteData(0x00);
JD9161_SpiWriteData(0xBF);
JD9161_SpiWriteData(0x01);
JD9161_SpiWriteData(0x00);
JD9161_SpiWriteData(0xBF);
JD9161_SpiWriteData(0x01);

//Set GIP output voltage level.
JD9161_SpiWriteCmd(0xBA); 
JD9161_SpiWriteData(0x34); //0X3E
JD9161_SpiWriteData(0x23); 
JD9161_SpiWriteData(0x00); 

//JD9161_SpiWriteCmd(0xC1); 
//JD9161_SpiWriteData(0x0C); //GS=1

//Set RGB_JDT[2:0] Column
JD9161_SpiWriteCmd(0xC3); 
JD9161_SpiWriteData(0x02); //column 04

//SET TCON
JD9161_SpiWriteCmd(0xC4); 
JD9161_SpiWriteData(0x00);
JD9161_SpiWriteData(0x64); //800 Lines

//Set POWER CTRL
JD9161_SpiWriteCmd(0xC7); 
JD9161_SpiWriteData(0x00); 
JD9161_SpiWriteData(0x01); 
JD9161_SpiWriteData(0x32); 		
JD9161_SpiWriteData(0x05); 
JD9161_SpiWriteData(0x65); 
JD9161_SpiWriteData(0x2A); 		
JD9161_SpiWriteData(0x07); 
JD9161_SpiWriteData(0xA5); 
JD9161_SpiWriteData(0xA5); 

//Set Gamma_2.2
JD9161_SpiWriteCmd(0xC8); 
JD9161_SpiWriteData(0x7C); //RPA18   0 
JD9161_SpiWriteData(0x6D); //RPA17   4 
JD9161_SpiWriteData(0x62); //RPA16   8 
JD9161_SpiWriteData(0x58); //RPA15  12 
JD9161_SpiWriteData(0x56); //RPA14  20 
JD9161_SpiWriteData(0x32); //RPA13  28 
JD9161_SpiWriteData(0x30); //RPA12  44 
JD9161_SpiWriteData(0x18); //RPA11  64 
JD9161_SpiWriteData(0x32); //RPA10  96 
JD9161_SpiWriteData(0x34); //RPA9   128
JD9161_SpiWriteData(0x38); //RPA8   159
JD9161_SpiWriteData(0x5D); //RPA7   191
JD9161_SpiWriteData(0x52); //RPA6   211
JD9161_SpiWriteData(0x64); //RPA5   227
JD9161_SpiWriteData(0x5D); //RPA4   235
JD9161_SpiWriteData(0x66); //RPA3   243
JD9161_SpiWriteData(0x61); //RPA2   247
JD9161_SpiWriteData(0x5B); //RPA1   251
JD9161_SpiWriteData(0x50); //RPA0   255
JD9161_SpiWriteData(0x7C); //RNA18   0  
JD9161_SpiWriteData(0x6D); //RNA17   4   
JD9161_SpiWriteData(0x62); //RNA16   8   
JD9161_SpiWriteData(0x58); //RNA15  12  
JD9161_SpiWriteData(0x56); //RNA14  20  
JD9161_SpiWriteData(0x32); //RNA13  28 
JD9161_SpiWriteData(0x30); //RNA12  44 	
JD9161_SpiWriteData(0x18); //RNA11  64 
JD9161_SpiWriteData(0x32); //RNA10  96 
JD9161_SpiWriteData(0x34); //RNA9   128
JD9161_SpiWriteData(0x38); //RNA8   159
JD9161_SpiWriteData(0x5D); //RNA7   191
JD9161_SpiWriteData(0x52); //RNA6   211
JD9161_SpiWriteData(0x64); //RNA5   227
JD9161_SpiWriteData(0x5D); //RNA4   235
JD9161_SpiWriteData(0x66); //RNA3   243
JD9161_SpiWriteData(0x61); //RNA2   247
JD9161_SpiWriteData(0x5B); //RNA1   251
JD9161_SpiWriteData(0x51); //RNA0   255     
 


//Set SETGIP1
JD9161_SpiWriteCmd(0xD8);  
JD9161_SpiWriteData(0x30); 
JD9161_SpiWriteData(0x00); 
JD9161_SpiWriteData(0x00); 		
JD9161_SpiWriteData(0x10); 
JD9161_SpiWriteData(0x03); 
JD9161_SpiWriteData(0x10); 		
JD9161_SpiWriteData(0x01); 
JD9161_SpiWriteData(0x02); 
JD9161_SpiWriteData(0x00); 		
JD9161_SpiWriteData(0x01); 
JD9161_SpiWriteData(0x02); 
JD9161_SpiWriteData(0x06); 		
JD9161_SpiWriteData(0x68); 
JD9161_SpiWriteData(0x00); 
JD9161_SpiWriteData(0x00); 		
JD9161_SpiWriteData(0x71); 
JD9161_SpiWriteData(0x05); 
JD9161_SpiWriteData(0x06); 		
JD9161_SpiWriteData(0x68); 
JD9161_SpiWriteData(0x0C); 

JD9161_SpiWriteCmd(0xD9); 
JD9161_SpiWriteData(0x00);              
JD9161_SpiWriteData(0x0A);              
JD9161_SpiWriteData(0x0A); 		
JD9161_SpiWriteData(0x88);              
JD9161_SpiWriteData(0x00);              
JD9161_SpiWriteData(0x00); 		
JD9161_SpiWriteData(0x06);            
JD9161_SpiWriteData(0x80);            
JD9161_SpiWriteData(0x00); 		
JD9161_SpiWriteData(0x80);            
JD9161_SpiWriteData(0x01);            
JD9161_SpiWriteData(0x5B); 		
JD9161_SpiWriteData(0x35);            
JD9161_SpiWriteData(0x00);            
JD9161_SpiWriteData(0x00); 		
JD9161_SpiWriteData(0x00);            
JD9161_SpiWriteData(0x00);            
JD9161_SpiWriteData(0x03); 		
JD9161_SpiWriteData(0x80); 

//PAGE 1
JD9161_SpiWriteCmd(0xBE); 
JD9161_SpiWriteData(0x01); //

//
JD9161_SpiWriteCmd(0xD7); 
JD9161_SpiWriteData(0x40); //

JD9161_SpiWriteCmd(0xDD); 
JD9161_SpiWriteData(0x11); //

//PAGE 0
JD9161_SpiWriteCmd(0xBE); 
JD9161_SpiWriteData(0x00); //

//
JD9161_SpiWriteCmd(0x3A); 
JD9161_SpiWriteData(0x55); //

JD9161_SpiWriteCmd(0x11);  // SLPOUT
mdelay(120);
JD9161_SpiWriteCmd(0x29);  // DSPON
mdelay(50);
#elif (CONFIG_ZYT_CUST == 76)
JD9161_SpiWriteCmd(0xBF00);JD9161_SpiWriteData(0x91);
	JD9161_SpiWriteCmd(0xBF01);JD9161_SpiWriteData(0x61);
	JD9161_SpiWriteCmd(0xBF02);JD9161_SpiWriteData(0xF2);
	
	JD9161_SpiWriteCmd(0xB300);JD9161_SpiWriteData(0x00);
	JD9161_SpiWriteCmd(0xB301);JD9161_SpiWriteData(0x97);
	
	JD9161_SpiWriteCmd(0xB400);JD9161_SpiWriteData(0x00); 
	JD9161_SpiWriteCmd(0xB401);JD9161_SpiWriteData(0x97);
	
	JD9161_SpiWriteCmd(0xB800);JD9161_SpiWriteData(0x00);
	JD9161_SpiWriteCmd(0xB801);JD9161_SpiWriteData(0xBF);
	JD9161_SpiWriteCmd(0xB802);JD9161_SpiWriteData(0x01);
	JD9161_SpiWriteCmd(0xB803);JD9161_SpiWriteData(0x00);
	JD9161_SpiWriteCmd(0xB804);JD9161_SpiWriteData(0xBF);
	JD9161_SpiWriteCmd(0xB805);JD9161_SpiWriteData(0x01);
	
	JD9161_SpiWriteCmd(0xBA00);JD9161_SpiWriteData(0x34); 
	JD9161_SpiWriteCmd(0xBA01);JD9161_SpiWriteData(0x23); 
	JD9161_SpiWriteCmd(0xBA02);JD9161_SpiWriteData(0x00); 
	
	JD9161_SpiWriteCmd(0xC300);JD9161_SpiWriteData(0x02);
	
	JD9161_SpiWriteCmd(0xC400);JD9161_SpiWriteData(0x00);
	JD9161_SpiWriteCmd(0xC401);JD9161_SpiWriteData(0x64);
	
	JD9161_SpiWriteCmd(0xC800);JD9161_SpiWriteData(0x7C); 
	JD9161_SpiWriteCmd(0xC801);JD9161_SpiWriteData(0x73); 
	JD9161_SpiWriteCmd(0xC802);JD9161_SpiWriteData(0x6C); 
	JD9161_SpiWriteCmd(0xC803);JD9161_SpiWriteData(0x5D); 
	JD9161_SpiWriteCmd(0xC804);JD9161_SpiWriteData(0x46); 
	JD9161_SpiWriteCmd(0xC805);JD9161_SpiWriteData(0x2A); 
	JD9161_SpiWriteCmd(0xC806);JD9161_SpiWriteData(0x1F); 		
	JD9161_SpiWriteCmd(0xC807);JD9161_SpiWriteData(0x05); 
	JD9161_SpiWriteCmd(0xC808);JD9161_SpiWriteData(0x1E); 
	JD9161_SpiWriteCmd(0xC809);JD9161_SpiWriteData(0x20); 		
	JD9161_SpiWriteCmd(0xC80A);JD9161_SpiWriteData(0x25); 
	JD9161_SpiWriteCmd(0xC80B);JD9161_SpiWriteData(0x4A); 
	JD9161_SpiWriteCmd(0xC80C);JD9161_SpiWriteData(0x40); 		
	JD9161_SpiWriteCmd(0xC80D);JD9161_SpiWriteData(0x52); 
	JD9161_SpiWriteCmd(0xC80E);JD9161_SpiWriteData(0x4C); 
	JD9161_SpiWriteCmd(0xC80F);JD9161_SpiWriteData(0x54); 		
	JD9161_SpiWriteCmd(0xC810);JD9161_SpiWriteData(0x4F); 
	JD9161_SpiWriteCmd(0xC811);JD9161_SpiWriteData(0x48); 
	JD9161_SpiWriteCmd(0xC812);JD9161_SpiWriteData(0x3C); 		
	JD9161_SpiWriteCmd(0xC813);JD9161_SpiWriteData(0x7C); 
	JD9161_SpiWriteCmd(0xC814);JD9161_SpiWriteData(0x73); 
	JD9161_SpiWriteCmd(0xC815);JD9161_SpiWriteData(0x6C); 		
	JD9161_SpiWriteCmd(0xC816);JD9161_SpiWriteData(0x5D); 
	JD9161_SpiWriteCmd(0xC817);JD9161_SpiWriteData(0x46); 
	JD9161_SpiWriteCmd(0xC818);JD9161_SpiWriteData(0x2A); 		
	JD9161_SpiWriteCmd(0xC819);JD9161_SpiWriteData(0x1F); 
	JD9161_SpiWriteCmd(0xC81A);JD9161_SpiWriteData(0x05); 
	JD9161_SpiWriteCmd(0xC81B);JD9161_SpiWriteData(0x1E); 		
	JD9161_SpiWriteCmd(0xC81C);JD9161_SpiWriteData(0x20); 
	JD9161_SpiWriteCmd(0xC81D);JD9161_SpiWriteData(0x25); 
	JD9161_SpiWriteCmd(0xC81E);JD9161_SpiWriteData(0x4A); 		
	JD9161_SpiWriteCmd(0xC81F);JD9161_SpiWriteData(0x40); 
	JD9161_SpiWriteCmd(0xC820);JD9161_SpiWriteData(0x52); 
	JD9161_SpiWriteCmd(0xC821);JD9161_SpiWriteData(0x4C); 		
	JD9161_SpiWriteCmd(0xC822);JD9161_SpiWriteData(0x54); 
	JD9161_SpiWriteCmd(0xC823);JD9161_SpiWriteData(0x4F); 
	JD9161_SpiWriteCmd(0xC824);JD9161_SpiWriteData(0x48); 		
	JD9161_SpiWriteCmd(0xC825);JD9161_SpiWriteData(0x3C); 
	
	JD9161_SpiWriteCmd(0xC700);JD9161_SpiWriteData(0x00); 
	JD9161_SpiWriteCmd(0xC701);JD9161_SpiWriteData(0x01); 
	JD9161_SpiWriteCmd(0xC702);JD9161_SpiWriteData(0x31); 		
	JD9161_SpiWriteCmd(0xC703);JD9161_SpiWriteData(0x05); 
	JD9161_SpiWriteCmd(0xC704);JD9161_SpiWriteData(0x65); 
	JD9161_SpiWriteCmd(0xC705);JD9161_SpiWriteData(0x2C); 		
	JD9161_SpiWriteCmd(0xC706);JD9161_SpiWriteData(0x13); 
	JD9161_SpiWriteCmd(0xC707);JD9161_SpiWriteData(0xA5); 
	JD9161_SpiWriteCmd(0xC708);JD9161_SpiWriteData(0xA5); 		
	                                                
	JD9161_SpiWriteCmd(0xD400);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD401);JD9161_SpiWriteData(0x1E);                             
	JD9161_SpiWriteCmd(0xD402);JD9161_SpiWriteData(0x05); 		               
	JD9161_SpiWriteCmd(0xD403);JD9161_SpiWriteData(0x07);                             
	JD9161_SpiWriteCmd(0xD404);JD9161_SpiWriteData(0x01);                             
	JD9161_SpiWriteCmd(0xD405);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD406);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD407);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD408);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD409);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD40A);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD40B);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD40C);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD40D);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD40E);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD40F);JD9161_SpiWriteData(0x1F);                             
	                                                 
	JD9161_SpiWriteCmd(0xD500);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD501);JD9161_SpiWriteData(0x1E);                             
	JD9161_SpiWriteCmd(0xD502);JD9161_SpiWriteData(0x04); 		               
	JD9161_SpiWriteCmd(0xD503);JD9161_SpiWriteData(0x06);                             
	JD9161_SpiWriteCmd(0xD504);JD9161_SpiWriteData(0x00);                             
	JD9161_SpiWriteCmd(0xD505);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD506);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD507);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD508);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD509);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD50A);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD50B);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD50C);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD50D);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD50E);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD50F);JD9161_SpiWriteData(0x1F);                             
	                                           
	JD9161_SpiWriteCmd(0xD600);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD601);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD602);JD9161_SpiWriteData(0x06); 		               
	JD9161_SpiWriteCmd(0xD603);JD9161_SpiWriteData(0x04);                             
	JD9161_SpiWriteCmd(0xD604);JD9161_SpiWriteData(0x00);                             
	JD9161_SpiWriteCmd(0xD605);JD9161_SpiWriteData(0x1E); 		               
	JD9161_SpiWriteCmd(0xD606);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD607);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD608);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD609);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD60A);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD60B);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD60C);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD60D);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD60E);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD60F);JD9161_SpiWriteData(0x1F);                             
	 
	JD9161_SpiWriteCmd(0xD700);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD701);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD702);JD9161_SpiWriteData(0x07); 		               
	JD9161_SpiWriteCmd(0xD703);JD9161_SpiWriteData(0x05);                             
	JD9161_SpiWriteCmd(0xD704);JD9161_SpiWriteData(0x01);                             
	JD9161_SpiWriteCmd(0xD705);JD9161_SpiWriteData(0x1E); 		               
	JD9161_SpiWriteCmd(0xD706);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD707);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD708);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD709);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD70A);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD70B);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD70C);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD70D);JD9161_SpiWriteData(0x1F);                             
	JD9161_SpiWriteCmd(0xD70E);JD9161_SpiWriteData(0x1F); 		               
	JD9161_SpiWriteCmd(0xD70F);JD9161_SpiWriteData(0x1F);                             
	
	JD9161_SpiWriteCmd(0xD800);JD9161_SpiWriteData(0x20); 
	JD9161_SpiWriteCmd(0xD801);JD9161_SpiWriteData(0x00); 
	JD9161_SpiWriteCmd(0xD802);JD9161_SpiWriteData(0x00); 		
	JD9161_SpiWriteCmd(0xD803);JD9161_SpiWriteData(0x10); 
	JD9161_SpiWriteCmd(0xD804);JD9161_SpiWriteData(0x03); 
	JD9161_SpiWriteCmd(0xD805);JD9161_SpiWriteData(0x20); 		
	JD9161_SpiWriteCmd(0xD806);JD9161_SpiWriteData(0x01); 
	JD9161_SpiWriteCmd(0xD807);JD9161_SpiWriteData(0x02); 
	JD9161_SpiWriteCmd(0xD808);JD9161_SpiWriteData(0x00); 		
	JD9161_SpiWriteCmd(0xD809);JD9161_SpiWriteData(0x01); 
	JD9161_SpiWriteCmd(0xD80A);JD9161_SpiWriteData(0x02); 
	JD9161_SpiWriteCmd(0xD80B);JD9161_SpiWriteData(0x36); 		
	JD9161_SpiWriteCmd(0xD80C);JD9161_SpiWriteData(0x4F); 
	JD9161_SpiWriteCmd(0xD80D);JD9161_SpiWriteData(0x00); 
	JD9161_SpiWriteCmd(0xD80E);JD9161_SpiWriteData(0x00); 		
	JD9161_SpiWriteCmd(0xD80F);JD9161_SpiWriteData(0x32); 
	JD9161_SpiWriteCmd(0xD810);JD9161_SpiWriteData(0x04); 
	JD9161_SpiWriteCmd(0xD811);JD9161_SpiWriteData(0x36); 		
	JD9161_SpiWriteCmd(0xD812);JD9161_SpiWriteData(0x4F); 
	JD9161_SpiWriteCmd(0xD813);JD9161_SpiWriteData(0x08); 
	                                
	JD9161_SpiWriteCmd(0xD900);JD9161_SpiWriteData(0x00);              
	JD9161_SpiWriteCmd(0xD901);JD9161_SpiWriteData(0x0A);              
	JD9161_SpiWriteCmd(0xD902);JD9161_SpiWriteData(0x0A); 		
	JD9161_SpiWriteCmd(0xD903);JD9161_SpiWriteData(0x88);              
	JD9161_SpiWriteCmd(0xD904);JD9161_SpiWriteData(0x00);              
	JD9161_SpiWriteCmd(0xD905);JD9161_SpiWriteData(0x00); 		
	JD9161_SpiWriteCmd(0xD906);JD9161_SpiWriteData(0x06);            
	JD9161_SpiWriteCmd(0xD907);JD9161_SpiWriteData(0x7B);            
	JD9161_SpiWriteCmd(0xD908);JD9161_SpiWriteData(0x00); 		
	JD9161_SpiWriteCmd(0xD909);JD9161_SpiWriteData(0x00);            
	JD9161_SpiWriteCmd(0xD90A);JD9161_SpiWriteData(0x00);            
	JD9161_SpiWriteCmd(0xD90B);JD9161_SpiWriteData(0x3B); 		
	JD9161_SpiWriteCmd(0xD90C);JD9161_SpiWriteData(0x2F);            
	JD9161_SpiWriteCmd(0xD90D);JD9161_SpiWriteData(0x1F);            
	JD9161_SpiWriteCmd(0xD90E);JD9161_SpiWriteData(0x00); 		
	JD9161_SpiWriteCmd(0xD90F);JD9161_SpiWriteData(0x00);            
	JD9161_SpiWriteCmd(0xD910);JD9161_SpiWriteData(0x00);            
	JD9161_SpiWriteCmd(0xD911);JD9161_SpiWriteData(0x03); 		
	JD9161_SpiWriteCmd(0xD912);JD9161_SpiWriteData(0x7B);            
	
	JD9161_SpiWriteCmd(0xBE00);JD9161_SpiWriteData(0x01);
	
	JD9161_SpiWriteCmd(0xDD00);JD9161_SpiWriteData(0x11);//01 10  11
	
	JD9161_SpiWriteCmd(0xBE00);JD9161_SpiWriteData(0x00);
	
	
	JD9161_SpiWriteCmd(0x3A00); JD9161_SpiWriteData(0x55); //0x77 24BIT 0X66 18BIT 0X55 16BIT
	
	JD9161_SpiWriteCmd(0x1100);
	LCD_DelayMS(150);
	JD9161_SpiWriteCmd(0x2900);
	LCD_DelayMS(20);
#else
// Change to Page 1
	JD9161_SpiWriteCmd(0xBF00); // Password
	JD9161_SpiWriteData(0x91); //
	JD9161_SpiWriteCmd(0xBF01);
	JD9161_SpiWriteData(0x61); //
	JD9161_SpiWriteCmd(0xBF02);
	JD9161_SpiWriteData(0xF2); //


	JD9161_SpiWriteCmd(0xB300); //VCOM
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xB301);
	JD9161_SpiWriteData(0xA9); //


	JD9161_SpiWriteCmd(0xB400); //VCOM_R
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xB401);
	JD9161_SpiWriteData(0xA9); //


	JD9161_SpiWriteCmd(0xB800); //VGMP, VGSP, VGMN, VGSN
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xB801);
	JD9161_SpiWriteData(0xBF); //VGMP[7:0] //
	JD9161_SpiWriteCmd(0xB802);
	JD9161_SpiWriteData(0x01); //VGSP[7:0] //
	JD9161_SpiWriteCmd(0xB803);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xB804);
	JD9161_SpiWriteData(0xBF); //VGMN[7:0] //
	JD9161_SpiWriteCmd(0xB805);
	JD9161_SpiWriteData(0x01); //VGSN[7:0] //

	JD9161_SpiWriteCmd(0xBA00); //GIP output voltage level.
	JD9161_SpiWriteData(0x34); //VGH_REG[6:0]
	JD9161_SpiWriteCmd(0xBA01); 
	JD9161_SpiWriteData(0x23); //VGL_REG[5:0]
	JD9161_SpiWriteCmd(0xBA02); 
	JD9161_SpiWriteData(0x00); //

	JD9161_SpiWriteCmd(0xC300); //SET RGB CYC
	JD9161_SpiWriteData(0x02); //RGB_JDT[2:0] 2-dot

	JD9161_SpiWriteCmd(0xC400); //SET TCON
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xC401); 
	JD9161_SpiWriteData(0x64); //800 LINE

	JD9161_SpiWriteCmd(0xC700); //POWER CTRL
	JD9161_SpiWriteData(0x00); //DCDCM[3:0]
	JD9161_SpiWriteCmd(0xC701);
	JD9161_SpiWriteData(0x01); //AVDD_RT[1:0]
	JD9161_SpiWriteCmd(0xC702);
	JD9161_SpiWriteData(0x32); //
	JD9161_SpiWriteCmd(0xC703);
	JD9161_SpiWriteData(0x05); //
	JD9161_SpiWriteCmd(0xC704);
	JD9161_SpiWriteData(0x65); //
	JD9161_SpiWriteCmd(0xC705);
	JD9161_SpiWriteData(0x2A); //
	JD9161_SpiWriteCmd(0xC706);
	JD9161_SpiWriteData(0x07); //
	JD9161_SpiWriteCmd(0xC707);
	JD9161_SpiWriteData(0xA5); //
	JD9161_SpiWriteCmd(0xC708);
	JD9161_SpiWriteData(0xA5); //

	JD9161_SpiWriteCmd(0xC800);JD9161_SpiWriteData(0x7C);  //
	JD9161_SpiWriteCmd(0xC801);JD9161_SpiWriteData(0x6D);  //
	JD9161_SpiWriteCmd(0xC802);JD9161_SpiWriteData(0x62);  //
	JD9161_SpiWriteCmd(0xC803);JD9161_SpiWriteData(0x58);  //
	JD9161_SpiWriteCmd(0xC804);JD9161_SpiWriteData(0x5C);  //
	JD9161_SpiWriteCmd(0xC805);JD9161_SpiWriteData(0x46);  //
	JD9161_SpiWriteCmd(0xC806);JD9161_SpiWriteData(0x42);  //
	JD9161_SpiWriteCmd(0xC807);JD9161_SpiWriteData(0x26);  //
	JD9161_SpiWriteCmd(0xC808);JD9161_SpiWriteData(0x3D);  //
	JD9161_SpiWriteCmd(0xC809);JD9161_SpiWriteData(0x3D);  //
	JD9161_SpiWriteCmd(0xC80a);JD9161_SpiWriteData(0x3F);  //
	JD9161_SpiWriteCmd(0xC80b);JD9161_SpiWriteData(0x62);  //
	JD9161_SpiWriteCmd(0xC80c);JD9161_SpiWriteData(0x57);  //
	JD9161_SpiWriteCmd(0xC80d);JD9161_SpiWriteData(0x68);  //
	JD9161_SpiWriteCmd(0xC80e);JD9161_SpiWriteData(0x61);  //
	JD9161_SpiWriteCmd(0xC80f);JD9161_SpiWriteData(0x69);  //
	JD9161_SpiWriteCmd(0xC810);JD9161_SpiWriteData(0x63);  //
	JD9161_SpiWriteCmd(0xC811);JD9161_SpiWriteData(0x5C);  //
	JD9161_SpiWriteCmd(0xC812);JD9161_SpiWriteData(0x50);  //
	JD9161_SpiWriteCmd(0xC813);JD9161_SpiWriteData(0x7C);  //
	JD9161_SpiWriteCmd(0xC814);JD9161_SpiWriteData(0x6D);  //
	JD9161_SpiWriteCmd(0xC815);JD9161_SpiWriteData(0x62);  //
	JD9161_SpiWriteCmd(0xC816);JD9161_SpiWriteData(0x58);  //
	JD9161_SpiWriteCmd(0xC817);JD9161_SpiWriteData(0x5C);  //
	JD9161_SpiWriteCmd(0xC818);JD9161_SpiWriteData(0x46);  //
	JD9161_SpiWriteCmd(0xC819);JD9161_SpiWriteData(0x42);  //
	JD9161_SpiWriteCmd(0xC81a);JD9161_SpiWriteData(0x26);  //
	JD9161_SpiWriteCmd(0xC81b);JD9161_SpiWriteData(0x3D);  //
	JD9161_SpiWriteCmd(0xC81c);JD9161_SpiWriteData(0x3D);  //
	JD9161_SpiWriteCmd(0xC81d);JD9161_SpiWriteData(0x3F);  //
	JD9161_SpiWriteCmd(0xC81e);JD9161_SpiWriteData(0x62);  //
	JD9161_SpiWriteCmd(0xC81f);JD9161_SpiWriteData(0x57);  //
	JD9161_SpiWriteCmd(0xC820);JD9161_SpiWriteData(0x68);  //
	JD9161_SpiWriteCmd(0xC821);JD9161_SpiWriteData(0x61);  //
	JD9161_SpiWriteCmd(0xC822);JD9161_SpiWriteData(0x69);  //
	JD9161_SpiWriteCmd(0xC823);JD9161_SpiWriteData(0x63);  //
	JD9161_SpiWriteCmd(0xC824);JD9161_SpiWriteData(0x5C);  //
	JD9161_SpiWriteCmd(0xC825);JD9161_SpiWriteData(0x50);  //

	JD9161_SpiWriteCmd(0xD800); //SETGIP1
	JD9161_SpiWriteData(0x30); //
	JD9161_SpiWriteCmd(0xD801);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD802);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD803);
	JD9161_SpiWriteData(0x10); //
	JD9161_SpiWriteCmd(0xD804);
	JD9161_SpiWriteData(0x03); //
	JD9161_SpiWriteCmd(0xD805);
	JD9161_SpiWriteData(0x10); //
	JD9161_SpiWriteCmd(0xD806);
	JD9161_SpiWriteData(0x01); //
	JD9161_SpiWriteCmd(0xD807);
	JD9161_SpiWriteData(0x02); //
	JD9161_SpiWriteCmd(0xD808);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD809);
	JD9161_SpiWriteData(0x01); //
	JD9161_SpiWriteCmd(0xD80A);
	JD9161_SpiWriteData(0x02); //
	JD9161_SpiWriteCmd(0xD80B);
	JD9161_SpiWriteData(0x06); //
	JD9161_SpiWriteCmd(0xD80C);
	JD9161_SpiWriteData(0x68); //
	JD9161_SpiWriteCmd(0xD80D);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD80E);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD80F);
	JD9161_SpiWriteData(0x71); //
	JD9161_SpiWriteCmd(0xD810);
	JD9161_SpiWriteData(0x05); //
	JD9161_SpiWriteCmd(0xD811);
	JD9161_SpiWriteData(0x06); //
	JD9161_SpiWriteCmd(0xD812);
	JD9161_SpiWriteData(0x68); //
	JD9161_SpiWriteCmd(0xD813);
	JD9161_SpiWriteData(0x0C); //

	JD9161_SpiWriteCmd(0xD900); // SETGIP2
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD901);
	JD9161_SpiWriteData(0x0A); //
	JD9161_SpiWriteCmd(0xD902);
	JD9161_SpiWriteData(0x0A); //
	JD9161_SpiWriteCmd(0xD903);
	JD9161_SpiWriteData(0x88); //
	JD9161_SpiWriteCmd(0xD904);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD905);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD906);
	JD9161_SpiWriteData(0x06); //
	JD9161_SpiWriteCmd(0xD907);
	JD9161_SpiWriteData(0x80); //
	JD9161_SpiWriteCmd(0xD908);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD909);
	JD9161_SpiWriteData(0x80); //
	JD9161_SpiWriteCmd(0xD90A);
	JD9161_SpiWriteData(0x01); //
	JD9161_SpiWriteCmd(0xD90B);
	JD9161_SpiWriteData(0x5B); //
	JD9161_SpiWriteCmd(0xD90C);
	JD9161_SpiWriteData(0x35); //
	JD9161_SpiWriteCmd(0xD90D);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD90E);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD90F);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD910);
	JD9161_SpiWriteData(0x00); //
	JD9161_SpiWriteCmd(0xD911);
	JD9161_SpiWriteData(0x03); //
	JD9161_SpiWriteCmd(0xD912);
	JD9161_SpiWriteData(0x80); //

	JD9161_SpiWriteCmd(0xBE00); // PAGE1
	JD9161_SpiWriteData(0x01); //



	//////////////////RGB½ÓæSOö¼Ó 
	JD9161_SpiWriteCmd(0xDD00); // //page1 Set RGB Interface
	JD9161_SpiWriteData(0x11); //
	/////////////////////////////////




	JD9161_SpiWriteCmd(0xD700); // EBBG GIP FUNCTION
	JD9161_SpiWriteData(0x40); //

	JD9161_SpiWriteCmd(0xBE00); // PAGE0
	JD9161_SpiWriteData(0x00); //


	JD9161_SpiWriteCmd(0x3A00); // PAGE0
	JD9161_SpiWriteData(0x55); //


	JD9161_SpiWriteCmd(0x1100); // SLPOUT
	mdelay(120); 

	JD9161_SpiWriteCmd(0x2900); // DSPON
    mdelay(20); 
#endif
}

static int32_t JD9161_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	printk("JD9161_enter_sleep-------------------is_sleep=%d\n", is_sleep);

	if(is_sleep==1){
		//Sleep In
		JD9161_SpiWriteCmd(0x2800);
		mdelay(120); 
		JD9161_SpiWriteCmd(0x1000);
		mdelay(10); 
	}else{
		//Sleep Out
		JD9161_SpiWriteCmd(0x1100);
		mdelay(120); 
		JD9161_SpiWriteCmd(0x2900);
		mdelay(10); 
	}

	return 0;
}




static int32_t JD9161_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	uint32_t *test_data[4] = {0};
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;
#if 0

	printk("zxdbg add -JD9161_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

	JD9161_SpiWriteCmd(0x2A00); 
	JD9161_SpiWriteData((left>>8));// set left address
	JD9161_SpiWriteData((left&0xff));
	JD9161_SpiWriteData((right>>8));// set right address
	JD9161_SpiWriteData((right&0xff));

	JD9161_SpiWriteCmd(0x2B00); 
	JD9161_SpiWriteData((top>>8));// set left address
	JD9161_SpiWriteData((top&0xff));
	JD9161_SpiWriteData((bottom>>8));// set bottom address
	JD9161_SpiWriteData((bottom&0xff));

//	JD9161_SpiWriteCmd(0x2C00);

//	JD9161_SpiWriteCmd(0x2A00); 
//	spi_read(test_data);
//	spi_read(test_data+1);
//	JD9161_SpiWriteCmd(0x2B00); 
//	spi_read(test_data+2);
//	spi_read(test_data+3);
#endif
//	printk("zxdbg add -JD9161_read read: %x, %x, %x, %x\n",test_data[0], test_data[1], test_data[2], test_data[3]);

	return 0;
}
static int32_t JD9161_invalidate(struct panel_spec *self)
{
//	printk("JD9161_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t JD9161_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
//	printk("JD9161_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t JD9161_read_id(struct panel_spec *self)
{
	int32_t  id = 0; 
	int32_t  id1 = 0; 
	int32_t  id2 = 0; 
	
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;

	printk("JD9161_read_id \n");

	JD9161_SpiWriteCmd(0xBF); 
	JD9161_SpiWriteCmd(0x91); 
	JD9161_SpiWriteCmd(0x61); 
	JD9161_SpiWriteCmd(0xF2); 

	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
	spi_read(8,&id1);  //0x98
	#else
	spi_read(&id1);  //0x98
	#endif
	printk(" JD9161_read_id u-boot id1 = 0x%x\n",id1);

	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
	spi_read(8,&id2);  //0x06
	#else
	spi_read(&id2);  //0x06
	#endif
	printk(" JD9161_read_id u-boot id2 = 0x%x\n",id2);
	id = ((id1&0x00ff)<<8)|(id2&0x00ff);
	return 0x9161;

}
#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t JD9161_esd_check(struct panel_spec *self)
{
	printk("JD9161_esd_check****************\n");
	uint32_t    r0a =0;
	uint32_t    r0b =0;
	uint32_t    r0c =0;
	uint32_t    r0d =0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;

	JD9161_SpiWriteCmd(0xFF); 
	JD9161_SpiWriteData(0xFF);  
	JD9161_SpiWriteData(0x98); 
	JD9161_SpiWriteData(0x06);  
	JD9161_SpiWriteData(0x04);  
	JD9161_SpiWriteData(0x01);
	 
	JD9161_SpiWriteCmd(0xFE); 
	JD9161_SpiWriteData(0x81);
	JD9161_SpiWriteCmd(0x0A);
	spi_read(8,&r0a);//9c
		printk(" JD9161_read_r0a = 0x%x\n",r0a);
	
	JD9161_SpiWriteCmd(0xFE); 
	JD9161_SpiWriteData(0x81);
	JD9161_SpiWriteCmd(0x0b);
	spi_read(8,&r0b);
			printk(" JD9161_read_r0b = 0x%x\n",r0b);

	JD9161_SpiWriteCmd(0xFE); 
	JD9161_SpiWriteData(0x81);
	JD9161_SpiWriteCmd(0x0c);
	spi_read(8,&r0c);
			printk(" JD9161_read_r0c = 0x%x\n",r0c);

	JD9161_SpiWriteCmd(0xFE); 
	JD9161_SpiWriteData(0x81);
	JD9161_SpiWriteCmd(0x0d);
	spi_read(8,&r0d);
			printk(" JD9161_read_r0d = 0x%x\n",r0d);

	JD9161_SpiWriteCmd(0xFE); 
	JD9161_SpiWriteData(0x00);
	/*
	if(0xa4 != x09[1] | 0x53 != x09[2] | 0x6 != x09[3] | 0x0 != x09[4] | 0x11 != xc0[1] | 0x11 != xc0[2] | 0x41 != xc1[1] |0x0 !=xc5[1] |0x17 != xc5[2] | 0x80 !=xc5[3] |0x2 != xb6[1] | 0x2 != xb6[2])
	{
		printk("result=============1\n");
		return 0;
	}
	else
	{
		printk("result===================0\n");
		return 1;
	}*/
	return 1;
}
#endif
static struct panel_operations lcd_JD9161_rgb_spi_operations = {
	.panel_init = JD9161_init,
	.panel_set_window = JD9161_set_window,
	.panel_invalidate_rect= JD9161_invalidate_rect,
	.panel_invalidate = JD9161_invalidate,
	.panel_enter_sleep = JD9161_enter_sleep,
	.panel_readid          = JD9161_read_id,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check = JD9161_esd_check,
#endif
};

static struct timing_rgb lcd_JD9161_rgb_timing = {
	.hfp = 50,//10,  /* unit: pixel */
	.hbp = 50,//6,
	.hsync = 10, //8,
	.vfp = 30, // 3, /*unit: line*/
	.vbp = 2, // 3,
	.vsync = 8, // 4,
};

static struct spi_info lcd_JD9161_rgb_spi_info = {
	.ops = NULL,
};

static struct info_rgb lcd_JD9161_rgb_info = {
	.cmd_bus_mode  = SPRDFB_RGB_BUS_TYPE_SPI,
	.video_bus_width = 16, /*18,16*/
	.h_sync_pol = SPRDFB_POLARITY_NEG,
	.v_sync_pol = SPRDFB_POLARITY_NEG,
	.de_pol = SPRDFB_POLARITY_POS,
	.timing = &lcd_JD9161_rgb_timing,
	.bus_info = {
		.spi = &lcd_JD9161_rgb_spi_info,
	}
};

struct panel_spec lcd_panel_JD9161_rgb_spi_spec = {
	.width = 480,
	.height = 800,
	.reset_timing = {20, 10, 10},
	.fps = 61,
	.type = LCD_MODE_RGB,
	.direction = LCD_DIRECT_NORMAL,
	.is_clean_lcd = true,
	.info = {
		.rgb = &lcd_JD9161_rgb_info
	},
	.ops = &lcd_JD9161_rgb_spi_operations,
};

struct panel_cfg lcd_JD9161_rgb_spi = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x9161,
	.lcd_name = "lcd_JD9161_rgb_spi",
	.panel = &lcd_panel_JD9161_rgb_spi_spec,
};
static int __init lcd_JD9161_rgb_spi_init(void)
{
	return sprdfb_panel_register(&lcd_JD9161_rgb_spi);
}

subsys_initcall(lcd_JD9161_rgb_spi_init);
