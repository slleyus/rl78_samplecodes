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
* File Name 	: r_systeminit.c
* Version 		: 1.0
* Device(s) 	: R5F10PMFK
* H/W Platform  : QB-R5F10PMFK-TB
* Description 	: This file implements Lin Uart system initializing function.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 21.12.2012 1.00 First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include "r_macrodriver.h"
#include "r_system_clock.h"
#include "r_port.h"
#include "r_serial.h"
/******************************************************************************
* Function Name	: R_CLOCK_Init(Internal high-speed oscillation clock)
* Description 	: This function initializes clock macro.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void R_CLOCK_Init(void)
{
	/* Set fMX */
	CMC = _40_CG_HISYS_OSC
		| _00_CG_SUB_PORT
		| _01_CG_SYSOSC_OVER10M;
	MSTOP = 1U;
	/* Set fMAIN */
	MCM0 = 0U;
	/* Set fSUB */
	XTSTOP = 1U;
	OSMC   = _10_CG_RTC_CLK_FIL;
	/* Set fCLK */
	CSS    = 0U;
	/* Set fIH */
	HIOSTOP = 0U;
}

/******************************************************************************
End of function R_CLOCK_Init
******************************************************************************/

/******************************************************************************
* Function Name	: hdwinit
* Description	: This function initializes hardware setting.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void hdwinit(void)
{
	DI();
	R_CLOCK_Init();	/* Initialize clock macro */
	R_PORT_Init();	/* initializes port macro */
	R_LIN0_Init();	/* initializes LIN0-UART0 macro */
	EI();
}
/******************************************************************************
End of function hdwinit
******************************************************************************/
