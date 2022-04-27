/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name	: rskrl78g13def.c
* Version 	: 1.00
* Device 	: R5F100LEAFB
* Tool Chain 	: RL Family C Compiler (IAR ICCRL78)
* H/W Platform	: RSKRL78G13
* Description   : Defines macros relating to the RL78G13 user LEDs and switches
*******************************************************************************/
/*******************************************************************************
* History 	: 21.06.2011 Ver. 1.00 First Release
*******************************************************************************/


#ifndef _RSKRL78G13_H
#define _RSKRL78G13_H
/*******************************************************************************
User Defines
*******************************************************************************/
#include "r_cg_macrodriver.h"


/* General Values */
#define LED_ON			(0)
#define LED_OFF			(1)
#define SET_BIT_HIGH	        (1)
#define SET_BIT_LOW		(0)
#define SET_BYTE_HIGH		(0xFF)
#define SET_BYTE_LOW		(0x00)
#define	TRUE			(1)
#define	FALSE			(0)

/* Switch */
#define	SW1			P5_bit.no0
#define	SW2			P5_bit.no1
#define	SW3			P3_bit.no1

/* LED port settings */
#define	LED0			P5_bit.no2
#define	LED1			P5_bit.no3
#define	LED2			P6_bit.no2
#define	LED3			P6_bit.no3


#endif

