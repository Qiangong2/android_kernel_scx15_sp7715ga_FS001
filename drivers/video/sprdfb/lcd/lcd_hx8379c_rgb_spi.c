/* drivers/video/sc8810/lcd_hx8379c_rgb_spi.c
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
#if (CONFIG_ZYT_CUST == 64) && (CONFIG_ZYT_PRJ==7675)
#define CONFIG_ZYT_8379C_7675_JZY_LCD
#elif (CONFIG_ZYT_CUST == 3) && ((CONFIG_ZYT_PRJ==7271) || (CONFIG_ZYT_PRJ==7675))
#define CONFIG_ZYT_8379C_7271_CFZZ_LCD
#elif (CONFIG_ZYT_PRJ==7006) || (CONFIG_ZYT_PRJ==7011)
#define CONFIG_ZYT_8379C_FS006_CFZZ_BEILIJIA_LCD
#elif (CONFIG_ZYT_CUST == 8) && (CONFIG_ZYT_PRJ==7675)
#define CONFIG_ZYT_8379C_7675_JLD_HELITAI_LCD
#endif 
#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
#if defined(CONFIG_ZYT_8379C_7675_JZY_LCD) || defined(CONFIG_ZYT_8379C_FS006_CFZZ_BEILIJIA_LCD)
#define HX8379C_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(16,(cmd & 0xFFFF));\
}

#define  HX8379C_SpiWriteData(data)\
{ \
	spi_send_data(16,(data & 0xFFFF));\
}
#else
#define HX8379C_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(8,(cmd & 0xFF));\
}

#define  HX8379C_SpiWriteData(data)\
{ \
	spi_send_data(8,(data & 0xFF));\
}
#endif
#else
#define HX8379C_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd((cmd & 0xFF));\
}

#define  HX8379C_SpiWriteData(data)\
{ \
	spi_send_data((data & 0xFF));\
}
#endif

static int32_t hx8379c_init(struct panel_spec *self)
{	
	uint32_t data = 0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read; 

	printk("hx8379c_init------------------------------------------------\n");
#if defined(CONFIG_ZYT_8379C_7675_JZY_LCD)
HX8379C_SpiWriteCmd(0xB900);
HX8379C_SpiWriteData(0xFF);
HX8379C_SpiWriteCmd(0xB901);
HX8379C_SpiWriteData(0X83);
HX8379C_SpiWriteCmd(0xB902);
HX8379C_SpiWriteData(0X79);
// Set Power

HX8379C_SpiWriteCmd(0xB100);
HX8379C_SpiWriteData(0x44);
HX8379C_SpiWriteCmd(0xB101);
HX8379C_SpiWriteData(0x18); 
HX8379C_SpiWriteCmd(0xB102);
HX8379C_SpiWriteData(0X18); 
HX8379C_SpiWriteCmd(0xB103);
HX8379C_SpiWriteData(0X31); 
HX8379C_SpiWriteCmd(0xB104);
HX8379C_SpiWriteData(0X31); 
HX8379C_SpiWriteCmd(0xB105);
HX8379C_SpiWriteData(0X90); 
HX8379C_SpiWriteCmd(0xB106);
HX8379C_SpiWriteData(0XD0); 
HX8379C_SpiWriteCmd(0xB107);
HX8379C_SpiWriteData(0Xee); 
HX8379C_SpiWriteCmd(0xB108);
HX8379C_SpiWriteData(0X94); 
HX8379C_SpiWriteCmd(0xB109);
HX8379C_SpiWriteData(0X80); 
HX8379C_SpiWriteCmd(0xB10a);
HX8379C_SpiWriteData(0X38); 
HX8379C_SpiWriteCmd(0xB10b);
HX8379C_SpiWriteData(0X38); 
HX8379C_SpiWriteCmd(0xB10c);
HX8379C_SpiWriteData(0XF8); 
HX8379C_SpiWriteCmd(0xB10d);
HX8379C_SpiWriteData(0X22); 
HX8379C_SpiWriteCmd(0xB10e);
HX8379C_SpiWriteData(0X22); 
HX8379C_SpiWriteCmd(0xB10f);
HX8379C_SpiWriteData(0X22); 
HX8379C_SpiWriteCmd(0xB110);
HX8379C_SpiWriteData(0X00); 
HX8379C_SpiWriteCmd(0xB111);
HX8379C_SpiWriteData(0X80); 
HX8379C_SpiWriteCmd(0xB112);
HX8379C_SpiWriteData(0X30); 
HX8379C_SpiWriteCmd(0xB113);
HX8379C_SpiWriteData(0X00);
// Set Display

HX8379C_SpiWriteCmd(0xB200);
HX8379C_SpiWriteData(0x80); 
HX8379C_SpiWriteCmd(0xB201);
HX8379C_SpiWriteData(0x3c); 
HX8379C_SpiWriteCmd(0xB202);
HX8379C_SpiWriteData(0x0b); 
HX8379C_SpiWriteCmd(0xB203);
HX8379C_SpiWriteData(0x04); 
HX8379C_SpiWriteCmd(0xB204);
HX8379C_SpiWriteData(0x10); 
HX8379C_SpiWriteCmd(0xB205);
HX8379C_SpiWriteData(0x50); 
HX8379C_SpiWriteCmd(0xB206);
HX8379C_SpiWriteData(0x11); 
HX8379C_SpiWriteCmd(0xB207);
HX8379C_SpiWriteData(0x42); 
HX8379C_SpiWriteCmd(0xB208);
HX8379C_SpiWriteData(0x1D);
// Set CYC

HX8379C_SpiWriteCmd(0xB400);
HX8379C_SpiWriteData(0x01); 
HX8379C_SpiWriteCmd(0xB401);
HX8379C_SpiWriteData(0x9b);
HX8379C_SpiWriteCmd(0xB402);
HX8379C_SpiWriteData(0x01);
HX8379C_SpiWriteCmd(0xB403);
HX8379C_SpiWriteData(0x9b);
HX8379C_SpiWriteCmd(0xB404);
HX8379C_SpiWriteData(0x01);
HX8379C_SpiWriteCmd(0xB405);
HX8379C_SpiWriteData(0x9b);
HX8379C_SpiWriteCmd(0xB406);
HX8379C_SpiWriteData(0x12);
HX8379C_SpiWriteCmd(0xB407);
HX8379C_SpiWriteData(0xa0);
HX8379C_SpiWriteCmd(0xB408);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xB409);
HX8379C_SpiWriteData(0xa0);
// Set Offset

HX8379C_SpiWriteCmd(0xd200);
HX8379C_SpiWriteData(0x33);
// Set GIP_0

HX8379C_SpiWriteCmd(0xD300);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD301);
HX8379C_SpiWriteData(0x07); 
HX8379C_SpiWriteCmd(0xD302);
HX8379C_SpiWriteData(0x00); 
HX8379C_SpiWriteCmd(0xD303);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD304);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD305);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD306);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD307);
HX8379C_SpiWriteData(0x32);
HX8379C_SpiWriteCmd(0xD308);
HX8379C_SpiWriteData(0x10);
HX8379C_SpiWriteCmd(0xD309);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD30a);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD30b);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD30c);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD30d);
HX8379C_SpiWriteData(0x70);
HX8379C_SpiWriteCmd(0xD30e);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD30f);
HX8379C_SpiWriteData(0x70);
HX8379C_SpiWriteCmd(0xD310);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD311);
HX8379C_SpiWriteData(0x08);
HX8379C_SpiWriteCmd(0xD312);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD313);
HX8379C_SpiWriteData(0x08);
HX8379C_SpiWriteCmd(0xD314);
HX8379C_SpiWriteData(0x11);
HX8379C_SpiWriteCmd(0xD315);
HX8379C_SpiWriteData(0x11);
HX8379C_SpiWriteCmd(0xD316);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD317);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD318);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xD319);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD31a);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD31b);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xD31c);
HX8379C_SpiWriteData(0x09);
// Set GIP_1

HX8379C_SpiWriteCmd(0xD500);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD501);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD502);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD503);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD504);
HX8379C_SpiWriteData(0x01);
HX8379C_SpiWriteCmd(0xD505);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD506);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD507);
HX8379C_SpiWriteData(0x02);
HX8379C_SpiWriteCmd(0xD508);
HX8379C_SpiWriteData(0x21);
HX8379C_SpiWriteCmd(0xD509);
HX8379C_SpiWriteData(0x20);
HX8379C_SpiWriteCmd(0xD50a);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50b);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50c);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50d);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50e);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50f);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD510);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD511);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD512);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD513);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD514);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD515);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD516);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD517);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD518);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD519);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51a);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51b);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51c);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51d);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51e);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51f);
HX8379C_SpiWriteData(0x18);
// Set GIP_2

HX8379C_SpiWriteCmd(0xD600);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD601);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD602);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD603);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD604);
HX8379C_SpiWriteData(0x02);
HX8379C_SpiWriteCmd(0xD605);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD606);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD607);
HX8379C_SpiWriteData(0x01);
HX8379C_SpiWriteCmd(0xD608);
HX8379C_SpiWriteData(0x20);
HX8379C_SpiWriteCmd(0xD609);
HX8379C_SpiWriteData(0x21);
HX8379C_SpiWriteCmd(0xD60a);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD60b);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD60c);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD60d);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD60e);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD60f);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD610);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD611);

HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD612);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD613);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD614);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD615);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD616);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD617);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD618);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD619);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61a);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61b);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61c);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61d);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61e);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61f);
HX8379C_SpiWriteData(0x18);
// Set Gamma
  	 ////////////////////////////////////////
HX8379C_SpiWriteCmd(0xE000);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE001);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE002);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xE003);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xE004);
HX8379C_SpiWriteData(0x1c);
HX8379C_SpiWriteCmd(0xE005);
HX8379C_SpiWriteData(0x3f);

HX8379C_SpiWriteCmd(0xE006);
HX8379C_SpiWriteData(0x31);
HX8379C_SpiWriteCmd(0xE007);
HX8379C_SpiWriteData(0x3f);

HX8379C_SpiWriteCmd(0xE008);
HX8379C_SpiWriteData(0x09);
HX8379C_SpiWriteCmd(0xE009);
HX8379C_SpiWriteData(0x10);
HX8379C_SpiWriteCmd(0xE00a);
HX8379C_SpiWriteData(0x12);
HX8379C_SpiWriteCmd(0xE00b);
HX8379C_SpiWriteData(0x1a);
HX8379C_SpiWriteCmd(0xE00c);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE00d);
HX8379C_SpiWriteData(0x16);
HX8379C_SpiWriteCmd(0xE00e);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xE00f);
HX8379C_SpiWriteData(0x16);
HX8379C_SpiWriteCmd(0xE010);
HX8379C_SpiWriteData(0x15);
HX8379C_SpiWriteCmd(0xE011);
HX8379C_SpiWriteData(0x07);
HX8379C_SpiWriteCmd(0xE012);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE013);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE014);
HX8379C_SpiWriteData(0x18);

HX8379C_SpiWriteCmd(0xE015);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE016);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE017);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xE018);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xE019);
HX8379C_SpiWriteData(0x1b);
HX8379C_SpiWriteCmd(0xE01a);
HX8379C_SpiWriteData(0x3f);

HX8379C_SpiWriteCmd(0xE01b);
HX8379C_SpiWriteData(0x31);
HX8379C_SpiWriteCmd(0xE01c);
HX8379C_SpiWriteData(0x3f);

HX8379C_SpiWriteCmd(0xE01d);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE01f);
HX8379C_SpiWriteData(0x0f);
HX8379C_SpiWriteCmd(0xE020);
HX8379C_SpiWriteData(0x11);
HX8379C_SpiWriteCmd(0xE021);
HX8379C_SpiWriteData(0x1a);
HX8379C_SpiWriteCmd(0xE022);
HX8379C_SpiWriteData(0x11);
HX8379C_SpiWriteCmd(0xE023);
HX8379C_SpiWriteData(0x15);
HX8379C_SpiWriteCmd(0xE024);
HX8379C_SpiWriteData(0x17);
HX8379C_SpiWriteCmd(0xE025);
HX8379C_SpiWriteData(0x15);
HX8379C_SpiWriteCmd(0xE026);
HX8379C_SpiWriteData(0x16);
HX8379C_SpiWriteCmd(0xE027);
HX8379C_SpiWriteData(0x07);
HX8379C_SpiWriteCmd(0xE028);
HX8379C_SpiWriteData(0x12);
HX8379C_SpiWriteCmd(0xE029);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE02a);
HX8379C_SpiWriteData(0x17);

// Set VCOM

HX8379C_SpiWriteCmd(0xB600);
HX8379C_SpiWriteData(0x76);
HX8379C_SpiWriteCmd(0xB601);
HX8379C_SpiWriteData(0x76);


HX8379C_SpiWriteCmd(0x3a00);
HX8379C_SpiWriteData(0x55);

HX8379C_SpiWriteCmd(0xcc00);
HX8379C_SpiWriteData(0x02);

	HX8379C_SpiWriteCmd(0x1100);
	HX8379C_SpiWriteData(0x00);  // Sleep Out
		
		mdelay(150);
           		
		HX8379C_SpiWriteCmd(0x2900);
		HX8379C_SpiWriteData(0x00);
			
		mdelay(20);	
				HX8379C_SpiWriteCmd(0x2C00);
				HX8379C_SpiWriteData(0x00);  
						mdelay(20);	
#elif defined(CONFIG_ZYT_8379C_7271_CFZZ_LCD)
		HX8379C_SpiWriteCmd(0xB9);
		HX8379C_SpiWriteData(0xFF);
		HX8379C_SpiWriteData(0x83);    
		HX8379C_SpiWriteData(0x79);     
		 
		HX8379C_SpiWriteCmd(0xB1);
		HX8379C_SpiWriteData(0x44);// => PA1
		HX8379C_SpiWriteData(0x14);// => PA2  
		HX8379C_SpiWriteData(0x14);// => PA3	
		HX8379C_SpiWriteData(0x31);// => PA4
		HX8379C_SpiWriteData(0x53);// => PA5
		HX8379C_SpiWriteData(0x50);// => PA6
		HX8379C_SpiWriteData(0xD0);// => PA7
		HX8379C_SpiWriteData(0xEA);// => PA8
		HX8379C_SpiWriteData(0x50);// => PA9
		HX8379C_SpiWriteData(0x80);// => PA10
		HX8379C_SpiWriteData(0x38);// => PA11
		HX8379C_SpiWriteData(0x38);// => PA12
		HX8379C_SpiWriteData(0xF8);// => PA13
		HX8379C_SpiWriteData(0x22);// => PA14
		HX8379C_SpiWriteData(0x22);// => PA15
		HX8379C_SpiWriteData(0x22);// => PA16
		HX8379C_SpiWriteData(0x00);// => PA17
		HX8379C_SpiWriteData(0x80);// => PA18
		HX8379C_SpiWriteData(0x30);// => PA19
		HX8379C_SpiWriteData(0x00);// => PA20      

		HX8379C_SpiWriteCmd(0xB2);
		HX8379C_SpiWriteData(0x82);	//0x80 COLUM 0x82 2dot
		HX8379C_SpiWriteData(0x3C);    
		HX8379C_SpiWriteData(0x0F);    
		HX8379C_SpiWriteData(0x05); 
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x50);	
		HX8379C_SpiWriteData(0x11);    
		HX8379C_SpiWriteData(0x42);    
		HX8379C_SpiWriteData(0x1d);   

		HX8379C_SpiWriteCmd(0xB4);
		HX8379C_SpiWriteData(0x08);    
		HX8379C_SpiWriteData(0x78);    
		HX8379C_SpiWriteData(0x08);    
		HX8379C_SpiWriteData(0x78);   
		HX8379C_SpiWriteData(0x08);    
		HX8379C_SpiWriteData(0x78);
		HX8379C_SpiWriteData(0x22);    
		HX8379C_SpiWriteData(0x90);   
		HX8379C_SpiWriteData(0x23);    
		HX8379C_SpiWriteData(0x90);    
		         		
		HX8379C_SpiWriteCmd(0xD2);
		HX8379C_SpiWriteData(0x00);	

		HX8379C_SpiWriteCmd(0xD3);
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x07);
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x00);				
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x08);
		HX8379C_SpiWriteData(0x08);
		HX8379C_SpiWriteData(0x32);
		HX8379C_SpiWriteData(0x10);
		HX8379C_SpiWriteData(0x08);
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x08);
		HX8379C_SpiWriteData(0x03);
		HX8379C_SpiWriteData(0x2D);
		HX8379C_SpiWriteData(0x03);
		HX8379C_SpiWriteData(0x2D);
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x08);
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x08);
		HX8379C_SpiWriteData(0x37);
		HX8379C_SpiWriteData(0x33);
		HX8379C_SpiWriteData(0x0B);
		HX8379C_SpiWriteData(0x0B);
		HX8379C_SpiWriteData(0x27);
		HX8379C_SpiWriteData(0x0B);
		HX8379C_SpiWriteData(0x0B);
		HX8379C_SpiWriteData(0x27);
		HX8379C_SpiWriteData(0x0d);
		        		
		HX8379C_SpiWriteCmd(0xCC);
		HX8379C_SpiWriteData(0x02);//0x02正扫，0x06反扫

		HX8379C_SpiWriteCmd(0xD5);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x23);
		HX8379C_SpiWriteData(0x22);
		HX8379C_SpiWriteData(0x21);
		HX8379C_SpiWriteData(0x20);
		HX8379C_SpiWriteData(0x01);
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x03);
		HX8379C_SpiWriteData(0x02);
		HX8379C_SpiWriteData(0x05);
		HX8379C_SpiWriteData(0x04);
		HX8379C_SpiWriteData(0x07);
		HX8379C_SpiWriteData(0x06);
		HX8379C_SpiWriteData(0x25);
		HX8379C_SpiWriteData(0x24);
		HX8379C_SpiWriteData(0x27);
		HX8379C_SpiWriteData(0x26);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);

		HX8379C_SpiWriteCmd(0xD6);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x26);
		HX8379C_SpiWriteData(0x27);
		HX8379C_SpiWriteData(0x24);
		HX8379C_SpiWriteData(0x25);
		HX8379C_SpiWriteData(0x02);
		HX8379C_SpiWriteData(0x03);
		HX8379C_SpiWriteData(0x00);
		HX8379C_SpiWriteData(0x01);
		HX8379C_SpiWriteData(0x06);
		HX8379C_SpiWriteData(0x07);
		HX8379C_SpiWriteData(0x04);
		HX8379C_SpiWriteData(0x05);
		HX8379C_SpiWriteData(0x22);
		HX8379C_SpiWriteData(0x23);
		HX8379C_SpiWriteData(0x20);
		HX8379C_SpiWriteData(0x21);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);
		HX8379C_SpiWriteData(0x18);

		         		
		HX8379C_SpiWriteCmd(0xE0); 		  // gamma2.2
		HX8379C_SpiWriteData(0x00);// => PA1 
		HX8379C_SpiWriteData(0x05);// => PA2 
		HX8379C_SpiWriteData(0x12);// => PA3 
		HX8379C_SpiWriteData(0x10);// => PA4 
		HX8379C_SpiWriteData(0x13);// => PA5 
		HX8379C_SpiWriteData(0x3e);// => PA6 
		HX8379C_SpiWriteData(0x22);// => PA7 
		HX8379C_SpiWriteData(0x34);// => PA8 
		HX8379C_SpiWriteData(0x0B);// => PA9 
		HX8379C_SpiWriteData(0x0e);// => PA10
		HX8379C_SpiWriteData(0x10);// => PA11
		HX8379C_SpiWriteData(0x18);// => PA12
		HX8379C_SpiWriteData(0x10);// => PA13
		HX8379C_SpiWriteData(0x16);// => PA14
		HX8379C_SpiWriteData(0x18);// => PA15
		HX8379C_SpiWriteData(0x15);// => PA16
		HX8379C_SpiWriteData(0x15);// => PA17
		HX8379C_SpiWriteData(0x07);// => PA18
		HX8379C_SpiWriteData(0x10);// => PA19
		HX8379C_SpiWriteData(0x13);// => PA20
		HX8379C_SpiWriteData(0x16);// => PA21

		HX8379C_SpiWriteData(0x00);// => PA1 
		HX8379C_SpiWriteData(0x05);// => PA2 
		HX8379C_SpiWriteData(0x12);// => PA3 
		HX8379C_SpiWriteData(0x10);// => PA4 
		HX8379C_SpiWriteData(0x13);// => PA5 
		HX8379C_SpiWriteData(0x3e);// => PA6 
		HX8379C_SpiWriteData(0x22);// => PA7 
		HX8379C_SpiWriteData(0x34);// => PA8 
		HX8379C_SpiWriteData(0x0B);// => PA9 
		HX8379C_SpiWriteData(0x0e);// => PA10
		HX8379C_SpiWriteData(0x10);// => PA11
		HX8379C_SpiWriteData(0x18);// => PA12
		HX8379C_SpiWriteData(0x10);// => PA13
		HX8379C_SpiWriteData(0x16);// => PA14
		HX8379C_SpiWriteData(0x18);// => PA15
		HX8379C_SpiWriteData(0x15);// => PA16
		HX8379C_SpiWriteData(0x15);// => PA17
		HX8379C_SpiWriteData(0x07);// => PA18
		HX8379C_SpiWriteData(0x10);// => PA19
		HX8379C_SpiWriteData(0x13);// => PA20
		HX8379C_SpiWriteData(0x16);// => PA21


		HX8379C_SpiWriteCmd(0xB6);
		HX8379C_SpiWriteData(0x77);//0x92
		HX8379C_SpiWriteData(0x77); //0x92 



		HX8379C_SpiWriteCmd(0x3A);
		HX8379C_SpiWriteData(0x55); //16BIT

		HX8379C_SpiWriteCmd(0x11); //Sleep Out
		mdelay(150);
		HX8379C_SpiWriteCmd(0x29); //Display On
		mdelay(20);
#elif defined(CONFIG_ZYT_8379C_FS006_CFZZ_BEILIJIA_LCD)
HX8379C_SpiWriteCmd(0xB900);
HX8379C_SpiWriteData(0xFF);
HX8379C_SpiWriteCmd(0xB901);
HX8379C_SpiWriteData(0x83);
HX8379C_SpiWriteCmd(0xB902);
HX8379C_SpiWriteData(0x79);

        
HX8379C_SpiWriteCmd(0xB100);
HX8379C_SpiWriteData(0x44);
HX8379C_SpiWriteCmd(0xB101);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xB102);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xB103);
HX8379C_SpiWriteData(0x31);
HX8379C_SpiWriteCmd(0xB104);
HX8379C_SpiWriteData(0x31);
HX8379C_SpiWriteCmd(0xB105);
HX8379C_SpiWriteData(0x90);
HX8379C_SpiWriteCmd(0xB106);
HX8379C_SpiWriteData(0xD0);
HX8379C_SpiWriteCmd(0xB107);
HX8379C_SpiWriteData(0xEE);
HX8379C_SpiWriteCmd(0xB108);
HX8379C_SpiWriteData(0x94);
HX8379C_SpiWriteCmd(0xB109);
HX8379C_SpiWriteData(0x80);
HX8379C_SpiWriteCmd(0xB10A);
HX8379C_SpiWriteData(0x38);
HX8379C_SpiWriteCmd(0xB10B);
HX8379C_SpiWriteData(0x38);
HX8379C_SpiWriteCmd(0xB10C);
HX8379C_SpiWriteData(0xF8);
HX8379C_SpiWriteCmd(0xB10D);
HX8379C_SpiWriteData(0x22);
HX8379C_SpiWriteCmd(0xB10E);
HX8379C_SpiWriteData(0x22);
HX8379C_SpiWriteCmd(0xB10F);
HX8379C_SpiWriteData(0x22);
HX8379C_SpiWriteCmd(0xB110);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xB111);
HX8379C_SpiWriteData(0x80);
HX8379C_SpiWriteCmd(0xB112);
HX8379C_SpiWriteData(0x30);
HX8379C_SpiWriteCmd(0xB113);
HX8379C_SpiWriteData(0x00);

       
HX8379C_SpiWriteCmd(0xB200);
HX8379C_SpiWriteData(0x80);
HX8379C_SpiWriteCmd(0xB201);
HX8379C_SpiWriteData(0x3C);
HX8379C_SpiWriteCmd(0xB202);
HX8379C_SpiWriteData(0x0B);
HX8379C_SpiWriteCmd(0xB203);
HX8379C_SpiWriteData(0x04);
HX8379C_SpiWriteCmd(0xB204);
HX8379C_SpiWriteData(0x30);
HX8379C_SpiWriteCmd(0xB205);
HX8379C_SpiWriteData(0x50);
HX8379C_SpiWriteCmd(0xB206);
HX8379C_SpiWriteData(0x11);
HX8379C_SpiWriteCmd(0xB207);
HX8379C_SpiWriteData(0x42);
HX8379C_SpiWriteCmd(0xB208);
HX8379C_SpiWriteData(0x1D);

       
HX8379C_SpiWriteCmd(0xB300);
HX8379C_SpiWriteData(0x01);

        
HX8379C_SpiWriteCmd(0xB400);
HX8379C_SpiWriteData(0x50);
HX8379C_SpiWriteCmd(0xB401);
HX8379C_SpiWriteData(0x51);
HX8379C_SpiWriteCmd(0xB402);
HX8379C_SpiWriteData(0x50);
HX8379C_SpiWriteCmd(0xB403);
HX8379C_SpiWriteData(0x51);
HX8379C_SpiWriteCmd(0xB404);
HX8379C_SpiWriteData(0x50);
HX8379C_SpiWriteCmd(0xB405);
HX8379C_SpiWriteData(0x51);
HX8379C_SpiWriteCmd(0xB406);
HX8379C_SpiWriteData(0x12);
HX8379C_SpiWriteCmd(0xB407);
HX8379C_SpiWriteData(0xA0);
HX8379C_SpiWriteCmd(0xB408);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xB409);
HX8379C_SpiWriteData(0xA0);

      
HX8379C_SpiWriteCmd(0xD300);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD301);
HX8379C_SpiWriteData(0x07);
HX8379C_SpiWriteCmd(0xD302);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD303);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD304);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD305);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD306);
HX8379C_SpiWriteData(0x06);
HX8379C_SpiWriteCmd(0xD307);
HX8379C_SpiWriteData(0x32);
HX8379C_SpiWriteCmd(0xD308);
HX8379C_SpiWriteData(0x10);
HX8379C_SpiWriteCmd(0xD309);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xD30A);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD30B);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xD30C);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD30D);
HX8379C_SpiWriteData(0x6F);
HX8379C_SpiWriteCmd(0xD30E);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD30F);
HX8379C_SpiWriteData(0x6F);
HX8379C_SpiWriteCmd(0xD310);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD311);
HX8379C_SpiWriteData(0x07);
HX8379C_SpiWriteCmd(0xD312);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD313);
HX8379C_SpiWriteData(0x07);
HX8379C_SpiWriteCmd(0xD314);
HX8379C_SpiWriteData(0x21);
HX8379C_SpiWriteCmd(0xD315);
HX8379C_SpiWriteData(0x22);
HX8379C_SpiWriteCmd(0xD316);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xD317);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xD318);
HX8379C_SpiWriteData(0x23);
HX8379C_SpiWriteCmd(0xD319);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xD31A);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xD31B);
HX8379C_SpiWriteData(0x23);
HX8379C_SpiWriteCmd(0xD31C);
HX8379C_SpiWriteData(0x09);

       
HX8379C_SpiWriteCmd(0xD500);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD501);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD502);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD503);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD504);
HX8379C_SpiWriteData(0x01);
HX8379C_SpiWriteCmd(0xD505);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD506);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD507);
HX8379C_SpiWriteData(0x02);
HX8379C_SpiWriteCmd(0xD508);
HX8379C_SpiWriteData(0x21);
HX8379C_SpiWriteCmd(0xD509);
HX8379C_SpiWriteData(0x20);
HX8379C_SpiWriteCmd(0xD50A);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50B);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50C);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50D);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50E);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD50F);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD510);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD511);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD512);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD513);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD514);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD515);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD516);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD517);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD518);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD519);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51A);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51B);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51C);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51D);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51E);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD51F);
HX8379C_SpiWriteData(0x18);
        
HX8379C_SpiWriteCmd(0xD600);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD601);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD602);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD603);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD604);
HX8379C_SpiWriteData(0x02);
HX8379C_SpiWriteCmd(0xD605);
HX8379C_SpiWriteData(0x03);
HX8379C_SpiWriteCmd(0xD606);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xD607);
HX8379C_SpiWriteData(0x01);
HX8379C_SpiWriteCmd(0xD608);
HX8379C_SpiWriteData(0x20);
HX8379C_SpiWriteCmd(0xD609);
HX8379C_SpiWriteData(0x21);
HX8379C_SpiWriteCmd(0xD60A);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD60B);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xD60C);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD60D);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD60E);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD60F);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD610);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD611);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD612);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD613);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD614);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD615);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD616);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD617);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD618);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD619);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61A);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61B);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61C);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61D);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61E);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xD61F);
HX8379C_SpiWriteData(0x18);

        
HX8379C_SpiWriteCmd(0xE000);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE001);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE002);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xE003);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xE004);
HX8379C_SpiWriteData(0x1C);
HX8379C_SpiWriteCmd(0xE005);
HX8379C_SpiWriteData(0x3F);
HX8379C_SpiWriteCmd(0xE006);
HX8379C_SpiWriteData(0x31);
HX8379C_SpiWriteCmd(0xE007);
HX8379C_SpiWriteData(0x3F);
HX8379C_SpiWriteCmd(0xE008);
HX8379C_SpiWriteData(0x09);
HX8379C_SpiWriteCmd(0xE009);
HX8379C_SpiWriteData(0x10);
HX8379C_SpiWriteCmd(0xE00A);
HX8379C_SpiWriteData(0x12);
HX8379C_SpiWriteCmd(0xE00B);
HX8379C_SpiWriteData(0x1A);
HX8379C_SpiWriteCmd(0xE00C);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE00D);
HX8379C_SpiWriteData(0x16);
HX8379C_SpiWriteCmd(0xE00E);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xE00F);
HX8379C_SpiWriteData(0x16);
HX8379C_SpiWriteCmd(0xE010);
HX8379C_SpiWriteData(0x15);
HX8379C_SpiWriteCmd(0xE011);
HX8379C_SpiWriteData(0x07);
HX8379C_SpiWriteCmd(0xE012);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE013);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE014);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xE015);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE016);
HX8379C_SpiWriteData(0x00);
HX8379C_SpiWriteCmd(0xE017);
HX8379C_SpiWriteData(0x05);
HX8379C_SpiWriteCmd(0xE018);
HX8379C_SpiWriteData(0x18);
HX8379C_SpiWriteCmd(0xE019);
HX8379C_SpiWriteData(0x1C);
HX8379C_SpiWriteCmd(0xE01A);
HX8379C_SpiWriteData(0x3F);
HX8379C_SpiWriteCmd(0xE01B);
HX8379C_SpiWriteData(0x31);
HX8379C_SpiWriteCmd(0xE01C);
HX8379C_SpiWriteData(0x3F);
HX8379C_SpiWriteCmd(0xE01D);
HX8379C_SpiWriteData(0x09);
HX8379C_SpiWriteCmd(0xE01E);
HX8379C_SpiWriteData(0x10);
HX8379C_SpiWriteCmd(0xE01F);
HX8379C_SpiWriteData(0x12);
HX8379C_SpiWriteCmd(0xE020);
HX8379C_SpiWriteData(0x1A);
HX8379C_SpiWriteCmd(0xE021);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE022);
HX8379C_SpiWriteData(0x16);
HX8379C_SpiWriteCmd(0xE023);
HX8379C_SpiWriteData(0x19);
HX8379C_SpiWriteCmd(0xE024);
HX8379C_SpiWriteData(0x16);
HX8379C_SpiWriteCmd(0xE025);
HX8379C_SpiWriteData(0x15);
HX8379C_SpiWriteCmd(0xE026);
HX8379C_SpiWriteData(0x07);
HX8379C_SpiWriteCmd(0xE027);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE028);
HX8379C_SpiWriteData(0x13);
HX8379C_SpiWriteCmd(0xE029);
HX8379C_SpiWriteData(0x18);

        
HX8379C_SpiWriteCmd(0xB600);
HX8379C_SpiWriteData(0x76);
HX8379C_SpiWriteCmd(0xB601);
HX8379C_SpiWriteData(0x76);

    
HX8379C_SpiWriteCmd(0xCC00);
HX8379C_SpiWriteData(0x02);

HX8379C_SpiWriteCmd(0x3500);
HX8379C_SpiWriteData(0x00);

HX8379C_SpiWriteCmd(0x3A00);
HX8379C_SpiWriteData(0x55);//0x55 16位，0x66 18位，0x77 24位，
 
    
HX8379C_SpiWriteCmd(0x1100);
HX8379C_SpiWriteData(0x00);
mdelay(150);      
        
HX8379C_SpiWriteCmd(0x2900);
HX8379C_SpiWriteData(0x00);
mdelay(20);
#elif defined(CONFIG_ZYT_8379C_7675_JLD_HELITAI_LCD)
	HX8379C_SpiWriteCmd(0xB9);
	HX8379C_SpiWriteData(0xFF);
	HX8379C_SpiWriteData(0x83);
	HX8379C_SpiWriteData(0x79);

	HX8379C_SpiWriteCmd(0xB1);
	HX8379C_SpiWriteData(0x44);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x31);
	HX8379C_SpiWriteData(0x31);
	HX8379C_SpiWriteData(0x50);
	HX8379C_SpiWriteData(0xD0);
	HX8379C_SpiWriteData(0xEE);
	HX8379C_SpiWriteData(0x54);
	HX8379C_SpiWriteData(0x80);
	HX8379C_SpiWriteData(0x38);
	HX8379C_SpiWriteData(0x38);
	HX8379C_SpiWriteData(0xF8);
	HX8379C_SpiWriteData(0x33);
	HX8379C_SpiWriteData(0x32);
	HX8379C_SpiWriteData(0x22);

	HX8379C_SpiWriteCmd(0xB2);
	HX8379C_SpiWriteData(0x82);
	HX8379C_SpiWriteData(0x3C);
	HX8379C_SpiWriteData(0x0A);
	HX8379C_SpiWriteData(0x04);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x50);
	HX8379C_SpiWriteData(0x11);
	HX8379C_SpiWriteData(0x42);
	HX8379C_SpiWriteData(0x1D);

	HX8379C_SpiWriteCmd(0xB4);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x6E);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x6E);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x6E);
	HX8379C_SpiWriteData(0x22);
	HX8379C_SpiWriteData(0x80);
	HX8379C_SpiWriteData(0x23);
	HX8379C_SpiWriteData(0x80);

	HX8379C_SpiWriteCmd(0xCC);
	HX8379C_SpiWriteData(0x02);

	HX8379C_SpiWriteCmd(0xD2);
	HX8379C_SpiWriteData(0x33);

	HX8379C_SpiWriteCmd(0xD3);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x07);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x3C);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x10);
	HX8379C_SpiWriteData(0x10);
	HX8379C_SpiWriteData(0x32);
	HX8379C_SpiWriteData(0x10);
	HX8379C_SpiWriteData(0x03);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x03);
	HX8379C_SpiWriteData(0x03);
	HX8379C_SpiWriteData(0x30);
	HX8379C_SpiWriteData(0x03);
	HX8379C_SpiWriteData(0x30);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x08);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x08);
	HX8379C_SpiWriteData(0x17);
	HX8379C_SpiWriteData(0x11);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x17);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x17);
	HX8379C_SpiWriteData(0x08);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x14);

	HX8379C_SpiWriteCmd(0xD5);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x02);
	HX8379C_SpiWriteData(0x03);
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x01);
	HX8379C_SpiWriteData(0x06);
	HX8379C_SpiWriteData(0x07);
	HX8379C_SpiWriteData(0x04);
	HX8379C_SpiWriteData(0x05);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x20);
	HX8379C_SpiWriteData(0x21);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x1A);
	HX8379C_SpiWriteData(0x1A);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x1B);
	HX8379C_SpiWriteData(0x1B);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);

	HX8379C_SpiWriteCmd(0xB3);
	HX8379C_SpiWriteData(0x01);

	HX8379C_SpiWriteCmd(0xB6);
	HX8379C_SpiWriteData(0x85);
	HX8379C_SpiWriteData(0x85);

	HX8379C_SpiWriteCmd(0x3A);
	HX8379C_SpiWriteData(0x55);

	HX8379C_SpiWriteCmd(0x11);
	mdelay(120);
	HX8379C_SpiWriteCmd(0x29);
	mdelay(20);
#else
	HX8379C_SpiWriteCmd(0xB9);  // SET password
	HX8379C_SpiWriteData(0xFF);  
	HX8379C_SpiWriteData(0x83);  
	HX8379C_SpiWriteData(0x79); 

	HX8379C_SpiWriteCmd(0xB1);
	HX8379C_SpiWriteData(0x44);// => PA1
	HX8379C_SpiWriteData(0x18);// => PA2  0x16
	HX8379C_SpiWriteData(0x18);// => PA3	0x16
	HX8379C_SpiWriteData(0x31);// => PA4
	HX8379C_SpiWriteData(0x31);// => PA5
	HX8379C_SpiWriteData(0x90);// => PA6
	HX8379C_SpiWriteData(0xD0);// => PA7
	HX8379C_SpiWriteData(0xEE);// => PA8
	HX8379C_SpiWriteData(0x94);// => PA9
	HX8379C_SpiWriteData(0x80);// => PA10
	HX8379C_SpiWriteData(0x38);// => PA11
	HX8379C_SpiWriteData(0x38);// => PA12
	HX8379C_SpiWriteData(0xF8);// => PA13
	HX8379C_SpiWriteData(0x22);// => PA14
	HX8379C_SpiWriteData(0x22);// => PA15
	HX8379C_SpiWriteData(0x22);// => PA16
	HX8379C_SpiWriteData(0x00);// => PA17
	HX8379C_SpiWriteData(0x80);// => PA18
	HX8379C_SpiWriteData(0x30);// => PA19
	HX8379C_SpiWriteData(0x00);// => PA20

	HX8379C_SpiWriteCmd(0x3A);
	HX8379C_SpiWriteData(0x55);

	HX8379C_SpiWriteCmd(0xB3);
	HX8379C_SpiWriteData(0x11);
	mdelay(120);

	HX8379C_SpiWriteCmd(0xB2);
	HX8379C_SpiWriteData(0x80);	  //2dot invertion
	HX8379C_SpiWriteData(0x3C);    
	HX8379C_SpiWriteData(0x0B); //3C=800 //fe  
	HX8379C_SpiWriteData(0x04);    
	HX8379C_SpiWriteData(0x00);    
	HX8379C_SpiWriteData(0x50);    
	HX8379C_SpiWriteData(0x11);    
	HX8379C_SpiWriteData(0x42);    
	HX8379C_SpiWriteData(0x1D); 

	HX8379C_SpiWriteCmd(0xB4);//
	HX8379C_SpiWriteData(0x50);// => PA1
	HX8379C_SpiWriteData(0x51);// => PA2
	HX8379C_SpiWriteData(0x50);// => PA3
	HX8379C_SpiWriteData(0x51);// => PA4
	HX8379C_SpiWriteData(0x50);// => PA5
	HX8379C_SpiWriteData(0x51);// => PA6
	HX8379C_SpiWriteData(0x12);// => PA7
	HX8379C_SpiWriteData(0xA0);// => PA8
	HX8379C_SpiWriteData(0x13);// => PA9
	HX8379C_SpiWriteData(0xA0);// => PA10

	HX8379C_SpiWriteCmd(0xD3);
	HX8379C_SpiWriteData(0x00);//00);//      00);                   //00 
	HX8379C_SpiWriteData(0x07);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x00);//0A);//      08);//GIP_OPT[17]=1'b  //08
	HX8379C_SpiWriteData(0x00);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x00);//01);//      01);                   //01
	HX8379C_SpiWriteData(0x06);//05);//      05);                   //05
	HX8379C_SpiWriteData(0x06);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x32);//00);//      02);                   //00
	HX8379C_SpiWriteData(0x10);//18);//      00);                   //18
	HX8379C_SpiWriteData(0x05);//88);//      88);//10               //88
	HX8379C_SpiWriteData(0x00);
	HX8379C_SpiWriteData(0x05);//98);//89      99);                   //99
	HX8379C_SpiWriteData(0x03);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x6F);//01);//      88);                   //01
	HX8379C_SpiWriteData(0x03);//45);//      01);                   //45
	HX8379C_SpiWriteData(0x6F);//88);//      23);                   //88
	HX8379C_SpiWriteData(0x00);//88);//      01);                   //88
	HX8379C_SpiWriteData(0x07);//01);//      23);                   //01
	HX8379C_SpiWriteData(0x00);//45);//      88);                   //45
	HX8379C_SpiWriteData(0x07);//23);//      01);                   //23
	HX8379C_SpiWriteData(0x21);//67);//      88);//20               //67
	HX8379C_SpiWriteData(0x22);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x05);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x05);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x23);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x05);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x05);//88);//      76);                   //76
	HX8379C_SpiWriteData(0x23);//88);//      54);                   //54
	HX8379C_SpiWriteData(0x09);//98);//89  

	HX8379C_SpiWriteCmd(0xD5);
	HX8379C_SpiWriteData(0x18);//00);//      00);                   //00 
	HX8379C_SpiWriteData(0x18);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x19);//0A);//      08);//GIP_OPT[17]=1'b  //08
	HX8379C_SpiWriteData(0x19);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x01);//01);//      01);                   //01
	HX8379C_SpiWriteData(0x00);//05);//      05);                   //05
	HX8379C_SpiWriteData(0x03);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x02);//00);//      02);                   //00
	HX8379C_SpiWriteData(0x21);//18);//      00);                   //18
	HX8379C_SpiWriteData(0x20);//88);//      88);//10               //88
	HX8379C_SpiWriteData(0x18);
	HX8379C_SpiWriteData(0x18);//98);//89      99);                   //99
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//01);//      88);                   //01
	HX8379C_SpiWriteData(0x18);//45);//      01);                   //45
	HX8379C_SpiWriteData(0x18);//88);//      23);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      01);                   //88
	HX8379C_SpiWriteData(0x18);//01);//      23);                   //01
	HX8379C_SpiWriteData(0x18);//45);//      88);                   //45
	HX8379C_SpiWriteData(0x18);//23);//      01);                   //23
	HX8379C_SpiWriteData(0x18);//67);//      88);//20               //67
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      76);                   //76
	HX8379C_SpiWriteData(0x18);//88);//      54);                   //54
	HX8379C_SpiWriteData(0x18);//98);//89      76);                   //76
	HX8379C_SpiWriteData(0x18);//88);//      76);                   //76
	HX8379C_SpiWriteData(0x18);//88);//      54);                   //54
	HX8379C_SpiWriteData(0x18);//98);//89   

	HX8379C_SpiWriteCmd(0xD6);
	HX8379C_SpiWriteData(0x18);//00);//      00);                   //00 
	HX8379C_SpiWriteData(0x18);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x18);//0A);//      08);//GIP_OPT[17]=1'b  //08
	HX8379C_SpiWriteData(0x18);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x02);//01);//      01);                   //01
	HX8379C_SpiWriteData(0x03);//05);//      05);                   //05
	HX8379C_SpiWriteData(0x00);//00);//      00);                   //00
	HX8379C_SpiWriteData(0x02);//00);//      02);                   //00
	HX8379C_SpiWriteData(0x20);//18);//      00);                   //18
	HX8379C_SpiWriteData(0x21);//88);//      88);//10               //88
	HX8379C_SpiWriteData(0x19);
	HX8379C_SpiWriteData(0x19);//98);//89      99);                   //99
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//01);//      88);                   //01
	HX8379C_SpiWriteData(0x18);//45);//      01);                   //45
	HX8379C_SpiWriteData(0x18);//88);//      23);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      01);                   //88
	HX8379C_SpiWriteData(0x18);//01);//      23);                   //01
	HX8379C_SpiWriteData(0x18);//45);//      88);                   //45
	HX8379C_SpiWriteData(0x18);//23);//      01);                   //23
	HX8379C_SpiWriteData(0x18);//67);//      88);//20               //67
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      88);                   //88
	HX8379C_SpiWriteData(0x18);//88);//      76);                   //76
	HX8379C_SpiWriteData(0x18);//88);//      54);                   //54
	HX8379C_SpiWriteData(0x18);//98);//89      76);                   //76
	HX8379C_SpiWriteData(0x18);//88);//      76);                   //76
	HX8379C_SpiWriteData(0x18);//88);//      54);                   //54
	HX8379C_SpiWriteData(0x18);//98);//89
	HX8379C_SpiWriteCmd(0xE0);
	HX8379C_SpiWriteData(0x00);// => PA1
	HX8379C_SpiWriteData(0x00);// => PA2
	HX8379C_SpiWriteData(0x05);// => PA3
	HX8379C_SpiWriteData(0x18);// => PA4
	HX8379C_SpiWriteData(0x1C);// => PA5
	HX8379C_SpiWriteData(0x3F);// => PA6

	HX8379C_SpiWriteData(0x31);// => PA7
	HX8379C_SpiWriteData(0x3F);// => PA8

	HX8379C_SpiWriteData(0x09);// => PA9
	HX8379C_SpiWriteData(0x10);// => PA10
	HX8379C_SpiWriteData(0x12);// => PA11
	HX8379C_SpiWriteData(0x1A);// => PA12
	HX8379C_SpiWriteData(0x13);// => PA13
	HX8379C_SpiWriteData(0x16);// => PA14
	HX8379C_SpiWriteData(0x19);// => PA15
	HX8379C_SpiWriteData(0x16);// => PA16
	HX8379C_SpiWriteData(0x15);// => PA17
	HX8379C_SpiWriteData(0x07);// => PA18
	HX8379C_SpiWriteData(0x13);// => PA19
	HX8379C_SpiWriteData(0x13);// => PA20
	HX8379C_SpiWriteData(0x18);// => PA21

	HX8379C_SpiWriteData(0x00);// => PA22
	HX8379C_SpiWriteData(0x00);// => PA23
	HX8379C_SpiWriteData(0x05);// => PA24
	HX8379C_SpiWriteData(0x19);// => PA25
	HX8379C_SpiWriteData(0x1B);// => PA26
	HX8379C_SpiWriteData(0x3F);// => PA27

	HX8379C_SpiWriteData(0x31);// => PA28
	HX8379C_SpiWriteData(0x3F);// => PA29

	HX8379C_SpiWriteData(0x09);// => PA30
	HX8379C_SpiWriteData(0x0F);// => PA31
	HX8379C_SpiWriteData(0x11);// => PA32
	HX8379C_SpiWriteData(0x1A);// => PA33
	HX8379C_SpiWriteData(0x11);// => PA34
	HX8379C_SpiWriteData(0x15);// => PA35
	HX8379C_SpiWriteData(0x17);// => PA36
	HX8379C_SpiWriteData(0x15);// => PA37
	HX8379C_SpiWriteData(0x16);// => PA38
	HX8379C_SpiWriteData(0x07);// => PA39
	HX8379C_SpiWriteData(0x12);// => PA40
	HX8379C_SpiWriteData(0x13);// => PA41
	HX8379C_SpiWriteData(0x17);// => PA42

	HX8379C_SpiWriteCmd(0xB6);	//Flicker adjust
	HX8379C_SpiWriteData(0x76);//45 -->0x47
	HX8379C_SpiWriteData(0x76);//45 -->0x47	

	HX8379C_SpiWriteCmd(0xcc);
	HX8379C_SpiWriteData(0x02);

	HX8379C_SpiWriteCmd(0x11);
	mdelay(120);
	HX8379C_SpiWriteCmd(0x29);
	mdelay(50);
#endif

}

static int32_t hx8379c_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
#if defined(CONFIG_ZYT_8379C_7675_JZY_LCD) || defined(CONFIG_ZYT_8379C_FS006_CFZZ_BEILIJIA_LCD)
	if(is_sleep==1){
		//Sleep In
		HX8379C_SpiWriteCmd(0x2800);
		mdelay(120); 
		HX8379C_SpiWriteCmd(0x1000);
		mdelay(10); 
	}else{
		//Sleep Out
		HX8379C_SpiWriteCmd(0x1100);
		mdelay(120); 
		HX8379C_SpiWriteCmd(0x2900);
		mdelay(10); 
	}
#else	
	if(is_sleep==1){
		//Sleep In
		HX8379C_SpiWriteCmd(0x28);
		mdelay(120); 
		HX8379C_SpiWriteCmd(0x10);
		mdelay(10); 
	}else{
		//Sleep Out
		HX8379C_SpiWriteCmd(0x11);
		mdelay(120); 
		HX8379C_SpiWriteCmd(0x29);
		mdelay(10); 
	}
#endif
	return 0;
}

static int32_t hx8379c_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	uint32_t *test_data[4] = {0};
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;
#if 0

	printk("zxdbg add -hx8379c_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

	HX8379C_SpiWriteCmd(0x2A00); 
	HX8379C_SpiWriteData((left>>8));// set left address
	HX8379C_SpiWriteData((left&0xff));
	HX8379C_SpiWriteData((right>>8));// set right address
	HX8379C_SpiWriteData((right&0xff));

	HX8379C_SpiWriteCmd(0x2B00); 
	HX8379C_SpiWriteData((top>>8));// set left address
	HX8379C_SpiWriteData((top&0xff));
	HX8379C_SpiWriteData((bottom>>8));// set bottom address
	HX8379C_SpiWriteData((bottom&0xff));

//	HX8379C_SpiWriteCmd(0x2C00);

	HX8379C_SpiWriteCmd(0x2A00); 
	spi_read(test_data);
	spi_read(test_data+1);
	HX8379C_SpiWriteCmd(0x2B00); 
	spi_read(test_data+2);
	spi_read(test_data+3);
#endif
//	printk("zxdbg add -hx8379c_read read: %x, %x, %x, %x\n",test_data[0], test_data[1], test_data[2], test_data[3]);

	return 0;
}

static int32_t hx8379c_invalidate(struct panel_spec *self)
{
//	printk("hx8379c_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}

static int32_t hx8379c_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
//	printk("hx8379c_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t hx8379c_read_id(struct panel_spec *self)
{
	int32_t  id = 0; 
	int32_t  id1 = 0; 
	int32_t  id2 = 0; 
	int32_t  id3= 0; 
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;

	LCD_PRINT("u-boot hx8379c_read_id \n");
	
	/*HX8379C_SpiWriteCmd(0xB9); 
	HX8379C_SpiWriteData(0xFF);  
	HX8379C_SpiWriteData(0x83); 
	HX8379C_SpiWriteData(0x79);
	udelay(10000);

	HX8379C_SpiWriteCmd(0xFE); // SET SPI READ INDEX
	HX8379C_SpiWriteData(0x04); // GETHXID
	HX8379C_SpiWriteCmd(0x00); // GET SPI READ
	spi_read(&id1);
	LCD_PRINT(" hx8379c_read_id u-boot id1 = 0x%x\n",id1);
	HX8379C_SpiWriteCmd(0xFE); // SET SPI READ INDEX
	HX8379C_SpiWriteData(0x04); // GETHXID
	HX8379C_SpiWriteCmd(0x01); // GET SPI READ
	spi_read(&id2);
	LCD_PRINT(" hx8379c_read_id u-boot id2 = 0x%x\n",id2);
	HX8379C_SpiWriteCmd(0xFE);
	HX8379C_SpiWriteData(0x00);
	id = ((id1&0x00ff)<<8)|(id2&0x00ff);
	LCD_PRINT(" hx8379c_read_id u-boot id = 0x%x\n",id);

	return id;
*/
	HX8379C_SpiWriteCmd(0xFF);  
	HX8379C_SpiWriteData(0x83); 
	HX8379C_SpiWriteData(0x79);
	HX8379C_SpiWriteCmd(0x04);  
	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
	spi_read(8,&id1);
	spi_read(8,&id2);//0x83
	spi_read(8,&id3);//0x79
	#else
	spi_read(&id1);
	spi_read(&id2);//0x83
	spi_read(&id3);//0x79
	#endif
	id=(id2<<8)||id3;
	return id;//0x8379;
//#endif
}

static struct panel_operations lcd_hx8379c_rgb_spi_operations = {
	.panel_init = hx8379c_init,
	.panel_set_window = hx8379c_set_window,
	.panel_invalidate_rect= hx8379c_invalidate_rect,
	.panel_invalidate = hx8379c_invalidate,
	.panel_enter_sleep = hx8379c_enter_sleep,
	.panel_readid          = hx8379c_read_id
};

static struct timing_rgb lcd_hx8379c_rgb_timing = {
#if defined(CONFIG_ZYT_8379C_7271_CFZZ_LCD)
	.hfp = 32,//10,  /* unit: pixel */
	.hbp = 32,//6,
	.hsync = 32, //8,
#else
	.hfp = 15,//10,  /* unit: pixel */
	.hbp = 15,//6,
	.hsync = 10, //8,
#endif
#if defined(CONFIG_ZYT_8379C_7675_JLD_HELITAI_LCD)
	.vfp = 12, // 3, /*unit: line*/
	.vbp = 6, // 3,
	.vsync = 4, // 4,
#elif defined(CONFIG_ZYT_8379C_7271_CFZZ_LCD)
	.vfp = 7, // 3, /*unit: line*/
	.vbp = 6, // 3,
	.vsync = 9, // 4,
#else
	.vfp = 10, // 3, /*unit: line*/
	.vbp = 10, // 3,
	.vsync = 5, // 4,
#endif
};

static struct spi_info lcd_hx8379c_rgb_spi_info = {
	.ops = NULL,
};

static struct info_rgb lcd_hx8379c_rgb_info = {
	.cmd_bus_mode  = SPRDFB_RGB_BUS_TYPE_SPI,
	.video_bus_width = 16, /*18,16*/
	.h_sync_pol = SPRDFB_POLARITY_NEG,
	.v_sync_pol = SPRDFB_POLARITY_NEG,
	.de_pol = SPRDFB_POLARITY_POS,
	.timing = &lcd_hx8379c_rgb_timing,
	.bus_info = {
		.spi = &lcd_hx8379c_rgb_spi_info,
	}
};

struct panel_spec lcd_panel_hx8379c_rgb_spi_spec = {
	.width = 480,
	.height = 800,
	.reset_timing = {20, 10, 100},
#if defined(CONFIG_ZYT_8379C_7271_CFZZ_LCD)
	.fps =58,
#else
	.fps = 40,
#endif
	.type = LCD_MODE_RGB,
	.direction = LCD_DIRECT_NORMAL,
	.is_clean_lcd = true,
	.info = {
		.rgb = &lcd_hx8379c_rgb_info
	},
	.ops = &lcd_hx8379c_rgb_spi_operations,
};

struct panel_cfg lcd_hx8379c_rgb_spi = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x8379,
	.lcd_name = "lcd_hx8379c_rgb_spi",
	.panel = &lcd_panel_hx8379c_rgb_spi_spec,
};

static int __init lcd_hx8379c_rgb_spi_init(void)
{
	return sprdfb_panel_register(&lcd_hx8379c_rgb_spi);
}

subsys_initcall(lcd_hx8379c_rgb_spi_init);
