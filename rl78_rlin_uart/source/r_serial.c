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
* File Name 	: r_serial.c
* Version 		: 1.0
* Device(s) 	: R5F10PMFK
* H/W Platform 	: QB-R5F10PMFK-TB
* Description 	: This file implements device driver for Lin Uart Serial module.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 21.12.2012 1.00 First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include "r_macrodriver.h"
#include "r_serial.h"

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
volatile u8 g_UartRxLen;			/* LIN0-UART0 receive data length */
volatile u8 g_UartRxCnt;			/* LIN0-UART0 receive data count */
volatile u8 *g_UartRxAddress;		/* LIN0-UART0 receive buffer address */
volatile u8 g_UartTxCnt;			/* LIN0-UART0 transmit data count */
volatile u8 *g_UartTxAddress;		/* LIN0-UART0 transmit buffer address */

/******************************************************************************
* Function Name	: R_LIN0_Init
* Description 	: This function initializes the LIN0 module.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void R_LIN0_Init(void)
{
	LCHSEL 	 = 0x00U;	/* select LIN0 channel*/
	LIN0EN   = 1U;		/* supply LIN0 clock */
	LIN0MCK  = 0U;		/* 0:fCLK,1:fMX */
	LIN0MCKE = 1U;		/* supply LIN0 engine clock */
	R_UART_Init();
}
/******************************************************************************
End of function R_LIN0_Init
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Init
* Description 	: This function initializes the LIN0-UART module.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void R_UART_Init(void)
{
	
	LBRP00 = 52-1;		/* LIN_UART Baud rate Prescaler 0 register(32MHz/NSPBê›íËíl[=16]/38400) */
	LBRP01 = 0x00U;		/* not use */
	LWBR0  = _000_LWBRn_LPRS_1 | _00_LWBRn_NSPB_16;
						/* set LIN Wake-up Baud Rate Selector register */
	LUSC0  = _00_LUSC_UWC | _00_LUSC_USEC | _00_LUSC_URDCC;
						/* disable the start-up of reception in the STOP mode */
						/* enable the generation of error interrupts */
						/* disable comparison of reception data with ID register in SNOOZE mode */
	LMD0   = _01_LMDn_LMD | _00_LMDn_LRDNFS;
						/* Uart mode select and use the noise filter */
	LEDE0  = _01_LEDEn_BERE_Enable | _04_LEDEn_OERE_Enable |  _08_LEDEn_FERE_Enable;
						/* Allow all error detection */
	LBFC0  = _00_LBFCn_UBLS_8bit | _00_LBFCn_UBOS_LSB | _00_LBFCn_USBLS_1bit | _00_LBFCn_UPS_NOT_USE | _00_LBFCn_URPS_R_Normal | _00_LBFCn_UTPS_T_Normal;
						/* 8bit communication ,LSB first ,1 stop bit , Parity Disabled ,Tx/Rx without inversion */
	LSC0   = 0x00U;		/* not use */
	LIE0   = _04_LIEn_ERRIE_Enable;		/* Error Detection Interrupt Enabled */
	LCUC0  = _01_LCUCn_OM0_Inactive;	/* SW Reset request is inactive */
	LUOR01 = _01_LUORn1_UEBE_Enable | _00_LUORn1_UEBDL_Value_0 | _08_LUORn1_UTIGTS_Complet | _00_LUORn1_UEBE_Enable;
						/* Enables expansion bit operation , Expansion bit comparison enable */
						/* Selects expansion bit value "0" as expansion bit detecition level */
						/* Outputs transmission interrupt request upon transmission completion */
	while( (LMST0&0x01) == 0x01);		/* Module in Reset state? */
	
}
/******************************************************************************
End of function R_UART_Init
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Start
* Description 	: This function starts the LIN0-UART module operation.
* Arguments 	: none
* Return Value	: none
******************************************************************************/
void R_UART_Start(void)
{
	int i;
	LIN0RVCIF = 0U;		/* clear  INTLIN0RVC interrupt flag */
	LIN0RVCMK = 0U;		/* enable INTLIN0RVC interrupt flag */
	LIN0TRMIF = 0U;		/* clear  INTLIN0TRM interrupt flag */
	LIN0TRMMK = 0U;		/* enable INTLIN0TRM interrupt flag */
	LIN0STAIF = 0U;		/* clear  INTLIN0STA interrupt flag */
	LIN0STAMK = 0U;		/* enable INTLIN0STA interrupt flag */

	LUOER0 = _01_LUOERn_UTOE_Enable | _02_LUOERn_UROE_Enable;
						/* Enables transmission operation, reception operation */
}
/******************************************************************************
End of function R_UART_Start
******************************************************************************/

/******************************************************************************
* Function Name	: R_LIN0_UART_Stop
* Description 	: This function stops the LIN0-UART module operation.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void R_UART_Stop(void)
{
	LIN0RVCMK = 1U;		/* disable INTLIN0RVC interrupt */
	LIN0RVCIF = 0U;		/* clear   INTLIN0RVC interrupt flag */
	LIN0TRMMK = 1U;		/* disable INTLIN0TRM interrupt */
	LIN0TRMIF = 0U;		/* clear   INTLIN0TRM interrupt flag */
	LIN0STAMK = 1U;		/* disable INTLIN0STA interrupt */
	LIN0STAIF = 0U;		/* clear   INTLIN0STA interrupt flag */
}
/******************************************************************************
End of function R_UART_Stop
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Receive
* Description 	: function receives LIN0-UART data.
* Arguments 	: rxbuf: receive buffer pointer
*             	  rxnum: buffer size
* Return Value 	: MD_OK
*                 MD_ARGERROR
******************************************************************************/
MD_STATUS R_UART_Receive(u8 *rxbuf, u8 rxnum)
{
	MD_STATUS status = MD_OK;

	if (rxnum < 1U){
		status = MD_ARGERROR;
	}else{
		g_UartRxCnt 		= 0U;
		g_UartRxLen 		= rxnum;
		g_UartRxAddress 	= rxbuf;
	}
	
	return (status);
}
/******************************************************************************
End of function R_UART_Receive
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Send
* Description 	: function transmit LIN0-UART data.
* Arguments 	: txbuf: transfer buffer pointer
*             	  txnum: buffer size
* Return Value 	: MD_OK
*                 MD_ARGERROR
******************************************************************************/
MD_STATUS R_UART_Send(u8 *txbuf, u8 txnum)
{
	MD_STATUS status = MD_OK;
	
	if(txnum < 1U){
		status = MD_ARGERROR;
	}else{
		g_UartTxCnt 	= txnum;
		g_UartTxAddress	= txbuf;
		LIN0TRMMK 		= 1U;	/* disable INTLIN0TRM interrupt */
		LUTDR0     		= *g_UartTxAddress;
		g_UartTxAddress++;
		g_UartTxCnt--;
		LIN0TRMMK 		= 0U;	/* enable INTLIN0TRM interrupt */
	}
	
	return (status);
}

/******************************************************************************
End of function R_UART_Send
******************************************************************************/
