/* drivers/video/sc8810/lcd_SH1282_rgb_spi.c
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


#define  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif


#if (CONFIG_ZYT_CUST==67)
#define CONFIG_ZYT_SH1282_7675_QIYANG
#endif

#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
#if defined(CONFIG_ZYT_SH1282_7675_QIYANG)
#define SH1282_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(8,(cmd & 0xFF));\
}

#define  SH1282_SpiWriteData(data)\
{ \
 	spi_send_data(8,(data & 0xFF));\
}
#else
#define SH1282_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(16,(cmd & 0xFFFF));\
}

#define  SH1282_SpiWriteData(data)\
{ \
 	spi_send_data(16,(data & 0xFFFF));\
}
#endif
#else
#define SH1282_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd((cmd & 0xFFFF));\
}

#define  SH1282_SpiWriteData(data)\
{ \
 	spi_send_data((data & 0xFFFF));\
}
#endif
static int32_t SH1282_init(struct panel_spec *self)
{	
	uint32_t data = 0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read; 

	LCD_PRINT("uboot SH1282_init\n");
#if defined(CONFIG_ZYT_SH1282_7675_QIYANG)
SH1282_SpiWriteCmd(0xF0);
SH1282_SpiWriteData(0x55);
SH1282_SpiWriteData(0xAA);
SH1282_SpiWriteData(0x52);
SH1282_SpiWriteData(0x08);
SH1282_SpiWriteData(0x01);

SH1282_SpiWriteCmd(0xB0);
SH1282_SpiWriteData(0x0A);

SH1282_SpiWriteCmd(0xB1);
SH1282_SpiWriteData(0x0A);

SH1282_SpiWriteCmd(0xB2);
SH1282_SpiWriteData(0x02);

SH1282_SpiWriteCmd(0xB3);
SH1282_SpiWriteData(0x10);

SH1282_SpiWriteCmd(0xB6);
SH1282_SpiWriteData(0x57);

SH1282_SpiWriteCmd(0xB7);
SH1282_SpiWriteData(0x27);

SH1282_SpiWriteCmd(0xB8);
SH1282_SpiWriteData(0x35);


SH1282_SpiWriteCmd(0xB9);
SH1282_SpiWriteData(0x36);

SH1282_SpiWriteCmd(0xBA);
SH1282_SpiWriteData(0x24);

SH1282_SpiWriteCmd(0xBC);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x78);
SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xBD);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x78);
SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xBE);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x7a );

SH1282_SpiWriteCmd(0xBF);
SH1282_SpiWriteData(0x01);

SH1282_SpiWriteCmd(0xD1);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x09);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x16);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x41);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x75);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0xDF);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x33);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x98);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0xD2);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x19);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x4A);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x4B);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x74);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x9F);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xB6);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xD3);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xE4);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xFA);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x09);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x1E);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x2D);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x45);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x79);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0xFE);

SH1282_SpiWriteCmd(0xD2);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x09);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x16);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x41);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x75);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0xDF);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x33);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x98);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0xD2);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x19);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x4A);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x4B);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x74);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0x9F);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xB6);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xD3);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xE4);
SH1282_SpiWriteData(0x02);
SH1282_SpiWriteData(0xFA);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x09);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x1E);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x2D);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x45);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x79);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0xFE);





SH1282_SpiWriteCmd(0xF0);
SH1282_SpiWriteData(0x55);
SH1282_SpiWriteData(0xaa);
SH1282_SpiWriteData(0x52);
SH1282_SpiWriteData(0x08);
SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xB0);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x0C);
SH1282_SpiWriteData(0x0a);
SH1282_SpiWriteData(0x5a);
SH1282_SpiWriteData(0x0a);

SH1282_SpiWriteCmd(0xB1);
SH1282_SpiWriteData(0x78);
SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xB4);
SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xB5);
SH1282_SpiWriteData(0x50);

SH1282_SpiWriteCmd(0xB6);
SH1282_SpiWriteData(0x08);

SH1282_SpiWriteCmd(0xB7);
SH1282_SpiWriteData(0x23);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x23);
SH1282_SpiWriteData(0x00);


SH1282_SpiWriteCmd(0xB8);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x10);//
SH1282_SpiWriteData(0x10);//
SH1282_SpiWriteData(0x10);//

SH1282_SpiWriteCmd(0xBB);
SH1282_SpiWriteData(0x33);

SH1282_SpiWriteCmd(0xBC);
SH1282_SpiWriteData(0x02);


SH1282_SpiWriteCmd(0xC1);
SH1282_SpiWriteData(0x00);    
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x06);	                       
SH1282_SpiWriteData(0x08);	                       
SH1282_SpiWriteData(0x02);	                       
SH1282_SpiWriteData(0x00);	                       
SH1282_SpiWriteData(0x01);	                       
SH1282_SpiWriteData(0x00);	                       
SH1282_SpiWriteData(0x00);	                       
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00); 
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00); 
SH1282_SpiWriteData(0x00); 
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00); 
SH1282_SpiWriteData(0x01); 
SH1282_SpiWriteData(0x00); 
SH1282_SpiWriteData(0x03); 
SH1282_SpiWriteData(0x09);
SH1282_SpiWriteData(0x07);
SH1282_SpiWriteData(0x01);
SH1282_SpiWriteData(0x00);

    
SH1282_SpiWriteCmd(0xC2);
SH1282_SpiWriteData(0x85); 
SH1282_SpiWriteData(0x11); 
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x84);
SH1282_SpiWriteData(0x00);

    
SH1282_SpiWriteCmd(0xC3);
SH1282_SpiWriteData(0x84); 
SH1282_SpiWriteData(0x31);
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x28); //
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00); 
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x83);  
SH1282_SpiWriteData(0x31); 
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x29); //
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00); 
SH1282_SpiWriteData(0x82);
SH1282_SpiWriteData(0x31); 
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x2A); //
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00); //
SH1282_SpiWriteData(0x81);
SH1282_SpiWriteData(0x31); 
SH1282_SpiWriteData(0x03);
SH1282_SpiWriteData(0x2B);//
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);
SH1282_SpiWriteData(0x00);

         
SH1282_SpiWriteCmd(0x8F);
SH1282_SpiWriteData(0x5A);
SH1282_SpiWriteData(0x96);
SH1282_SpiWriteData(0x3C);
SH1282_SpiWriteData(0xC3);
SH1282_SpiWriteData(0xA5);
SH1282_SpiWriteData(0x69);

SH1282_SpiWriteCmd(0x8A);
SH1282_SpiWriteData(0x13);

SH1282_SpiWriteCmd(0x85);
SH1282_SpiWriteData(0x11);
SH1282_SpiWriteData(0x44);
SH1282_SpiWriteData(0x44);
SH1282_SpiWriteData(0x11);
SH1282_SpiWriteData(0x44);
SH1282_SpiWriteData(0x44);

    
SH1282_SpiWriteCmd(0x3A);
SH1282_SpiWriteData(0x50);

SH1282_SpiWriteCmd(0x35);//Tearing Effect Line
SH1282_SpiWriteData(0x00);


SH1282_SpiWriteCmd(0x11);

mdelay(120);
SH1282_SpiWriteCmd(0x29);
//SH1282_SpiWriteCmd(0x2C); 
mdelay(120);
#else
SH1282_SpiWriteCmd(0xff00);SH1282_SpiWriteData(0xcc);
SH1282_SpiWriteCmd(0xff01);SH1282_SpiWriteData(0x33);
SH1282_SpiWriteCmd(0xff02);SH1282_SpiWriteData(0x12);
SH1282_SpiWriteCmd(0xff03);SH1282_SpiWriteData(0x82);

SH1282_SpiWriteCmd(0xF700);SH1282_SpiWriteData(0x65);
SH1282_SpiWriteCmd(0xF701);SH1282_SpiWriteData(0xC0);
SH1282_SpiWriteCmd(0xF702);SH1282_SpiWriteData(0x40);
SH1282_SpiWriteCmd(0xF703);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xF704);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xF705);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xF706);SH1282_SpiWriteData(0xB0);
SH1282_SpiWriteCmd(0xF707);SH1282_SpiWriteData(0x98);
SH1282_SpiWriteCmd(0xF708);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xF709);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xF70A);SH1282_SpiWriteData(0x40); 
SH1282_SpiWriteCmd(0xF70B);SH1282_SpiWriteData(0x54);
SH1282_SpiWriteCmd(0xF70C);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xF70D);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xF70E);SH1282_SpiWriteData(0xC0);
SH1282_SpiWriteCmd(0xF70F);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xF710);SH1282_SpiWriteData(0x42);
SH1282_SpiWriteCmd(0xF711);SH1282_SpiWriteData(0x28);



SH1282_SpiWriteCmd(0xF000);SH1282_SpiWriteData(0x55);
SH1282_SpiWriteCmd(0xF001);SH1282_SpiWriteData(0xAA);
SH1282_SpiWriteCmd(0xF002);SH1282_SpiWriteData(0x52);
SH1282_SpiWriteCmd(0xF003);SH1282_SpiWriteData(0x08);
SH1282_SpiWriteCmd(0xF004);SH1282_SpiWriteData(0x01);


SH1282_SpiWriteCmd(0xB000);SH1282_SpiWriteData(0x0A);
SH1282_SpiWriteCmd(0xB001);SH1282_SpiWriteData(0x0A);
SH1282_SpiWriteCmd(0xB002);SH1282_SpiWriteData(0x0A);

SH1282_SpiWriteCmd(0xB600);SH1282_SpiWriteData(0x34);
SH1282_SpiWriteCmd(0xB601);SH1282_SpiWriteData(0x34);
SH1282_SpiWriteCmd(0xB602);SH1282_SpiWriteData(0x34);

SH1282_SpiWriteCmd(0xB100);SH1282_SpiWriteData(0x0A);
SH1282_SpiWriteCmd(0xB101);SH1282_SpiWriteData(0x0A);
SH1282_SpiWriteCmd(0xB102);SH1282_SpiWriteData(0x0A);

SH1282_SpiWriteCmd(0xB700);SH1282_SpiWriteData(0x24);
SH1282_SpiWriteCmd(0xB701);SH1282_SpiWriteData(0x24);
SH1282_SpiWriteCmd(0xB702);SH1282_SpiWriteData(0x24);

SH1282_SpiWriteCmd(0xB300);SH1282_SpiWriteData(0x08);
SH1282_SpiWriteCmd(0xB301);SH1282_SpiWriteData(0x08);
SH1282_SpiWriteCmd(0xB302);SH1282_SpiWriteData(0x08);

SH1282_SpiWriteCmd(0xB900);SH1282_SpiWriteData(0x34);
SH1282_SpiWriteCmd(0xB901);SH1282_SpiWriteData(0x34);
SH1282_SpiWriteCmd(0xB902);SH1282_SpiWriteData(0x34);

SH1282_SpiWriteCmd(0xBF00);SH1282_SpiWriteData(0x01);

SH1282_SpiWriteCmd(0xB500);SH1282_SpiWriteData(0x0B);
SH1282_SpiWriteCmd(0xB501);SH1282_SpiWriteData(0x0B);
SH1282_SpiWriteCmd(0xB502);SH1282_SpiWriteData(0x0B);

SH1282_SpiWriteCmd(0xBa00);SH1282_SpiWriteData(0x24);
SH1282_SpiWriteCmd(0xBa01);SH1282_SpiWriteData(0x24);
SH1282_SpiWriteCmd(0xBa02);SH1282_SpiWriteData(0x24);

SH1282_SpiWriteCmd(0xBc00);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xBc01);SH1282_SpiWriteData(0x99);
SH1282_SpiWriteCmd(0xBc02);SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xBd00);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xBd01);SH1282_SpiWriteData(0x99);
SH1282_SpiWriteCmd(0xBd02);SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xBe00);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xBe01);SH1282_SpiWriteData(0x65);

SH1282_SpiWriteCmd(0xD100);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD101);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD102);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD103);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD104);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD105);SH1282_SpiWriteData(0x04);
SH1282_SpiWriteCmd(0xD106);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD107);SH1282_SpiWriteData(0x09);
SH1282_SpiWriteCmd(0xD108);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD109);SH1282_SpiWriteData(0x94);
SH1282_SpiWriteCmd(0xD10A);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD10B);SH1282_SpiWriteData(0xb9);
SH1282_SpiWriteCmd(0xD10C);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD10D);SH1282_SpiWriteData(0xd5);
SH1282_SpiWriteCmd(0xD10E);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD10F);SH1282_SpiWriteData(0x05);
SH1282_SpiWriteCmd(0xD110);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD111);SH1282_SpiWriteData(0x2a);
SH1282_SpiWriteCmd(0xD112);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD113);SH1282_SpiWriteData(0x63);
SH1282_SpiWriteCmd(0xD114);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD115);SH1282_SpiWriteData(0x92);
SH1282_SpiWriteCmd(0xD116);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD117);SH1282_SpiWriteData(0xda);
SH1282_SpiWriteCmd(0xD118);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD119);SH1282_SpiWriteData(0x15);
SH1282_SpiWriteCmd(0xD11A);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD11B);SH1282_SpiWriteData(0x17);
SH1282_SpiWriteCmd(0xD11C);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD11D);SH1282_SpiWriteData(0x4C);
SH1282_SpiWriteCmd(0xD11E);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD11F);SH1282_SpiWriteData(0x87);
SH1282_SpiWriteCmd(0xD120);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD121);SH1282_SpiWriteData(0xae);
SH1282_SpiWriteCmd(0xD122);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD123);SH1282_SpiWriteData(0xe5);
SH1282_SpiWriteCmd(0xD124);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD125);SH1282_SpiWriteData(0x0e);
SH1282_SpiWriteCmd(0xD126);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD127);SH1282_SpiWriteData(0x4c);
SH1282_SpiWriteCmd(0xD128);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD129);SH1282_SpiWriteData(0x79);
SH1282_SpiWriteCmd(0xD12A);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD12B);SH1282_SpiWriteData(0xb2);
SH1282_SpiWriteCmd(0xD12C);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD12D);SH1282_SpiWriteData(0xea);
SH1282_SpiWriteCmd(0xD12E);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD12F);SH1282_SpiWriteData(0xf1);
SH1282_SpiWriteCmd(0xD130);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD131);SH1282_SpiWriteData(0xf2);
SH1282_SpiWriteCmd(0xD132);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD133);SH1282_SpiWriteData(0xf3);

SH1282_SpiWriteCmd(0xD200);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD201);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD202);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD203);SH1282_SpiWriteData(0x32);
SH1282_SpiWriteCmd(0xD204);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD205);SH1282_SpiWriteData(0x5e);
SH1282_SpiWriteCmd(0xD206);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD207);SH1282_SpiWriteData(0xfb);
SH1282_SpiWriteCmd(0xD208);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD209);SH1282_SpiWriteData(0x94);
SH1282_SpiWriteCmd(0xD20A);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD20B);SH1282_SpiWriteData(0xb9);
SH1282_SpiWriteCmd(0xD20C);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD20D);SH1282_SpiWriteData(0xd5);
SH1282_SpiWriteCmd(0xD20E);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD20F);SH1282_SpiWriteData(0x05);
SH1282_SpiWriteCmd(0xD210);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD211);SH1282_SpiWriteData(0x2a);
SH1282_SpiWriteCmd(0xD212);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD213);SH1282_SpiWriteData(0x63);
SH1282_SpiWriteCmd(0xD214);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD215);SH1282_SpiWriteData(0x92);
SH1282_SpiWriteCmd(0xD216);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD217);SH1282_SpiWriteData(0xda);
SH1282_SpiWriteCmd(0xD218);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD219);SH1282_SpiWriteData(0x15);
SH1282_SpiWriteCmd(0xD21A);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD21B);SH1282_SpiWriteData(0x17);
SH1282_SpiWriteCmd(0xD21C);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD21D);SH1282_SpiWriteData(0x4C);
SH1282_SpiWriteCmd(0xD21E);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD21F);SH1282_SpiWriteData(0x87);
SH1282_SpiWriteCmd(0xD220);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD221);SH1282_SpiWriteData(0xae);
SH1282_SpiWriteCmd(0xD222);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD223);SH1282_SpiWriteData(0xe5);
SH1282_SpiWriteCmd(0xD224);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD225);SH1282_SpiWriteData(0x0e);
SH1282_SpiWriteCmd(0xD226);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD227);SH1282_SpiWriteData(0x4c);
SH1282_SpiWriteCmd(0xD228);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD229);SH1282_SpiWriteData(0x79);
SH1282_SpiWriteCmd(0xD22A);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD22B);SH1282_SpiWriteData(0xb2);
SH1282_SpiWriteCmd(0xD22C);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD22D);SH1282_SpiWriteData(0xea);
SH1282_SpiWriteCmd(0xD22E);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD22F);SH1282_SpiWriteData(0xf1);
SH1282_SpiWriteCmd(0xD230);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD231);SH1282_SpiWriteData(0xf2);
SH1282_SpiWriteCmd(0xD232);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD233);SH1282_SpiWriteData(0xf3);

SH1282_SpiWriteCmd(0xD500);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD501);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD502);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD503);SH1282_SpiWriteData(0x32);
SH1282_SpiWriteCmd(0xD504);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD505);SH1282_SpiWriteData(0x5e);
SH1282_SpiWriteCmd(0xD506);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD507);SH1282_SpiWriteData(0x7b);
SH1282_SpiWriteCmd(0xD508);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD509);SH1282_SpiWriteData(0x94);
SH1282_SpiWriteCmd(0xD50A);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD50B);SH1282_SpiWriteData(0xb9);
SH1282_SpiWriteCmd(0xD50C);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD50D);SH1282_SpiWriteData(0xd5);
SH1282_SpiWriteCmd(0xD50E);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD50F);SH1282_SpiWriteData(0x05);
SH1282_SpiWriteCmd(0xD510);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD511);SH1282_SpiWriteData(0x2a);
SH1282_SpiWriteCmd(0xD512);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD513);SH1282_SpiWriteData(0x63);
SH1282_SpiWriteCmd(0xD514);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD515);SH1282_SpiWriteData(0x92);
SH1282_SpiWriteCmd(0xD516);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD517);SH1282_SpiWriteData(0xda);
SH1282_SpiWriteCmd(0xD518);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD519);SH1282_SpiWriteData(0x15);
SH1282_SpiWriteCmd(0xD51A);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD51B);SH1282_SpiWriteData(0x17);
SH1282_SpiWriteCmd(0xD51C);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD51D);SH1282_SpiWriteData(0x4C);
SH1282_SpiWriteCmd(0xD51E);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD51F);SH1282_SpiWriteData(0x87);
SH1282_SpiWriteCmd(0xD520);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD521);SH1282_SpiWriteData(0xae);
SH1282_SpiWriteCmd(0xD522);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD523);SH1282_SpiWriteData(0xe5);
SH1282_SpiWriteCmd(0xD524);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD525);SH1282_SpiWriteData(0x0e);
SH1282_SpiWriteCmd(0xD526);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD527);SH1282_SpiWriteData(0x4c);
SH1282_SpiWriteCmd(0xD528);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD529);SH1282_SpiWriteData(0x79);
SH1282_SpiWriteCmd(0xD52A);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD52B);SH1282_SpiWriteData(0xb2);
SH1282_SpiWriteCmd(0xD52C);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD52D);SH1282_SpiWriteData(0xea);
SH1282_SpiWriteCmd(0xD52E);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD52F);SH1282_SpiWriteData(0xf1);
SH1282_SpiWriteCmd(0xD530);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD531);SH1282_SpiWriteData(0xf2);
SH1282_SpiWriteCmd(0xD532);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD533);SH1282_SpiWriteData(0xf3);

SH1282_SpiWriteCmd(0xD300);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD301);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD302);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD303);SH1282_SpiWriteData(0x32);
SH1282_SpiWriteCmd(0xD304);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD305);SH1282_SpiWriteData(0x5e);
SH1282_SpiWriteCmd(0xD306);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD307);SH1282_SpiWriteData(0x7b);
SH1282_SpiWriteCmd(0xD308);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD309);SH1282_SpiWriteData(0x94);
SH1282_SpiWriteCmd(0xD30A);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD30B);SH1282_SpiWriteData(0xb9);
SH1282_SpiWriteCmd(0xD30C);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD30D);SH1282_SpiWriteData(0xd5);
SH1282_SpiWriteCmd(0xD30E);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD30F);SH1282_SpiWriteData(0x05);
SH1282_SpiWriteCmd(0xD310);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD311);SH1282_SpiWriteData(0x2a);
SH1282_SpiWriteCmd(0xD312);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD313);SH1282_SpiWriteData(0x63);
SH1282_SpiWriteCmd(0xD314);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD315);SH1282_SpiWriteData(0x92);
SH1282_SpiWriteCmd(0xD316);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD317);SH1282_SpiWriteData(0xda);
SH1282_SpiWriteCmd(0xD318);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD319);SH1282_SpiWriteData(0x15);
SH1282_SpiWriteCmd(0xD31A);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD31B);SH1282_SpiWriteData(0x17);
SH1282_SpiWriteCmd(0xD31C);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD31D);SH1282_SpiWriteData(0x4C);
SH1282_SpiWriteCmd(0xD31E);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD31F);SH1282_SpiWriteData(0x87);
SH1282_SpiWriteCmd(0xD320);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD321);SH1282_SpiWriteData(0xae);
SH1282_SpiWriteCmd(0xD322);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD323);SH1282_SpiWriteData(0xe5);
SH1282_SpiWriteCmd(0xD324);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD325);SH1282_SpiWriteData(0x0e);
SH1282_SpiWriteCmd(0xD326);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD327);SH1282_SpiWriteData(0x4c);
SH1282_SpiWriteCmd(0xD328);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD329);SH1282_SpiWriteData(0x79);
SH1282_SpiWriteCmd(0xD32A);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD32B);SH1282_SpiWriteData(0xb2);
SH1282_SpiWriteCmd(0xD32C);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD32D);SH1282_SpiWriteData(0xea);
SH1282_SpiWriteCmd(0xD32E);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD32F);SH1282_SpiWriteData(0xf1);
SH1282_SpiWriteCmd(0xD330);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD331);SH1282_SpiWriteData(0xf2);
SH1282_SpiWriteCmd(0xD332);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD333);SH1282_SpiWriteData(0xf3);

SH1282_SpiWriteCmd(0xD600);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD601);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD602);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD603);SH1282_SpiWriteData(0x32);
SH1282_SpiWriteCmd(0xD604);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD605);SH1282_SpiWriteData(0x5e);
SH1282_SpiWriteCmd(0xD606);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD607);SH1282_SpiWriteData(0x7b);
SH1282_SpiWriteCmd(0xD608);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD609);SH1282_SpiWriteData(0x94);
SH1282_SpiWriteCmd(0xD60A);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD60B);SH1282_SpiWriteData(0xb9);
SH1282_SpiWriteCmd(0xD60C);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xD60D);SH1282_SpiWriteData(0xd5);
SH1282_SpiWriteCmd(0xD60E);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD60F);SH1282_SpiWriteData(0x05);
SH1282_SpiWriteCmd(0xD610);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD611);SH1282_SpiWriteData(0x2a);
SH1282_SpiWriteCmd(0xD612);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD613);SH1282_SpiWriteData(0x63);
SH1282_SpiWriteCmd(0xD614);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD615);SH1282_SpiWriteData(0x92);
SH1282_SpiWriteCmd(0xD616);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xD617);SH1282_SpiWriteData(0xda);
SH1282_SpiWriteCmd(0xD618);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD619);SH1282_SpiWriteData(0x15);
SH1282_SpiWriteCmd(0xD61A);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD61B);SH1282_SpiWriteData(0x17);
SH1282_SpiWriteCmd(0xD61C);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD61D);SH1282_SpiWriteData(0x4C);
SH1282_SpiWriteCmd(0xD61E);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD61F);SH1282_SpiWriteData(0x87);
SH1282_SpiWriteCmd(0xD620);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD621);SH1282_SpiWriteData(0xae);
SH1282_SpiWriteCmd(0xD622);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xD623);SH1282_SpiWriteData(0xe5);
SH1282_SpiWriteCmd(0xD624);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD625);SH1282_SpiWriteData(0x0e);
SH1282_SpiWriteCmd(0xD626);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD627);SH1282_SpiWriteData(0x4c);
SH1282_SpiWriteCmd(0xD628);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD629);SH1282_SpiWriteData(0x79);
SH1282_SpiWriteCmd(0xD62A);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD62B);SH1282_SpiWriteData(0xb2);
SH1282_SpiWriteCmd(0xD62C);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD62D);SH1282_SpiWriteData(0xea);
SH1282_SpiWriteCmd(0xD62E);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD62F);SH1282_SpiWriteData(0xf1);
SH1282_SpiWriteCmd(0xD630);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD631);SH1282_SpiWriteData(0xf2);
SH1282_SpiWriteCmd(0xD632);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xD633);SH1282_SpiWriteData(0xf3);

SH1282_SpiWriteCmd(0xF000);SH1282_SpiWriteData(0x55);
SH1282_SpiWriteCmd(0xF001);SH1282_SpiWriteData(0xAA);
SH1282_SpiWriteCmd(0xF002);SH1282_SpiWriteData(0x52);
SH1282_SpiWriteCmd(0xF003);SH1282_SpiWriteData(0x08);
SH1282_SpiWriteCmd(0xF004);SH1282_SpiWriteData(0x00);


SH1282_SpiWriteCmd(0xB000);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xB001);SH1282_SpiWriteData(0x1E);
SH1282_SpiWriteCmd(0xB002);SH1282_SpiWriteData(0x0A);
SH1282_SpiWriteCmd(0xB003);SH1282_SpiWriteData(0x5A);
SH1282_SpiWriteCmd(0xB004);SH1282_SpiWriteData(0x0A);

SH1282_SpiWriteCmd(0xB400);SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xB500);SH1282_SpiWriteData(0x50);

SH1282_SpiWriteCmd(0xB600);SH1282_SpiWriteData(0x02);

SH1282_SpiWriteCmd(0xB700);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xB701);SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xB800);SH1282_SpiWriteData(0x01);
SH1282_SpiWriteCmd(0xB801);SH1282_SpiWriteData(0x05);
SH1282_SpiWriteCmd(0xB802);SH1282_SpiWriteData(0x05);
SH1282_SpiWriteCmd(0xB803);SH1282_SpiWriteData(0x05);

SH1282_SpiWriteCmd(0xBc00);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xBc01);SH1282_SpiWriteData(0x02);
SH1282_SpiWriteCmd(0xBc02);SH1282_SpiWriteData(0x02);

SH1282_SpiWriteCmd(0xcc00);SH1282_SpiWriteData(0x03);
SH1282_SpiWriteCmd(0xcc01);SH1282_SpiWriteData(0x00);
SH1282_SpiWriteCmd(0xcc02);SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0xB100);SH1282_SpiWriteData(0xF8);
SH1282_SpiWriteCmd(0xB101);SH1282_SpiWriteData(0x00);


SH1282_SpiWriteCmd(0x3600);SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0x3500);SH1282_SpiWriteData(0x00);

SH1282_SpiWriteCmd(0x3a00);SH1282_SpiWriteData(0x55);//0x77 24bit


//SH1282_SpiWriteCmd(0x3000);SH1282_SpiWriteData(0x00);
//SH1282_SpiWriteCmd(0x3001);SH1282_SpiWriteData(0x00);
//SH1282_SpiWriteCmd(0x3002);SH1282_SpiWriteData(0x03);
//SH1282_SpiWriteCmd(0x3003);SH1282_SpiWriteData(0x1F);


SH1282_SpiWriteCmd(0x1100); 

mdelay(120);

SH1282_SpiWriteCmd(0x2900);                                                                                                             
mdelay(200);            
#endif


}

static int32_t SH1282_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	
	if(is_sleep==1){
		//Sleep In
		SH1282_SpiWriteCmd(0x2800);
		mdelay(120); 
		SH1282_SpiWriteCmd(0x1000);
		mdelay(10); 
	}else{
		//Sleep Out
		SH1282_SpiWriteCmd(0x1100);
		mdelay(120); 
		SH1282_SpiWriteCmd(0x2900);
		mdelay(10); 
	}

	return 0;
}




static int32_t SH1282_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	uint32_t *test_data[4] = {0};

		spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read; 
	

#if 0

	LCD_PRINT("zxdbg add -SH1282_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

	SH1282_SpiWriteCmd(0x2A00); 
	SH1282_SpiWriteData((left>>8));// set left address
	SH1282_SpiWriteData((left&0xff));
	SH1282_SpiWriteData((right>>8));// set right address
	SH1282_SpiWriteData((right&0xff));

	SH1282_SpiWriteCmd(0x2B00); 
	SH1282_SpiWriteData((top>>8));// set left address
	SH1282_SpiWriteData((top&0xff));
	SH1282_SpiWriteData((bottom>>8));// set bottom address
	SH1282_SpiWriteData((bottom&0xff));

//	SH1282_SpiWriteCmd(0x2C00);

	SH1282_SpiWriteCmd(0x2A00); 
	spi_read(test_data);
	spi_read(test_data+1);
	SH1282_SpiWriteCmd(0x2B00); 
	spi_read(test_data+2);
	spi_read(test_data+3);
#endif
	LCD_PRINT("zxdbg add -SH1282_read read: %x, %x, %x, %x\n",test_data[0], test_data[1], test_data[2], test_data[3]);

	return 0;
}
static int32_t SH1282_invalidate(struct panel_spec *self)
{
	LCD_PRINT("SH1282_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t SH1282_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("SH1282_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t SH1282_read_id(struct panel_spec *self)
{

	int32_t id1=0,id2=0,id;

	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;

	
	SH1282_SpiWriteCmd(0xF000); SH1282_SpiWriteData(0x55);
	SH1282_SpiWriteCmd(0xF001); SH1282_SpiWriteData(0xAA);
	SH1282_SpiWriteCmd(0xF002); SH1282_SpiWriteData(0x52);
	SH1282_SpiWriteCmd(0xF003); SH1282_SpiWriteData(0x08);
	SH1282_SpiWriteCmd(0xF004); SH1282_SpiWriteData(0x01);

			SH1282_SpiWriteCmd(0xC600);
	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
			spi_read(16,&id1);
	#else
		spi_read(&id1);
	#endif


			SH1282_SpiWriteCmd(0xC601);
	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
			spi_read(16,&id2);
	#else
			spi_read(&id2);
	#endif
		printk("PHC SH1282_read_id u-boot id = 0x%x, 0x%x   0x%x \n",id1,id2,id);
	if ((id1==0x12)&&(id2==0x12))
		return 0x1202;
	else
		return (id=(id1<<8)||id2);

}

static struct panel_operations lcd_SH1282_rgb_spi_operations = {
	.panel_init = SH1282_init,
	.panel_set_window = SH1282_set_window,
	.panel_invalidate_rect= SH1282_invalidate_rect,
	.panel_invalidate = SH1282_invalidate,
	.panel_enter_sleep = SH1282_enter_sleep,
	.panel_readid          = SH1282_read_id
};

static struct timing_rgb lcd_SH1282_rgb_timing = {
#if defined(CONFIG_ZYT_SH1282_7675_QIYANG)
	.hfp = 10,//10,  /* unit: pixel */
	.hbp = 80, //6,
	.hsync = 4, //8,
	.vfp = 10, // 3, /*unit: line*/
	.vbp = 8, // 3,
	.vsync = 4, // 4,
#else
	.hfp = 10,//10,  /* unit: pixel */
	.hbp = 80, //6,
	.hsync = 10, //8,
	.vfp = 10, // 3, /*unit: line*/
	.vbp = 20, // 3,
	.vsync = 10, // 4,
#endif
};

static struct spi_info lcd_SH1282_rgb_spi_info = {
	.ops = NULL,
};

static struct info_rgb lcd_SH1282_rgb_info = {
	.cmd_bus_mode  = SPRDFB_RGB_BUS_TYPE_SPI,
	.video_bus_width = 16, /*18,16*/
	.h_sync_pol = SPRDFB_POLARITY_NEG,
	.v_sync_pol = SPRDFB_POLARITY_NEG,
	.de_pol = SPRDFB_POLARITY_POS,
	.timing = &lcd_SH1282_rgb_timing,
	.bus_info = {
		.spi = &lcd_SH1282_rgb_spi_info,
	}
};

struct panel_spec lcd_panel_SH1282_rgb_spi_spec = {
	.width = 480,
	.height = 800,
#if defined(CONFIG_ZYT_SH1282_7675_QIYANG)
	.fps = 56,
#else
	.fps = 61,
#endif
	.type = LCD_MODE_RGB,
	.direction = LCD_DIRECT_NORMAL,
	.info = {
		.rgb = &lcd_SH1282_rgb_info
	},
	.ops = &lcd_SH1282_rgb_spi_operations,
};
struct panel_cfg lcd_sh1282_rgb_spi = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x1202,
	.lcd_name = "lcd_sh1282_rgb_spi",
	.panel = &lcd_panel_SH1282_rgb_spi_spec,
};
static int __init lcd_sh1282_rgb_spi_init(void)
{
	return sprdfb_panel_register(&lcd_sh1282_rgb_spi);
}

subsys_initcall(lcd_sh1282_rgb_spi_init);
