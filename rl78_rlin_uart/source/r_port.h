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
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name 	: r_port.h
* Version 		: 1.0
* Device(s) 	: R5F100LE
* H/W Platform 	: QB-R5F100LE-TB
* Description 	: This file implements device driver for PORT module.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 21.12.2012 1.00 First Release
******************************************************************************/

#ifndef _MDPORT_
#define _MDPORT_

/******************************************************************************
Macro definitions
******************************************************************************/
/*
	AD port configuration register (ADPC)
*/
/* Analog input/digital input switching (ADPC4 - ADPC0) */
#define _00_ADPC_DI_OFF		0x00U		/* use ANI0/P33 - ANI17/P97 as analog input */
#define _01_ADPC_DI_ON		0x01U		/* use ANI0/P33 - ANI17/P97 as digital input */

/*
Port I/O Redirection Register(PIOR4)
*/
/* P4n pin I/O redirection function selection (PIOR4n)*/
#define _00_PIOR40_NO_USE		0x00U	/* not use PIOR40 */
#define _00_PIOR41_NO_USE		0x00U	/* not use PIOR41 */
#define _00_PIOR42_NO_USE		0x00U	/* not use PIOR42 */
#define _00_PIOR43_NO_USE		0x00U	/* not use PIOR43 */
#define _00_PIOR44_NO_USE		0x00U	/* not use PIOR44 */
#define _00_PIOR45_NO_USE		0x00U	/* not use PIOR45 */
#define _00_PIOR46_NO_USE		0x00U	/* not use PIOR46 */
#define _00_PIOR47_NO_USE		0x00U	/* not use PIOR47 */
#define _00_PIOR44_SET_0		0x00U	/* set PIOR44   0 */
#define _10_PIOR44_SET_1		0x10U	/* set PIOR44	1 */

/*
Port Mode Register (PMm)
*/
/* Pmn pin I/O mode selection (PMm7 - PMm0) */
#define _01_PMm0_NOT_USE		0x01U	/* not use Pm0 as digital I/O */
#define _01_PMm0_MODE_INPUT		0x01U	/* use Pm0 as input mode */
#define _00_PMm0_MODE_OUTPUT	0x00U	/* use Pm0 as output mode */
#define _02_PMm1_NOT_USE		0x02U	/* not use Pm1 as digital I/O */
#define _02_PMm1_MODE_INPUT		0x02U	/* use Pm1 as input mode */
#define _00_PMm1_MODE_OUTPUT	0x00U	/* use Pm1 as output mode */
#define _04_PMm2_NOT_USE		0x04U	/* not use Pm2 as digital I/O */
#define _04_PMm2_MODE_INPUT		0x04U	/* use Pm2 as input mode */
#define _00_PMm2_MODE_OUTPUT	0x00U	/* use Pm2 as output mode */
#define _08_PMm3_NOT_USE		0x08U	/* not use Pm3 as digital I/O */
#define _08_PMm3_MODE_INPUT		0x08U	/* use Pm3 as input mode */
#define _00_PMm3_MODE_OUTPUT	0x00U	/* use Pm3 as output mode */
#define _10_PMm4_NOT_USE		0x10U	/* not use Pm4 as digital I/O */
#define _10_PMm4_MODE_INPUT		0x10U	/* use Pm4 as input mode */
#define _00_PMm4_MODE_OUTPUT	0x00U	/* use Pm4 as output mode */
#define _20_PMm5_NOT_USE		0x20U	/* not use Pm5 as digital I/O */
#define _20_PMm5_MODE_INPUT		0x20U	/* use Pm5 as input mode */
#define _00_PMm5_MODE_OUTPUT	0x00U	/* use Pm5 as output mode */
#define _40_PMm6_NOT_USE		0x40U	/* not use Pm6 as digital I/O */
#define _40_PMm6_MODE_INPUT		0x40U	/* use Pm6 as input mode */
#define _00_PMm6_MODE_OUTPUT	0x00U	/* use Pm6 as output mode */
#define _80_PMm7_NOT_USE		0x80U	/* not use Pm7 as digital I/O */
#define _80_PMm7_MODE_INPUT		0x80U	/* use Pm7 as input mode */
#define _00_PMm7_MODE_OUTPUT	0x00U	/* use Pm7 as output mode */
#define _FF_PM1_DEFAULT			0xFFU	/* PM1 default value */
#define _FF_PM4_DEFAULT			0xFFU	/* PM4 default value */
#define _00_PM0_ALLOUTPUT		0x00U	/* use P0 as output mode */
/*
Port Register (Pm)
*/
/* Pmn pin data (Pm0 to Pm7) */
#define _00_Pm0_OUTPUT_0		0x00U	/* Pm0 output 0 */
#define _01_Pm0_OUTPUT_1		0x01U	/* Pm0 output 1 */
#define _00_Pm1_OUTPUT_0		0x00U	/* Pm1 output 0 */
#define _02_Pm1_OUTPUT_1		0x02U	/* Pm1 output 1 */
#define _00_Pm2_OUTPUT_0		0x00U	/* Pm2 output 0 */
#define _04_Pm2_OUTPUT_1		0x04U	/* Pm2 output 1 */
#define _00_Pm3_OUTPUT_0		0x00U	/* Pm3 output 0 */
#define _08_Pm3_OUTPUT_1		0x08U	/* Pm3 output 1 */
#define _00_Pm4_OUTPUT_0		0x00U	/* Pm4 output 0 */
#define _10_Pm4_OUTPUT_1		0x10U	/* Pm4 output 1 */
#define _00_Pm5_OUTPUT_0		0x00U	/* Pm5 output 0 */
#define _20_Pm5_OUTPUT_1		0x20U	/* Pm5 output 1 */
#define _00_Pm6_OUTPUT_0		0x00U	/* Pm6 output 0 */
#define _40_Pm6_OUTPUT_1		0x40U	/* Pm6 output 1 */
#define _00_Pm7_OUTPUT_0		0x00U	/* Pm7 output 0 */
#define _80_Pm7_OUTPUT_1		0x80U	/* Pm7 output 1 */

/*
Pull-up Resistor Option Register (PUm)
*/
/* Pmn pin on-chip pull-up resistor selection (PUmn) */
#define _00_PUm0_PULLUP_OFF		0x00U	/* Pm0 pull-up resistor not connected */
#define _01_PUm0_PULLUP_ON		0x01U	/* Pm0 pull-up resistor connected */
#define _00_PUm1_PULLUP_OFF		0x00U	/* Pm1 pull-up resistor not connected */
#define _02_PUm1_PULLUP_ON		0x02U	/* Pm1 pull-up resistor connected */
#define _00_PUm2_PULLUP_OFF		0x00U	/* Pm2 Pull-up resistor not connected */
#define _04_PUm2_PULLUP_ON		0x04U	/* Pm2 pull-up resistor connected */
#define _00_PUm3_PULLUP_OFF		0x00U	/* Pm3 pull-up resistor not connected */
#define _08_PUm3_PULLUP_ON		0x08U	/* Pm3 pull-up resistor connected */
#define _00_PUm4_PULLUP_OFF		0x00U	/* Pm4 pull-up resistor not connected */
#define _10_PUm4_PULLUP_ON		0x10U	/* Pm4 pull-up resistor connected */
#define _00_PUm5_PULLUP_OFF		0x00U	/* Pm5 pull-up resistor not connected */
#define _20_PUm5_PULLUP_ON		0x20U	/* Pm5 pull-up resistor connected */
#define _00_PUm6_PULLUP_OFF		0x00U	/* Pm6 pull-up resistor not connected */
#define _40_PUm6_PULLUP_ON		0x40U	/* Pm6 pull-up resistor connected */
#define _00_PUm7_PULLUP_OFF		0x00U	/* Pm7 pull-up resistor not connected */
#define _80_PUm7_PULLUP_ON		0x80U	/* Pm7 pull-up resistor connected */

/*
Port Input Mode Register (PIMm)
*/
/* Pmn pin input buffer selection (PIM1) */
#define _00_PIM1_DEFAULT		0x00U	/* PIM1  default setting */

/*
Port Output Mode Register (POMm)
*/
/* Pmn pin output mode selection (POM1) */
#define _00_POM1_DEFAULT		0x00U	/* POM1 default setting */

/*
Port Input Threshold Control Register (PITHLm)
*/
/* Pmn pin threshold control selection (PITHL1,PITHL4) */
#define _00_PITHL1_DEFAULT		0x00U	/* PITHL1 defalut setting */
#define _00_PITHL4_DEFAULT		0x00U	/* PITHL4 defalut setting */

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void R_PORT_Init(void);

#endif
