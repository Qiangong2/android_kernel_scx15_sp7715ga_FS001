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


static int32_t hx8363_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	printk("hx8363_init\n");

	LCD_DelayMS(120);
	send_cmd(0xB9); //SETEXTC: enable extention command (B9h)
	send_data(0xFF);  
	send_data(0x83);  
	send_data(0x63); 

	send_cmd(0xB1);//SETPOWER: Set power (B1h)
	send_data(0x01);//
	send_data(0x00);//
	send_data(0x44);//
	send_data(0x07);//	 
	send_data(0x01);//	 
	send_data(0x11);//	
	send_data(0x11);//	
	send_data(0x3A);//	
	send_data(0x42);// 
	send_data(0x3F);//
	send_data(0x3F);//
	send_data(0x40);// 
	send_data(0x32);// 
	send_data(0x00);// 
	send_data(0x00);// 
	send_data(0x00);// 
	send_data(0x00);// 
	send_data(0x00);// 
	send_data(0x00);// 

	send_cmd(0xB4);//SETCYC: Set display waveform cycle (B4h)
	send_data(0x00);//NW_PE[2:0] NW[2:0]  1-dot inversion  
	send_data(0x18);//	
	send_data(0x9C);//
	send_data(0x08);//
	send_data(0x18);//
	send_data(0x04);// 
	send_data(0x72);// 

	send_cmd(0xB2);//SETDISP: Set display related register (B2h)
	send_data(0x08);//	
	send_data(0x00);// 

	send_cmd(0xBF);  // SET PTBA for VCOM=-2.5V 
	send_data(0x05);   
	send_data(0x60);   
	send_data(0x00);   
	send_data(0x10);		

	send_cmd(0xB6); //SETVCOM: Set VCOM voltage (B6h)
	send_data(0x2B);  //Optimal VCOM Tuning

	send_cmd(0xE0);  // SET Gamma
	//Positive of GAMMA
	send_data(0x0D);//1
	send_data(0x1E);//2
	send_data(0x25);//3
	send_data(0x16);//4
	send_data(0x15);//5
	send_data(0x38);//6    
	send_data(0x34);//7
	send_data(0x3C);//8    
	send_data(0x04);//9
	send_data(0x0B);//10
	send_data(0x12);//11
	send_data(0x17);//12
	send_data(0x18);//13
	send_data(0x16);//14
	send_data(0x17);//15
	send_data(0x16);//16
	send_data(0x19);//17

	//Negtive of GAMMA
	send_data(0x0D);//1
	send_data(0x1E);//2
	send_data(0x25);//3
	send_data(0x16);//4
	send_data(0x15);//5
	send_data(0x38);//6   
	send_data(0x34);//7
	send_data(0x3C);//8
	send_data(0x04);//9
	send_data(0x0B);//10
	send_data(0x12);//11
	send_data(0x17);//12
	send_data(0x18);//13
	send_data(0x16);//14
	send_data(0x17);//15
	send_data(0x16);//16
	send_data(0x19);//17  //18718523212

	send_cmd(0x3A);// 
	send_data(0x55); //  

	send_cmd(0x36);//
	send_data(0x00);//88//00

	send_cmd(0xCC);//SETPANEL (CCh)
	send_data(0x0b);//0E//0B

	send_cmd(0xC2); //SETDSPIF: Set display interface mode (C2h) 
	send_data(0x02); //0x00

	send_cmd(0xB7);// 
	send_data(0x00); // 

	send_data(0x01);
	send_data(0x10);	


	send_cmd(0xB0);// 
	send_data(0x01); // 
	send_data(0x06);

	send_cmd(0x44);// 
	send_data(0x10);
	send_data(0x20);

	send_cmd(0x35);// 
	send_data(0x00); //    

	send_cmd(0x2A);// 
	send_data(0x00); // 
	send_data(0x00);
	send_data(0x01);
	send_data(0xDF);

	send_cmd(0x2B);// 
	send_data(0x00);// 
	send_data(0x00);
	send_data(0x03);
	send_data(0x1F);

	send_cmd(0x11); //
	LCD_DelayMS(120);
	send_cmd(0x29);//

	send_cmd(0x2C);//

    return 0; 
}

static int32_t hx8363_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	if(is_sleep==1){
		//Sleep In
		send_cmd(0x28);
		LCD_DelayMS(120); 
		send_cmd(0x10);
		LCD_DelayMS(120); 
	}else{
		//Sleep Out
		send_cmd(0x11);
		LCD_DelayMS(120); 
		send_cmd(0x29);
		LCD_DelayMS(120); 
	}

	return 0;
}




static int32_t hx8363_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;
	
	pr_debug("hx8363_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

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
static int32_t hx8363_invalidate(struct panel_spec *self)
{
	pr_debug("hx8363_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t hx8363_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	pr_debug("hx8363_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t hx8363_read_id(struct panel_spec *self)
{
	int32_t id  = 0;
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	send_data_t send_data = self->info.mcu->ops->send_data;

	//Get ID

	
	return 0x18; // id;
}

static struct panel_operations lcd_hx8363_mcu_operations = {
	.panel_init = hx8363_init,
	.panel_set_window = hx8363_set_window,
	.panel_invalidate_rect= hx8363_invalidate_rect,
	.panel_invalidate = hx8363_invalidate,
	.panel_enter_sleep = hx8363_enter_sleep,
	.panel_readid          = hx8363_read_id
};

static struct timing_mcu lcd_hx8363_mcu_timing[] = {
[MCU_LCD_REGISTER_TIMING] = {                    // read/write register timing
		.rcss = 15,  // 15ns
		.rlpw = 60,
		.rhpw = 60,
		.wcss = 10,
		.wlpw = 35,
		.whpw = 35,
	},
[MCU_LCD_GRAM_TIMING] = {                    // read/write gram timing
		.rcss = 15,  // 15ns
		.rlpw = 60,
		.rhpw = 60,
		.wcss = 0,
		.wlpw = 16,
		.whpw = 16,
	},
};

static struct info_mcu lcd_hx8363_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing =lcd_hx8363_mcu_timing,
	.ops = NULL,
};

struct panel_spec lcd_hx8363_mcu_spec = {
	.width = 480,
#if defined(CONFIG_LCD_FWVGA)
	.height = 854,
#else
	.height = 800,
#endif
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {
		.mcu = &lcd_hx8363_mcu_info
	},
	.ops = &lcd_hx8363_mcu_operations,
};

struct panel_cfg lcd_hx8363_mcu = {
	/* this panel may on both CS0/1 */
	.dev_id = SPRDFB_UNDEFINELCD_ID,
	.lcd_id = 0x18,
	.lcd_name = "lcd_hx8363_mcu",
	.panel = &lcd_hx8363_mcu_spec,
};

static int __init lcd_hx8363_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_hx8363_mcu);
}

subsys_initcall(lcd_hx8363_mcu_init);
