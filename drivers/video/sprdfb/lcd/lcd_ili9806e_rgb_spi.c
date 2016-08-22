/* drivers/video/sc8810/lcd_ili9806e_rgb_spi.c
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
#define ILI9806E_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(8,(cmd & 0xFF));\
}

#define  ILI9806E_SpiWriteData(data)\
{ \
	spi_send_data(8,(data & 0xFF));\
}
#else
#define ILI9806E_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd((cmd & 0xFF));\
}

#define  ILI9806E_SpiWriteData(data)\
{ \
	spi_send_data((data & 0xFF));\
}
#endif
    //  #define   mdelay(x/1000) udelay(x)  
#if (CONFIG_ZYT_PRJ == 7672)
	//#define CONFIG_ZYT_ILI9806E_7672_XXX_YTQ  // 目前还不知道客户名称，先用XXX代替 优泰奇屏
	//#define CONFIG_ZYT_ILI9806E_7672_XXX_SD   // 目前还不知道客户名称，先用XXX代替 盛大屏
   	#if (CONFIG_ZYT_CUST==9)
	 	 #define CONFIG_ZYT_ILI9806E_7672_WANDA_CTC   //万达 7672  9806E 盛大 CTC玻璃
	#else
		#define CONFIG_ZYT_ILI9806E_7672_XXX_SD   // 目前还不知道客户名称，先用XXX代替 盛大屏
	#endif

#endif

#if (CONFIG_ZYT_PRJ == 7675) || (CONFIG_ZYT_PRJ == 7271)
#if (CONFIG_ZYT_CUST == 69) || (CONFIG_ZYT_CUST == 8)
	#define CONFIG_ZYT_ILI9806E_7675_ANGU
#elif (CONFIG_ZYT_CUST == 67)
	#define CONFIG_ZYT_ILI9806E_7675_QY
#elif (CONFIG_ZYT_CUST == 75)
	#define CONFIG_ZYT_ILI9806E_7675_GUOWEI
#else
#define CONFIG_ZYT_ILI9806E_7675_CFZZ
#endif
#elif (CONFIG_ZYT_PRJ == 7271) && (CONFIG_ZYT_CUST == 3)
#define CONFIG_ZYT_ILI9806E_7271_CFZZ
#elif (CONFIG_ZYT_PRJ == 7006)
#define CONFIG_ZYT_ILI9806E_FS011_CFZZ
#elif (CONFIG_ZYT_PRJ == 7003)
#define CONFIG_ZYT_ILI9806E_FS003_LHTC_HFG   // FS003 联合同创  海菲光屏
#elif (CONFIG_ZYT_PRJ == 7012)
#if (CONFIG_ZYT_CUST == 77)
#define CONFIG_ZYT_ILI9806E_FS012_HONGNIAN
#endif
#endif


#if (CONFIG_ZYT_PRJ == 7011)
#define CONFIG_ZYT_ILI9806E_FS011_CFZZ
#endif
#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t ili9806e_esd_check(struct panel_spec *self);
#endif
static int32_t ili9806e_init(struct panel_spec *self)
{	
	uint32_t data = 0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read; 

	printk("ili9806e_init\n");
#if defined(CONFIG_ZYT_ILI9806E_FS011_CFZZ)


ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x01);


//ILI9806E_SpiWriteCmd(0x08);//need
//ILI9806E_SpiWriteData(0x10);//need    remove for esd-check ,read  0x0a equal to 0x9c


ILI9806E_SpiWriteCmd(0x21);
ILI9806E_SpiWriteData(0x01);



ILI9806E_SpiWriteCmd(0x30);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0x31);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0x50);
ILI9806E_SpiWriteData(0x78);


ILI9806E_SpiWriteCmd(0x51);
ILI9806E_SpiWriteData(0x78);


ILI9806E_SpiWriteCmd(0x52);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x53);
ILI9806E_SpiWriteData(0x78);


ILI9806E_SpiWriteCmd(0x60);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0x61);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x62);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0x63);
ILI9806E_SpiWriteData(0x03);

ILI9806E_SpiWriteCmd(0x40);
ILI9806E_SpiWriteData(0x15);


ILI9806E_SpiWriteCmd(0x41);
ILI9806E_SpiWriteData(0x45);


ILI9806E_SpiWriteCmd(0x42);
ILI9806E_SpiWriteData(0x03);


ILI9806E_SpiWriteCmd(0x43);
ILI9806E_SpiWriteData(0x09);


ILI9806E_SpiWriteCmd(0x44);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x01);


ILI9806E_SpiWriteCmd(0xA0);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0xA1);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xA2);
ILI9806E_SpiWriteData(0x0B);


ILI9806E_SpiWriteCmd(0xA3);
ILI9806E_SpiWriteData(0x11);


ILI9806E_SpiWriteCmd(0xA4);
ILI9806E_SpiWriteData(0x0A);


ILI9806E_SpiWriteCmd(0xA5);
ILI9806E_SpiWriteData(0x1A);


ILI9806E_SpiWriteCmd(0xA6);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xA7);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xA8);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0xA9);
ILI9806E_SpiWriteData(0x07);


ILI9806E_SpiWriteCmd(0xAA);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0xAB);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xAC);
ILI9806E_SpiWriteData(0x0F);


ILI9806E_SpiWriteCmd(0xAD);
ILI9806E_SpiWriteData(0x2D);


ILI9806E_SpiWriteCmd(0xAE);
ILI9806E_SpiWriteData(0x27);


ILI9806E_SpiWriteCmd(0xAF);
ILI9806E_SpiWriteData(0x00);



ILI9806E_SpiWriteCmd(0xC0);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0xC1);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xC2);
ILI9806E_SpiWriteData(0x0B);


ILI9806E_SpiWriteCmd(0xC3);
ILI9806E_SpiWriteData(0x10);


ILI9806E_SpiWriteCmd(0xC4);
ILI9806E_SpiWriteData(0x09);


ILI9806E_SpiWriteCmd(0xC5);
ILI9806E_SpiWriteData(0x1A);


ILI9806E_SpiWriteCmd(0xC6);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xC7);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xC8);
ILI9806E_SpiWriteData(0x03);


ILI9806E_SpiWriteCmd(0xC9);
ILI9806E_SpiWriteData(0x07);


ILI9806E_SpiWriteCmd(0xCA);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0xCB);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xCC);
ILI9806E_SpiWriteData(0x0F);


ILI9806E_SpiWriteCmd(0xCD);
ILI9806E_SpiWriteData(0x2C);


ILI9806E_SpiWriteCmd(0xCE);
ILI9806E_SpiWriteData(0x27);


ILI9806E_SpiWriteCmd(0xCF);
ILI9806E_SpiWriteData(0x00);




ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0x00);
ILI9806E_SpiWriteData(0x3C);


ILI9806E_SpiWriteCmd(0x01);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0x02);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x03);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x04);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x05);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x06);
ILI9806E_SpiWriteData(0x80);


ILI9806E_SpiWriteCmd(0x07);
ILI9806E_SpiWriteData(0x04);


ILI9806E_SpiWriteCmd(0x08);
ILI9806E_SpiWriteData(0x03);


ILI9806E_SpiWriteCmd(0x09);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0A);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0B);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0C);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x0D);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x0E);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0F);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x10);
ILI9806E_SpiWriteData(0x50);


ILI9806E_SpiWriteCmd(0x11);
ILI9806E_SpiWriteData(0xD0);


ILI9806E_SpiWriteCmd(0x12);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x13);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x14);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x15);
ILI9806E_SpiWriteData(0xC0);


ILI9806E_SpiWriteCmd(0x16);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0x17);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x18);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x19);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x1A);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x1B);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x1C);
ILI9806E_SpiWriteData(0x48);


ILI9806E_SpiWriteCmd(0x1D);
ILI9806E_SpiWriteData(0x00);



ILI9806E_SpiWriteCmd(0x20);
ILI9806E_SpiWriteData(0x01);


ILI9806E_SpiWriteCmd(0x21);
ILI9806E_SpiWriteData(0x23);


ILI9806E_SpiWriteCmd(0x22);
ILI9806E_SpiWriteData(0x45);


ILI9806E_SpiWriteCmd(0x23);
ILI9806E_SpiWriteData(0x67);


ILI9806E_SpiWriteCmd(0x24);
ILI9806E_SpiWriteData(0x01);


ILI9806E_SpiWriteCmd(0x25);
ILI9806E_SpiWriteData(0x23);


ILI9806E_SpiWriteCmd(0x26);
ILI9806E_SpiWriteData(0x45);


ILI9806E_SpiWriteCmd(0x27);
ILI9806E_SpiWriteData(0x67);



ILI9806E_SpiWriteCmd(0x30);
ILI9806E_SpiWriteData(0x13);


ILI9806E_SpiWriteCmd(0x31);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x32);
ILI9806E_SpiWriteData(0xDD);


ILI9806E_SpiWriteCmd(0x33);
ILI9806E_SpiWriteData(0xCC);


ILI9806E_SpiWriteCmd(0x34);
ILI9806E_SpiWriteData(0xBB);


ILI9806E_SpiWriteCmd(0x35);
ILI9806E_SpiWriteData(0xAA);


ILI9806E_SpiWriteCmd(0x36);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x37);
ILI9806E_SpiWriteData(0x26);


ILI9806E_SpiWriteCmd(0x38);
ILI9806E_SpiWriteData(0x72);


ILI9806E_SpiWriteCmd(0x39);
ILI9806E_SpiWriteData(0xFF);


ILI9806E_SpiWriteCmd(0x3A);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3B);
ILI9806E_SpiWriteData(0xEE);


ILI9806E_SpiWriteCmd(0x3C);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3D);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3E);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3F);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x40);
ILI9806E_SpiWriteData(0x22);



ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x07);


ILI9806E_SpiWriteCmd(0x17);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0x06);
ILI9806E_SpiWriteData(0x03);



ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x11);
ILI9806E_SpiWriteData(0x00);
mdelay(120);

ILI9806E_SpiWriteCmd(0x29);
ILI9806E_SpiWriteData(0x00);
mdelay(10);

ILI9806E_SpiWriteCmd(0x3A);
ILI9806E_SpiWriteData(0x55);
#elif defined(CONFIG_ZYT_ILI9806E_FS012_HONGNIAN)
ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x01);


//ILI9806E_SpiWriteCmd(0x08);//need
//ILI9806E_SpiWriteData(0x10);//need    remove for esd-check ,read  0x0a equal to 0x9c


ILI9806E_SpiWriteCmd(0x21);
ILI9806E_SpiWriteData(0x01);



ILI9806E_SpiWriteCmd(0x30);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0x31);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0x50);
ILI9806E_SpiWriteData(0x78);


ILI9806E_SpiWriteCmd(0x51);
ILI9806E_SpiWriteData(0x78);


ILI9806E_SpiWriteCmd(0x52);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x53);
ILI9806E_SpiWriteData(0x78);


ILI9806E_SpiWriteCmd(0x60);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0x61);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x62);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0x63);
ILI9806E_SpiWriteData(0x03);

ILI9806E_SpiWriteCmd(0x40);
ILI9806E_SpiWriteData(0x15);


ILI9806E_SpiWriteCmd(0x41);
ILI9806E_SpiWriteData(0x45);


ILI9806E_SpiWriteCmd(0x42);
ILI9806E_SpiWriteData(0x03);


ILI9806E_SpiWriteCmd(0x43);
ILI9806E_SpiWriteData(0x09);


ILI9806E_SpiWriteCmd(0x44);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x01);


ILI9806E_SpiWriteCmd(0xA0);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0xA1);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xA2);
ILI9806E_SpiWriteData(0x0B);


ILI9806E_SpiWriteCmd(0xA3);
ILI9806E_SpiWriteData(0x11);


ILI9806E_SpiWriteCmd(0xA4);
ILI9806E_SpiWriteData(0x0A);


ILI9806E_SpiWriteCmd(0xA5);
ILI9806E_SpiWriteData(0x1A);


ILI9806E_SpiWriteCmd(0xA6);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xA7);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xA8);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0xA9);
ILI9806E_SpiWriteData(0x07);


ILI9806E_SpiWriteCmd(0xAA);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0xAB);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xAC);
ILI9806E_SpiWriteData(0x0F);


ILI9806E_SpiWriteCmd(0xAD);
ILI9806E_SpiWriteData(0x2D);


ILI9806E_SpiWriteCmd(0xAE);
ILI9806E_SpiWriteData(0x27);


ILI9806E_SpiWriteCmd(0xAF);
ILI9806E_SpiWriteData(0x00);



ILI9806E_SpiWriteCmd(0xC0);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0xC1);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xC2);
ILI9806E_SpiWriteData(0x0B);


ILI9806E_SpiWriteCmd(0xC3);
ILI9806E_SpiWriteData(0x10);


ILI9806E_SpiWriteCmd(0xC4);
ILI9806E_SpiWriteData(0x09);


ILI9806E_SpiWriteCmd(0xC5);
ILI9806E_SpiWriteData(0x1A);


ILI9806E_SpiWriteCmd(0xC6);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xC7);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0xC8);
ILI9806E_SpiWriteData(0x03);


ILI9806E_SpiWriteCmd(0xC9);
ILI9806E_SpiWriteData(0x07);


ILI9806E_SpiWriteCmd(0xCA);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0xCB);
ILI9806E_SpiWriteData(0x05);


ILI9806E_SpiWriteCmd(0xCC);
ILI9806E_SpiWriteData(0x0F);


ILI9806E_SpiWriteCmd(0xCD);
ILI9806E_SpiWriteData(0x2C);


ILI9806E_SpiWriteCmd(0xCE);
ILI9806E_SpiWriteData(0x27);


ILI9806E_SpiWriteCmd(0xCF);
ILI9806E_SpiWriteData(0x00);




ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0x00);
ILI9806E_SpiWriteData(0x3C);


ILI9806E_SpiWriteCmd(0x01);
ILI9806E_SpiWriteData(0x06);


ILI9806E_SpiWriteCmd(0x02);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x03);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x04);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x05);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x06);
ILI9806E_SpiWriteData(0x80);


ILI9806E_SpiWriteCmd(0x07);
ILI9806E_SpiWriteData(0x04);


ILI9806E_SpiWriteCmd(0x08);
ILI9806E_SpiWriteData(0x03);


ILI9806E_SpiWriteCmd(0x09);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0A);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0B);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0C);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x0D);
ILI9806E_SpiWriteData(0x0D);


ILI9806E_SpiWriteCmd(0x0E);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x0F);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x10);
ILI9806E_SpiWriteData(0x50);


ILI9806E_SpiWriteCmd(0x11);
ILI9806E_SpiWriteData(0xD0);


ILI9806E_SpiWriteCmd(0x12);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x13);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x14);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x15);
ILI9806E_SpiWriteData(0xC0);


ILI9806E_SpiWriteCmd(0x16);
ILI9806E_SpiWriteData(0x08);


ILI9806E_SpiWriteCmd(0x17);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x18);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x19);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x1A);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x1B);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x1C);
ILI9806E_SpiWriteData(0x48);


ILI9806E_SpiWriteCmd(0x1D);
ILI9806E_SpiWriteData(0x00);



ILI9806E_SpiWriteCmd(0x20);
ILI9806E_SpiWriteData(0x01);


ILI9806E_SpiWriteCmd(0x21);
ILI9806E_SpiWriteData(0x23);


ILI9806E_SpiWriteCmd(0x22);
ILI9806E_SpiWriteData(0x45);


ILI9806E_SpiWriteCmd(0x23);
ILI9806E_SpiWriteData(0x67);


ILI9806E_SpiWriteCmd(0x24);
ILI9806E_SpiWriteData(0x01);


ILI9806E_SpiWriteCmd(0x25);
ILI9806E_SpiWriteData(0x23);


ILI9806E_SpiWriteCmd(0x26);
ILI9806E_SpiWriteData(0x45);


ILI9806E_SpiWriteCmd(0x27);
ILI9806E_SpiWriteData(0x67);



ILI9806E_SpiWriteCmd(0x30);
ILI9806E_SpiWriteData(0x13);


ILI9806E_SpiWriteCmd(0x31);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x32);
ILI9806E_SpiWriteData(0xDD);


ILI9806E_SpiWriteCmd(0x33);
ILI9806E_SpiWriteData(0xCC);


ILI9806E_SpiWriteCmd(0x34);
ILI9806E_SpiWriteData(0xBB);


ILI9806E_SpiWriteCmd(0x35);
ILI9806E_SpiWriteData(0xAA);


ILI9806E_SpiWriteCmd(0x36);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x37);
ILI9806E_SpiWriteData(0x26);


ILI9806E_SpiWriteCmd(0x38);
ILI9806E_SpiWriteData(0x72);


ILI9806E_SpiWriteCmd(0x39);
ILI9806E_SpiWriteData(0xFF);


ILI9806E_SpiWriteCmd(0x3A);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3B);
ILI9806E_SpiWriteData(0xEE);


ILI9806E_SpiWriteCmd(0x3C);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3D);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3E);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x3F);
ILI9806E_SpiWriteData(0x22);


ILI9806E_SpiWriteCmd(0x40);
ILI9806E_SpiWriteData(0x22);



ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x07);


ILI9806E_SpiWriteCmd(0x17);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0x06);
ILI9806E_SpiWriteData(0x03);



ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x00);


ILI9806E_SpiWriteCmd(0x11);
ILI9806E_SpiWriteData(0x00);
mdelay(120);

ILI9806E_SpiWriteCmd(0x29);
ILI9806E_SpiWriteData(0x00);
mdelay(10);

ILI9806E_SpiWriteCmd(0x3A);
ILI9806E_SpiWriteData(0x55);
#elif defined(CONFIG_ZYT_ILI9806E_7675_GUOWEI)  
ILI9806E_SpiWriteCmd(0xFF);ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x08);ILI9806E_SpiWriteData(0x10); 
ILI9806E_SpiWriteCmd(0x21);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x30);ILI9806E_SpiWriteData(0x02); 
ILI9806E_SpiWriteCmd(0x31);ILI9806E_SpiWriteData(0x02); 


ILI9806E_SpiWriteCmd(0x60);ILI9806E_SpiWriteData(0x07); 
ILI9806E_SpiWriteCmd(0x61);ILI9806E_SpiWriteData(0x06); 
ILI9806E_SpiWriteCmd(0x62);ILI9806E_SpiWriteData(0x06); 
ILI9806E_SpiWriteCmd(0x63);ILI9806E_SpiWriteData(0x04); 


ILI9806E_SpiWriteCmd(0x40);ILI9806E_SpiWriteData(0x1A); 
ILI9806E_SpiWriteCmd(0x41);ILI9806E_SpiWriteData(0x47); 
ILI9806E_SpiWriteCmd(0x42);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x43);ILI9806E_SpiWriteData(0x09); 
ILI9806E_SpiWriteCmd(0x44);ILI9806E_SpiWriteData(0x09); 
ILI9806E_SpiWriteCmd(0x45);ILI9806E_SpiWriteData(0x1B); 
ILI9806E_SpiWriteCmd(0x46);ILI9806E_SpiWriteData(0x33); 
ILI9806E_SpiWriteCmd(0x47);ILI9806E_SpiWriteData(0x33); 



ILI9806E_SpiWriteCmd(0x50);ILI9806E_SpiWriteData(0x78); 
ILI9806E_SpiWriteCmd(0x51);ILI9806E_SpiWriteData(0x78); 
ILI9806E_SpiWriteCmd(0x52);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x53);ILI9806E_SpiWriteData(0x75); 


 

ILI9806E_SpiWriteCmd(0xA0);ILI9806E_SpiWriteData(0x00);//Gamma0 
ILI9806E_SpiWriteCmd(0xA1);ILI9806E_SpiWriteData(0x04);//Gamma4 
ILI9806E_SpiWriteCmd(0xA2);ILI9806E_SpiWriteData(0x09);//Gamma8 
ILI9806E_SpiWriteCmd(0xA3);ILI9806E_SpiWriteData(0x0E);//Gamma16 
ILI9806E_SpiWriteCmd(0xA4);ILI9806E_SpiWriteData(0x08);//Gamma24 
ILI9806E_SpiWriteCmd(0xA5);ILI9806E_SpiWriteData(0x1C);//Gamma52 
ILI9806E_SpiWriteCmd(0xA6);ILI9806E_SpiWriteData(0x0F);//Gamma80 
ILI9806E_SpiWriteCmd(0xA7);ILI9806E_SpiWriteData(0x0E);//Gamma108 
ILI9806E_SpiWriteCmd(0xA8);ILI9806E_SpiWriteData(0x01);//Gamma147 
ILI9806E_SpiWriteCmd(0xA9);ILI9806E_SpiWriteData(0x06);//Gamma175 
ILI9806E_SpiWriteCmd(0xAA);ILI9806E_SpiWriteData(0x01);//Gamma203 
ILI9806E_SpiWriteCmd(0xAB);ILI9806E_SpiWriteData(0x00);//Gamma231 
ILI9806E_SpiWriteCmd(0xAC);ILI9806E_SpiWriteData(0x0A);//Gamma239 
ILI9806E_SpiWriteCmd(0xAD);ILI9806E_SpiWriteData(0x2E);//Gamma247 
ILI9806E_SpiWriteCmd(0xAE);ILI9806E_SpiWriteData(0x29);//Gamma251 
ILI9806E_SpiWriteCmd(0xAF);ILI9806E_SpiWriteData(0x00);//Gamma255 
 
ILI9806E_SpiWriteCmd(0xC0);ILI9806E_SpiWriteData(0x00);//Gamma0 
ILI9806E_SpiWriteCmd(0xC1);ILI9806E_SpiWriteData(0x02);//Gamma4 
ILI9806E_SpiWriteCmd(0xC2);ILI9806E_SpiWriteData(0x09);//Gamma8 
ILI9806E_SpiWriteCmd(0xC3);ILI9806E_SpiWriteData(0x0D);//Gamma16 
ILI9806E_SpiWriteCmd(0xC4);ILI9806E_SpiWriteData(0x08);//Gamma24 
ILI9806E_SpiWriteCmd(0xC5);ILI9806E_SpiWriteData(0x1C);//Gamma52 
ILI9806E_SpiWriteCmd(0xC6);ILI9806E_SpiWriteData(0x0B);//Gamma80 
ILI9806E_SpiWriteCmd(0xC7);ILI9806E_SpiWriteData(0x09);//Gamma108 
ILI9806E_SpiWriteCmd(0xC8);ILI9806E_SpiWriteData(0x02);//Gamma147 
ILI9806E_SpiWriteCmd(0xC9);ILI9806E_SpiWriteData(0x07);//Gamma175 
ILI9806E_SpiWriteCmd(0xCA);ILI9806E_SpiWriteData(0x06);//Gamma203 
ILI9806E_SpiWriteCmd(0xCB);ILI9806E_SpiWriteData(0x06);//Gamma231 
ILI9806E_SpiWriteCmd(0xCC);ILI9806E_SpiWriteData(0x0C);//Gamma239 
ILI9806E_SpiWriteCmd(0xCD);ILI9806E_SpiWriteData(0x21);//Gamma247 
ILI9806E_SpiWriteCmd(0xCE);ILI9806E_SpiWriteData(0x1D);//Gamma251 
ILI9806E_SpiWriteCmd(0xCF);ILI9806E_SpiWriteData(0x00);//Gamma255 
//**************************************************************** 
//Page6Command 
ILI9806E_SpiWriteCmd(0xFF);ILI9806E_SpiWriteData(0xFF);ILI9806E_SpiWriteData(0x98);ILI9806E_SpiWriteData(0x06);ILI9806E_SpiWriteData(0x04);ILI9806E_SpiWriteData(0x06);//ChangetoPage6 
ILI9806E_SpiWriteCmd(0x00);ILI9806E_SpiWriteData(0xA0); 
ILI9806E_SpiWriteCmd(0x01);ILI9806E_SpiWriteData(0x05); 
ILI9806E_SpiWriteCmd(0x02);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x03);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x04);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x05);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x06);ILI9806E_SpiWriteData(0x88); 
ILI9806E_SpiWriteCmd(0x07);ILI9806E_SpiWriteData(0x04); 
ILI9806E_SpiWriteCmd(0x08);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x09);ILI9806E_SpiWriteData(0x90); 
ILI9806E_SpiWriteCmd(0x0A);ILI9806E_SpiWriteData(0x04); 
ILI9806E_SpiWriteCmd(0x0B);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x0C);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x0D);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x0E);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x0F);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x10);ILI9806E_SpiWriteData(0x55); 
ILI9806E_SpiWriteCmd(0x11);ILI9806E_SpiWriteData(0x50); 
ILI9806E_SpiWriteCmd(0x12);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x13);ILI9806E_SpiWriteData(0x85); 
ILI9806E_SpiWriteCmd(0x14);ILI9806E_SpiWriteData(0x85); 
ILI9806E_SpiWriteCmd(0x15);ILI9806E_SpiWriteData(0xC0); 
ILI9806E_SpiWriteCmd(0x16);ILI9806E_SpiWriteData(0x0B); 
ILI9806E_SpiWriteCmd(0x17);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x18);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x19);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x1A);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x1B);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x1C);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x1D);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x20);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x21);ILI9806E_SpiWriteData(0x23); 
ILI9806E_SpiWriteCmd(0x22);ILI9806E_SpiWriteData(0x45); 
ILI9806E_SpiWriteCmd(0x23);ILI9806E_SpiWriteData(0x67); 
ILI9806E_SpiWriteCmd(0x24);ILI9806E_SpiWriteData(0x01); 
ILI9806E_SpiWriteCmd(0x25);ILI9806E_SpiWriteData(0x23); 
ILI9806E_SpiWriteCmd(0x26);ILI9806E_SpiWriteData(0x45); 
ILI9806E_SpiWriteCmd(0x27);ILI9806E_SpiWriteData(0x67); 


ILI9806E_SpiWriteCmd(0x30);ILI9806E_SpiWriteData(0x02); 
ILI9806E_SpiWriteCmd(0x31);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x32);ILI9806E_SpiWriteData(0x11); 
ILI9806E_SpiWriteCmd(0x33);ILI9806E_SpiWriteData(0xAA); 
ILI9806E_SpiWriteCmd(0x34);ILI9806E_SpiWriteData(0xBB); 
ILI9806E_SpiWriteCmd(0x35);ILI9806E_SpiWriteData(0x66); 
ILI9806E_SpiWriteCmd(0x36);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x37);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x38);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x39);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x3A);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x3B);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x3C);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x3D);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x3E);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x3F);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x40);ILI9806E_SpiWriteData(0x22); 

ILI9806E_SpiWriteCmd(0x52);ILI9806E_SpiWriteData(0x10); 
ILI9806E_SpiWriteCmd(0x53);ILI9806E_SpiWriteData(0x10); 
 
//**************************************************************** 
//Page7Command 
ILI9806E_SpiWriteCmd(0xFF);ILI9806E_SpiWriteData(0xFF);ILI9806E_SpiWriteData(0x98);ILI9806E_SpiWriteData(0x06);ILI9806E_SpiWriteData(0x04);ILI9806E_SpiWriteData(0x07); 
ILI9806E_SpiWriteCmd(0x18);ILI9806E_SpiWriteData(0x1D); 
ILI9806E_SpiWriteCmd(0x17);ILI9806E_SpiWriteData(0x22); 
ILI9806E_SpiWriteCmd(0x02);ILI9806E_SpiWriteData(0x77); 
ILI9806E_SpiWriteCmd(0xE1);ILI9806E_SpiWriteData(0x79); 
ILI9806E_SpiWriteCmd(0x06);ILI9806E_SpiWriteData(0x13); 
 
//**************************************************************** 
ILI9806E_SpiWriteCmd(0xFF);ILI9806E_SpiWriteData(0xFF);ILI9806E_SpiWriteData(0x98);ILI9806E_SpiWriteData(0x06);ILI9806E_SpiWriteData(0x04);ILI9806E_SpiWriteData(0x00); 
mdelay(10);
//ILI9806E_SpiWriteCmd(0x35);ILI9806E_SpiWriteData(0x00);                 // Sleep-Out
mdelay(10);                // Sleep-Out
ILI9806E_SpiWriteCmd(0x36);ILI9806E_SpiWriteData(0x00); 
ILI9806E_SpiWriteCmd(0x3a);ILI9806E_SpiWriteData(0x57);
mdelay(10);
ILI9806E_SpiWriteCmd(0x11);ILI9806E_SpiWriteData(0x00);                 // Sleep-Out
mdelay(120);
ILI9806E_SpiWriteCmd(0x29);ILI9806E_SpiWriteData(0x00);                 // Display On	
mdelay(10);  
#elif defined(CONFIG_ZYT_ILI9806E_7675_QY)
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x01);	 // Change to Page 1
	
	ILI9806E_SpiWriteCmd(0x08);ILI9806E_SpiWriteData(0x10);   // output SDA	
	//spi_send_cmd_data(0x21);ILI9806E_SpiWriteData(0x00);   // DE = 1 Active	
	ILI9806E_SpiWriteCmd(0x21);ILI9806E_SpiWriteData(0x01);   // DE = 1 Active	
	ILI9806E_SpiWriteCmd(0x30);ILI9806E_SpiWriteData(0x02);   // 480 X 800	
	ILI9806E_SpiWriteCmd(0x31);ILI9806E_SpiWriteData(0x02);   // 2dot Inversion	
	ILI9806E_SpiWriteCmd(0x60);ILI9806E_SpiWriteData(0x07);   // SDTI	
	ILI9806E_SpiWriteCmd(0x61);ILI9806E_SpiWriteData(0x06);   // CRTI	
	ILI9806E_SpiWriteCmd(0x62);ILI9806E_SpiWriteData(0x06);   // EQTI	
	ILI9806E_SpiWriteCmd(0x63);ILI9806E_SpiWriteData(0x04);   // PCTI	
	ILI9806E_SpiWriteCmd(0x40);ILI9806E_SpiWriteData(0x15);   // BT  +2.5/-2.5 pump for DDVDH-L	
	ILI9806E_SpiWriteCmd(0x41);ILI9806E_SpiWriteData(0x22);   // DVDDH DVDDL clamp  	 
	ILI9806E_SpiWriteCmd(0x42);ILI9806E_SpiWriteData(0x00);   // VGH/VGL 31.37ma 32.7ma  0.03	
	ILI9806E_SpiWriteCmd(0x43);ILI9806E_SpiWriteData(0x8B);   // VGH/VGL 13 -9  5 -3.8 1.6 -1.5 -2.75	
	ILI9806E_SpiWriteCmd(0x44);ILI9806E_SpiWriteData(0x8C);   // VGH/VGL 	
	ILI9806E_SpiWriteCmd(0x45);ILI9806E_SpiWriteData(0x1B);   // VGL_REG  -10V 	
	ILI9806E_SpiWriteCmd(0x50);ILI9806E_SpiWriteData(0x5b);   // VGMP	50
	ILI9806E_SpiWriteCmd(0x51);ILI9806E_SpiWriteData(0x5b);   // VGMN	50
	ILI9806E_SpiWriteCmd(0x52);ILI9806E_SpiWriteData(0x00);   //Flicker	
	ILI9806E_SpiWriteCmd(0x53);ILI9806E_SpiWriteData(0x69);   //Flicker4F	65
	///==============Positive Gamma==============///	
	ILI9806E_SpiWriteCmd(0xA0);ILI9806E_SpiWriteData(0x00);  // Gamma 0 /255	
	ILI9806E_SpiWriteCmd(0xA1);ILI9806E_SpiWriteData(0x14);  // Gamma 4 /251	
	ILI9806E_SpiWriteCmd(0xA2);ILI9806E_SpiWriteData(0x1A);  // Gamma 8 /247	
	ILI9806E_SpiWriteCmd(0xA3);ILI9806E_SpiWriteData(0x06);  // Gamma 16	/239	
	ILI9806E_SpiWriteCmd(0xA4);ILI9806E_SpiWriteData(0x02);  // Gamma 24 /231	
	ILI9806E_SpiWriteCmd(0xA5);ILI9806E_SpiWriteData(0x04);  // Gamma 52 / 203	
	ILI9806E_SpiWriteCmd(0xA6);ILI9806E_SpiWriteData(0x1C);  // Gamma 80 / 175	
	ILI9806E_SpiWriteCmd(0xA7);ILI9806E_SpiWriteData(0x00);  // Gamma 108 /147	
	ILI9806E_SpiWriteCmd(0xA8);ILI9806E_SpiWriteData(0x19);  // Gamma 147 /108	
	ILI9806E_SpiWriteCmd(0xA9);ILI9806E_SpiWriteData(0x0C);  // Gamma 175 / 80	
	ILI9806E_SpiWriteCmd(0xAA);ILI9806E_SpiWriteData(0x0E);  // Gamma 203 / 52	
	ILI9806E_SpiWriteCmd(0xAB);ILI9806E_SpiWriteData(0x04);  // Gamma 231 / 24	
	ILI9806E_SpiWriteCmd(0xAC);ILI9806E_SpiWriteData(0x07);  // Gamma 239 / 16	
	ILI9806E_SpiWriteCmd(0xAD);ILI9806E_SpiWriteData(0x1A);  // Gamma 247 / 8	
	ILI9806E_SpiWriteCmd(0xAE);ILI9806E_SpiWriteData(0x02);  // Gamma 251 / 4	
	ILI9806E_SpiWriteCmd(0xAF);ILI9806E_SpiWriteData(0x00);  // Gamma 255 / 0	
	///==============Nagitive Gamma==============///	
	ILI9806E_SpiWriteCmd(0xC0);ILI9806E_SpiWriteData(0x00);  // Gamma 0 	
	ILI9806E_SpiWriteCmd(0xC1);ILI9806E_SpiWriteData(0x01);  // Gamma 4	
	ILI9806E_SpiWriteCmd(0xC2);ILI9806E_SpiWriteData(0x0A);  // Gamma 8	
	ILI9806E_SpiWriteCmd(0xC3);ILI9806E_SpiWriteData(0x10);  // Gamma 16	
	ILI9806E_SpiWriteCmd(0xC4);ILI9806E_SpiWriteData(0x07);  // Gamma 24	
	ILI9806E_SpiWriteCmd(0xC5);ILI9806E_SpiWriteData(0x10);  // Gamma 52	
	ILI9806E_SpiWriteCmd(0xC6);ILI9806E_SpiWriteData(0x01);  // Gamma 80	
	ILI9806E_SpiWriteCmd(0xC7);ILI9806E_SpiWriteData(0x0F);  // Gamma 108	
	ILI9806E_SpiWriteCmd(0xC8);ILI9806E_SpiWriteData(0x06);  // Gamma 147
	ILI9806E_SpiWriteCmd(0xC9);ILI9806E_SpiWriteData(0x0A);  // Gamma 175	
	ILI9806E_SpiWriteCmd(0xCA);ILI9806E_SpiWriteData(0x12);  // Gamma 203	
	ILI9806E_SpiWriteCmd(0xCB);ILI9806E_SpiWriteData(0x0B);  // Gamma 231	
	ILI9806E_SpiWriteCmd(0xCC);ILI9806E_SpiWriteData(0x14);  // Gamma 239	
	ILI9806E_SpiWriteCmd(0xCD);ILI9806E_SpiWriteData(0x17);  // Gamma 247	
	ILI9806E_SpiWriteCmd(0xCE);ILI9806E_SpiWriteData(0x16);  // Gamma 251	
	ILI9806E_SpiWriteCmd(0xCF);ILI9806E_SpiWriteData(0x00);  // Gamma 255	
	//**************************************************************************//
	//****************************** Page 6 Command ****************************//
	//**************************************************************************//
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x06);	 // Change to Page 6
	
	ILI9806E_SpiWriteCmd(0x00);ILI9806E_SpiWriteData(0x20);	
	ILI9806E_SpiWriteCmd(0x01);ILI9806E_SpiWriteData(0x05);	
	ILI9806E_SpiWriteCmd(0x02);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x03);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x04);ILI9806E_SpiWriteData(0x01);	
	ILI9806E_SpiWriteCmd(0x05);ILI9806E_SpiWriteData(0x01);	
	ILI9806E_SpiWriteCmd(0x06);ILI9806E_SpiWriteData(0x88);
	ILI9806E_SpiWriteCmd(0x07);ILI9806E_SpiWriteData(0x04);	
	ILI9806E_SpiWriteCmd(0x08);ILI9806E_SpiWriteData(0x01);	
	ILI9806E_SpiWriteCmd(0x09);ILI9806E_SpiWriteData(0x90);	
	ILI9806E_SpiWriteCmd(0x0A);ILI9806E_SpiWriteData(0x04);	
	ILI9806E_SpiWriteCmd(0x0B);ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0C);ILI9806E_SpiWriteData(0x01);	
	ILI9806E_SpiWriteCmd(0x0D);ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0E);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x0F);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x10);ILI9806E_SpiWriteData(0x55);	
	ILI9806E_SpiWriteCmd(0x11);ILI9806E_SpiWriteData(0x50);	
	ILI9806E_SpiWriteCmd(0x12);ILI9806E_SpiWriteData(0x01);	
	ILI9806E_SpiWriteCmd(0x13);ILI9806E_SpiWriteData(0x0C);	
	ILI9806E_SpiWriteCmd(0x14);ILI9806E_SpiWriteData(0x0D);	
	ILI9806E_SpiWriteCmd(0x15);ILI9806E_SpiWriteData(0x43);	
	ILI9806E_SpiWriteCmd(0x16);ILI9806E_SpiWriteData(0x0B);	
	ILI9806E_SpiWriteCmd(0x17);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x18);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x19);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x1A);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x1B);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x1C);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x1D);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x20);ILI9806E_SpiWriteData(0x01);	
	ILI9806E_SpiWriteCmd(0x21);ILI9806E_SpiWriteData(0x23);	
	ILI9806E_SpiWriteCmd(0x22);ILI9806E_SpiWriteData(0x45);	
	ILI9806E_SpiWriteCmd(0x23);ILI9806E_SpiWriteData(0x67);	
	ILI9806E_SpiWriteCmd(0x24);ILI9806E_SpiWriteData(0x01);	
	ILI9806E_SpiWriteCmd(0x25);ILI9806E_SpiWriteData(0x23);	
	ILI9806E_SpiWriteCmd(0x26);ILI9806E_SpiWriteData(0x45);	
	ILI9806E_SpiWriteCmd(0x27);ILI9806E_SpiWriteData(0x67);
	
	ILI9806E_SpiWriteCmd(0x30);ILI9806E_SpiWriteData(0x02);	
	ILI9806E_SpiWriteCmd(0x31);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x32);ILI9806E_SpiWriteData(0x11);	
	ILI9806E_SpiWriteCmd(0x33);ILI9806E_SpiWriteData(0xAA);	
	ILI9806E_SpiWriteCmd(0x34);ILI9806E_SpiWriteData(0xBB);	
	ILI9806E_SpiWriteCmd(0x35);ILI9806E_SpiWriteData(0x66);	
	ILI9806E_SpiWriteCmd(0x36);ILI9806E_SpiWriteData(0x00);	
	ILI9806E_SpiWriteCmd(0x37);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x38);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x39);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x3A);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x3B);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x3C);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x3D);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x3E);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x3F);ILI9806E_SpiWriteData(0x22);
	
	ILI9806E_SpiWriteCmd(0x40);ILI9806E_SpiWriteData(0x22);	
	ILI9806E_SpiWriteCmd(0x52);ILI9806E_SpiWriteData(0x10);	
	ILI9806E_SpiWriteCmd(0x53);ILI9806E_SpiWriteData(0x10);  // 0x10:VGLO tie VGL; 0x12:VGLO tie VGL_REG
	
	//**************************************************************************//
	//****************************** Page 7 Command ****************************//
	//**************************************************************************//
	
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x07);// Change to Page 7
	
	ILI9806E_SpiWriteCmd(0x17);ILI9806E_SpiWriteData(0x22);	// VGL_REG ON  // 0x22:VGLO tie VGL; 0x32:VGLO tie VGL_REG	
	ILI9806E_SpiWriteCmd(0x18);ILI9806E_SpiWriteData(0x1D);	
	ILI9806E_SpiWriteCmd(0xE1);ILI9806E_SpiWriteData(0x79);  	
	ILI9806E_SpiWriteCmd(0x02);ILI9806E_SpiWriteData(0x77); 	
	//**************************************************************************//
	//****************************** Page 0 Command ****************************//
	//**************************************************************************//
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x00);   // Change to Page 0

	ILI9806E_SpiWriteCmd(0x35);ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x36);ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x3A);ILI9806E_SpiWriteData(0x55);
	
	ILI9806E_SpiWriteCmd(0x11);  // Sleep-Out
	mdelay(15);
	ILI9806E_SpiWriteCmd(0x29);  // Display On
#elif  defined(CONFIG_ZYT_ILI9806E_7672_WANDA_CTC)
        ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 1
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x01); 
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x08); ILI9806E_SpiWriteData(0x10);                 // output SDA
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x21); ILI9806E_SpiWriteData(0x01);                 // DE = 1 Active
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x30); ILI9806E_SpiWriteData(0x02);                 // 480 X 800
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x31); ILI9806E_SpiWriteData(0x02);                 // Column Inversion
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x60); ILI9806E_SpiWriteData(0x07);                 // SDTI
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x61); ILI9806E_SpiWriteData(0x00);                // CRTI
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x62); ILI9806E_SpiWriteData(0x08);                 // EQTI
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x63); ILI9806E_SpiWriteData(0x00);                // PCTI
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x40); ILI9806E_SpiWriteData(0x1A);                // BT  +2.5/-2.5 pump for DDVDH-L
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x41); ILI9806E_SpiWriteData(0x44);                 // DVDDH DVDDL clamp  
		mdelay(10); 
	ILI9806E_SpiWriteCmd(0x42); ILI9806E_SpiWriteData(0x01);                 // VGH/VGL 
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x43); ILI9806E_SpiWriteData(0x09);                 // VGH/VGL 
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x44); ILI9806E_SpiWriteData(0x08);                 // VGH/VGL 
	

		mdelay(10);
	ILI9806E_SpiWriteCmd(0x50); ILI9806E_SpiWriteData(0x78);                 // VGMP
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x51); ILI9806E_SpiWriteData(0x78);                 // VGMN
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x52); ILI9806E_SpiWriteData(0x00);                   //Flicker
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x53); ILI9806E_SpiWriteData(0x61);                 //Flicker4F
		mdelay(10);

	//++++++++++++++++++ Gamma Setting ++++++++++++++++++//

	ILI9806E_SpiWriteCmd(0xA0); ILI9806E_SpiWriteData(0x00);  // Gamma 0 /255
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA1); ILI9806E_SpiWriteData(0x06);  // Gamma 4 /251
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA2); ILI9806E_SpiWriteData(0x0C);  // Gamma 8 /247
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA3); ILI9806E_SpiWriteData(0x12);  // Gamma 16	/239
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA4); ILI9806E_SpiWriteData(0x0D);  // Gamma 24 /231
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA5); ILI9806E_SpiWriteData(0x1F);  // Gamma 52 / 203
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA6); ILI9806E_SpiWriteData(0x0B);  // Gamma 80 / 175
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA7); ILI9806E_SpiWriteData(0x08);  // Gamma 108 /147
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA8); ILI9806E_SpiWriteData(0x04);  // Gamma 147 /108
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xA9); ILI9806E_SpiWriteData(0x0A);  // Gamma 175 / 80
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xAA); ILI9806E_SpiWriteData(0x06);  // Gamma 203 / 52
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xAB); ILI9806E_SpiWriteData(0x04);  // Gamma 231 / 24
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xAC); ILI9806E_SpiWriteData(0x10);  // Gamma 239 / 16
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xAD); ILI9806E_SpiWriteData(0x37);  // Gamma 247 / 8
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xAE); ILI9806E_SpiWriteData(0x30);  // Gamma 251 / 4
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xAF); ILI9806E_SpiWriteData(0x07);  // Gamma 255 / 0
		mdelay(10);
	//==============Nagitive
	ILI9806E_SpiWriteCmd(0xC0); ILI9806E_SpiWriteData(0x00);  // Gamma 0 
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC1); ILI9806E_SpiWriteData(0x06);  // Gamma 4
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC2); ILI9806E_SpiWriteData(0x1F);  // Gamma 8
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC3); ILI9806E_SpiWriteData(0x06);  // Gamma 16
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC4); ILI9806E_SpiWriteData(0x04);  // Gamma 24
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC5); ILI9806E_SpiWriteData(0x13);  // Gamma 52
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC6); ILI9806E_SpiWriteData(0x05);  // Gamma 80
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC7); ILI9806E_SpiWriteData(0x05);  // Gamma 108
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC8); ILI9806E_SpiWriteData(0x03);  // Gamma 147
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xC9); ILI9806E_SpiWriteData(0x06);  // Gamma 175
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xCA); ILI9806E_SpiWriteData(0x06);  // Gamma 203
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xCB); ILI9806E_SpiWriteData(0x03);  // Gamma 231
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xCC); ILI9806E_SpiWriteData(0x01);  // Gamma 239
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xCD); ILI9806E_SpiWriteData(0x27);  // Gamma 247
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xCE); ILI9806E_SpiWriteData(0x24);  // Gamma 251
		mdelay(10);
	ILI9806E_SpiWriteCmd(0xCF); ILI9806E_SpiWriteData(0x07);  // Gamma 255
		mdelay(10);

	ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 7
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x07); 
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x17); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x02); ILI9806E_SpiWriteData(0x77);  // VGL_REG ON
		mdelay(10);
        ILI9806E_SpiWriteCmd(0xE1); ILI9806E_SpiWriteData(0x79);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++//

	//****************************************************************************//
	//****************************** Page 6 Command ******************************//
	//****************************************************************************//

	ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 6
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x06); 
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x00); ILI9806E_SpiWriteData(0x3C);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x01); ILI9806E_SpiWriteData(0x06);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x02); ILI9806E_SpiWriteData(0x00);    
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x03); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x04); ILI9806E_SpiWriteData(0x0D);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x05); ILI9806E_SpiWriteData(0x0D);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x06); ILI9806E_SpiWriteData(0x80);    
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x07); ILI9806E_SpiWriteData(0x04);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x08); ILI9806E_SpiWriteData(0x03);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x09); ILI9806E_SpiWriteData(0x00);    
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x0A); ILI9806E_SpiWriteData(0x00);    
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x0B); ILI9806E_SpiWriteData(0x00);    
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x0C); ILI9806E_SpiWriteData(0x0D);				 //01
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x0D); ILI9806E_SpiWriteData(0x0D);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x0E); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x0F); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x10); ILI9806E_SpiWriteData(0x50);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x11); ILI9806E_SpiWriteData(0xD0);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x12); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x13); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x14); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x15); ILI9806E_SpiWriteData(0xC0);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x16); ILI9806E_SpiWriteData(0x08);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x17); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x18); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x19); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x1A); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x1B); ILI9806E_SpiWriteData(0x00);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x1C); ILI9806E_SpiWriteData(0x48);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x1D); ILI9806E_SpiWriteData(0x00);
		mdelay(10);

	ILI9806E_SpiWriteCmd(0x20); ILI9806E_SpiWriteData(0x01);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x21); ILI9806E_SpiWriteData(0x23);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x22); ILI9806E_SpiWriteData(0x45);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x23); ILI9806E_SpiWriteData(0x67);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x24); ILI9806E_SpiWriteData(0x01);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x25); ILI9806E_SpiWriteData(0x23);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x26); ILI9806E_SpiWriteData(0x45);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x27); ILI9806E_SpiWriteData(0x67);
		mdelay(10);

	ILI9806E_SpiWriteCmd(0x30); ILI9806E_SpiWriteData(0x13);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x31); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x32); ILI9806E_SpiWriteData(0xDD);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x33); ILI9806E_SpiWriteData(0xCC);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x34); ILI9806E_SpiWriteData(0xBB);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x35); ILI9806E_SpiWriteData(0xAA);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x36); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x37); ILI9806E_SpiWriteData(0x26);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x38); ILI9806E_SpiWriteData(0x72);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x39); ILI9806E_SpiWriteData(0xFF);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x3A); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x3B); ILI9806E_SpiWriteData(0xEE);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x3C); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x3D); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x3E); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x3F); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x40); ILI9806E_SpiWriteData(0x22);
		mdelay(10);
	

	//****************************************************************************//
	//****************************** Page 0 Command ******************************//
	//****************************************************************************//

		mdelay(10);
	ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 0
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x00); 
		mdelay(10);

         ILI9806E_SpiWriteCmd(0x3A); 
	ILI9806E_SpiWriteData(0x55);  
		mdelay(10);
	ILI9806E_SpiWriteCmd(0x11); ILI9806E_SpiWriteData(0x00);      // Sleep-Out
	mdelay(120);
	ILI9806E_SpiWriteCmd(0x29); ILI9806E_SpiWriteData(0x00);      // Display On
	mdelay(120);


#elif defined(CONFIG_ZYT_ILI9806E_7672_XXX_YTQ)
	ILI9806E_SpiWriteCmd(0xFF); // EXTC Command Set enable register
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x10);

	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x31);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x1A);

	ILI9806E_SpiWriteCmd(0x41);
	ILI9806E_SpiWriteData(0x66);

	ILI9806E_SpiWriteCmd(0x42);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x43);
	ILI9806E_SpiWriteData(0x09);

	ILI9806E_SpiWriteCmd(0x44);
	ILI9806E_SpiWriteData(0x86);

	ILI9806E_SpiWriteCmd(0x50);
	ILI9806E_SpiWriteData(0x78);

	ILI9806E_SpiWriteCmd(0x51);
	ILI9806E_SpiWriteData(0x78);

	ILI9806E_SpiWriteCmd(0x52);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x53);
	ILI9806E_SpiWriteData(0x50);

	ILI9806E_SpiWriteCmd(0x60);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0x61);
	ILI9806E_SpiWriteData(0x04);

	ILI9806E_SpiWriteCmd(0x62);
	ILI9806E_SpiWriteData(0x06);

	ILI9806E_SpiWriteCmd(0x63);
	ILI9806E_SpiWriteData(0x02);

	//++++++++++++++++++ Gamma Setting ++++++++++++++++++//

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0xA0);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xA1);
	ILI9806E_SpiWriteData(0x0A);

	ILI9806E_SpiWriteCmd(0xA2);
	ILI9806E_SpiWriteData(0x09);

	ILI9806E_SpiWriteCmd(0xA3);
	ILI9806E_SpiWriteData(0x17);

	ILI9806E_SpiWriteCmd(0xA4);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0xA5);
	ILI9806E_SpiWriteData(0X1D);

	ILI9806E_SpiWriteCmd(0xA6);
	ILI9806E_SpiWriteData(0x0A);

	ILI9806E_SpiWriteCmd(0xA7);
	ILI9806E_SpiWriteData(0x0B);

	ILI9806E_SpiWriteCmd(0xA8);
	ILI9806E_SpiWriteData(0x03);

	ILI9806E_SpiWriteCmd(0xA9);
	ILI9806E_SpiWriteData(0x0E);

	ILI9806E_SpiWriteCmd(0xAA);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0xAB);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0xAC);
	ILI9806E_SpiWriteData(0x14);

	ILI9806E_SpiWriteCmd(0xAD);
	ILI9806E_SpiWriteData(0x30);

	ILI9806E_SpiWriteCmd(0xAE);
	ILI9806E_SpiWriteData(0x33);

	ILI9806E_SpiWriteCmd(0xAF);
	ILI9806E_SpiWriteData(0x00);

	//==============Nagitive==============//

	ILI9806E_SpiWriteCmd(0xC0);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xC1);
	ILI9806E_SpiWriteData(0x0C);

	ILI9806E_SpiWriteCmd(0xC2);
	ILI9806E_SpiWriteData(0x25);

	ILI9806E_SpiWriteCmd(0xC3);
	ILI9806E_SpiWriteData(0x0F);

	ILI9806E_SpiWriteCmd(0xC4);
	ILI9806E_SpiWriteData(0x0F);

	ILI9806E_SpiWriteCmd(0xC5);
	ILI9806E_SpiWriteData(0x14);

	ILI9806E_SpiWriteCmd(0xC6);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0xC7);
	ILI9806E_SpiWriteData(0x03);

	ILI9806E_SpiWriteCmd(0xC8);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0xC9);
	ILI9806E_SpiWriteData(0x03);

	ILI9806E_SpiWriteCmd(0xCA);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0xCB);
	ILI9806E_SpiWriteData(0x06);

	ILI9806E_SpiWriteCmd(0xCC);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0xCD);
	ILI9806E_SpiWriteData(0x1F);

	ILI9806E_SpiWriteCmd(0xCE);
	ILI9806E_SpiWriteData(0x16);

	ILI9806E_SpiWriteCmd(0xCF);
	ILI9806E_SpiWriteData(0x00);

	//************ Page 6 Command **************//

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x06);

	ILI9806E_SpiWriteCmd(0x00);
	ILI9806E_SpiWriteData(0x20);

	ILI9806E_SpiWriteCmd(0x01);
	ILI9806E_SpiWriteData(0x06);

	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x20);

	ILI9806E_SpiWriteCmd(0x03);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x04);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x05);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x98);

	ILI9806E_SpiWriteCmd(0x07);
	ILI9806E_SpiWriteData(0x04);

	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0x09);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0A);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0B);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0C);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x0D);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x0E);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0F);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x10);
	ILI9806E_SpiWriteData(0xFF);

	ILI9806E_SpiWriteCmd(0x11);
	ILI9806E_SpiWriteData(0xF0);

	ILI9806E_SpiWriteCmd(0x12);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x13);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x14);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x15);
	ILI9806E_SpiWriteData(0xC0);

	ILI9806E_SpiWriteCmd(0x16);
	ILI9806E_SpiWriteData(0x08);

	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x19);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1A);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1B);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1C);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1D);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x20);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x23);

	ILI9806E_SpiWriteCmd(0x22);
	ILI9806E_SpiWriteData(0x45);

	ILI9806E_SpiWriteCmd(0x23);
	ILI9806E_SpiWriteData(0x67);

	ILI9806E_SpiWriteCmd(0x24);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x25);
	ILI9806E_SpiWriteData(0x23);

	ILI9806E_SpiWriteCmd(0x26);
	ILI9806E_SpiWriteData(0x45);

	ILI9806E_SpiWriteCmd(0x27);
	ILI9806E_SpiWriteData(0x67);

	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x12);

	ILI9806E_SpiWriteCmd(0x31);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x32);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x33);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x34);
	ILI9806E_SpiWriteData(0x87);

	ILI9806E_SpiWriteCmd(0x35);
	ILI9806E_SpiWriteData(0x96);

	ILI9806E_SpiWriteCmd(0x36);
	ILI9806E_SpiWriteData(0xBA);

	ILI9806E_SpiWriteCmd(0x37);
	ILI9806E_SpiWriteData(0xAB);

	ILI9806E_SpiWriteCmd(0x38);
	ILI9806E_SpiWriteData(0xDC);

	ILI9806E_SpiWriteCmd(0x39);
	ILI9806E_SpiWriteData(0xCD);

	ILI9806E_SpiWriteCmd(0x3A); 
	ILI9806E_SpiWriteData(0x78);

	ILI9806E_SpiWriteCmd(0x3B);
	ILI9806E_SpiWriteData(0x69);

	ILI9806E_SpiWriteCmd(0x3C);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3D);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3E);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3F);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x22);

	//************* Page 7 Command ***************//

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x1D);

	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x12);

	//************** Page 0 Command ****************//

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x55);

	ILI9806E_SpiWriteCmd(0x11);
	mdelay(120);
	ILI9806E_SpiWriteCmd(0x29);
#elif defined(CONFIG_ZYT_ILI9806E_7675_CFZZ)
ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x08);
ILI9806E_SpiWriteData(0x10);

ILI9806E_SpiWriteCmd(0x21);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x30);
ILI9806E_SpiWriteData(0x02);

ILI9806E_SpiWriteCmd(0x31);
ILI9806E_SpiWriteData(0x02);

ILI9806E_SpiWriteCmd(0x40);
ILI9806E_SpiWriteData(0x1A);

ILI9806E_SpiWriteCmd(0x41);
ILI9806E_SpiWriteData(0x66);

ILI9806E_SpiWriteCmd(0x42);
ILI9806E_SpiWriteData(0x02);

ILI9806E_SpiWriteCmd(0x43);
ILI9806E_SpiWriteData(0x09);

ILI9806E_SpiWriteCmd(0x44);
ILI9806E_SpiWriteData(0x86);

ILI9806E_SpiWriteCmd(0x50);
ILI9806E_SpiWriteData(0x78);

ILI9806E_SpiWriteCmd(0x51);
ILI9806E_SpiWriteData(0x78);

ILI9806E_SpiWriteCmd(0x52);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x53);
ILI9806E_SpiWriteData(0x5d);

ILI9806E_SpiWriteCmd(0x60);
ILI9806E_SpiWriteData(0x07);

ILI9806E_SpiWriteCmd(0x61);
ILI9806E_SpiWriteData(0x04);

ILI9806E_SpiWriteCmd(0x62);
ILI9806E_SpiWriteData(0x06);

ILI9806E_SpiWriteCmd(0x63);
ILI9806E_SpiWriteData(0x02);


ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0xA0);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0xA1);
ILI9806E_SpiWriteData(0x0A);

ILI9806E_SpiWriteCmd(0xA2);
ILI9806E_SpiWriteData(0x09);

ILI9806E_SpiWriteCmd(0xA3);
ILI9806E_SpiWriteData(0x17);

ILI9806E_SpiWriteCmd(0xA4);
ILI9806E_SpiWriteData(0x07);

ILI9806E_SpiWriteCmd(0xA5);
ILI9806E_SpiWriteData(0x1D);

ILI9806E_SpiWriteCmd(0xA6);
ILI9806E_SpiWriteData(0x0A);

ILI9806E_SpiWriteCmd(0xA7);
ILI9806E_SpiWriteData(0x0B);

ILI9806E_SpiWriteCmd(0xA8);
ILI9806E_SpiWriteData(0x03);

ILI9806E_SpiWriteCmd(0xA9);
ILI9806E_SpiWriteData(0x0E);

ILI9806E_SpiWriteCmd(0xAA);
ILI9806E_SpiWriteData(0x05);

ILI9806E_SpiWriteCmd(0xAB);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0xAC);
ILI9806E_SpiWriteData(0x14);

ILI9806E_SpiWriteCmd(0xAD);
ILI9806E_SpiWriteData(0x30);

ILI9806E_SpiWriteCmd(0xAE);
ILI9806E_SpiWriteData(0x33);

ILI9806E_SpiWriteCmd(0xAF);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0xC0);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0xC1);
ILI9806E_SpiWriteData(0x0C);

ILI9806E_SpiWriteCmd(0xC2);
ILI9806E_SpiWriteData(0x25);

ILI9806E_SpiWriteCmd(0xC3);
ILI9806E_SpiWriteData(0x0F);

ILI9806E_SpiWriteCmd(0xC4);
ILI9806E_SpiWriteData(0x0F);

ILI9806E_SpiWriteCmd(0xC5);
ILI9806E_SpiWriteData(0x14);

ILI9806E_SpiWriteCmd(0xC6);
ILI9806E_SpiWriteData(0x07);

ILI9806E_SpiWriteCmd(0xC7);
ILI9806E_SpiWriteData(0x03);

ILI9806E_SpiWriteCmd(0xC8);
ILI9806E_SpiWriteData(0x05);

ILI9806E_SpiWriteCmd(0xC9);
ILI9806E_SpiWriteData(0x03);

ILI9806E_SpiWriteCmd(0xCA);
ILI9806E_SpiWriteData(0x07);

ILI9806E_SpiWriteCmd(0xCB);
ILI9806E_SpiWriteData(0x06);

ILI9806E_SpiWriteCmd(0xCC);
ILI9806E_SpiWriteData(0x05);

ILI9806E_SpiWriteCmd(0xCD);
ILI9806E_SpiWriteData(0x1F);

ILI9806E_SpiWriteCmd(0xCE);
ILI9806E_SpiWriteData(0x16);

ILI9806E_SpiWriteCmd(0xCF);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x06);

ILI9806E_SpiWriteCmd(0x00);
ILI9806E_SpiWriteData(0x20);

ILI9806E_SpiWriteCmd(0x01);
ILI9806E_SpiWriteData(0x06);

ILI9806E_SpiWriteCmd(0x02);
ILI9806E_SpiWriteData(0x20);

ILI9806E_SpiWriteCmd(0x03);
ILI9806E_SpiWriteData(0x02);

ILI9806E_SpiWriteCmd(0x04);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x05);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x06);
ILI9806E_SpiWriteData(0x98);

ILI9806E_SpiWriteCmd(0x07);
ILI9806E_SpiWriteData(0x04);

ILI9806E_SpiWriteCmd(0x08);
ILI9806E_SpiWriteData(0x05);

ILI9806E_SpiWriteCmd(0x09);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x0A);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x0B);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x0C);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x0D);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x0E);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x0F);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x10);
ILI9806E_SpiWriteData(0xFF);

ILI9806E_SpiWriteCmd(0x11);
ILI9806E_SpiWriteData(0xF0);

ILI9806E_SpiWriteCmd(0x12);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x13);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x14);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x15);
ILI9806E_SpiWriteData(0xC0);

ILI9806E_SpiWriteCmd(0x16);
ILI9806E_SpiWriteData(0x08);

ILI9806E_SpiWriteCmd(0x17);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x18);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x19);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x1A);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x1B);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x1C);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x1D);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x20);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x21);
ILI9806E_SpiWriteData(0x23);

ILI9806E_SpiWriteCmd(0x22);
ILI9806E_SpiWriteData(0x45);

ILI9806E_SpiWriteCmd(0x23);
ILI9806E_SpiWriteData(0x67);

ILI9806E_SpiWriteCmd(0x24);
ILI9806E_SpiWriteData(0x01);

ILI9806E_SpiWriteCmd(0x25);
ILI9806E_SpiWriteData(0x23);

ILI9806E_SpiWriteCmd(0x26);
ILI9806E_SpiWriteData(0x45);

ILI9806E_SpiWriteCmd(0x27);
ILI9806E_SpiWriteData(0x67);

ILI9806E_SpiWriteCmd(0x30);
ILI9806E_SpiWriteData(0x12);

ILI9806E_SpiWriteCmd(0x31);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0x32);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0x33);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0x34);
ILI9806E_SpiWriteData(0x87);

ILI9806E_SpiWriteCmd(0x35);
ILI9806E_SpiWriteData(0x96);

ILI9806E_SpiWriteCmd(0x36);
ILI9806E_SpiWriteData(0xBA);

ILI9806E_SpiWriteCmd(0x37);
ILI9806E_SpiWriteData(0xAB);

ILI9806E_SpiWriteCmd(0x38);
ILI9806E_SpiWriteData(0xDC);

ILI9806E_SpiWriteCmd(0x39);
ILI9806E_SpiWriteData(0xCD);

ILI9806E_SpiWriteCmd(0x3A);
ILI9806E_SpiWriteData(0x78);

ILI9806E_SpiWriteCmd(0x3B);
ILI9806E_SpiWriteData(0x69);

ILI9806E_SpiWriteCmd(0x3C);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0x3D);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0x3E);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0x3F);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0x40);
ILI9806E_SpiWriteData(0x22);

ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x07);

ILI9806E_SpiWriteCmd(0x18);
ILI9806E_SpiWriteData(0x1D);

ILI9806E_SpiWriteCmd(0x17);
ILI9806E_SpiWriteData(0x12);

ILI9806E_SpiWriteCmd(0x02);
ILI9806E_SpiWriteData(0x77);

ILI9806E_SpiWriteCmd(0xE1);
ILI9806E_SpiWriteData(0x79);

ILI9806E_SpiWriteCmd(0xFF);
ILI9806E_SpiWriteData(0xFF);
ILI9806E_SpiWriteData(0x98);
ILI9806E_SpiWriteData(0x06);
ILI9806E_SpiWriteData(0x04);
ILI9806E_SpiWriteData(0x00);

ILI9806E_SpiWriteCmd(0x3A);
ILI9806E_SpiWriteData(0x55);

ILI9806E_SpiWriteCmd(0x11);
mdelay(150);
ILI9806E_SpiWriteCmd(0x29);
mdelay(10);


#elif defined(CONFIG_ZYT_ILI9806E_7271_CFZZ)
	ILI9806E_SpiWriteCmd(0xFF);  

	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98);  
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x01);	 // Change to Page 1
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x08);  

	ILI9806E_SpiWriteData(0x10);				 // output SDA
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x21);  

	ILI9806E_SpiWriteData(0x01);				 // DE = 1 Active
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x30);  

	ILI9806E_SpiWriteData(0x02);				 // 480 X 800
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x31);  

	ILI9806E_SpiWriteData(0x02);				// 2-dot Inversion
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x40);  

	ILI9806E_SpiWriteData(0x15);				// DDVDH/DDVDL 2.5/-2.5(15)
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x41);  

	ILI9806E_SpiWriteData(0x77);				 // DDVDH/DDVDL 5.4 -5.4
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x42);  

	ILI9806E_SpiWriteData(0x03);				// VGH/VGL 2DDVDH-DDVDL/DDVDL-DDVDH
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x43);  

	ILI9806E_SpiWriteData(0x09);				 // VGH/VGL 15V 89
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x44);  

	ILI9806E_SpiWriteData(0x02);				 // VGH/VGL -8.0 82
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x50);  

	ILI9806E_SpiWriteData(0x78);				 // VGMP 4.5
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x51);  

	ILI9806E_SpiWriteData(0x78);				 // VGMN -4.5
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x52);  

	ILI9806E_SpiWriteData(0x00);				   //Flicker
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x53);  

	ILI9806E_SpiWriteData(0x59);				   //Flicker
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x57);  

	ILI9806E_SpiWriteData(0x50);				   //Flicker
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x60);   
	ILI9806E_SpiWriteData(0x07);				 // SDTI  05
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x61);  
	ILI9806E_SpiWriteData(0x00);				// CRTI   00
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x62);   
	ILI9806E_SpiWriteData(0x07);				 // EQTI   06
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x63);  
	ILI9806E_SpiWriteData(0x00);				// PCTI    03
	mdelay(10);

	//++++++++++++++++++ Gamma Setting ++++++++++++++++++//


	ILI9806E_SpiWriteCmd(0xFF);  

	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98);  
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x01);	 // Change to Page 1
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA0);  

	ILI9806E_SpiWriteData(0x00);  // Gamma 0 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA1);  

	ILI9806E_SpiWriteData(0x05);  // Gamma 4 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA2);  

	ILI9806E_SpiWriteData(0x0b);  // Gamma 8
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA3);  

	ILI9806E_SpiWriteData(0x10);  // Gamma 16
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA4);  

	ILI9806E_SpiWriteData(0x07);  // Gamma 24
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA5);  

	ILI9806E_SpiWriteData(0x1E);  // Gamma 52
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA6);  

	ILI9806E_SpiWriteData(0x0A);  // Gamma 80
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA7);  

	ILI9806E_SpiWriteData(0x09);  // Gamma 108
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA8);  

	ILI9806E_SpiWriteData(0x02);  // Gamma 147
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA9);  

	ILI9806E_SpiWriteData(0x09);  // Gamma 175
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAA);  

	ILI9806E_SpiWriteData(0x02);  // Gamma 203
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAB);  

	ILI9806E_SpiWriteData(0x06); // Gamma 231
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAC);  

	ILI9806E_SpiWriteData(0x0D);  // Gamma 239
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAD);  

	ILI9806E_SpiWriteData(0x35);  // Gamma 247
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAE);  

	ILI9806E_SpiWriteData(0x31);  // Gamma 251
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAF);  

	ILI9806E_SpiWriteData(0x07);  // Gamma 255
	mdelay(10);
	///==============Nagitive
	ILI9806E_SpiWriteCmd(0xC0);  

	ILI9806E_SpiWriteData(0x00);  // Gamma 0 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC1);  

	ILI9806E_SpiWriteData(0x03);  // Gamma 4
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC2);  

	ILI9806E_SpiWriteData(0x0A); // Gamma 8
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC3);  

	ILI9806E_SpiWriteData(0x0F);  // Gamma 16
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC4);  

	ILI9806E_SpiWriteData(0x08);  // Gamma 24
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC5);  

	ILI9806E_SpiWriteData(0x18);  // Gamma 52
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC6);  

	ILI9806E_SpiWriteData(0x0A);  // Gamma 80
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC7);  

	ILI9806E_SpiWriteData(0x09);  // Gamma 108
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC8);  

	ILI9806E_SpiWriteData(0x05);  // Gamma 147
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC9);  

	ILI9806E_SpiWriteData(0x09);  // Gamma 175
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCA);  

	ILI9806E_SpiWriteData(0x06);  // Gamma 203
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCB);  

	ILI9806E_SpiWriteData(0x06);  // Gamma 231
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCC);  

	ILI9806E_SpiWriteData(0x0C);  // Gamma 239
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCD);  

	ILI9806E_SpiWriteData(0x28);  // Gamma 247
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCE);  

	ILI9806E_SpiWriteData(0x24);  // Gamma 251
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCF);  

	ILI9806E_SpiWriteData(0x07);  // Gamma 255
	mdelay(10);



	//+++++++++++++++++++++++++++++++++++++++++++++++++++//

	//****************************************************************************//
	//****************************** Page 6 Command ******************************//
	//****************************************************************************//
	ILI9806E_SpiWriteCmd(0xFF);  

	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98);  
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x06);	// Change to Page 6
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x00);  

	ILI9806E_SpiWriteData(0x3C);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x01);  

	ILI9806E_SpiWriteData(0x06);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x02);  

	ILI9806E_SpiWriteData(0x00);	
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x03);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x04);  

	ILI9806E_SpiWriteData(0x0D);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x05);  

	ILI9806E_SpiWriteData(0x0D);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x06);  

	ILI9806E_SpiWriteData(0x80);	
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x07);  

	ILI9806E_SpiWriteData(0x04);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x08);  

	ILI9806E_SpiWriteData(0x03);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x09);  

	ILI9806E_SpiWriteData(0x00);	
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0A);  

	ILI9806E_SpiWriteData(0x00);	
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0B);  

	ILI9806E_SpiWriteData(0x00);	
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0C);  

	ILI9806E_SpiWriteData(0x0D);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0D);  

	ILI9806E_SpiWriteData(0x0D);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0E);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0F);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x10);  

	ILI9806E_SpiWriteData(0x50);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x11);  

	ILI9806E_SpiWriteData(0xD0);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x12);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x13);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x14);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x15);  

	ILI9806E_SpiWriteData(0xC0);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x16);  

	ILI9806E_SpiWriteData(0x08);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x17);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x18);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x19);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1A);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1B);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1C);  

	ILI9806E_SpiWriteData(0x48);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1D);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x20);  

	ILI9806E_SpiWriteData(0x10);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x21);  

	ILI9806E_SpiWriteData(0x23);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x22);  

	ILI9806E_SpiWriteData(0x45);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x23);  

	ILI9806E_SpiWriteData(0x67);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x24);  

	ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x25);  

	ILI9806E_SpiWriteData(0x23);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x26);  

	ILI9806E_SpiWriteData(0x45);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x27);  

	ILI9806E_SpiWriteData(0x67);
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x30);  

	ILI9806E_SpiWriteData(0x13);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x31);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x32);  

	ILI9806E_SpiWriteData(0xDD);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x33);  

	ILI9806E_SpiWriteData(0xCC);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x34);  

	ILI9806E_SpiWriteData(0xBB);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x35);  

	ILI9806E_SpiWriteData(0xAA);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x36);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x37);  

	ILI9806E_SpiWriteData(0x26);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x38);  

	ILI9806E_SpiWriteData(0x72);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x39);  

	ILI9806E_SpiWriteData(0xFF);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3A);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3B);  

	ILI9806E_SpiWriteData(0xEE);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3C);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3D);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3E);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3F);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x40);  

	ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x58);  

	ILI9806E_SpiWriteData(0xA7);
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x52);  

	ILI9806E_SpiWriteData(0x10);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x53);  

	ILI9806E_SpiWriteData(0x10);
	mdelay(10);



	//****************************************************************************//
	//****************************** Page 7 Command ******************************//
	//****************************************************************************//
	ILI9806E_SpiWriteCmd(0xFF);  

	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98);  
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x07);	// Change to Page 7
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x06);  

	ILI9806E_SpiWriteData(0x13);  
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x17);  

	ILI9806E_SpiWriteData(0x22); 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x02);  

	ILI9806E_SpiWriteData(0x77);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xE1);  

	ILI9806E_SpiWriteData(0x79);
	mdelay(10);


	//****************************************************************************//

	ILI9806E_SpiWriteCmd(0xFF);  

	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98);  
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x00);	 // Change to Page 0

	ILI9806E_SpiWriteCmd(0x3A);  

	ILI9806E_SpiWriteData(0x55);
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x35);  

	ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x11);  

				// Sleep-Out
	mdelay(120);
	ILI9806E_SpiWriteCmd(0x29); // Display On 
			  
	mdelay(10);
#elif defined(CONFIG_ZYT_ILI9806E_7672_XXX_SD)
	ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 1
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x01); 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x08); ILI9806E_SpiWriteData(0x10);                 // output SDA
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x21); ILI9806E_SpiWriteData(0x01);                 // DE = 1 Active
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x30); ILI9806E_SpiWriteData(0x02);                 // 480 X 800
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x31); ILI9806E_SpiWriteData(0x02);                 // Column Inversion
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x60); ILI9806E_SpiWriteData(0x07);                 // SDTI
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x61); ILI9806E_SpiWriteData(0x06);                // CRTI
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x62); ILI9806E_SpiWriteData(0x06);                 // EQTI
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x63); ILI9806E_SpiWriteData(0x04);                // PCTI
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x40); ILI9806E_SpiWriteData(0x15);                // BT  +2.5/-2.5 pump for DDVDH-L
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x41); ILI9806E_SpiWriteData(0x22);                 // DVDDH DVDDL clamp  
	mdelay(10); 
	ILI9806E_SpiWriteCmd(0x42); ILI9806E_SpiWriteData(0x00);                 // VGH/VGL 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x43); ILI9806E_SpiWriteData(0x8B);                 // VGH/VGL 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x44); ILI9806E_SpiWriteData(0x8C);                 // VGH/VGL 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x45); ILI9806E_SpiWriteData(0x1B);                 // VGL_REG  -10V 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x50); ILI9806E_SpiWriteData(0x78);                 // VGMP
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x51); ILI9806E_SpiWriteData(0x78);                 // VGMN
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x52); ILI9806E_SpiWriteData(0x00);                   //Flicker
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x53); ILI9806E_SpiWriteData(0x6F);                 //Flicker4F
	mdelay(10);

	//++++++++++++++++++ Gamma Setting ++++++++++++++++++//

	ILI9806E_SpiWriteCmd(0xA0); ILI9806E_SpiWriteData(0x00);  // Gamma 0 /255
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA1); ILI9806E_SpiWriteData(0x14);  // Gamma 4 /251
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA2); ILI9806E_SpiWriteData(0x1A);  // Gamma 8 /247
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA3); ILI9806E_SpiWriteData(0x06);  // Gamma 16	/239
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA4); ILI9806E_SpiWriteData(0x02);  // Gamma 24 /231
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA5); ILI9806E_SpiWriteData(0x04);  // Gamma 52 / 203
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA6); ILI9806E_SpiWriteData(0x1C);  // Gamma 80 / 175
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA7); ILI9806E_SpiWriteData(0x00);  // Gamma 108 /147
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA8); ILI9806E_SpiWriteData(0x19);  // Gamma 147 /108
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xA9); ILI9806E_SpiWriteData(0x0C);  // Gamma 175 / 80
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAA); ILI9806E_SpiWriteData(0x0E);  // Gamma 203 / 52
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAB); ILI9806E_SpiWriteData(0x04);  // Gamma 231 / 24
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAC); ILI9806E_SpiWriteData(0x07);  // Gamma 239 / 16
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAD); ILI9806E_SpiWriteData(0x1A);  // Gamma 247 / 8
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAE); ILI9806E_SpiWriteData(0x02);  // Gamma 251 / 4
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xAF); ILI9806E_SpiWriteData(0x00);  // Gamma 255 / 0
	mdelay(10);
	//==============Nagitive
	ILI9806E_SpiWriteCmd(0xC0); ILI9806E_SpiWriteData(0x00);  // Gamma 0 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC1); ILI9806E_SpiWriteData(0x01);  // Gamma 4
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC2); ILI9806E_SpiWriteData(0x0A);  // Gamma 8
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC3); ILI9806E_SpiWriteData(0x10);  // Gamma 16
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC4); ILI9806E_SpiWriteData(0x07);  // Gamma 24
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC5); ILI9806E_SpiWriteData(0x10);  // Gamma 52
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC6); ILI9806E_SpiWriteData(0x01);  // Gamma 80
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC7); ILI9806E_SpiWriteData(0x0F);  // Gamma 108
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC8); ILI9806E_SpiWriteData(0x06);  // Gamma 147
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xC9); ILI9806E_SpiWriteData(0x0A);  // Gamma 175
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCA); ILI9806E_SpiWriteData(0x12);  // Gamma 203
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCB); ILI9806E_SpiWriteData(0x0B);  // Gamma 231
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCC); ILI9806E_SpiWriteData(0x14);  // Gamma 239
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCD); ILI9806E_SpiWriteData(0x17);  // Gamma 247
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCE); ILI9806E_SpiWriteData(0x16);  // Gamma 251
	mdelay(10);
	ILI9806E_SpiWriteCmd(0xCF); ILI9806E_SpiWriteData(0x00);  // Gamma 255
	mdelay(10);

	ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 7
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x07); 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x18); ILI9806E_SpiWriteData(0x1D);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x17); ILI9806E_SpiWriteData(0x12);  // VGL_REG ON
	mdelay(10);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++//

	//****************************************************************************//
	//****************************** Page 6 Command ******************************//
	//****************************************************************************//

	ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 6
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x06); 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x00); ILI9806E_SpiWriteData(0x20);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x01); ILI9806E_SpiWriteData(0x05);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x02); ILI9806E_SpiWriteData(0x00);    
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x03); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x04); ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x05); ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x06); ILI9806E_SpiWriteData(0x88);    
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x07); ILI9806E_SpiWriteData(0x04);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x08); ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x09); ILI9806E_SpiWriteData(0x90);    
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0A); ILI9806E_SpiWriteData(0x04);    
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0B); ILI9806E_SpiWriteData(0x01);    
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0C); ILI9806E_SpiWriteData(0x01);				 //01
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0D); ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0E); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0F); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x10); ILI9806E_SpiWriteData(0x55);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x11); ILI9806E_SpiWriteData(0x50);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x12); ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x13); ILI9806E_SpiWriteData(0x0C);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x14); ILI9806E_SpiWriteData(0x0D);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x15); ILI9806E_SpiWriteData(0x43);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x16); ILI9806E_SpiWriteData(0x0B);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x17); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x18); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x19); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1A); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1B); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1C); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x1D); ILI9806E_SpiWriteData(0x00);
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x20); ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x21); ILI9806E_SpiWriteData(0x23);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x22); ILI9806E_SpiWriteData(0x45);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x23); ILI9806E_SpiWriteData(0x67);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x24); ILI9806E_SpiWriteData(0x01);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x25); ILI9806E_SpiWriteData(0x23);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x26); ILI9806E_SpiWriteData(0x45);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x27); ILI9806E_SpiWriteData(0x67);
	mdelay(10);

	ILI9806E_SpiWriteCmd(0x30); ILI9806E_SpiWriteData(0x02);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x31); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x32); ILI9806E_SpiWriteData(0x11);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x33); ILI9806E_SpiWriteData(0xAA);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x34); ILI9806E_SpiWriteData(0xBB);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x35); ILI9806E_SpiWriteData(0x66);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x36); ILI9806E_SpiWriteData(0x00);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x37); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x38); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x39); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3A); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3B); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3C); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3D); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3E); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x3F); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x40); ILI9806E_SpiWriteData(0x22);
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x53); ILI9806E_SpiWriteData(0x1A);  //VGLO refer VGL_REG
	mdelay(10);

	//****************************************************************************//
	//****************************** Page 0 Command ******************************//
	//****************************************************************************//

	mdelay(10);
	ILI9806E_SpiWriteCmd(0xFF);     // Change to Page 0
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x00); 
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x0C); ILI9806E_SpiWriteData(0x50);			 //01
	mdelay(10);
	ILI9806E_SpiWriteCmd(0x11); ILI9806E_SpiWriteData(0x00);      // Sleep-Out
	mdelay(120);
	ILI9806E_SpiWriteCmd(0x29); ILI9806E_SpiWriteData(0x00);      // Display On
	mdelay(120);
	ILI9806E_SpiWriteCmd(0x36); 
	ILI9806E_SpiWriteData(0x80); 

	ILI9806E_SpiWriteCmd(0x3A); 
	ILI9806E_SpiWriteData(0x55);   
	             
	ILI9806E_SpiWriteCmd(0x11);  
	mdelay(120);
	ILI9806E_SpiWriteCmd(0x29);
#elif defined(CONFIG_ZYT_ILI9806E_7675_ANGU)
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x01);     

	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x10);                

	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x01);                 

	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x02); 
	               
	ILI9806E_SpiWriteCmd(0x31);   // 480x800
	ILI9806E_SpiWriteData(0x02);  

	ILI9806E_SpiWriteCmd(0x60);
	ILI9806E_SpiWriteData(0x07); 
	                
	ILI9806E_SpiWriteCmd(0x61);
	ILI9806E_SpiWriteData(0x06); 
	              
	ILI9806E_SpiWriteCmd(0x62);
	ILI9806E_SpiWriteData(0x06);   
	             
	ILI9806E_SpiWriteCmd(0x63);
	ILI9806E_SpiWriteData(0x04);              

	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x14);  
	           
	ILI9806E_SpiWriteCmd(0x41);   // DVDDH DVDDL clamp
	ILI9806E_SpiWriteData(0x44);  
	            
	ILI9806E_SpiWriteCmd(0x42);
	ILI9806E_SpiWriteData(0x01);     
	            
	ILI9806E_SpiWriteCmd(0x43);   // VGH/VGL
	ILI9806E_SpiWriteData(0x89);  
	               
	ILI9806E_SpiWriteCmd(0x44);   // VGH/VGL
	ILI9806E_SpiWriteData(0x89); 
	 
	ILI9806E_SpiWriteCmd(0x45);   // VGL_REG 
	ILI9806E_SpiWriteData(0x1B);    

	ILI9806E_SpiWriteCmd(0x46);   // DDVDH/L_PK1 2 
	ILI9806E_SpiWriteData(0x44); 

	ILI9806E_SpiWriteCmd(0x47);   // VCL VGHL_PK3 4  
	ILI9806E_SpiWriteData(0x44);         

	ILI9806E_SpiWriteCmd(0x50);   // VGMP
	ILI9806E_SpiWriteData(0x85); 
	               
	ILI9806E_SpiWriteCmd(0x51);   // VGMN
	ILI9806E_SpiWriteData(0x85);  
	             
	ILI9806E_SpiWriteCmd(0x52);   //Flicker
	ILI9806E_SpiWriteData(0x00);  
	                 
	ILI9806E_SpiWriteCmd(0x53);   //Flicker
	ILI9806E_SpiWriteData(0x64);           
	//****************************************************************
	// Gamma Setting 
	ILI9806E_SpiWriteCmd(0xA0);
	ILI9806E_SpiWriteData(0x00);  // Gamma 0 

	ILI9806E_SpiWriteCmd(0xA1);
	ILI9806E_SpiWriteData(0x00);  // Gamma 4 

	ILI9806E_SpiWriteCmd(0xA2);
	ILI9806E_SpiWriteData(0x03);  // Gamma 8

	ILI9806E_SpiWriteCmd(0xA3);
	ILI9806E_SpiWriteData(0x0E);  // Gamma 16

	ILI9806E_SpiWriteCmd(0xA4);
	ILI9806E_SpiWriteData(0x08);  // Gamma 24

	ILI9806E_SpiWriteCmd(0xA5);
	ILI9806E_SpiWriteData(0x1F);  // Gamma 52

	ILI9806E_SpiWriteCmd(0xA6);
	ILI9806E_SpiWriteData(0x0F);  // Gamma 80

	ILI9806E_SpiWriteCmd(0xA7);
	ILI9806E_SpiWriteData(0x0B);  // Gamma 108

	ILI9806E_SpiWriteCmd(0xA8);
	ILI9806E_SpiWriteData(0x03);  // Gamma 147

	ILI9806E_SpiWriteCmd(0xA9);
	ILI9806E_SpiWriteData(0x06);  // Gamma 175

	ILI9806E_SpiWriteCmd(0xAA);
	ILI9806E_SpiWriteData(0x05);  // Gamma 203

	ILI9806E_SpiWriteCmd(0xAB);
	ILI9806E_SpiWriteData(0x02);  // Gamma 231

	ILI9806E_SpiWriteCmd(0xAC);
	ILI9806E_SpiWriteData(0x0E);  // Gamma 239

	ILI9806E_SpiWriteCmd(0xAD);
	ILI9806E_SpiWriteData(0x25);  // Gamma 247

	ILI9806E_SpiWriteCmd(0xAE);
	ILI9806E_SpiWriteData(0x1D);  // Gamma 251

	ILI9806E_SpiWriteCmd(0xAF);
	ILI9806E_SpiWriteData(0x00);  // Gamma 255

	//****************************************************************
	//Nagitive

	ILI9806E_SpiWriteCmd(0xC0);
	ILI9806E_SpiWriteData(0x00);  // Gamma 0 

	ILI9806E_SpiWriteCmd(0xC1);
	ILI9806E_SpiWriteData(0x04);  // Gamma 4

	ILI9806E_SpiWriteCmd(0xC2);
	ILI9806E_SpiWriteData(0x0F);  // Gamma 8

	ILI9806E_SpiWriteCmd(0xC3);
	ILI9806E_SpiWriteData(0x10);  // Gamma 16

	ILI9806E_SpiWriteCmd(0xC4);
	ILI9806E_SpiWriteData(0x0B);  // Gamma 24

	ILI9806E_SpiWriteCmd(0xC5);
	ILI9806E_SpiWriteData(0x1E); // Gamma 52

	ILI9806E_SpiWriteCmd(0xC6);
	ILI9806E_SpiWriteData(0x09);  // Gamma 80

	ILI9806E_SpiWriteCmd(0xC7);
	ILI9806E_SpiWriteData(0x0A);  // Gamma 108

	ILI9806E_SpiWriteCmd(0xC8);
	ILI9806E_SpiWriteData(0x00);  // Gamma 147

	ILI9806E_SpiWriteCmd(0xC9);
	ILI9806E_SpiWriteData(0x0A);  // Gamma 175

	ILI9806E_SpiWriteCmd(0xCA);
	ILI9806E_SpiWriteData(0x01);  // Gamma 203

	ILI9806E_SpiWriteCmd(0xCB);
	ILI9806E_SpiWriteData(0x06);  // Gamma 231

	ILI9806E_SpiWriteCmd(0xCC);
	ILI9806E_SpiWriteData(0x09);  // Gamma 239

	ILI9806E_SpiWriteCmd(0xCD);
	ILI9806E_SpiWriteData(0x2A);  // Gamma 247

	ILI9806E_SpiWriteCmd(0xCE);
	ILI9806E_SpiWriteData(0x28);  // Gamma 251

	ILI9806E_SpiWriteCmd(0xCF);
	ILI9806E_SpiWriteData(0x00);  // Gamma 255

	//****************************************************************
	//Page 6 Command 
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x06);     // Change to Page 6

	ILI9806E_SpiWriteCmd(0x00);
	ILI9806E_SpiWriteData(0xA0);

	ILI9806E_SpiWriteCmd(0x01);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x00);    

	ILI9806E_SpiWriteCmd(0x03);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x04);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x05);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x88);    

	ILI9806E_SpiWriteCmd(0x07);
	ILI9806E_SpiWriteData(0x04);

	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x09);
	ILI9806E_SpiWriteData(0x90);    

	ILI9806E_SpiWriteCmd(0x0A);
	ILI9806E_SpiWriteData(0x04);    

	ILI9806E_SpiWriteCmd(0x0B);
	ILI9806E_SpiWriteData(0x01);    

	ILI9806E_SpiWriteCmd(0x0C);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x0D);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x0E);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0F);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x10);
	ILI9806E_SpiWriteData(0x55);

	ILI9806E_SpiWriteCmd(0x11);
	ILI9806E_SpiWriteData(0x50);

	ILI9806E_SpiWriteCmd(0x12);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x13);
	ILI9806E_SpiWriteData(0x85);

	ILI9806E_SpiWriteCmd(0x14);
	ILI9806E_SpiWriteData(0x85);

	ILI9806E_SpiWriteCmd(0x15);
	ILI9806E_SpiWriteData(0xC0);

	ILI9806E_SpiWriteCmd(0x16);
	ILI9806E_SpiWriteData(0x0B);

	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x19);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1A);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1B);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1C);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1D);
	ILI9806E_SpiWriteData(0x00);


	ILI9806E_SpiWriteCmd(0x20);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x23);

	ILI9806E_SpiWriteCmd(0x22);
	ILI9806E_SpiWriteData(0x45);

	ILI9806E_SpiWriteCmd(0x23);
	ILI9806E_SpiWriteData(0x67);

	ILI9806E_SpiWriteCmd(0x24);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x25);
	ILI9806E_SpiWriteData(0x23);

	ILI9806E_SpiWriteCmd(0x26);
	ILI9806E_SpiWriteData(0x45);

	ILI9806E_SpiWriteCmd(0x27);
	ILI9806E_SpiWriteData(0x67);


	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x31);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x32);
	ILI9806E_SpiWriteData(0x11);

	ILI9806E_SpiWriteCmd(0x33);
	ILI9806E_SpiWriteData(0xAA);

	ILI9806E_SpiWriteCmd(0x34);
	ILI9806E_SpiWriteData(0xBB);

	ILI9806E_SpiWriteCmd(0x35);
	ILI9806E_SpiWriteData(0x66);

	ILI9806E_SpiWriteCmd(0x36);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x37);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x38);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x39);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x22);  //22

	ILI9806E_SpiWriteCmd(0x3B);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3C);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3D);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3E);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3F);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x53);
	ILI9806E_SpiWriteData(0x1A);  //VGLO refer VGL_REG

	//****************************************************************
	ILI9806E_SpiWriteCmd(0xFF);        // Change to Page 7 CMD for user command 
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x07); 

	ILI9806E_SpiWriteCmd(0x18);        //  
	ILI9806E_SpiWriteData(0x1D);
	 
	ILI9806E_SpiWriteCmd(0x17);        //  
	ILI9806E_SpiWriteData(0x12);        //Default 
	  
	ILI9806E_SpiWriteCmd(0x02);        //  
	ILI9806E_SpiWriteData(0x77);        // 

	ILI9806E_SpiWriteCmd(0xE1);        //  
	ILI9806E_SpiWriteData(0x79);

	ILI9806E_SpiWriteCmd(0x06);        //  
	ILI9806E_SpiWriteData(0x13);
	//****************************************************************

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x00);   

	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x55);

	ILI9806E_SpiWriteCmd(0x11);        // Sleep-Out
	ILI9806E_SpiWriteData(0x00);        
	mdelay(120); 

	ILI9806E_SpiWriteCmd(0x29);        // Display On
	ILI9806E_SpiWriteData(0x00);   
	mdelay(10); 
#elif defined(CONFIG_ZYT_ILI9806E_FS006_CFZZ)
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x10);

	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x31);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x15);

	ILI9806E_SpiWriteCmd(0x41);
	ILI9806E_SpiWriteData(0x33);

	ILI9806E_SpiWriteCmd(0x42);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x43);
	ILI9806E_SpiWriteData(0x09);

	ILI9806E_SpiWriteCmd(0x44);
	ILI9806E_SpiWriteData(0x09);

	ILI9806E_SpiWriteCmd(0x50);
	ILI9806E_SpiWriteData(0x80);

	ILI9806E_SpiWriteCmd(0x51);
	ILI9806E_SpiWriteData(0x80);

	ILI9806E_SpiWriteCmd(0x52);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x53);
	ILI9806E_SpiWriteData(0x53);

	ILI9806E_SpiWriteCmd(0x57);
	ILI9806E_SpiWriteData(0x50);

	ILI9806E_SpiWriteCmd(0x60);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0x61);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x62);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0x63);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xA0);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xA1);
	ILI9806E_SpiWriteData(0x2A);

	ILI9806E_SpiWriteCmd(0xA2);
	ILI9806E_SpiWriteData(0x2B);

	ILI9806E_SpiWriteCmd(0xA3);
	ILI9806E_SpiWriteData(0x0A);

	ILI9806E_SpiWriteCmd(0xA4);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0xA5);
	ILI9806E_SpiWriteData(0x0D);

	ILI9806E_SpiWriteCmd(0xA6);
	ILI9806E_SpiWriteData(0x08);

	ILI9806E_SpiWriteCmd(0xA7);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0xA8);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0xA9);
	ILI9806E_SpiWriteData(0x0B);

	ILI9806E_SpiWriteCmd(0xAA);
	ILI9806E_SpiWriteData(0x0A);

	ILI9806E_SpiWriteCmd(0xAB);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0xAC);
	ILI9806E_SpiWriteData(0x14);

	ILI9806E_SpiWriteCmd(0xAD);
	ILI9806E_SpiWriteData(0x2C);

	ILI9806E_SpiWriteCmd(0xAE);
	ILI9806E_SpiWriteData(0x2D);

	ILI9806E_SpiWriteCmd(0xAF);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xC0);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xC1);
	ILI9806E_SpiWriteData(0x29);

	ILI9806E_SpiWriteCmd(0xC2);
	ILI9806E_SpiWriteData(0x2B);

	ILI9806E_SpiWriteCmd(0xC3);
	ILI9806E_SpiWriteData(0x08);

	ILI9806E_SpiWriteCmd(0xC4);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xC5);
	ILI9806E_SpiWriteData(0x0B);

	ILI9806E_SpiWriteCmd(0xC6);
	ILI9806E_SpiWriteData(0x08);

	ILI9806E_SpiWriteCmd(0xC7);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0xC8);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0xC9);
	ILI9806E_SpiWriteData(0x08);

	ILI9806E_SpiWriteCmd(0xCA);
	ILI9806E_SpiWriteData(0x0A);

	ILI9806E_SpiWriteCmd(0xCB);
	ILI9806E_SpiWriteData(0x03);

	ILI9806E_SpiWriteCmd(0xCC);
	ILI9806E_SpiWriteData(0x06);

	ILI9806E_SpiWriteCmd(0xCD);
	ILI9806E_SpiWriteData(0x29);

	ILI9806E_SpiWriteCmd(0xCE);
	ILI9806E_SpiWriteData(0x21);

	ILI9806E_SpiWriteCmd(0xCF);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x06);

	ILI9806E_SpiWriteCmd(0x00);
	ILI9806E_SpiWriteData(0x21);

	ILI9806E_SpiWriteCmd(0x01);
	ILI9806E_SpiWriteData(0x06);

	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x03);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x04);
	ILI9806E_SpiWriteData(0x16);

	ILI9806E_SpiWriteCmd(0x05);
	ILI9806E_SpiWriteData(0x16);

	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x98);

	ILI9806E_SpiWriteCmd(0x07);
	ILI9806E_SpiWriteData(0x02);

	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x05);

	ILI9806E_SpiWriteCmd(0x09);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0A);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0B);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0C);
	ILI9806E_SpiWriteData(0x15);

	ILI9806E_SpiWriteCmd(0x0D);
	ILI9806E_SpiWriteData(0x15);

	ILI9806E_SpiWriteCmd(0x0E);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x0F);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x10);
	ILI9806E_SpiWriteData(0x77);

	ILI9806E_SpiWriteCmd(0x11);
	ILI9806E_SpiWriteData(0xF0);

	ILI9806E_SpiWriteCmd(0x12);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x13);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x14);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x15);
	ILI9806E_SpiWriteData(0xC0);

	ILI9806E_SpiWriteCmd(0x16);
	ILI9806E_SpiWriteData(0x08);

	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x19);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1A);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1B);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1C);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x1D);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x20);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x23);

	ILI9806E_SpiWriteCmd(0x22);
	ILI9806E_SpiWriteData(0x45);

	ILI9806E_SpiWriteCmd(0x23);
	ILI9806E_SpiWriteData(0x67);

	ILI9806E_SpiWriteCmd(0x24);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x25);
	ILI9806E_SpiWriteData(0x23);

	ILI9806E_SpiWriteCmd(0x26);
	ILI9806E_SpiWriteData(0x45);

	ILI9806E_SpiWriteCmd(0x27);
	ILI9806E_SpiWriteData(0x67);

	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x01);

	ILI9806E_SpiWriteCmd(0x31);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x32);
	ILI9806E_SpiWriteData(0x11);

	ILI9806E_SpiWriteCmd(0x33);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x34);
	ILI9806E_SpiWriteData(0x86);

	ILI9806E_SpiWriteCmd(0x35);
	ILI9806E_SpiWriteData(0x68);

	ILI9806E_SpiWriteCmd(0x36);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x37);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x38);
	ILI9806E_SpiWriteData(0xDA);

	ILI9806E_SpiWriteCmd(0x39);
	ILI9806E_SpiWriteData(0xCB);

	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0xBC);

	ILI9806E_SpiWriteCmd(0x3B);
	ILI9806E_SpiWriteData(0xAD);

	ILI9806E_SpiWriteCmd(0x3C);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3D);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3E);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x3F);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x52);
	ILI9806E_SpiWriteData(0x10);

	ILI9806E_SpiWriteCmd(0x53);
	ILI9806E_SpiWriteData(0x10);

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x12);

	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x77);

	ILI9806E_SpiWriteCmd(0xE1);
	ILI9806E_SpiWriteData(0x79);

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x07);

	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x22);

	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x77);

	ILI9806E_SpiWriteCmd(0xE1);
	ILI9806E_SpiWriteData(0x79);

	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x55);

	ILI9806E_SpiWriteCmd(0x11);
	ILI9806E_SpiWriteData(0x00);
	mdelay(120);

	ILI9806E_SpiWriteCmd(0x29);
	ILI9806E_SpiWriteData(0x00);
#elif defined(CONFIG_ZYT_ILI9806E_FS003_LHTC_HFG)
	ILI9806E_SpiWriteCmd(0xFF); 
	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x01); 

	ILI9806E_SpiWriteCmd(0x08); // output SDA
	ILI9806E_SpiWriteData(0x10);
	ILI9806E_SpiWriteCmd(0x21); // DE = 1 Active
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x30); // 480 X 800
	ILI9806E_SpiWriteData(0x02);
	ILI9806E_SpiWriteCmd(0x31); // 2-Dot Inversion
	ILI9806E_SpiWriteData(0x02);
	ILI9806E_SpiWriteCmd(0x60); // SDTI
	ILI9806E_SpiWriteData(0x07);
	ILI9806E_SpiWriteCmd(0x61); // CRTI
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteCmd(0x62); // EQTI
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteCmd(0x63); // PCTI
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteCmd(0x40); // BT  +2.5/-2.5 pump for DDVD
	ILI9806E_SpiWriteData(0x14); 
	ILI9806E_SpiWriteCmd(0x41); // DVDDH DVDDL clamp
	ILI9806E_SpiWriteData(0x44); 
	ILI9806E_SpiWriteCmd(0x42); // VGH/VGL
	ILI9806E_SpiWriteData(0x01); 
	ILI9806E_SpiWriteCmd(0x43); // VGH/VGL
	ILI9806E_SpiWriteData(0x89); 
	ILI9806E_SpiWriteCmd(0x44); // VGH/VGL
	ILI9806E_SpiWriteData(0x89); 
	ILI9806E_SpiWriteCmd(0x45); // VGL_REG 
	ILI9806E_SpiWriteData(0x1B);
	ILI9806E_SpiWriteCmd(0x46); // DDVDL_PK1 2 
	ILI9806E_SpiWriteData(0x44);
	ILI9806E_SpiWriteCmd(0x47); // DDVDL_PK1 2
	ILI9806E_SpiWriteData(0x44);
	ILI9806E_SpiWriteCmd(0x50); // VGMP(+4.2)
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x51); // VGMN(-4.2)
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x52); //Flicker
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x53); //Flicker
	ILI9806E_SpiWriteData(0x64);
	
	//++++++++++ Gamma Setting ++++++++++++++++++// 
	ILI9806E_SpiWriteCmd(0xA0);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xA1);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xA2); 
	ILI9806E_SpiWriteData(0x03);  
	ILI9806E_SpiWriteCmd(0xA3);
	ILI9806E_SpiWriteData(0x0E);  
	ILI9806E_SpiWriteCmd(0xA4);
	ILI9806E_SpiWriteData(0x08);
	ILI9806E_SpiWriteCmd(0xA5);
	ILI9806E_SpiWriteData(0x1F);
	ILI9806E_SpiWriteCmd(0xA6); 
	ILI9806E_SpiWriteData(0x0F);
	ILI9806E_SpiWriteCmd(0xA7);
	ILI9806E_SpiWriteData(0x0B);
	ILI9806E_SpiWriteCmd(0xA8);
	ILI9806E_SpiWriteData(0x03);
	ILI9806E_SpiWriteCmd(0xA9);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteCmd(0xAA);
	ILI9806E_SpiWriteData(0x05);
	ILI9806E_SpiWriteCmd(0xAB);  
	ILI9806E_SpiWriteData(0x02); 
	ILI9806E_SpiWriteCmd(0xAC); 
	ILI9806E_SpiWriteData(0x0E); 
	ILI9806E_SpiWriteCmd(0xAD);  
	ILI9806E_SpiWriteData(0x25);
	ILI9806E_SpiWriteCmd(0xAE);
	ILI9806E_SpiWriteData(0x1D);
	ILI9806E_SpiWriteCmd(0xAF);
	ILI9806E_SpiWriteData(0x00);
	
	////==============Nagitive 
	ILI9806E_SpiWriteCmd(0xC0);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xC1);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteCmd(0xC2);
	ILI9806E_SpiWriteData(0x0F);
	ILI9806E_SpiWriteCmd(0xC3);
	ILI9806E_SpiWriteData(0x10);
	ILI9806E_SpiWriteCmd(0xC4);
	ILI9806E_SpiWriteData(0x0B);
	ILI9806E_SpiWriteCmd(0xC5);
	ILI9806E_SpiWriteData(0x1E);
	ILI9806E_SpiWriteCmd(0xC6);
	ILI9806E_SpiWriteData(0x09);
	ILI9806E_SpiWriteCmd(0xC7);
	ILI9806E_SpiWriteData(0x0A);
	ILI9806E_SpiWriteCmd(0xC8);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xC9);
	ILI9806E_SpiWriteData(0x0A);
	ILI9806E_SpiWriteCmd(0xCA);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0xCB);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteCmd(0xCC);
	ILI9806E_SpiWriteData(0x09);
	ILI9806E_SpiWriteCmd(0xCD);
	ILI9806E_SpiWriteData(0x2A);
	ILI9806E_SpiWriteCmd(0xCE);
	ILI9806E_SpiWriteData(0x28);
	ILI9806E_SpiWriteCmd(0xCF);
	ILI9806E_SpiWriteData(0x00);
	
	// Change to Page 6  
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteCmd(0x00);
	ILI9806E_SpiWriteData(0xA0);
	ILI9806E_SpiWriteCmd(0x01);
	ILI9806E_SpiWriteData(0x05);
	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x03);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x04);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x05);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x88);
	ILI9806E_SpiWriteCmd(0x07);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x09);
	ILI9806E_SpiWriteData(0x90);
	ILI9806E_SpiWriteCmd(0x0A); 
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteCmd(0x0B);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0C);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0D);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0E);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x0F);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x10);
	ILI9806E_SpiWriteData(0x55);
	ILI9806E_SpiWriteCmd(0x11);
	ILI9806E_SpiWriteData(0x50);
	ILI9806E_SpiWriteCmd(0x12);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x13);
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x14);
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x15);
	ILI9806E_SpiWriteData(0xC0);
	ILI9806E_SpiWriteCmd(0x16);
	ILI9806E_SpiWriteData(0x0B);
	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x19);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1A);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1B);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1C);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1D);
	ILI9806E_SpiWriteData(0x00);
	
	ILI9806E_SpiWriteCmd(0x20);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x23);
	ILI9806E_SpiWriteCmd(0x22);
	ILI9806E_SpiWriteData(0x45);
	ILI9806E_SpiWriteCmd(0x23);
	ILI9806E_SpiWriteData(0x67);
	ILI9806E_SpiWriteCmd(0x24);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x25);
	ILI9806E_SpiWriteData(0x23);
	ILI9806E_SpiWriteCmd(0x26);
	ILI9806E_SpiWriteData(0x45);
	ILI9806E_SpiWriteCmd(0x27);
	ILI9806E_SpiWriteData(0x67);
	
	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x02);
	ILI9806E_SpiWriteCmd(0x31);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x32);
	ILI9806E_SpiWriteData(0x11);
	ILI9806E_SpiWriteCmd(0x33);
	ILI9806E_SpiWriteData(0xAA);
	ILI9806E_SpiWriteCmd(0x34);
	ILI9806E_SpiWriteData(0xBB);
	ILI9806E_SpiWriteCmd(0x35);
	ILI9806E_SpiWriteData(0x66);
	ILI9806E_SpiWriteCmd(0x36);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x37);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x38);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x39);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3B);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3C);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3D);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3E);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3F);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x53);
	ILI9806E_SpiWriteData(0x1A);
	
	 // Change to Page 7
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x07);
	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x1D);
	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x12);
	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x77);
	ILI9806E_SpiWriteCmd(0xE1);
	ILI9806E_SpiWriteData(0x79);
	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x13);
	
	//==== Page 0 Command ======================//
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x55);
	
	ILI9806E_SpiWriteCmd(0x11);
	mdelay(120);
	
	ILI9806E_SpiWriteCmd(0x29);
	mdelay(10);
#else
	// Change to Page 1
	ILI9806E_SpiWriteCmd(0xFF); 
	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x01); 

	ILI9806E_SpiWriteCmd(0x08); // output SDA
	ILI9806E_SpiWriteData(0x10);
	ILI9806E_SpiWriteCmd(0x21); // DE = 1 Active
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x30); // 480 X 800
	ILI9806E_SpiWriteData(0x02);
	ILI9806E_SpiWriteCmd(0x31); // 2-Dot Inversion
	ILI9806E_SpiWriteData(0x02);
	ILI9806E_SpiWriteCmd(0x60); // SDTI
	ILI9806E_SpiWriteData(0x07);
	ILI9806E_SpiWriteCmd(0x61); // CRTI
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteCmd(0x62); // EQTI
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteCmd(0x63); // PCTI
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteCmd(0x40); // BT  +2.5/-2.5 pump for DDVD
	ILI9806E_SpiWriteData(0x14); 
	ILI9806E_SpiWriteCmd(0x41); // DVDDH DVDDL clamp
	ILI9806E_SpiWriteData(0x44); 
	ILI9806E_SpiWriteCmd(0x42); // VGH/VGL
	ILI9806E_SpiWriteData(0x01); 
	ILI9806E_SpiWriteCmd(0x43); // VGH/VGL
	ILI9806E_SpiWriteData(0x89); 
	ILI9806E_SpiWriteCmd(0x44); // VGH/VGL
	ILI9806E_SpiWriteData(0x89); 
	ILI9806E_SpiWriteCmd(0x45); // VGL_REG 
	ILI9806E_SpiWriteData(0x1B);
	ILI9806E_SpiWriteCmd(0x46); // DDVDL_PK1 2 
	ILI9806E_SpiWriteData(0x44);
	ILI9806E_SpiWriteCmd(0x47); // DDVDL_PK1 2
	ILI9806E_SpiWriteData(0x44);
	ILI9806E_SpiWriteCmd(0x50); // VGMP(+4.2)
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x51); // VGMN(-4.2)
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x52); //Flicker
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x53); //Flicker
	ILI9806E_SpiWriteData(0x64);
	
	//++++++++++ Gamma Setting ++++++++++++++++++// 
	ILI9806E_SpiWriteCmd(0xA0);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xA1);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xA2); 
	ILI9806E_SpiWriteData(0x03);  
	ILI9806E_SpiWriteCmd(0xA3);
	ILI9806E_SpiWriteData(0x0E);  
	ILI9806E_SpiWriteCmd(0xA4);
	ILI9806E_SpiWriteData(0x08);
	ILI9806E_SpiWriteCmd(0xA5);
	ILI9806E_SpiWriteData(0x1F);
	ILI9806E_SpiWriteCmd(0xA6); 
	ILI9806E_SpiWriteData(0x0F);
	ILI9806E_SpiWriteCmd(0xA7);
	ILI9806E_SpiWriteData(0x0B);
	ILI9806E_SpiWriteCmd(0xA8);
	ILI9806E_SpiWriteData(0x03);
	ILI9806E_SpiWriteCmd(0xA9);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteCmd(0xAA);
	ILI9806E_SpiWriteData(0x05);
	ILI9806E_SpiWriteCmd(0xAB);  
	ILI9806E_SpiWriteData(0x02); 
	ILI9806E_SpiWriteCmd(0xAC); 
	ILI9806E_SpiWriteData(0x0E); 
	ILI9806E_SpiWriteCmd(0xAD);  
	ILI9806E_SpiWriteData(0x25);
	ILI9806E_SpiWriteCmd(0xAE);
	ILI9806E_SpiWriteData(0x1D);
	ILI9806E_SpiWriteCmd(0xAF);
	ILI9806E_SpiWriteData(0x00);
	
	////==============Nagitive 
	ILI9806E_SpiWriteCmd(0xC0);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xC1);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteCmd(0xC2);
	ILI9806E_SpiWriteData(0x0F);
	ILI9806E_SpiWriteCmd(0xC3);
	ILI9806E_SpiWriteData(0x10);
	ILI9806E_SpiWriteCmd(0xC4);
	ILI9806E_SpiWriteData(0x0B);
	ILI9806E_SpiWriteCmd(0xC5);
	ILI9806E_SpiWriteData(0x1E);
	ILI9806E_SpiWriteCmd(0xC6);
	ILI9806E_SpiWriteData(0x09);
	ILI9806E_SpiWriteCmd(0xC7);
	ILI9806E_SpiWriteData(0x0A);
	ILI9806E_SpiWriteCmd(0xC8);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0xC9);
	ILI9806E_SpiWriteData(0x0A);
	ILI9806E_SpiWriteCmd(0xCA);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0xCB);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteCmd(0xCC);
	ILI9806E_SpiWriteData(0x09);
	ILI9806E_SpiWriteCmd(0xCD);
	ILI9806E_SpiWriteData(0x2A);
	ILI9806E_SpiWriteCmd(0xCE);
	ILI9806E_SpiWriteData(0x28);
	ILI9806E_SpiWriteCmd(0xCF);
	ILI9806E_SpiWriteData(0x00);
	
	// Change to Page 6  
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteCmd(0x00);
	ILI9806E_SpiWriteData(0xA0);
	ILI9806E_SpiWriteCmd(0x01);
	ILI9806E_SpiWriteData(0x05);
	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x03);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x04);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x05);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x88);
	ILI9806E_SpiWriteCmd(0x07);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteCmd(0x08);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x09);
	ILI9806E_SpiWriteData(0x90);
	ILI9806E_SpiWriteCmd(0x0A); 
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteCmd(0x0B);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0C);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0D);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x0E);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x0F);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x10);
	ILI9806E_SpiWriteData(0x55);
	ILI9806E_SpiWriteCmd(0x11);
	ILI9806E_SpiWriteData(0x50);
	ILI9806E_SpiWriteCmd(0x12);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x13);
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x14);
	ILI9806E_SpiWriteData(0x85);
	ILI9806E_SpiWriteCmd(0x15);
	ILI9806E_SpiWriteData(0xC0);
	ILI9806E_SpiWriteCmd(0x16);
	ILI9806E_SpiWriteData(0x0B);
	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x19);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1A);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1B);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1C);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x1D);
	ILI9806E_SpiWriteData(0x00);
	
	ILI9806E_SpiWriteCmd(0x20);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x21);
	ILI9806E_SpiWriteData(0x23);
	ILI9806E_SpiWriteCmd(0x22);
	ILI9806E_SpiWriteData(0x45);
	ILI9806E_SpiWriteCmd(0x23);
	ILI9806E_SpiWriteData(0x67);
	ILI9806E_SpiWriteCmd(0x24);
	ILI9806E_SpiWriteData(0x01);
	ILI9806E_SpiWriteCmd(0x25);
	ILI9806E_SpiWriteData(0x23);
	ILI9806E_SpiWriteCmd(0x26);
	ILI9806E_SpiWriteData(0x45);
	ILI9806E_SpiWriteCmd(0x27);
	ILI9806E_SpiWriteData(0x67);
	
	ILI9806E_SpiWriteCmd(0x30);
	ILI9806E_SpiWriteData(0x02);
	ILI9806E_SpiWriteCmd(0x31);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x32);
	ILI9806E_SpiWriteData(0x11);
	ILI9806E_SpiWriteCmd(0x33);
	ILI9806E_SpiWriteData(0xAA);
	ILI9806E_SpiWriteCmd(0x34);
	ILI9806E_SpiWriteData(0xBB);
	ILI9806E_SpiWriteCmd(0x35);
	ILI9806E_SpiWriteData(0x66);
	ILI9806E_SpiWriteCmd(0x36);
	ILI9806E_SpiWriteData(0x00);
	ILI9806E_SpiWriteCmd(0x37);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x38);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x39);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3B);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3C);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3D);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3E);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x3F);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x40);
	ILI9806E_SpiWriteData(0x22);
	ILI9806E_SpiWriteCmd(0x53);
	ILI9806E_SpiWriteData(0x1A);
	
	 // Change to Page 7
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x07);
	ILI9806E_SpiWriteCmd(0x18);
	ILI9806E_SpiWriteData(0x1D);
	ILI9806E_SpiWriteCmd(0x17);
	ILI9806E_SpiWriteData(0x12);
	ILI9806E_SpiWriteCmd(0x02);
	ILI9806E_SpiWriteData(0x77);
	ILI9806E_SpiWriteCmd(0xE1);
	ILI9806E_SpiWriteData(0x79);
	ILI9806E_SpiWriteCmd(0x06);
	ILI9806E_SpiWriteData(0x13);
	
	//==== Page 0 Command ======================//
	ILI9806E_SpiWriteCmd(0xFF);
	ILI9806E_SpiWriteData(0xFF);
	ILI9806E_SpiWriteData(0x98);
	ILI9806E_SpiWriteData(0x06);
	ILI9806E_SpiWriteData(0x04);
	ILI9806E_SpiWriteData(0x00);

	ILI9806E_SpiWriteCmd(0x3A);
	ILI9806E_SpiWriteData(0x55);
#ifdef CONFIG_MACH_SP7715GA_5701
	ILI9806E_SpiWriteCmd(0x36);
	ILI9806E_SpiWriteData(0x03);
#endif
	ILI9806E_SpiWriteCmd(0x11);
	mdelay(120);
	
	ILI9806E_SpiWriteCmd(0x29);
	mdelay(10);
#endif
}

static int32_t ili9806e_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	
	if(is_sleep==1){
		//Sleep In
		ILI9806E_SpiWriteCmd(0x28);
		mdelay(120); 
		ILI9806E_SpiWriteCmd(0x10);
		mdelay(10); 
	}else{
		//Sleep Out
		ILI9806E_SpiWriteCmd(0x11);
		mdelay(120); 
		ILI9806E_SpiWriteCmd(0x29);
		mdelay(10); 
	}

	return 0;
}




static int32_t ili9806e_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	uint32_t *test_data[4] = {0};
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;
#if 0

	printk("zxdbg add -ili9806e_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

	ILI9806E_SpiWriteCmd(0x2A00); 
	ILI9806E_SpiWriteData((left>>8));// set left address
	ILI9806E_SpiWriteData((left&0xff));
	ILI9806E_SpiWriteData((right>>8));// set right address
	ILI9806E_SpiWriteData((right&0xff));

	ILI9806E_SpiWriteCmd(0x2B00); 
	ILI9806E_SpiWriteData((top>>8));// set left address
	ILI9806E_SpiWriteData((top&0xff));
	ILI9806E_SpiWriteData((bottom>>8));// set bottom address
	ILI9806E_SpiWriteData((bottom&0xff));

//	ILI9806E_SpiWriteCmd(0x2C00);

	ILI9806E_SpiWriteCmd(0x2A00); 
	spi_read(test_data);
	spi_read(test_data+1);
	ILI9806E_SpiWriteCmd(0x2B00); 
	spi_read(test_data+2);
	spi_read(test_data+3);
#endif
//	printk("zxdbg add -ili9806e_read read: %x, %x, %x, %x\n",test_data[0], test_data[1], test_data[2], test_data[3]);

	return 0;
}
static int32_t ili9806e_invalidate(struct panel_spec *self)
{
//	printk("ili9806e_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t ili9806e_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
//	printk("ili9806e_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t ili9806e_read_id(struct panel_spec *self)
{
	int32_t id  = 0;
	int32_t  id1 = 0; 
	int32_t  id2 = 0; 
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;

	printk("sprdfb ili9806e_read_id \n");

#ifdef CONFIG_MACH_SP7715GA_5701
	return 0x9806;
#else
	ILI9806E_SpiWriteCmd(0xFF); 
	ILI9806E_SpiWriteData(0xFF);  
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06);  
	ILI9806E_SpiWriteData(0x04);  
	ILI9806E_SpiWriteData(0x01); 

	ILI9806E_SpiWriteCmd(0xFE); // SET SPI READ INDEX
	ILI9806E_SpiWriteData(0x81); // GETHXID
	ILI9806E_SpiWriteCmd(0x00); // SET SPI READ INDEX
	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
	spi_read(8,&id1);  //0x98
	#else
	spi_read(&id1);  //0x98
	#endif
	printk(" ili9806e_read_id kernel id1 = 0x%x\n",id1);
	ILI9806E_SpiWriteCmd(0xFE); // SET SPI READ INDEX
	ILI9806E_SpiWriteData(0x81); // GETHXID
	ILI9806E_SpiWriteCmd(0x01); // GETHXID
	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
	spi_read(8,&id2);  //0x06
	#else
	spi_read(&id2);  //0x06
	#endif
	printk(" ili9806e_read_id kernel id2 = 0x%x\n",id2);
	ILI9806E_SpiWriteCmd(0xFE);
	ILI9806E_SpiWriteData(0x00);
	id = ((id1&0x00ff)<<8)|(id2&0x00ff);
	printk(" ili9806e_read_id kernel id = 0x%x\n",id);
//#ifndef CONFIG_SPX15
	return id;
/*#else
	return 0x9806;
#endif*/
#endif
}
#ifdef CONFIG_FB_ESD_SUPPORT
static uint32_t ili9806e_esd_check(struct panel_spec *self)
{
	printk("ili9806e_esd_check****************\n");
	uint32_t    r0a =0;
    //static  uint32_t esd_fail = 0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;
	ILI9806E_SpiWriteCmd(0xFF); 
	ILI9806E_SpiWriteData(0xFF); 
	ILI9806E_SpiWriteData(0x98); 
	ILI9806E_SpiWriteData(0x06); 
	ILI9806E_SpiWriteData(0x04); 
	ILI9806E_SpiWriteData(0x00); 
	 
//	ILI9806E_SpiWriteCmd(0xFE); 
//	ILI9806E_SpiWriteData(0x81);
//	mdelay(2000);

	ILI9806E_SpiWriteCmd(0x0A);
	spi_read(8,&r0a);//9c
//	printk(" \n----w--ili9806e_read_r0a = 0x%x\n",r0a);


    if (r0a!= 0x9c)
    {
//		printk("result=============0\n");
		//if (esd_fail == 1)
		return 0;
	}
	else
	{
//		printk("result===================1\n");
		return 1;
	}
	//esd_fail = 1;
return 1;
}
#endif
static struct panel_operations lcd_ili9806e_rgb_spi_operations = {
	.panel_init = ili9806e_init,
	.panel_set_window = ili9806e_set_window,
	.panel_invalidate_rect= ili9806e_invalidate_rect,
	.panel_invalidate = ili9806e_invalidate,
	.panel_enter_sleep = ili9806e_enter_sleep,
	.panel_readid          = ili9806e_read_id,
#ifdef CONFIG_FB_ESD_SUPPORT
	.panel_esd_check = ili9806e_esd_check,
#endif
};

static struct timing_rgb lcd_ili9806e_rgb_timing = {
#if (CONFIG_ZYT_PRJ == 7675) && (CONFIG_ZYT_CUST == 8)
	.hfp = 20,//30->20
	.hbp = 20,//30->20
	.hsync = 10, //8,
	.vfp = 12, // 8, 
	.vbp = 12, // 8
	.vsync = 4, // 6,4,
#elif defined(CONFIG_ZYT_ILI9806E_7675_CFZZ)
     .hfp = 20,//30->20
	.hbp = 20,//30->20
	.hsync = 10, //8,
	.vfp = 12, // 8, 
	.vbp = 12, // 8
	.vsync = 4, // 6,4,
#elif defined(CONFIG_ZYT_ILI9806E_FS011_CFZZ)
	.hfp = 30,//10,  /* unit: pixel */
	.hbp = 30,//6,
	.hsync = 10, //8,
	.vfp = 20, // 3, /*unit: line*/
	.vbp =20, // 3,
	.vsync = 4, // 4,
#elif defined(CONFIG_ZYT_ILI9806E_7675_QY)
	.hfp = 260,  /* unit: pixel */
	.hbp = 60,
	.hsync = 10,
	
	.vfp = 20, /*unit: line*/
	.vbp = 18,
	.vsync = 2,
#else
#if defined(CONFIG_ZYT_ILI9806E_7271_CFZZ)
	.hfp = 18,//  /* unit: pixel */ 50
	.hbp = 34,// 50
	.hsync = 2, //8,  10
#else
	.hfp = 50,//10,  /* unit: pixel */
	.hbp = 50,//6,
	.hsync = 10, //8,
#endif
	.vfp = 30, // 3, /*unit: line*/
	.vbp = 30, // 3,
	.vsync = 10, // 4,
#endif
};

static struct spi_info lcd_ili9806e_rgb_spi_info = {
	.ops = NULL,
};

static struct info_rgb lcd_ili9806e_rgb_info = {
	.cmd_bus_mode  = SPRDFB_RGB_BUS_TYPE_SPI,
	.video_bus_width = 16, /*18,16*/
	.h_sync_pol = SPRDFB_POLARITY_NEG,
	.v_sync_pol = SPRDFB_POLARITY_NEG,
	.de_pol = SPRDFB_POLARITY_POS,
	.timing = &lcd_ili9806e_rgb_timing,
	.bus_info = {
		.spi = &lcd_ili9806e_rgb_spi_info,
	}
};

struct panel_spec lcd_panel_ili9806e_rgb_spi_spec = {
	.width = 480,
	.height = 800,
	.reset_timing = {20, 10, 10},
#ifdef CONFIG_MACH_SP7715EA_7671
	.fps = 80,
#else
	.fps = 61,
#endif
	.type = LCD_MODE_RGB,
	.direction = LCD_DIRECT_NORMAL,
	.is_clean_lcd = true,
	.info = {
		.rgb = &lcd_ili9806e_rgb_info
	},
	.ops = &lcd_ili9806e_rgb_spi_operations,
};

struct panel_cfg lcd_ili9806e_rgb_spi = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x9806,
	.lcd_name = "lcd_ili9806e_rgb_spi",
	.panel = &lcd_panel_ili9806e_rgb_spi_spec,
};
static int __init lcd_ili9806e_rgb_spi_init(void)
{
	return sprdfb_panel_register(&lcd_ili9806e_rgb_spi);
}

subsys_initcall(lcd_ili9806e_rgb_spi_init);
