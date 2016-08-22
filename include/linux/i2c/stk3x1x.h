/*
 *
 * $Id: stk3x1x.h
 *
 * Copyright (C) 2012~2013 Lex Hsieh     <lex_hsieh@sensortek.com.tw> 
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 */
#ifndef __STK3X1X_H__
#define __STK3X1X_H__

#define STK3310_DEVICE_NAME		"stk3x1x_ps"
#define STK3310_ADDRESS			0x48
#ifdef CONFIG_ZYT_PLSENSOR_COMPATIBLE
// Alex.shi ����stk3310��stk3101��i2c��ַһ��,��������Ҫ��һ����ƭ  
#define STK3310_ADDRESS_INFO	0x02
#else
#define STK3310_ADDRESS_INFO	STK3310_ADDRESS
#endif

/* platform data */
struct stk3x1x_platform_data
{
	uint8_t		state_reg;
	uint8_t		psctrl_reg;
	uint8_t		alsctrl_reg;
	uint8_t		ledctrl_reg;
	uint8_t		wait_reg;	
	uint16_t	ps_thd_h;
	uint16_t	ps_thd_l;
	int			int_pin;
	uint32_t	transmittance;
};

#endif // __STK3X1X_H__

