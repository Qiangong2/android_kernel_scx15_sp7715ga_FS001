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


static int32_t nt35510_init(struct panel_spec *self)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("nt35510_init\n");

	LCD_DelayMS(200);
	//Enable Page1 
	send_cmd(0xF000); send_data(0x55); 
	send_cmd(0xF001); send_data(0xAA); 
	send_cmd(0xF002); send_data(0x52); 
	send_cmd(0xF003); send_data(0x08); 
	send_cmd(0xF004); send_data(0x01); 

	// AVDD: manual
	send_cmd(0xB600); send_data(0x36); 
	send_cmd(0xB601); send_data(0x34); 
	send_cmd(0xB602); send_data(0x34); 
	send_cmd(0xB000); send_data(0x09); 
	send_cmd(0xB001); send_data(0x09); 
	send_cmd(0xB002); send_data(0x09); 

	// AVEE: manual  ©\©\6V 
	                   
	send_cmd(0xB700); send_data(0x26); 
	send_cmd(0xB701); send_data(0x24); 
	send_cmd(0xB702); send_data(0x24); 
	send_cmd(0xB100); send_data(0x09); 
	send_cmd(0xB101); send_data(0x09); 
	send_cmd(0xB102); send_data(0x09); 

	//Power Control for VCL 
	send_cmd(0xB800); send_data(0x37); 
	send_cmd(0xB200); send_data(0x00); 
	                                 
	// VGH: Clamp Enable 
	send_cmd(0xB900); send_data(0x26); 
	send_cmd(0xB901); send_data(0x26); 
	send_cmd(0xB902); send_data(0x26); 
	send_cmd(0xB300); send_data(0x05); 
	send_cmd(0xB301); send_data(0x05); 
	send_cmd(0xB302); send_data(0x05); 
	           
	send_cmd(0xBF00); send_data(0x00); 

	// VGL(LVGL) 
	send_cmd(0xBA00); send_data(0x36); 
	send_cmd(0xBA01); send_data(0x36); 
	send_cmd(0xBA02); send_data(0x36); 

	// VGL_REG(VGLO) 
	send_cmd(0xB500); send_data(0x0B); 
	send_cmd(0xB501); send_data(0x0B); 
	send_cmd(0xB502); send_data(0x0B); 

	// VGMP/VGSP 
	send_cmd(0xBC00); send_data(0X00); 
	send_cmd(0xBC01); send_data(0xA3); 
	send_cmd(0xBC02); send_data(0X00); 

	//VGMN/VGSN 
	send_cmd(0xBD00); send_data(0x00); 
	send_cmd(0xBD01); send_data(0xA3); 
	send_cmd(0xBD02); send_data(0x00); 

	// VCOM= ©\©\0.1 
	      
	send_cmd(0xBE00); send_data(0x00); 
	send_cmd(0xBE01); send_data(0x50); 
	                  
	//R+ 
	send_cmd(0xD100); send_data(0x00); 
	send_cmd(0xD101); send_data(0x37); 
	send_cmd(0xD102); send_data(0x00); 
	send_cmd(0xD103); send_data(0x52); 
	send_cmd(0xD104); send_data(0x00); 
	send_cmd(0xD105); send_data(0x7B); 
	send_cmd(0xD106); send_data(0x00); 
	send_cmd(0xD107); send_data(0x99); 
	send_cmd(0xD108); send_data(0x00); 
	send_cmd(0xD109); send_data(0xB1); 
	send_cmd(0xD10A); send_data(0x00); 
	send_cmd(0xD10B); send_data(0xD2); 
	send_cmd(0xD10C); send_data(0x00); 
	send_cmd(0xD10D); send_data(0xF6); 
	send_cmd(0xD10E); send_data(0x01); 
	send_cmd(0xD10F); send_data(0x27); 
	send_cmd(0xD110); send_data(0x01); 
	send_cmd(0xD111); send_data(0x4E); 
	send_cmd(0xD112); send_data(0x01); 
	send_cmd(0xD113); send_data(0x8C); 
	send_cmd(0xD114); send_data(0x01); 
	send_cmd(0xD115); send_data(0xBE); 
	send_cmd(0xD116); send_data(0x02); 
	send_cmd(0xD117); send_data(0x0B); 
	send_cmd(0xD118); send_data(0x02); 
	send_cmd(0xD119); send_data(0x48); 
	send_cmd(0xD11A); send_data(0x02); 
	send_cmd(0xD11B); send_data(0x4A); 
	send_cmd(0xD11C); send_data(0x02); 
	send_cmd(0xD11D); send_data(0x7E); 
	send_cmd(0xD11E); send_data(0x02); 
	send_cmd(0xD11F); send_data(0xBC); 
	send_cmd(0xD120); send_data(0x02); 
	send_cmd(0xD121); send_data(0xE1); 
	send_cmd(0xD122); send_data(0x03); 
	send_cmd(0xD123); send_data(0x10); 
	send_cmd(0xD124); send_data(0x03); 
	send_cmd(0xD125); send_data(0x31); 
	send_cmd(0xD126); send_data(0x03); 
	send_cmd(0xD127); send_data(0x5A); 
	send_cmd(0xD128); send_data(0x03); 
	send_cmd(0xD129); send_data(0x73); 
	send_cmd(0xD12A); send_data(0x03); 
	send_cmd(0xD12B); send_data(0x94); 
	send_cmd(0xD12C); send_data(0x03); 
	send_cmd(0xD12D); send_data(0x9F); 
	send_cmd(0xD12E); send_data(0x03); 
	send_cmd(0xD12F); send_data(0xB3); 
	send_cmd(0xD130); send_data(0x03); 
	send_cmd(0xD131); send_data(0xB9); 
	send_cmd(0xD132); send_data(0x03); 
	send_cmd(0xD133); send_data(0xC1); 
	                  
	//G+                  
	send_cmd(0xD200); send_data(0x00); 
	send_cmd(0xD201); send_data(0x37); 
	send_cmd(0xD202); send_data(0x00); 
	send_cmd(0xD203); send_data(0x52); 
	send_cmd(0xD204); send_data(0x00); 
	send_cmd(0xD205); send_data(0x7B); 
	send_cmd(0xD206); send_data(0x00); 
	send_cmd(0xD207); send_data(0x99); 
	send_cmd(0xD208); send_data(0x00); 
	send_cmd(0xD209); send_data(0xB1); 
	send_cmd(0xD20A); send_data(0x00); 
	send_cmd(0xD20B); send_data(0xD2); 
	send_cmd(0xD20C); send_data(0x00); 
	send_cmd(0xD20D); send_data(0xF6); 
	send_cmd(0xD20E); send_data(0x01); 
	send_cmd(0xD20F); send_data(0x27); 
	send_cmd(0xD210); send_data(0x01); 
	send_cmd(0xD211); send_data(0x4E); 
	send_cmd(0xD212); send_data(0x01); 
	send_cmd(0xD213); send_data(0x8C); 
	send_cmd(0xD214); send_data(0x01); 
	send_cmd(0xD215); send_data(0xBE); 
	send_cmd(0xD216); send_data(0x02); 
	send_cmd(0xD217); send_data(0x0B); 
	send_cmd(0xD218); send_data(0x02); 
	send_cmd(0xD219); send_data(0x48); 
	send_cmd(0xD21A); send_data(0x02); 
	send_cmd(0xD21B); send_data(0x4A); 
	send_cmd(0xD21C); send_data(0x02); 
	send_cmd(0xD21D); send_data(0x7E); 
	send_cmd(0xD21E); send_data(0x02); 
	send_cmd(0xD21F); send_data(0xBC); 
	send_cmd(0xD220); send_data(0x02); 
	send_cmd(0xD221); send_data(0xE1); 
	send_cmd(0xD222); send_data(0x03); 
	send_cmd(0xD223); send_data(0x10); 
	send_cmd(0xD224); send_data(0x03); 
	send_cmd(0xD225); send_data(0x31); 
	send_cmd(0xD226); send_data(0x03); 
	send_cmd(0xD227); send_data(0x5A); 
	send_cmd(0xD228); send_data(0x03); 
	send_cmd(0xD229); send_data(0x73); 
	send_cmd(0xD22A); send_data(0x03); 
	send_cmd(0xD22B); send_data(0x94); 
	send_cmd(0xD22C); send_data(0x03); 
	send_cmd(0xD22D); send_data(0x9F); 
	send_cmd(0xD22E); send_data(0x03); 
	send_cmd(0xD22F); send_data(0xB3); 
	send_cmd(0xD230); send_data(0x03); 
	send_cmd(0xD231); send_data(0xB9); 
	send_cmd(0xD232); send_data(0x03); 
	send_cmd(0xD233); send_data(0xC1); 
	                  
	//B+                  
	send_cmd(0xD300); send_data(0x00); 
	send_cmd(0xD301); send_data(0x37); 
	send_cmd(0xD302); send_data(0x00); 
	send_cmd(0xD303); send_data(0x52); 
	send_cmd(0xD304); send_data(0x00); 
	send_cmd(0xD305); send_data(0x7B); 
	send_cmd(0xD306); send_data(0x00); 
	send_cmd(0xD307); send_data(0x99); 
	send_cmd(0xD308); send_data(0x00); 
	send_cmd(0xD309); send_data(0xB1); 
	send_cmd(0xD30A); send_data(0x00); 
	send_cmd(0xD30B); send_data(0xD2); 
	send_cmd(0xD30C); send_data(0x00); 
	send_cmd(0xD30D); send_data(0xF6); 
	send_cmd(0xD30E); send_data(0x01); 
	send_cmd(0xD30F); send_data(0x27); 
	send_cmd(0xD310); send_data(0x01); 
	send_cmd(0xD311); send_data(0x4E); 
	send_cmd(0xD312); send_data(0x01); 
	send_cmd(0xD313); send_data(0x8C); 
	send_cmd(0xD314); send_data(0x01); 
	send_cmd(0xD315); send_data(0xBE); 
	send_cmd(0xD316); send_data(0x02); 
	send_cmd(0xD317); send_data(0x0B); 
	send_cmd(0xD318); send_data(0x02); 
	send_cmd(0xD319); send_data(0x48); 
	send_cmd(0xD31A); send_data(0x02); 
	send_cmd(0xD31B); send_data(0x4A); 
	send_cmd(0xD31C); send_data(0x02); 
	send_cmd(0xD31D); send_data(0x7E); 
	send_cmd(0xD31E); send_data(0x02); 
	send_cmd(0xD31F); send_data(0xBC); 
	send_cmd(0xD320); send_data(0x02); 
	send_cmd(0xD321); send_data(0xE1); 
	send_cmd(0xD322); send_data(0x03); 
	send_cmd(0xD323); send_data(0x10); 
	send_cmd(0xD324); send_data(0x03); 
	send_cmd(0xD325); send_data(0x31); 
	send_cmd(0xD326); send_data(0x03); 
	send_cmd(0xD327); send_data(0x5A); 
	send_cmd(0xD328); send_data(0x03); 
	send_cmd(0xD329); send_data(0x73); 
	send_cmd(0xD32A); send_data(0x03); 
	send_cmd(0xD32B); send_data(0x94); 
	send_cmd(0xD32C); send_data(0x03); 
	send_cmd(0xD32D); send_data(0x9F); 
	send_cmd(0xD32E); send_data(0x03); 
	send_cmd(0xD32F); send_data(0xB3); 
	send_cmd(0xD330); send_data(0x03); 
	send_cmd(0xD331); send_data(0xB9); 
	send_cmd(0xD332); send_data(0x03); 
	send_cmd(0xD333); send_data(0xC1); 
	                  
	//R-                  
	send_cmd(0xD400); send_data(0x00); 
	send_cmd(0xD401); send_data(0x37); 
	send_cmd(0xD402); send_data(0x00); 
	send_cmd(0xD403); send_data(0x52); 
	send_cmd(0xD404); send_data(0x00); 
	send_cmd(0xD405); send_data(0x7B); 
	send_cmd(0xD406); send_data(0x00); 
	send_cmd(0xD407); send_data(0x99); 
	send_cmd(0xD408); send_data(0x00); 
	send_cmd(0xD409); send_data(0xB1); 
	send_cmd(0xD40A); send_data(0x00); 
	send_cmd(0xD40B); send_data(0xD2); 
	send_cmd(0xD40C); send_data(0x00); 
	send_cmd(0xD40D); send_data(0xF6); 
	send_cmd(0xD40E); send_data(0x01); 
	send_cmd(0xD40F); send_data(0x27); 
	send_cmd(0xD410); send_data(0x01); 
	send_cmd(0xD411); send_data(0x4E); 
	send_cmd(0xD412); send_data(0x01); 
	send_cmd(0xD413); send_data(0x8C); 
	send_cmd(0xD414); send_data(0x01); 
	send_cmd(0xD415); send_data(0xBE); 
	send_cmd(0xD416); send_data(0x02); 
	send_cmd(0xD417); send_data(0x0B); 
	send_cmd(0xD418); send_data(0x02); 
	send_cmd(0xD419); send_data(0x48); 
	send_cmd(0xD41A); send_data(0x02); 
	send_cmd(0xD41B); send_data(0x4A); 
	send_cmd(0xD41C); send_data(0x02); 
	send_cmd(0xD41D); send_data(0x7E); 
	send_cmd(0xD41E); send_data(0x02); 
	send_cmd(0xD41F); send_data(0xBC); 
	send_cmd(0xD420); send_data(0x02); 
	send_cmd(0xD421); send_data(0xE1); 
	send_cmd(0xD422); send_data(0x03); 
	send_cmd(0xD423); send_data(0x10); 
	send_cmd(0xD424); send_data(0x03); 
	send_cmd(0xD425); send_data(0x31); 
	send_cmd(0xD426); send_data(0x03); 
	send_cmd(0xD427); send_data(0x5A); 
	send_cmd(0xD428); send_data(0x03); 
	send_cmd(0xD429); send_data(0x73); 
	send_cmd(0xD42A); send_data(0x03); 
	send_cmd(0xD42B); send_data(0x94); 
	send_cmd(0xD42C); send_data(0x03); 
	send_cmd(0xD42D); send_data(0x9F); 
	send_cmd(0xD42E); send_data(0x03); 
	send_cmd(0xD42F); send_data(0xB3); 
	send_cmd(0xD430); send_data(0x03); 
	send_cmd(0xD431); send_data(0xB9); 
	send_cmd(0xD432); send_data(0x03); 
	send_cmd(0xD433); send_data(0xC1); 
	                  
	//G-                  
	send_cmd(0xD500); send_data(0x00); 
	send_cmd(0xD501); send_data(0x37); 
	send_cmd(0xD502); send_data(0x00); 
	send_cmd(0xD503); send_data(0x52); 
	send_cmd(0xD504); send_data(0x00); 
	send_cmd(0xD505); send_data(0x7B); 
	send_cmd(0xD506); send_data(0x00); 
	send_cmd(0xD507); send_data(0x99); 
	send_cmd(0xD508); send_data(0x00); 
	send_cmd(0xD509); send_data(0xB1); 
	send_cmd(0xD50A); send_data(0x00); 
	send_cmd(0xD50B); send_data(0xD2); 
	send_cmd(0xD50C); send_data(0x00); 
	send_cmd(0xD50D); send_data(0xF6); 
	send_cmd(0xD50E); send_data(0x01); 
	send_cmd(0xD50F); send_data(0x27); 
	send_cmd(0xD510); send_data(0x01); 
	send_cmd(0xD511); send_data(0x4E); 
	send_cmd(0xD512); send_data(0x01); 
	send_cmd(0xD513); send_data(0x8C); 
	send_cmd(0xD514); send_data(0x01); 
	send_cmd(0xD515); send_data(0xBE); 
	send_cmd(0xD516); send_data(0x02); 
	send_cmd(0xD517); send_data(0x0B); 
	send_cmd(0xD518); send_data(0x02); 
	send_cmd(0xD519); send_data(0x48); 
	send_cmd(0xD51A); send_data(0x02);
	send_cmd(0xD51B); send_data(0x4A); 
	send_cmd(0xD51C); send_data(0x02); 
	send_cmd(0xD51D); send_data(0x7E); 
	send_cmd(0xD51E); send_data(0x02); 
	send_cmd(0xD51F); send_data(0xBC); 
	send_cmd(0xD520); send_data(0x02); 
	send_cmd(0xD521); send_data(0xE1); 
	send_cmd(0xD522); send_data(0x03); 
	send_cmd(0xD523); send_data(0x10); 
	send_cmd(0xD524); send_data(0x03); 
	send_cmd(0xD525); send_data(0x31); 
	send_cmd(0xD526); send_data(0x03); 
	send_cmd(0xD527); send_data(0x5A); 
	send_cmd(0xD528); send_data(0x03); 
	send_cmd(0xD529); send_data(0x73); 
	send_cmd(0xD52A); send_data(0x03); 
	send_cmd(0xD52B); send_data(0x94); 
	send_cmd(0xD52C); send_data(0x03); 
	send_cmd(0xD52D); send_data(0x9F); 
	send_cmd(0xD52E); send_data(0x03); 
	send_cmd(0xD52F); send_data(0xB3); 
	send_cmd(0xD530); send_data(0x03); 
	send_cmd(0xD531); send_data(0xB9); 
	send_cmd(0xD532); send_data(0x03); 
	send_cmd(0xD533); send_data(0xC1); 
	                  
	//B-                  
	send_cmd(0xD600); send_data(0x00); 
	send_cmd(0xD601); send_data(0x37); 
	send_cmd(0xD602); send_data(0x00); 
	send_cmd(0xD603); send_data(0x52); 
	send_cmd(0xD604); send_data(0x00); 
	send_cmd(0xD605); send_data(0x7B); 
	send_cmd(0xD606); send_data(0x00); 
	send_cmd(0xD607); send_data(0x99); 
	send_cmd(0xD608); send_data(0x00); 
	send_cmd(0xD609); send_data(0xB1); 
	send_cmd(0xD60A); send_data(0x00); 
	send_cmd(0xD60B); send_data(0xD2); 
	send_cmd(0xD60C); send_data(0x00); 
	send_cmd(0xD60D); send_data(0xF6); 
	send_cmd(0xD60E); send_data(0x01); 
	send_cmd(0xD60F); send_data(0x27); 
	send_cmd(0xD610); send_data(0x01); 
	send_cmd(0xD611); send_data(0x4E); 
	send_cmd(0xD612); send_data(0x01); 
	send_cmd(0xD613); send_data(0x8C); 
	send_cmd(0xD614); send_data(0x01); 
	send_cmd(0xD615); send_data(0xBE); 
	send_cmd(0xD616); send_data(0x02); 
	send_cmd(0xD617); send_data(0x0B); 
	send_cmd(0xD618); send_data(0x02); 
	send_cmd(0xD619); send_data(0x48); 
	send_cmd(0xD61A); send_data(0x02); 
	send_cmd(0xD61B); send_data(0x4A); 
	send_cmd(0xD61C); send_data(0x02); 
	send_cmd(0xD61D); send_data(0x7E); 
	send_cmd(0xD61E); send_data(0x02); 
	send_cmd(0xD61F); send_data(0xBC); 
	send_cmd(0xD620); send_data(0x02); 
	send_cmd(0xD621); send_data(0xE1); 
	send_cmd(0xD622); send_data(0x03); 
	send_cmd(0xD623); send_data(0x10); 
	send_cmd(0xD624); send_data(0x03); 
	send_cmd(0xD625); send_data(0x31); 
	send_cmd(0xD626); send_data(0x03); 
	send_cmd(0xD627); send_data(0x5A); 
	send_cmd(0xD628); send_data(0x03); 
	send_cmd(0xD629); send_data(0x73); 
	send_cmd(0xD62A); send_data(0x03); 
	send_cmd(0xD62B); send_data(0x94); 
	send_cmd(0xD62C); send_data(0x03); 
	send_cmd(0xD62D); send_data(0x9F); 
	send_cmd(0xD62E); send_data(0x03); 
	send_cmd(0xD62F); send_data(0xB3); 
	send_cmd(0xD630); send_data(0x03); 
	send_cmd(0xD631); send_data(0xB9); 
	send_cmd(0xD632); send_data(0x03); 
	send_cmd(0xD633); send_data(0xC1); 
	                  
	//Enable Page0       
	send_cmd(0xF000); send_data(0x55); 
	send_cmd(0xF001); send_data(0xAA); 
	send_cmd(0xF002); send_data(0x52); 
	send_cmd(0xF003); send_data(0x08); 
	send_cmd(0xF004); send_data(0x00); 

	//#Display control
	send_cmd(0xB100);send_data(0xFC);
	send_cmd(0xB101);send_data(0x00);
	                   
	//RGB I/F Setting 
	send_cmd(0xB000); send_data(0x08); 
	send_cmd(0xB001); send_data(0x05); 
	send_cmd(0xB002); send_data(0x02); 
	send_cmd(0xB003); send_data(0x05); 
	send_cmd(0xB004); send_data(0x02); 

	// SDT: 
	send_cmd(0xB600); send_data(0x0A); 

	// Gate EQ: 
	send_cmd(0xB700); send_data(0x00); 
	send_cmd(0xB701); send_data(0x70); 

	// Source EQ 
	send_cmd(0xB800); send_data(0x01); 
	send_cmd(0xB801); send_data(0x05); 
	send_cmd(0xB802); send_data(0x05); 
	send_cmd(0xB803); send_data(0x05); 
	                   
	///Inversion: Column inversion (NVT) 
	send_cmd(0xBC00); send_data(0x00);
	send_cmd(0xBC01); send_data(0x00);
	send_cmd(0xBC02); send_data(0x00); 

	//BOE's Setting (default) 
	send_cmd(0xCC00); send_data(0x03); 
	send_cmd(0xCC01); send_data(0x00); 
	send_cmd(0xCC02); send_data(0x00); 

	//Display Timing 
	send_cmd(0xBD00); send_data(0x01); 
	send_cmd(0xBD01); send_data(0x6c); 
	send_cmd(0xBD02); send_data(0x07); 
	send_cmd(0xBD03); send_data(0x31); 
	send_cmd(0xBD04); send_data(0x00); 

	send_cmd(0xBA00); send_data(0x01); 
	send_cmd(0xFF00); send_data(0xAA); 
	send_cmd(0xFF01); send_data(0x55); 
	send_cmd(0xFF02); send_data(0x25); 
	send_cmd(0xFF03); send_data(0x01); 
	                   
	send_cmd(0x3A00); send_data(0x55); 

	send_cmd(0x3600); send_data(0x00); 
	send_cmd(0x3500); send_data(0x00);
	send_cmd(0x4400); send_data(0x01);
	send_cmd(0x4401); send_data(0x0f);

	send_cmd(0x1100); 
	LCD_DelayMS(200);
	send_cmd(0x2900);
	LCD_DelayMS(200);
	send_cmd(0x2c00); 
	
	return 0;
}

static int32_t nt35510_enter_sleep(struct panel_spec *self, uint8_t is_sleep)
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




static int32_t nt35510_set_window(struct panel_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	send_data_t send_data = self->info.mcu->ops->send_data;

	pr_debug("nt35510_set_window: %d, %d, %d, %d\n",left, top, right, bottom);

	send_cmd(0x2A00); send_data((left>>8));// set left address
	send_cmd(0x2A01); send_data((left&0xff));
	send_cmd(0x2A02); send_data((right>>8));// set right address
	send_cmd(0x2A03); send_data((right&0xff));

	send_cmd(0x2B00); send_data((top>>8));// set left address
	send_cmd(0x2B01); send_data((top&0xff));
	send_cmd(0x2B02); send_data((bottom>>8));// set bottom address
	send_cmd(0x2B03); send_data((bottom&0xff));

	send_cmd(0x2C00);

	return 0;
}
static int32_t nt35510_invalidate(struct panel_spec *self)
{
	pr_debug("nt35510_invalidate\n");

	return self->ops->panel_set_window(self, 0, 0,
		self->width - 1, self->height - 1);
}



static int32_t nt35510_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	pr_debug("nt35510_invalidate_rect \n");

	return self->ops->panel_set_window(self, left, top,
			right, bottom);
}

static int32_t nt35510_read_id(struct panel_spec *self)
{
	int32_t id  = 0;
	int16_t id_high = 0;
	int16_t id_low = 0;
	int32_t  value = 0;
	
	send_data_t send_cmd = self->info.mcu->ops->send_cmd;
	read_data_t read_data = self->info.mcu->ops->read_data;
	send_data_t send_data = self->info.mcu->ops->send_data;

	send_cmd(0xDB00);
	value = read_data();
	pr_debug("nt35510_read_id(DB00):id=0x%x.", value);	// 0x80

	//PAGE1
	send_cmd(0xF000);	 send_data(0x55);
	send_cmd(0xF001);	 send_data(0xAA);
	send_cmd(0xF002);	 send_data(0x52);
	send_cmd(0xF003);	 send_data(0x08);
	send_cmd(0xF004);	 send_data(0x01);

	send_cmd(0xc500);
	id_high=read_data();//0x55
	pr_debug("lcd_id   (id_high =0x%x)\n",  id_high);
	
	send_cmd(0xc501);
	id_low=read_data();
	pr_debug("lcd_id   (id_low =0x%x)\n",  id_low);
	
	id= (id_high<<8)+id_low;
	pr_debug("lcd_id   %s(id =0x%x)\n", __func__, id);

	if((value == 0x80)||(id==0x5510))
		value = 0x80;

	return value;
}

static struct panel_operations lcd_nt35510_mcu_operations = {
	.panel_init = nt35510_init,
	.panel_set_window = nt35510_set_window,
	.panel_invalidate_rect= nt35510_invalidate_rect,
	.panel_invalidate = nt35510_invalidate,
	.panel_enter_sleep = nt35510_enter_sleep,
	.panel_readid          = nt35510_read_id
};

static struct timing_mcu lcd_nt35510_mcu_timing[] = {
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

static struct info_mcu lcd_nt35510_mcu_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.bpp = 16,
	.timing = lcd_nt35510_mcu_timing,
	.ops = NULL,
};

//lcd_panel_nt35510
struct panel_spec lcd_nt35510_mcu_spec = {
	.width = 480,
#if defined(CONFIG_LCD_FWVGA)
	.height = 854,
#else
	.height = 800,
#endif
	.type = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {
		.mcu = &lcd_nt35510_mcu_info
	},
	.ops = &lcd_nt35510_mcu_operations,
};

struct panel_cfg lcd_nt35510_mcu = {
	/* this panel may on both CS0/1 */
	.dev_id = SPRDFB_UNDEFINELCD_ID,
	.lcd_id = 0x80,
	.lcd_name = "lcd_nt35510_mcu",
	.panel = &lcd_nt35510_mcu_spec,
};

static int __init lcd_nt35510_mcu_init(void)
{
	return sprdfb_panel_register(&lcd_nt35510_mcu);
}

subsys_initcall(lcd_nt35510_mcu_init);
