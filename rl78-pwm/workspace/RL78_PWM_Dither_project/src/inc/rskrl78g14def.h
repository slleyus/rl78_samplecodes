/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : rskrl78g14def.h
* Device(s)    : R5F104PJ
* Tool-Chain   : GNURL78 v12.01
* Description  : Defines macros relating to the RL78G14 user LEDs and switches
* Creation Date: 21/05/2012
***********************************************************************************************************************/


#ifndef _RSKRL78G14_H
#define _RSKRL78G14_H
/***********************************************************************************************************************
User Defines
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"


/* General Values */
#define LED_ON				(0)
#define LED_OFF				(1)
#define SET_BIT_HIGH	    (1)
#define SET_BIT_LOW			(0)
#define SET_BYTE_HIGH		(0xFF)
#define SET_BYTE_LOW		(0x00)
#define	TRUE				(1)
#define	FALSE				(0)

/* Switches */
#define	SW1					P7_bit.no4
#define	SW2					P7_bit.no5
#define	SW3					P7_bit.no6

/* LED port settings */
#define	LED0				P4_bit.no3
#define	LED1				P4_bit.no4
#define	LED2				P6_bit.no2
#define	LED3				P6_bit.no3

/* Switch 1 pressed flag mask */
#define SWITCHPRESS_1		0x80u
/* Switch 1 held down flag mask */
#define SWITCHHOLD_1		0x08u
/* Switch 2 pressed flag mask */
#define SWITCHPRESS_2		0x40u
/* Switch 2 held down flag mask */
#define SWITCHHOLD_2		0x04u
/* Switch 3 pressed flag mask */
#define SWITCHPRESS_3		0x20u
/* Switch 3 held down flag mask */
#define SWITCHHOLD_3		0x02u

#endif

