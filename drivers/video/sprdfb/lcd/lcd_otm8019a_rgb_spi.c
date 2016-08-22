/* drivers/video/sc8810/lcd_OTM8019A_rgb_spi.c
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

#undef  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif


#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
#define OTM8019A_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd(16,(cmd & 0xFFFF));\
}

#define  OTM8019A_SpiWriteData(data)\
{ \
	spi_send_data(16,(data & 0xFFFF));\
}
#else
#define OTM8019A_SpiWriteCmd(cmd) \ 
{ \
	spi_send_cmd((cmd & 0xFFFF));\
}

#define  OTM8019A_SpiWriteData(data)\
{ \
	spi_send_data((data & 0xFFFF));\
}
#endif



static int32_t OTM8019A_init(struct panel_spec *self)
{	
	uint32_t data = 0;
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read; 
//	spi_reset_t spi_reset = self->info.rgb->bus_info.spi->ops->spi_reset; 
	LCD_PRINT("uboot OTM8019A_init\n");

	OTM8019A_SpiWriteCmd(0xFF00); OTM8019A_SpiWriteData(0x80);
	OTM8019A_SpiWriteCmd(0xFF01); OTM8019A_SpiWriteData(0x19);
	OTM8019A_SpiWriteCmd(0xFF02); OTM8019A_SpiWriteData(0x01);
	                                                          
	OTM8019A_SpiWriteCmd(0xFF80); OTM8019A_SpiWriteData(0x80);
	OTM8019A_SpiWriteCmd(0xFF81); OTM8019A_SpiWriteData(0x19);
	                                                          
	OTM8019A_SpiWriteCmd(0xC48A); OTM8019A_SpiWriteData(0x40);
	                                                          
	OTM8019A_SpiWriteCmd(0xB3A6); OTM8019A_SpiWriteData(0x20);
	OTM8019A_SpiWriteCmd(0xB3A7); OTM8019A_SpiWriteData(0x01);
	                                                          
	OTM8019A_SpiWriteCmd(0xC090); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xC091); OTM8019A_SpiWriteData(0x15);
	OTM8019A_SpiWriteCmd(0xC092); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xC093); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xC094); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xC095); OTM8019A_SpiWriteData(0x03);

	OTM8019A_SpiWriteCmd(0xC0B4); OTM8019A_SpiWriteData(0x70);
	OTM8019A_SpiWriteCmd(0xC0B5); OTM8019A_SpiWriteData(0x48);

	OTM8019A_SpiWriteCmd(0xC181); OTM8019A_SpiWriteData(0x33);
	                                                          
	OTM8019A_SpiWriteCmd(0xC481); OTM8019A_SpiWriteData(0x81);
	                                                          
	OTM8019A_SpiWriteCmd(0xC487); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xC489); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xC582); OTM8019A_SpiWriteData(0xB0);
	                                                          
	OTM8019A_SpiWriteCmd(0xC590); OTM8019A_SpiWriteData(0x4E);
	OTM8019A_SpiWriteCmd(0xC591); OTM8019A_SpiWriteData(0x79);
	OTM8019A_SpiWriteCmd(0xC592); OTM8019A_SpiWriteData(0x06);
	OTM8019A_SpiWriteCmd(0xC593); OTM8019A_SpiWriteData(0x91);
	OTM8019A_SpiWriteCmd(0xC594); OTM8019A_SpiWriteData(0x33);
	OTM8019A_SpiWriteCmd(0xC595); OTM8019A_SpiWriteData(0x34);
	OTM8019A_SpiWriteCmd(0xC596); OTM8019A_SpiWriteData(0x23);
	                                                          
	OTM8019A_SpiWriteCmd(0xC5B1); OTM8019A_SpiWriteData(0xA8);
	                                                          
	OTM8019A_SpiWriteCmd(0xD800); OTM8019A_SpiWriteData(0x68);
	OTM8019A_SpiWriteCmd(0xD801); OTM8019A_SpiWriteData(0x68);

	OTM8019A_SpiWriteCmd(0xD900); OTM8019A_SpiWriteData(0x67);//67

	OTM8019A_SpiWriteCmd(0xCE80); OTM8019A_SpiWriteData(0x86);
	OTM8019A_SpiWriteCmd(0xCE81); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCE82); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCE83); OTM8019A_SpiWriteData(0x85);
	OTM8019A_SpiWriteCmd(0xCE84); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCE85); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCE86); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCE87); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCE88); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCE89); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCE8A); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCE8B); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCEA0); OTM8019A_SpiWriteData(0x18);
	OTM8019A_SpiWriteCmd(0xCEA1); OTM8019A_SpiWriteData(0x05);
	OTM8019A_SpiWriteCmd(0xCEA2); OTM8019A_SpiWriteData(0x83);
	OTM8019A_SpiWriteCmd(0xCEA3); OTM8019A_SpiWriteData(0x39);
	OTM8019A_SpiWriteCmd(0xCEA4); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEA5); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEA6); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEA7); OTM8019A_SpiWriteData(0x18);
	OTM8019A_SpiWriteCmd(0xCEA8); OTM8019A_SpiWriteData(0x04);
	OTM8019A_SpiWriteCmd(0xCEA9); OTM8019A_SpiWriteData(0x83);
	OTM8019A_SpiWriteCmd(0xCEAA); OTM8019A_SpiWriteData(0x3A);
	OTM8019A_SpiWriteCmd(0xCEAB); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEAC); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEAD); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCEB0); OTM8019A_SpiWriteData(0x18);
	OTM8019A_SpiWriteCmd(0xCEB1); OTM8019A_SpiWriteData(0x03);
	OTM8019A_SpiWriteCmd(0xCEB2); OTM8019A_SpiWriteData(0x83);
	OTM8019A_SpiWriteCmd(0xCEB3); OTM8019A_SpiWriteData(0x3B);
	OTM8019A_SpiWriteCmd(0xCEB4); OTM8019A_SpiWriteData(0x86);
	OTM8019A_SpiWriteCmd(0xCEB5); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEB6); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEB7); OTM8019A_SpiWriteData(0x18);
	OTM8019A_SpiWriteCmd(0xCEB8); OTM8019A_SpiWriteData(0x02);
	OTM8019A_SpiWriteCmd(0xCEB9); OTM8019A_SpiWriteData(0x83);
	OTM8019A_SpiWriteCmd(0xCEBA); OTM8019A_SpiWriteData(0x3C);
	OTM8019A_SpiWriteCmd(0xCEBB); OTM8019A_SpiWriteData(0x88);
	OTM8019A_SpiWriteCmd(0xCEBC); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCEBD); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCFC0); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCFC1); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCFC2); OTM8019A_SpiWriteData(0x20);
	OTM8019A_SpiWriteCmd(0xCFC3); OTM8019A_SpiWriteData(0x20);
	OTM8019A_SpiWriteCmd(0xCFC4); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCFC5); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCFC6); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCFC7); OTM8019A_SpiWriteData(0x02);
	OTM8019A_SpiWriteCmd(0xCFC8); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCFC9); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCFD0); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCBC0); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBC1); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBC2); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBC3); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBC4); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBC5); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBC6); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBC7); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBC8); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBC9); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBCA); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBCB); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBCC); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBCD); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBCE); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCBD0); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCBD5); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBD6); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBD7); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBD8); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBD9); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBDA); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBDB); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBDC); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBDD); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCBDE); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCBE0); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBE1); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBE2); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBE3); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBE4); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCBE5); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCC80); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC81); OTM8019A_SpiWriteData(0x26);
	OTM8019A_SpiWriteCmd(0xCC82); OTM8019A_SpiWriteData(0x09);
	OTM8019A_SpiWriteCmd(0xCC83); OTM8019A_SpiWriteData(0x0B);
	OTM8019A_SpiWriteCmd(0xCC84); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCC85); OTM8019A_SpiWriteData(0x25);
	OTM8019A_SpiWriteCmd(0xCC86); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC87); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC88); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC89); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCC90); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC91); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC92); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC93); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC94); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC95); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCC9A); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC9B); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC9C); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC9D); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCC9E); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCCA0); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCA1); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCA2); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCA3); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCA4); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCA5); OTM8019A_SpiWriteData(0x25);
	OTM8019A_SpiWriteCmd(0xCCA6); OTM8019A_SpiWriteData(0x02);
	OTM8019A_SpiWriteCmd(0xCCA7); OTM8019A_SpiWriteData(0x0C);
	OTM8019A_SpiWriteCmd(0xCCA8); OTM8019A_SpiWriteData(0x0A);
	OTM8019A_SpiWriteCmd(0xCCA9); OTM8019A_SpiWriteData(0x26);
	OTM8019A_SpiWriteCmd(0xCCAA); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCCB0); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCB1); OTM8019A_SpiWriteData(0x25);
	OTM8019A_SpiWriteCmd(0xCCB2); OTM8019A_SpiWriteData(0x0C);
	OTM8019A_SpiWriteCmd(0xCCB3); OTM8019A_SpiWriteData(0x0A);
	OTM8019A_SpiWriteCmd(0xCCB4); OTM8019A_SpiWriteData(0x02);
	OTM8019A_SpiWriteCmd(0xCCB5); OTM8019A_SpiWriteData(0x26);
	OTM8019A_SpiWriteCmd(0xCCB6); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCB7); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCB8); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCB9); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCCC0); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCC1); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCC2); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCC3); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCC4); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCC5); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCCCA); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCCB); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCCC); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCCD); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCCE); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xCCD0); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCD1); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCD2); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCD3); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCD4); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xCCD5); OTM8019A_SpiWriteData(0x26);
	OTM8019A_SpiWriteCmd(0xCCD6); OTM8019A_SpiWriteData(0x01);
	OTM8019A_SpiWriteCmd(0xCCD7); OTM8019A_SpiWriteData(0x09);
	OTM8019A_SpiWriteCmd(0xCCD8); OTM8019A_SpiWriteData(0x0B);
	OTM8019A_SpiWriteCmd(0xCCD9); OTM8019A_SpiWriteData(0x25);
	OTM8019A_SpiWriteCmd(0xCCDA); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xE100); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xE101); OTM8019A_SpiWriteData(0x04);
	OTM8019A_SpiWriteCmd(0xE102); OTM8019A_SpiWriteData(0x08);
	OTM8019A_SpiWriteCmd(0xE103); OTM8019A_SpiWriteData(0x10);
	OTM8019A_SpiWriteCmd(0xE104); OTM8019A_SpiWriteData(0x24);
	OTM8019A_SpiWriteCmd(0xE105); OTM8019A_SpiWriteData(0x40);
	OTM8019A_SpiWriteCmd(0xE106); OTM8019A_SpiWriteData(0x55);
	OTM8019A_SpiWriteCmd(0xE107); OTM8019A_SpiWriteData(0x94);
	OTM8019A_SpiWriteCmd(0xE108); OTM8019A_SpiWriteData(0x85);
	OTM8019A_SpiWriteCmd(0xE109); OTM8019A_SpiWriteData(0x9B);
	OTM8019A_SpiWriteCmd(0xE10A); OTM8019A_SpiWriteData(0x6C);
	OTM8019A_SpiWriteCmd(0xE10B); OTM8019A_SpiWriteData(0x5C);
	OTM8019A_SpiWriteCmd(0xE10C); OTM8019A_SpiWriteData(0x76);
	OTM8019A_SpiWriteCmd(0xE10D); OTM8019A_SpiWriteData(0x62);
	OTM8019A_SpiWriteCmd(0xE10E); OTM8019A_SpiWriteData(0x68);
	OTM8019A_SpiWriteCmd(0xE10F); OTM8019A_SpiWriteData(0x61);
	OTM8019A_SpiWriteCmd(0xE110); OTM8019A_SpiWriteData(0x5B);
	OTM8019A_SpiWriteCmd(0xE111); OTM8019A_SpiWriteData(0x4D);
	OTM8019A_SpiWriteCmd(0xE112); OTM8019A_SpiWriteData(0x47);
	OTM8019A_SpiWriteCmd(0xE113); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xE200); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xE201); OTM8019A_SpiWriteData(0x04);
	OTM8019A_SpiWriteCmd(0xE202); OTM8019A_SpiWriteData(0x08);
	OTM8019A_SpiWriteCmd(0xE203); OTM8019A_SpiWriteData(0x10);
	OTM8019A_SpiWriteCmd(0xE204); OTM8019A_SpiWriteData(0x24);
	OTM8019A_SpiWriteCmd(0xE205); OTM8019A_SpiWriteData(0x40);
	OTM8019A_SpiWriteCmd(0xE206); OTM8019A_SpiWriteData(0x55);
	OTM8019A_SpiWriteCmd(0xE207); OTM8019A_SpiWriteData(0x93);
	OTM8019A_SpiWriteCmd(0xE208); OTM8019A_SpiWriteData(0x85);
	OTM8019A_SpiWriteCmd(0xE209); OTM8019A_SpiWriteData(0x9B);
	OTM8019A_SpiWriteCmd(0xE20A); OTM8019A_SpiWriteData(0x6C);
	OTM8019A_SpiWriteCmd(0xE20B); OTM8019A_SpiWriteData(0x5C);
	OTM8019A_SpiWriteCmd(0xE20C); OTM8019A_SpiWriteData(0x75);
	OTM8019A_SpiWriteCmd(0xE20D); OTM8019A_SpiWriteData(0x62);
	OTM8019A_SpiWriteCmd(0xE20E); OTM8019A_SpiWriteData(0x68);
	OTM8019A_SpiWriteCmd(0xE20F); OTM8019A_SpiWriteData(0x61);
	OTM8019A_SpiWriteCmd(0xE210); OTM8019A_SpiWriteData(0x5A);
	OTM8019A_SpiWriteCmd(0xE211); OTM8019A_SpiWriteData(0x4D);
	OTM8019A_SpiWriteCmd(0xE212); OTM8019A_SpiWriteData(0x46);
	OTM8019A_SpiWriteCmd(0xE213); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xC480); OTM8019A_SpiWriteData(0x30);
	                                                          
	OTM8019A_SpiWriteCmd(0xC098); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xC0A9); OTM8019A_SpiWriteData(0x0A);
	                                                          
	OTM8019A_SpiWriteCmd(0xC1B0); OTM8019A_SpiWriteData(0x20);
	OTM8019A_SpiWriteCmd(0xC1B1); OTM8019A_SpiWriteData(0x00);
	OTM8019A_SpiWriteCmd(0xC1B2); OTM8019A_SpiWriteData(0x00);
	                                                          
	OTM8019A_SpiWriteCmd(0xC0E1); OTM8019A_SpiWriteData(0x40);
	OTM8019A_SpiWriteCmd(0xC0E2); OTM8019A_SpiWriteData(0x30);
	                                                          
	OTM8019A_SpiWriteCmd(0xC180); OTM8019A_SpiWriteData(0x03);
	OTM8019A_SpiWriteCmd(0xC181); OTM8019A_SpiWriteData(0x33);
	                                                          
	OTM8019A_SpiWriteCmd(0xC1A0); OTM8019A_SpiWriteData(0xE8);
	                                                          
	OTM8019A_SpiWriteCmd(0xB690); OTM8019A_SpiWriteData(0xB4);
	mdelay(10);                                              
	                                                          
	OTM8019A_SpiWriteCmd(0xFB00); OTM8019A_SpiWriteData(0x01);
	                                                          
	OTM8019A_SpiWriteCmd(0xFF00); OTM8019A_SpiWriteData(0xFF);
	OTM8019A_SpiWriteCmd(0xFF01); OTM8019A_SpiWriteData(0xFF);
	OTM8019A_SpiWriteCmd(0xFF02); OTM8019A_SpiWriteData(0xFF);
	                                                          
	OTM8019A_SpiWriteCmd(0x3A00); OTM8019A_SpiWriteData(0x55);
	                                                          
	OTM8019A_SpiWriteCmd(0x1100);
	mdelay(120);                                             
	                                                          
	OTM8019A_SpiWriteCmd(0x2900);
	mdelay(50);
}

static int32_t OTM8019A_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	
	LCD_PRINT("uboot OTM8019A_enter_sleep: is_sleep = %d\n",is_sleep);
	
	if(is_sleep==1){
		//Sleep In
		OTM8019A_SpiWriteCmd(0x2800);
		mdelay(120); 
		OTM8019A_SpiWriteCmd(0x1000);
		mdelay(10); 
	}else{
		//Sleep Out
		OTM8019A_SpiWriteCmd(0x1100);
		mdelay(120); 
		OTM8019A_SpiWriteCmd(0x2900);
		mdelay(10); 
	}

	return 0;
}




static int32_t OTM8019A_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	uint32_t *test_data[4] = {0};
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd; 
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data; 
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;
#if 0

	LCD_PRINT("zxdbg add -OTM8019A_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

	OTM8019A_SpiWriteCmd(0x2A00); 
	OTM8019A_SpiWriteData((left>>8));// set left address
	OTM8019A_SpiWriteData((left&0xff));
	OTM8019A_SpiWriteData((right>>8));// set right address
	OTM8019A_SpiWriteData((right&0xff));

	OTM8019A_SpiWriteCmd(0x2B00); 
	OTM8019A_SpiWriteData((top>>8));// set left address
	OTM8019A_SpiWriteData((top&0xff));
	OTM8019A_SpiWriteData((bottom>>8));// set bottom address
	OTM8019A_SpiWriteData((bottom&0xff));

//	OTM8019A_SpiWriteCmd(0x2C00);

	OTM8019A_SpiWriteCmd(0x2A00); 
	spi_read(test_data);
	spi_read(test_data+1);
	OTM8019A_SpiWriteCmd(0x2B00); 
	spi_read(test_data+2);
	spi_read(test_data+3);
#endif
	LCD_PRINT("zxdbg add -OTM8019A_read read: %x, %x, %x, %x\n",test_data[0], test_data[1], test_data[2], test_data[3]);

	return 0;
}
static int32_t OTM8019A_invalidate(struct panel_spec *self)
{
	LCD_PRINT("OTM8019A_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t OTM8019A_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("OTM8019A_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t OTM8019A_read_id(struct panel_spec *self)
{
  int32_t ID1,ID2,ID3,ID4,ID5,ID6,ID;
	
	spi_send_cmd_t spi_send_cmd = self->info.rgb->bus_info.spi->ops->spi_send_cmd;
	spi_send_data_t spi_send_data = self->info.rgb->bus_info.spi->ops->spi_send_data;
	spi_read_t spi_read = self->info.rgb->bus_info.spi->ops->spi_read;

	LCD_PRINT("uboot OTM8019A_read_id\n");
	

              

      OTM8019A_SpiWriteCmd(0xa100); udelay(2000);//
      	#if defined(CONFIG_FB_SC7715_RGB_SPI_16BIT_AND_8BIT_ZYT_SUPPORT)
      spi_read(16,&ID1); //0X71
      spi_read(16,&ID2); //0X71
       spi_read(16,&ID3); //0X71
       spi_read(16,&ID4); //0X71
              spi_read(16,&ID5); //0X71
                spi_read(16,&ID6); //0X71
	#else
      spi_read(&ID1); //0X71
      spi_read(&ID2); //0X71
      spi_read(&ID3); //0X71
      spi_read(&ID4); //0X71
	#endif

 
     ID=((ID4 & 0xff) << 8) | ID5; //0x8019
     //      ID=((ID4 & 0xff) << 8) | ID5; //0x8009
	LCD_PRINT("<<<<<<<<<<<<<<<<<<<<<<<<<OTM8019A>>>>>>>>>>>>>>>>\n");
LCD_PRINT("OTM8019A_read_id ID==%x;  ID1=%x; ID2=%x; ID3=%x;    ID4====%x ,ID5=%x; ID6=%x; \n",ID,ID1,ID2,ID3,ID4,ID5,ID6);
///  ID1=1; ID2=1; ID3=8b;    ID4====80 ,ID5=9; ID6=ff; 

 	return ID;//0x8019

}

static struct panel_operations lcd_OTM8019A_rgb_spi_operations = {
	.panel_init = OTM8019A_init,
	.panel_set_window = OTM8019A_set_window,
	.panel_invalidate_rect= OTM8019A_invalidate_rect,
	.panel_invalidate = OTM8019A_invalidate,
	.panel_enter_sleep = OTM8019A_enter_sleep,
	.panel_readid          = OTM8019A_read_id
};

static struct timing_rgb lcd_OTM8019A_rgb_timing = {
	.hfp = 46,//10,  /* unit: pixel */
	.hbp = 44, //6,
	.hsync = 4, //8,
	.vfp = 15, // 3, /*unit: line*/
	.vbp = 16, // 3,
	.vsync = 1, // 4,
};

static struct spi_info lcd_OTM8019A_rgb_spi_info = {
	.ops = NULL,
};

static struct info_rgb lcd_OTM8019A_rgb_info = {
	.cmd_bus_mode  = SPRDFB_RGB_BUS_TYPE_SPI,
	.video_bus_width = 16, /*18,16*/
	.h_sync_pol = SPRDFB_POLARITY_NEG,
	.v_sync_pol = SPRDFB_POLARITY_NEG,
	.de_pol = SPRDFB_POLARITY_POS,
	.timing = &lcd_OTM8019A_rgb_timing,
	.bus_info = {
		.spi = &lcd_OTM8019A_rgb_spi_info,
	}
};

struct panel_spec lcd_panel_OTM8019A_rgb_spi_spec = {
	.width = 480,
	.height = 800,

	.fps = 61,

	.type = LCD_MODE_RGB,
	.direction = LCD_DIRECT_NORMAL,

	.info = {
		.rgb = &lcd_OTM8019A_rgb_info
	},
	.ops = &lcd_OTM8019A_rgb_spi_operations,
};
struct panel_cfg lcd_otm8019a_rgb_spi = {
	/* this panel can only be main lcd */
	.dev_id = SPRDFB_MAINLCD_ID,
	.lcd_id = 0x8019,
	.lcd_name = "lcd_otm8019_rgb_spi",
	.panel = &lcd_panel_OTM8019A_rgb_spi_spec,
};
static int __init lcd_otm8019a_rgb_spi_init(void)
{
	return sprdfb_panel_register(&lcd_otm8019a_rgb_spi);
}
subsys_initcall(lcd_otm8019a_rgb_spi_init);