
/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
*
* File Name          : mma845x.h
* Authors            : MH - C&I BU - Application Team
*		     : Carmine Iascone (carmine.iascone@st.com)
*		     : Matteo Dameno (matteo.dameno@st.com)
* Version            : V 1.0.5
* Date               : 26/08/2010
*
********************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* THE PRESENT SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES
* OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, FOR THE SOLE
* PURPOSE TO SUPPORT YOUR APPLICATION DEVELOPMENT.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/
/*******************************************************************************
Version History.

Revision 1-0-0 05/11/2009
First Release
Revision 1-0-1 26/01/2010
Linux K&R Compliant Release
Revision 1-0-5 16/08/2010
Interrupt Management

*******************************************************************************/

#ifndef	__MMA845X_H__
#define	__MMA845X_H__

#include	<linux/ioctl.h>	/* For IOCTL macros */
#include	<linux/input.h>

#ifndef DEBUG
#define DEBUG
#endif

/************************************************/
/* 	Accelerometer defines section	 	*/
/************************************************/
//Alex.shi I2C_ADD have two choice:0x1D when SA0=1;0x1C when SA0=0;
#define MMA845X_I2C_ADDR	0x1C

#define	MMA845X_ACC_DEV_NAME	"mma845x"

/* The sensitivity is represented in counts/g. In 2g mode the
sensitivity is 1024 counts/g. In 4g mode the sensitivity is 512
counts/g and in 8g mode the sensitivity is 256 counts/g.
 */
enum {
	MODE_2G = 0,
	MODE_4G,
	MODE_8G,
};

#ifdef	__KERNEL__
struct mma845x_acc_platform_data {
	int poll_interval;
	int min_interval;
	int max_interval;

	u8 g_range;

	u8 g_position;
};
#endif	/* __KERNEL__ */

#endif	/* __MMA845X_H__ */



