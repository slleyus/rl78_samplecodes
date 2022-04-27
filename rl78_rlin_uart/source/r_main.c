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
* File Name		: r_main.c
* Version      	: 1.0
* Device(s)    	: R5F10PMFK
* H/W Platform 	: QB-R5F10PMFK-TB
* Description  	: This file implements main function.
* Operation    	: 1. Compile and download the sample code. Click ÅeReset GoÅf
*                 to start the software.
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
#include "r_serial.h"

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
u8 g_UartRxBuf;			/* LIN0-UART0 receive buffer */
u8 g_UartRxError;		/* LIN0-UART0 receive error status */
MD_STATUS g_UartTxEnd;	/* LIN0-UART0 transmission end */

/******************************************************************************
Private global variables and functions
******************************************************************************/
static const u8 messageOK_0[6] = {"OK_0\r\n"};	/* Message for "OK_0" 			*/
static const u8 messageOK_1[6] = {"OK_1\r\n"};	/* Message for "OK_1" 			*/
static const u8 messageBE[4]   = {"BE\r\n"};	/* Bit     error message   		*/
static const u8 messageOE[4]   = {"OE\r\n"};	/* Overrun error message   		*/
static const u8 messageFE[4]   = {"FE\r\n"};	/* Framing error message   		*/
static const u8 messagePE[4]   = {"PE\r\n"};	/* Parity  error message   		*/
static const u8 messageEXB[5]  = {"EXB\r\n"};	/* Expansion bitdetect message 	*/

/******************************************************************************
* Function Name	: main
* Description 	: This function implements main function.
* Arguments 	: none
* Return Value 	: none
******************************************************************************/
void main(void)
{
	MD_STATUS	status;		/* UART Status */
	
	/* LIN0-UART0 receive buffer setting */	
	status = R_UART_Receive(&g_UartRxBuf, 1);

	/* Start the LIN0-UART0 Tx/Rx operation */
	R_UART_Start();
	
	while(1U){
		HALT();				/* Wait for INTLIN0RVC interrupt */
		/* Finish Reception and check result */
		LIN0RVCMK = 1U;		/* disable  INTLIN0RVC interrupt */
		LIN0STAMK = 1U;		/* disable  INTLIN0STA interrupt */
		
		if(g_UartRxError){		/* Check reception error */
			if(g_UartRxError & 0x01U){		/* Bit error */
				g_UartTxEnd = R_UART_Send(messageBE, 4);	/* Transmit "Bit error" */
				while(g_UartTxEnd == 0){}					/* Wait for final transmit */
			}
			if(g_UartRxError & 0x04U){		/* Overrun error */
				g_UartTxEnd = R_UART_Send(messageOE, 4);	/* Transmit "Overrun error" */
				while(g_UartTxEnd == 0){}					/* Wait for final transmit */
			}
			if(g_UartRxError & 0x08U){		/* Framing error */
				g_UartTxEnd = R_UART_Send(messageFE, 4);	/* Transmit "Framing error" */
				while(g_UartTxEnd == 0){}					/* Wait for final transmit */
			}
			if(g_UartRxError & 0x10U){		/* Expansion bit detect */
				g_UartTxEnd = R_UART_Send(messageEXB, 5);	/* Transmit "Expansion bit detect" */
				while(g_UartTxEnd == 0){}					/* Wait for final transmit */
			}
			if(g_UartRxError & 0x40U){		/* Parity error */
				g_UartTxEnd = R_UART_Send(messagePE, 4);	/* Transmit "Parity error" */
				while(g_UartTxEnd == 0){}					/* Wait for final transmit */
			}
		}else{				/* Normal reception */
			if(g_UartRxBuf == 'T'){			/* Reception data is 'T' */
				g_UartTxEnd = R_UART_Send(messageOK_0, 6);	/* Transmit "OK_0" */
			}else{							/* Received another character */
				g_UartTxEnd = R_UART_Send(messageOK_1, 6);	/* Transmit "OK_1" */
			}
			/* Wait for final transmit */
			while(g_UartTxEnd == 0){}
		}
		
		/* For next reception */
		
		/* LIN0-UART0 receive buffer settig */	
		status = R_UART_Receive(&g_UartRxBuf, 1);
		
		LIN0RVCMK = 0U;		/* enable  INTLIN0RVC interrupt */
		LIN0STAMK = 0U;		/* enable  INTLIN0STA interrupt */
	}
}

/******************************************************************************
End of function main
******************************************************************************/

