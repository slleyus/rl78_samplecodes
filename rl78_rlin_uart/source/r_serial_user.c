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
* File Name 	: r_serial_user.c
* Version 		: 1.0
* Device(s) 	: R5F10PMFK
* H/W Platform 	: QB-R5F10PMFK-TB
* Description 	: This file implements device driver for Serial module.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 21.12.2012 1.00 First Release
******************************************************************************/

/*
*******************************************************************************
** Pragma directive
*******************************************************************************
*/
#pragma sfr
#pragma	interrupt	INTLIN0TRM	R_UART_Interrupt_Send
#pragma	interrupt	INTLIN0RVC	R_UART_Interrupt_Receive
#pragma	interrupt	INTLIN0STA	R_UART_Interrupt_Error

/******************************************************************************
Includes <System Includes> , gProject Includesh
******************************************************************************/
#include "r_macrodriver.h"
#include "r_serial.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern volatile u8 g_UartRxLen;			/* LIN0-UART0 receive data length */
extern volatile u8 g_UartRxCnt;			/* LIN0-UART0 receive data count */
extern volatile u8 *g_UartRxAddress;	/* LIN0-UART0 receive buffer address */
extern volatile u8 g_UartTxCnt;			/* LIN0-UART0 transmit data count */
extern volatile u8 *g_UartTxAddress;	/* LIN0-UART0 transmit buffer address */

extern u8 g_UartRxError;		/* LIN0-UART0 receive error status */
extern MD_STATUS g_UartTxEnd;	/* LIN0-UART0 transmission end */

/******************************************************************************
* Function Name	: L_UART0_Interrupt_Send
* Description 	: This function is INTLIN0TRM interrupt service routine.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
__interrupt void R_UART_Interrupt_Send(void)
{
	if(g_UartTxCnt > 0U){
		LUTDR0L = *g_UartTxAddress;
		g_UartTxAddress++;
		g_UartTxCnt--;
	}else{
		R_UART_Callback_SendEnd();
	}
}

/******************************************************************************
End of function R_UART_Interrupt_Send
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Interrupt_Receive
* Description 	: This function is INTLIN0RVC interrupt service routine.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
__interrupt void R_UART_Interrupt_Receive(void)
{
	u8 rx_data;
	
	rx_data = LURDR0L;
	if(g_UartRxLen > g_UartRxCnt){
		*g_UartRxAddress = rx_data;
		g_UartRxAddress++;
		g_UartRxCnt++;
		if(g_UartRxLen == g_UartRxCnt){
			R_UART_Callback_ReceiveEnd();
		}
	}
}

/******************************************************************************
End of function R_UART_Interrupt_Receive
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Interrupt_Error
* Description 	: This function is INTLIN0STA interrupt service routine.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
__interrupt void R_UART_Interrupt_Error(void)
{
	u8 error_type;
	
	*g_UartRxAddress = LURDR0L;
	error_type = LEST0 & 0x5DU;
	R_UART_Callback_Error(error_type);
	LEST0 = 0x00U;
}

/******************************************************************************
End of function R_UART_Interrupt_Error
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Callback_ReceiveEnd
* Description 	: This function is INTLIN0RVC interrupt service routine.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void R_UART_Callback_ReceiveEnd(void)
{
	g_UartRxError = 0U;		/* Clear error flag */
}
/******************************************************************************
End of function R_UART_Callback_ReceiveEnd
******************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Callback_SendEnd
* Description 	: This function is INTLIN0TRM interrupt service routine.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void R_UART_Callback_SendEnd(void)
{
	g_UartTxEnd = 1U;		/* Set transmission end flag */
}
/******************************************************************************
End of function R_UART_Callback_SendEnd
****d**************************************************************************/

/******************************************************************************
* Function Name	: R_UART_Callback_Error
* Description	: This function is a callback function when LIN0-UART reception error occurs.
* Arguments 	: err_type: occurred error type
* Return Value 	: none
******************************************************************************/
void R_UART_Callback_Error(u8 err_type)
{
	/* Get error type */
	g_UartRxError = err_type;
}
/******************************************************************************
End of function R_UART_Callback_Error
******************************************************************************/
