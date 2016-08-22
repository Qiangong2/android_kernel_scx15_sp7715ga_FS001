/* drivers/video/sc8810/lcd_RM68171_rgb_spi.c
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

#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
#define RM68171_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(16,(cmd & 0xFFFF));\
}

#define  RM68171_SpiWriteData(data)\
{ \
	spi_send_data(16,(data & 0xFFFF));\
}
#else
#define RM68171_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd((cmd & 0xFFFF));\
}

#define  RM68171_SpiWriteData(data)\
{ \
	spi_send_data((data & 0xFFFF));\
}
#endif

static int32_t RM68171_init(struct panel_spec *self)
{	
		uint32_t data = 0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read; 

	LCD_PRINT("kernel RM68171_init\n");

RM68171_SpiWriteCmd(0xF000);RM68171_SpiWriteData(0x55);
RM68171_SpiWriteCmd(0xF001);RM68171_SpiWriteData(0xAA);
RM68171_SpiWriteCmd(0xF002);RM68171_SpiWriteData(0x52);
RM68171_SpiWriteCmd(0xF003);RM68171_SpiWriteData(0x08);
RM68171_SpiWriteCmd(0xF004);RM68171_SpiWriteData(0x01);

					   
RM68171_SpiWriteCmd(0xB000);RM68171_SpiWriteData(0x0E);	
RM68171_SpiWriteCmd(0xB001);RM68171_SpiWriteData(0x0E);
RM68171_SpiWriteCmd(0xB002);RM68171_SpiWriteData(0x0E);


RM68171_SpiWriteCmd(0xB100);RM68171_SpiWriteData(0x09);	
RM68171_SpiWriteCmd(0xB101);RM68171_SpiWriteData(0x09);
RM68171_SpiWriteCmd(0xB102);RM68171_SpiWriteData(0x09);



RM68171_SpiWriteCmd(0xB600);RM68171_SpiWriteData(0x34);	
RM68171_SpiWriteCmd(0xB601);RM68171_SpiWriteData(0x34);
RM68171_SpiWriteCmd(0xB602);RM68171_SpiWriteData(0x34);


RM68171_SpiWriteCmd(0xB700);RM68171_SpiWriteData(0x34);	
RM68171_SpiWriteCmd(0xB701);RM68171_SpiWriteData(0x34);
RM68171_SpiWriteCmd(0xB702);RM68171_SpiWriteData(0x34);

RM68171_SpiWriteCmd(0xB800);RM68171_SpiWriteData(0x24);	
RM68171_SpiWriteCmd(0xB801);RM68171_SpiWriteData(0x24);
RM68171_SpiWriteCmd(0xB802);RM68171_SpiWriteData(0x24);


RM68171_SpiWriteCmd(0xB900);RM68171_SpiWriteData(0x34);	
RM68171_SpiWriteCmd(0xB901);RM68171_SpiWriteData(0x34);
RM68171_SpiWriteCmd(0xB902);RM68171_SpiWriteData(0x34);


RM68171_SpiWriteCmd(0xBA00);RM68171_SpiWriteData(0x04);	
RM68171_SpiWriteCmd(0xBA01);RM68171_SpiWriteData(0x04);
RM68171_SpiWriteCmd(0xBA02);RM68171_SpiWriteData(0x04);


RM68171_SpiWriteCmd(0xBC00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xBC01);RM68171_SpiWriteData(0x78); //90
RM68171_SpiWriteCmd(0xBC02); RM68171_SpiWriteData(0x13);  //00


RM68171_SpiWriteCmd(0xBD00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xBD01);RM68171_SpiWriteData(0x78);  
RM68171_SpiWriteCmd(0xBD02);RM68171_SpiWriteData(0x13);  


RM68171_SpiWriteCmd(0xBE00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xBE01);RM68171_SpiWriteData(0x70);  //75


//Gamma Setting
RM68171_SpiWriteCmd(0xD100);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD101);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD102);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD103);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD104);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD105);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD106);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD107);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD108);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD109);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD10A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD10B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD10C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD10D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD10E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD10F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD110);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD111);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD112);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD113);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD114);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD115);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD116);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD117);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD118);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD119);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD11A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD11B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD11C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD11D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD11E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD11F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD120);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD121);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD122);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD123);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD124);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD125);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD126);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD127);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD128);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD129);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD12A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD12B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD12C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD12D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD12E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD12F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD130);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD131);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD132);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD133);RM68171_SpiWriteData(0xFD);


RM68171_SpiWriteCmd(0xD200);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD201);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD202);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD203);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD204);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD205);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD206);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD207);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD208);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD209);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD20A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD20B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD20C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD20D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD20E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD20F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD210);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD211);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD212);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD213);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD214);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD215);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD216);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD217);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD218);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD219);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD21A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD21B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD21C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD21D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD21E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD21F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD220);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD221);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD222);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD223);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD224);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD225);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD226);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD227);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD228);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD229);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD22A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD22B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD22C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD22D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD22E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD22F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD230);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD231);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD232);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD233);RM68171_SpiWriteData(0xFD);



RM68171_SpiWriteCmd(0xD300);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD301);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD302);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD303);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD304);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD305);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD306);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD307);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD308);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD309);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD30A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD30B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD30C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD30D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD30E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD30F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD310);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD311);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD312);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD313);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD314);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD315);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD316);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD317);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD318);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD319);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD31A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD31B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD31C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD31D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD31E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD31F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD320);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD321);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD322);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD323);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD324);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD325);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD326);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD327);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD328);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD329);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD32A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD32B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD32C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD32D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD32E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD32F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD330);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD331);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD332);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD333);RM68171_SpiWriteData(0xFD);



RM68171_SpiWriteCmd(0xD400);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD401);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD402);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD403);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD404);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD405);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD406);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD407);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD408);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD409);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD40A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD40B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD40C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD40D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD40E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD40F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD410);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD411);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD412);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD413);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD414);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD415);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD416);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD417);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD418);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD419);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD41A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD41B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD41C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD41D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD41E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD41F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD420);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD421);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD422);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD423);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD424);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD425);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD426);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD427);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD428);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD429);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD42A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD42B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD42C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD42D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD42E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD42F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD430);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD431);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD432);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD433);RM68171_SpiWriteData(0xFD);


RM68171_SpiWriteCmd(0xD500);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD501);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD502);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD503);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD504);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD505);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD506);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD507);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD508);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD509);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD50A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD50B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD50C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD50D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD50E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD50F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD510);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD511);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD512);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD513);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD514);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD515);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD516);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD517);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD518);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD519);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD51A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD51B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD51C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD51D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD51E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD51F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD520);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD521);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD522);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD523);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD524);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD525);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD526);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD527);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD528);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD529);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD52A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD52B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD52C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD52D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD52E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD52F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD530);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD531);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD532);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD533);RM68171_SpiWriteData(0xFD);

		   
					   
RM68171_SpiWriteCmd(0xD600);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD601);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD602);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD603);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD604);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD605);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD606);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD607);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD608);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD609);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD60A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD60B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD60C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD60D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD60E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD60F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD610);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD611);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD612);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD613);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD614);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD615);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD616);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD617);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD618);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD619);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD61A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD61B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD61C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD61D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD61E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD61F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD620);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD621);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD622);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD623);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD624);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD625);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD626);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD627);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD628);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD629);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD62A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD62B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD62C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD62D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD62E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD62F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD630);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD631);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD632);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD633);RM68171_SpiWriteData(0xFD);

RM68171_SpiWriteCmd(0xF000);RM68171_SpiWriteData(0x55);
RM68171_SpiWriteCmd(0xF001);RM68171_SpiWriteData(0xAA);
RM68171_SpiWriteCmd(0xF002);RM68171_SpiWriteData(0x52);
RM68171_SpiWriteCmd(0xF003);RM68171_SpiWriteData(0x08);
RM68171_SpiWriteCmd(0xF004);RM68171_SpiWriteData(0x00);


RM68171_SpiWriteCmd(0xB000);RM68171_SpiWriteData(0x00);  
RM68171_SpiWriteCmd(0xB001);RM68171_SpiWriteData(0x1c);	//1C
RM68171_SpiWriteCmd(0xB002);RM68171_SpiWriteData(0x1c); 
RM68171_SpiWriteCmd(0xB003);RM68171_SpiWriteData(0x10);  	
RM68171_SpiWriteCmd(0xB004);RM68171_SpiWriteData(0x10);

RM68171_SpiWriteCmd(0xB400); RM68171_SpiWriteData(0x10); 

RM68171_SpiWriteCmd(0xB600); RM68171_SpiWriteData(0x02); 

RM68171_SpiWriteCmd(0xB700); RM68171_SpiWriteData(0x22); 
RM68171_SpiWriteCmd(0xB701); RM68171_SpiWriteData(0x22); 


RM68171_SpiWriteCmd(0xB800);RM68171_SpiWriteData(0x01); 
RM68171_SpiWriteCmd(0xB801); RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xB802);RM68171_SpiWriteData(0x03); 
RM68171_SpiWriteCmd(0xB803);RM68171_SpiWriteData(0x03);  
					   
					   
RM68171_SpiWriteCmd(0xBC00);RM68171_SpiWriteData(0x02);  
RM68171_SpiWriteCmd(0xBC01);RM68171_SpiWriteData(0x02);  
RM68171_SpiWriteCmd(0xBC02);RM68171_SpiWriteData(0x02); 					   
					   
RM68171_SpiWriteCmd(0xBA00); RM68171_SpiWriteData(0x01);

RM68171_SpiWriteCmd(0xBD00);RM68171_SpiWriteData(0x01);	
RM68171_SpiWriteCmd(0xBD01);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBD02);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xBD03);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBD04);RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0xBE00);RM68171_SpiWriteData(0x01);	
RM68171_SpiWriteCmd(0xBE01);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBE02);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xBE03);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBE04);RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0xBF00);RM68171_SpiWriteData(0x01);	
RM68171_SpiWriteCmd(0xBF01);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBF02);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xBF03);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBF04);RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0xC900);RM68171_SpiWriteData(0xC0);  
RM68171_SpiWriteCmd(0xC901);RM68171_SpiWriteData(0x01); 

RM68171_SpiWriteCmd(0xF000);RM68171_SpiWriteData(0x55);
RM68171_SpiWriteCmd(0xF001);RM68171_SpiWriteData(0xAA);
RM68171_SpiWriteCmd(0xF002);RM68171_SpiWriteData(0x52);
RM68171_SpiWriteCmd(0xF003);RM68171_SpiWriteData(0x08);
RM68171_SpiWriteCmd(0xF004);RM68171_SpiWriteData(0x02);


RM68171_SpiWriteCmd(0xFE00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xFE01);RM68171_SpiWriteData(0x80);
RM68171_SpiWriteCmd(0xFE02);RM68171_SpiWriteData(0x09);
RM68171_SpiWriteCmd(0xFE03);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xFE04);RM68171_SpiWriteData(0x8D);

RM68171_SpiWriteCmd(0xF600);RM68171_SpiWriteData(0x60);

RM68171_SpiWriteCmd(0x3500);
RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0x3A00);
RM68171_SpiWriteData(0x55); // 0x66


//SLPOUT
RM68171_SpiWriteCmd(0x1100);
//RM68171_SpiWriteData(0x00);
mdelay(100);
//DISPLAY ON
RM68171_SpiWriteCmd(0x2900);
//RM68171_SpiWriteData(0x00);

mdelay(50);
//RM68171_SpiWriteCmd(0x2C00);
//mdelay(200);   

}

static int32_t RM68171_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	
	LCD_PRINT("kernel RM68171_enter_sleep: is_sleep = %d\n",is_sleep);
	
	if(is_sleep==1){
		//Sleep In
		
		RM68171_SpiWriteCmd(0x0100);
		mdelay(50); 
		RM68171_SpiWriteCmd(0x2800);
		mdelay(50); 
		RM68171_SpiWriteCmd(0x1000);
		mdelay(10); 
	}else{
		//Sleep Out
		//# if 1
		//self->ops->panel_reset(self);
		//self->ops->panel_init(self);
		//#else
		RM68171_SpiWriteCmd(0x1100);
		mdelay(120); 
		RM68171_SpiWriteCmd(0x2900);
		mdelay(10); 
		//#endif
#if 0
		RM68171_SpiWriteCmd(0x0100);
		mdelay(120); 
		RM68171_SpiWriteCmd(0xF000);RM68171_SpiWriteData(0x55);
RM68171_SpiWriteCmd(0xF001);RM68171_SpiWriteData(0xAA);
RM68171_SpiWriteCmd(0xF002);RM68171_SpiWriteData(0x52);
RM68171_SpiWriteCmd(0xF003);RM68171_SpiWriteData(0x08);
RM68171_SpiWriteCmd(0xF004);RM68171_SpiWriteData(0x01);

					   
RM68171_SpiWriteCmd(0xB000);RM68171_SpiWriteData(0x0E);	
RM68171_SpiWriteCmd(0xB001);RM68171_SpiWriteData(0x0E);
RM68171_SpiWriteCmd(0xB002);RM68171_SpiWriteData(0x0E);


RM68171_SpiWriteCmd(0xB100);RM68171_SpiWriteData(0x09);	
RM68171_SpiWriteCmd(0xB101);RM68171_SpiWriteData(0x09);
RM68171_SpiWriteCmd(0xB102);RM68171_SpiWriteData(0x09);



RM68171_SpiWriteCmd(0xB600);RM68171_SpiWriteData(0x34);	
RM68171_SpiWriteCmd(0xB601);RM68171_SpiWriteData(0x34);
RM68171_SpiWriteCmd(0xB602);RM68171_SpiWriteData(0x34);


RM68171_SpiWriteCmd(0xB700);RM68171_SpiWriteData(0x34);	
RM68171_SpiWriteCmd(0xB701);RM68171_SpiWriteData(0x34);
RM68171_SpiWriteCmd(0xB702);RM68171_SpiWriteData(0x34);

RM68171_SpiWriteCmd(0xB800);RM68171_SpiWriteData(0x24);	
RM68171_SpiWriteCmd(0xB801);RM68171_SpiWriteData(0x24);
RM68171_SpiWriteCmd(0xB802);RM68171_SpiWriteData(0x24);


RM68171_SpiWriteCmd(0xB900);RM68171_SpiWriteData(0x34);	
RM68171_SpiWriteCmd(0xB901);RM68171_SpiWriteData(0x34);
RM68171_SpiWriteCmd(0xB902);RM68171_SpiWriteData(0x34);


RM68171_SpiWriteCmd(0xBA00);RM68171_SpiWriteData(0x04);	
RM68171_SpiWriteCmd(0xBA01);RM68171_SpiWriteData(0x04);
RM68171_SpiWriteCmd(0xBA02);RM68171_SpiWriteData(0x04);


RM68171_SpiWriteCmd(0xBC00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xBC01);RM68171_SpiWriteData(0x78); //90
RM68171_SpiWriteCmd(0xBC02); RM68171_SpiWriteData(0x13);  //00


RM68171_SpiWriteCmd(0xBD00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xBD01);RM68171_SpiWriteData(0x78);  
RM68171_SpiWriteCmd(0xBD02);RM68171_SpiWriteData(0x13);  


RM68171_SpiWriteCmd(0xBE00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xBE01);RM68171_SpiWriteData(0x70);  //75


//Gamma Setting
RM68171_SpiWriteCmd(0xD100);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD101);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD102);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD103);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD104);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD105);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD106);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD107);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD108);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD109);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD10A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD10B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD10C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD10D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD10E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD10F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD110);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD111);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD112);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD113);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD114);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD115);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD116);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD117);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD118);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD119);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD11A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD11B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD11C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD11D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD11E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD11F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD120);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD121);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD122);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD123);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD124);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD125);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD126);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD127);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD128);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD129);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD12A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD12B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD12C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD12D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD12E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD12F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD130);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD131);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD132);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD133);RM68171_SpiWriteData(0xFD);


RM68171_SpiWriteCmd(0xD200);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD201);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD202);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD203);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD204);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD205);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD206);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD207);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD208);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD209);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD20A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD20B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD20C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD20D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD20E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD20F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD210);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD211);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD212);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD213);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD214);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD215);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD216);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD217);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD218);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD219);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD21A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD21B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD21C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD21D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD21E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD21F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD220);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD221);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD222);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD223);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD224);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD225);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD226);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD227);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD228);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD229);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD22A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD22B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD22C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD22D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD22E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD22F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD230);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD231);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD232);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD233);RM68171_SpiWriteData(0xFD);



RM68171_SpiWriteCmd(0xD300);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD301);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD302);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD303);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD304);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD305);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD306);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD307);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD308);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD309);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD30A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD30B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD30C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD30D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD30E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD30F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD310);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD311);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD312);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD313);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD314);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD315);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD316);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD317);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD318);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD319);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD31A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD31B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD31C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD31D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD31E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD31F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD320);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD321);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD322);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD323);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD324);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD325);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD326);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD327);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD328);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD329);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD32A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD32B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD32C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD32D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD32E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD32F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD330);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD331);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD332);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD333);RM68171_SpiWriteData(0xFD);



RM68171_SpiWriteCmd(0xD400);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD401);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD402);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD403);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD404);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD405);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD406);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD407);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD408);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD409);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD40A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD40B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD40C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD40D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD40E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD40F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD410);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD411);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD412);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD413);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD414);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD415);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD416);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD417);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD418);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD419);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD41A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD41B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD41C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD41D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD41E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD41F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD420);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD421);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD422);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD423);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD424);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD425);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD426);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD427);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD428);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD429);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD42A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD42B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD42C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD42D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD42E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD42F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD430);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD431);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD432);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD433);RM68171_SpiWriteData(0xFD);


RM68171_SpiWriteCmd(0xD500);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD501);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD502);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD503);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD504);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD505);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD506);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD507);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD508);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD509);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD50A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD50B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD50C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD50D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD50E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD50F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD510);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD511);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD512);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD513);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD514);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD515);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD516);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD517);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD518);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD519);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD51A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD51B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD51C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD51D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD51E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD51F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD520);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD521);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD522);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD523);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD524);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD525);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD526);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD527);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD528);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD529);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD52A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD52B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD52C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD52D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD52E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD52F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD530);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD531);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD532);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD533);RM68171_SpiWriteData(0xFD);

		   
					   
RM68171_SpiWriteCmd(0xD600);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD601);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD602);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD603);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xD604);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD605);RM68171_SpiWriteData(0x21);
RM68171_SpiWriteCmd(0xD606);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD607);RM68171_SpiWriteData(0x43);
RM68171_SpiWriteCmd(0xD608);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD609);RM68171_SpiWriteData(0x6D);
RM68171_SpiWriteCmd(0xD60A);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD60B);RM68171_SpiWriteData(0xAE);
RM68171_SpiWriteCmd(0xD60C);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xD60D);RM68171_SpiWriteData(0xDD);
RM68171_SpiWriteCmd(0xD60E);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD60F);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD610);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD611);RM68171_SpiWriteData(0x4D);
RM68171_SpiWriteCmd(0xD612);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD613);RM68171_SpiWriteData(0x8A);
RM68171_SpiWriteCmd(0xD614);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD615);RM68171_SpiWriteData(0xB4);
RM68171_SpiWriteCmd(0xD616);RM68171_SpiWriteData(0x01);
RM68171_SpiWriteCmd(0xD617);RM68171_SpiWriteData(0xF1);
RM68171_SpiWriteCmd(0xD618);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD619);RM68171_SpiWriteData(0x1F);
RM68171_SpiWriteCmd(0xD61A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD61B);RM68171_SpiWriteData(0x20);
RM68171_SpiWriteCmd(0xD61C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD61D);RM68171_SpiWriteData(0x48);
RM68171_SpiWriteCmd(0xD61E);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD61F);RM68171_SpiWriteData(0x71);
RM68171_SpiWriteCmd(0xD620);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD621);RM68171_SpiWriteData(0x87);
RM68171_SpiWriteCmd(0xD622);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD623);RM68171_SpiWriteData(0xA2);
RM68171_SpiWriteCmd(0xD624);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD625);RM68171_SpiWriteData(0xB3);
RM68171_SpiWriteCmd(0xD626);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD627);RM68171_SpiWriteData(0xCB);
RM68171_SpiWriteCmd(0xD628);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD629);RM68171_SpiWriteData(0xD5);
RM68171_SpiWriteCmd(0xD62A);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD62B);RM68171_SpiWriteData(0xE9);
RM68171_SpiWriteCmd(0xD62C);RM68171_SpiWriteData(0x02);
RM68171_SpiWriteCmd(0xD62D);RM68171_SpiWriteData(0xF7);
RM68171_SpiWriteCmd(0xD62E);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD62F);RM68171_SpiWriteData(0x0D);
RM68171_SpiWriteCmd(0xD630);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD631);RM68171_SpiWriteData(0x3F);
RM68171_SpiWriteCmd(0xD632);RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xD633);RM68171_SpiWriteData(0xFD);

RM68171_SpiWriteCmd(0xF000);RM68171_SpiWriteData(0x55);
RM68171_SpiWriteCmd(0xF001);RM68171_SpiWriteData(0xAA);
RM68171_SpiWriteCmd(0xF002);RM68171_SpiWriteData(0x52);
RM68171_SpiWriteCmd(0xF003);RM68171_SpiWriteData(0x08);
RM68171_SpiWriteCmd(0xF004);RM68171_SpiWriteData(0x00);


RM68171_SpiWriteCmd(0xB000);RM68171_SpiWriteData(0x00);  
RM68171_SpiWriteCmd(0xB001);RM68171_SpiWriteData(0x1c);	//1C
RM68171_SpiWriteCmd(0xB002);RM68171_SpiWriteData(0x1c); 
RM68171_SpiWriteCmd(0xB003);RM68171_SpiWriteData(0x10);  	
RM68171_SpiWriteCmd(0xB004);RM68171_SpiWriteData(0x10);

RM68171_SpiWriteCmd(0xB400); RM68171_SpiWriteData(0x10); 

RM68171_SpiWriteCmd(0xB600); RM68171_SpiWriteData(0x02); 

RM68171_SpiWriteCmd(0xB700); RM68171_SpiWriteData(0x22); 
RM68171_SpiWriteCmd(0xB701); RM68171_SpiWriteData(0x22); 


RM68171_SpiWriteCmd(0xB800);RM68171_SpiWriteData(0x01); 
RM68171_SpiWriteCmd(0xB801); RM68171_SpiWriteData(0x03);
RM68171_SpiWriteCmd(0xB802);RM68171_SpiWriteData(0x03); 
RM68171_SpiWriteCmd(0xB803);RM68171_SpiWriteData(0x03);  
					   
					   
RM68171_SpiWriteCmd(0xBC00);RM68171_SpiWriteData(0x02);  
RM68171_SpiWriteCmd(0xBC01);RM68171_SpiWriteData(0x02);  
RM68171_SpiWriteCmd(0xBC02);RM68171_SpiWriteData(0x02); 					   
					   
RM68171_SpiWriteCmd(0xBA00); RM68171_SpiWriteData(0x01);

RM68171_SpiWriteCmd(0xBD00);RM68171_SpiWriteData(0x01);	
RM68171_SpiWriteCmd(0xBD01);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBD02);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xBD03);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBD04);RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0xBE00);RM68171_SpiWriteData(0x01);	
RM68171_SpiWriteCmd(0xBE01);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBE02);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xBE03);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBE04);RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0xBF00);RM68171_SpiWriteData(0x01);	
RM68171_SpiWriteCmd(0xBF01);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBF02);RM68171_SpiWriteData(0x0C);
RM68171_SpiWriteCmd(0xBF03);RM68171_SpiWriteData(0x10);
RM68171_SpiWriteCmd(0xBF04);RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0xC900);RM68171_SpiWriteData(0xC0);  
RM68171_SpiWriteCmd(0xC901);RM68171_SpiWriteData(0x01); 

RM68171_SpiWriteCmd(0xF000);RM68171_SpiWriteData(0x55);
RM68171_SpiWriteCmd(0xF001);RM68171_SpiWriteData(0xAA);
RM68171_SpiWriteCmd(0xF002);RM68171_SpiWriteData(0x52);
RM68171_SpiWriteCmd(0xF003);RM68171_SpiWriteData(0x08);
RM68171_SpiWriteCmd(0xF004);RM68171_SpiWriteData(0x02);


RM68171_SpiWriteCmd(0xFE00);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xFE01);RM68171_SpiWriteData(0x80);
RM68171_SpiWriteCmd(0xFE02);RM68171_SpiWriteData(0x09);
RM68171_SpiWriteCmd(0xFE03);RM68171_SpiWriteData(0x00);
RM68171_SpiWriteCmd(0xFE04);RM68171_SpiWriteData(0x8D);

RM68171_SpiWriteCmd(0xF600);RM68171_SpiWriteData(0x60);

RM68171_SpiWriteCmd(0x3500);
RM68171_SpiWriteData(0x00);

RM68171_SpiWriteCmd(0x3A00);
RM68171_SpiWriteData(0x55); // 0x66


//SLPOUT
RM68171_SpiWriteCmd(0x1100);
//RM68171_SpiWriteData(0x00);
mdelay(100);
//DISPLAY ON
RM68171_SpiWriteCmd(0x2900);
//RM68171_SpiWriteData(0x00);

mdelay(50);
#endif
	}

	return 0;
}




static int32_t RM68171_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	uint32_t *test_data[4] = {0};
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;
#if 0

	LCD_PRINT("zxdbg add -RM68171_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

	RM68171_SpiWriteCmd(0x2A00); 
	RM68171_SpiWriteData((left>>8));// set left address
	RM68171_SpiWriteData((left&0xff));
	RM68171_SpiWriteData((right>>8));// set right address
	RM68171_SpiWriteData((right&0xff));

	RM68171_SpiWriteCmd(0x2B00); 
	RM68171_SpiWriteData((top>>8));// set left address
	RM68171_SpiWriteData((top&0xff));
	RM68171_SpiWriteData((bottom>>8));// set bottom address
	RM68171_SpiWriteData((bottom&0xff));

//	RM68171_SpiWriteCmd(0x2C00);

	RM68171_SpiWriteCmd(0x2A00); 
	spi_read(test_data);
	spi_read(test_data+1);
	RM68171_SpiWriteCmd(0x2B00); 
	spi_read(test_data+2);
	spi_read(test_data+3);
#endif
	LCD_PRINT("zxdbg add -RM68171_read read: %x, %x, %x, %x\n",test_data[0], test_data[1], test_data[2], test_data[3]);

	return 0;
}
static int32_t RM68171_invalidate(struct panel_spec *self)
{
	LCD_PRINT("RM68171_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t RM68171_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("RM68171_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t RM68171_read_id(struct panel_spec *self)
{
  int32_t ID1,ID2,ID;
	
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;

	LCD_PRINT("kernel RM68171_init\n");
	
//LV2 Page 1 enable
RM68171_SpiWriteCmd(0xF000); RM68171_SpiWriteData(0x55);
RM68171_SpiWriteCmd(0xF001); RM68171_SpiWriteData(0xAA);
RM68171_SpiWriteCmd(0xF002); RM68171_SpiWriteData(0x52);
RM68171_SpiWriteCmd(0xF003); RM68171_SpiWriteData(0x08);
RM68171_SpiWriteCmd(0xF004); RM68171_SpiWriteData(0x01);
     
              

      RM68171_SpiWriteCmd(0xC500); udelay(2000);//SH1282
      	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
      spi_read(16,&ID1); //0X71
	#else
      spi_read(&ID1); //0X71
	#endif
     RM68171_SpiWriteCmd(0xC501); udelay(2000);//SH1282  
        #if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
         spi_read(16,&ID2); // //0X81    
         #else
         spi_read(&ID2); // //0X81    
	 #endif
 
     ID=((ID2 & 0xff) << 8) | ID1; //0x8171
	LCD_PRINT("<<<<<<<<<<<<<<<<<<<<<<<<<rm68171>>>>>>>>>>>>>>>>\n");
	LCD_PRINT("RM68171_read_id ID==%x;  ID1=%x;    ID2====%x \n",ID,ID1,ID2);
if ((ID2==0x71)&&(ID1==0x71))
	return 0x8171;
else
 	return ID;

}

static struct panel_operations lcd_RM68171_rgb_spi_operations = {
	.panel_init = RM68171_init,
	.panel_set_window = RM68171_set_window,
	.panel_invalidate_rect= RM68171_invalidate_rect,
	.panel_invalidate = RM68171_invalidate,
	.panel_enter_sleep = RM68171_enter_sleep,
	.panel_readid          = RM68171_read_id
};

static struct timing_rgb lcd_RM68171_rgb_timing = {
	.hfp = 120,//10,  /* unit: pixel */
	.hbp = 22, //6,
	.hsync = 6, //8,
	.vfp = 4, // 3, /*unit: line*/
	.vbp = 8, // 3,
	.vsync = 4, // 4,
};

static struct spi_info lcd_RM68171_rgb_spi_info = {
	.ops = NULL,
};

static struct info_rgb lcd_RM68171_rgb_info = {
	.cmd_bus_mode  = SPRDFB_RGB_BUS_TYPE_SPI,
	.video_bus_width = 16, /*18,16*/
	.h_sync_pol = SPRDFB_POLARITY_NEG,
	.v_sync_pol = SPRDFB_POLARITY_NEG,
	.de_pol = SPRDFB_POLARITY_POS,
	.timing = &lcd_RM68171_rgb_timing,
	.bus_info = {
		.spi = &lcd_RM68171_rgb_spi_info,
	}
};

struct panel_spec lcd_panel_RM68171_rgb_spi_spec = {
	.width = 480,
	.height = 800,
	.reset_timing = {20, 10, 10},
	.fps = 61,

	.type = LCD_MODE_RGB,
	.direction = LCD_DIRECT_NORMAL,
	.is_clean_lcd = true,
	.info = {
		.rgb = &lcd_RM68171_rgb_info
	},
	.ops = &lcd_RM68171_rgb_spi_operations,
};
struct panel_cfg lcd_rm68171_rgb_spi = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x8171,
	.lcd_name = "lcd_rm68171_rgb_spi",
	.panel = &lcd_panel_RM68171_rgb_spi_spec,
};
static int __init lcd_rm68171_rgb_spi_init(void)
{
	return sprdfb_panel_register(&lcd_rm68171_rgb_spi);
}

subsys_initcall(lcd_rm68171_rgb_spi_init);