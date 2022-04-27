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
* File Name 	: r_port.c
* Version 		: 1.0
* Device(s) 	: R5F10PMFK
* H/W Platform  : QB-R5F10PMFK-TB
* Description 	: This file implements port initializing function.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 21.12.2012 1.00 First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include "r_macrodriver.h"
#include "r_port.h"

/******************************************************************************
* Function Name	: R_PORT_Init
* Description 	: This function initializes port macro.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void R_PORT_Init(void)
{
	ADPC = _01_ADPC_DI_ON;

//	PM0 = _00_PM0_ALLOUTPUT;	/* test port */
	/* set PIOR44 buffer */
	PIOR4 = _00_PIOR40_NO_USE    | _00_PIOR41_NO_USE    | _00_PIOR42_NO_USE   | _00_PIOR43_NO_USE
		  | _00_PIOR44_SET_0     | _00_PIOR45_NO_USE    | _00_PIOR46_NO_USE   | _00_PIOR47_NO_USE;
	
	switch( PIOR4 & 0x10 ){
		case _00_PIOR44_SET_0:	/* P13/LTxD0	P14/LRxD0 */
			PM1   = _01_PMm0_NOT_USE	 | _02_PMm1_NOT_USE		| _04_PMm2_NOT_USE    | _00_PMm3_MODE_OUTPUT
				  | _10_PMm4_MODE_INPUT  | _20_PMm5_NOT_USE  	| _40_PMm6_NOT_USE    | _80_PMm7_NOT_USE;
			P1    = _00_Pm0_OUTPUT_0     | _00_Pm1_OUTPUT_0     | _00_Pm2_OUTPUT_0    | _08_Pm3_OUTPUT_1
			      | _00_Pm4_OUTPUT_0     | _00_Pm5_OUTPUT_0     | _00_Pm6_OUTPUT_0    | _00_Pm7_OUTPUT_0;
			PU1   = _00_PUm0_PULLUP_OFF  | _00_PUm1_PULLUP_OFF  | _00_PUm2_PULLUP_OFF | _08_PUm3_PULLUP_ON
			      | _10_PUm4_PULLUP_ON   | _00_PUm5_PULLUP_OFF  | _00_PUm6_PULLUP_OFF | _00_PUm7_PULLUP_OFF;
			PIM1  = _00_PIM1_DEFAULT;
			POM1  = _00_POM1_DEFAULT;
			PITHL1= _00_PITHL1_DEFAULT;
			break;
		case _10_PIOR44_SET_1:	/* P42/LTxD0	P43/LRxD0 */
			PM4   = _01_PMm0_NOT_USE     | _02_PMm1_NOT_USE		| _00_PMm2_MODE_OUTPUT | _08_PMm3_MODE_INPUT
			      | _10_PMm4_NOT_USE	 | _20_PMm5_NOT_USE   	| _40_PMm6_NOT_USE     | _80_PMm7_NOT_USE;
			P4    = _00_Pm0_OUTPUT_0 	 | _00_Pm1_OUTPUT_0 	| _04_Pm2_OUTPUT_1     | _00_Pm3_OUTPUT_0
				  | _00_Pm4_OUTPUT_0  	 | _00_Pm5_OUTPUT_0     | _00_Pm6_OUTPUT_0     | _00_Pm7_OUTPUT_0;
			PU4   = _00_PUm0_PULLUP_OFF  | _00_PUm1_PULLUP_OFF  | _04_PUm2_PULLUP_ON   | _08_PUm3_PULLUP_ON
			      | _00_PUm4_PULLUP_OFF  | _00_PUm5_PULLUP_OFF  | _00_PUm6_PULLUP_OFF  | _00_PUm7_PULLUP_OFF;
			PITHL4= _00_PITHL4_DEFAULT;
			break;
		default:;
	}

}

/******************************************************************************
End of function R_PORT_Init
******************************************************************************/

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
