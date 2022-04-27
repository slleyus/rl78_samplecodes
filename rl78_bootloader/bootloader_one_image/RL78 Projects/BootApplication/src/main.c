/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G14 V2.05.04.02 [20 Nov 2019]
* Device(s)    : R5F104ML
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 6/21/2020
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <r_cg_macrodriver.h>
#include <r_cg_userdefine.h>

#include "r_cg_cgc.h"
#include "bsp.h"
#include "r_cg_serial.h"
#include "utility.h"
#include "flash_interface.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//#define USE_LED_0

/* This section is to hold the version number */
#pragma section APP_VERSION
//__far const unsigned long version = 0x01010101;
__far const unsigned long version = 0x01020304;
//__far const unsigned long version = 0x02020202;
#pragma section

/* This sections is meant as a placeholder for the CRC, which is placed at the end of ROM */
#pragma section PROGRAM_CRC
__far const uint16_t program_crc;
#pragma section

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

uint8_t uart_rcv_buffer[UART_PACKET_LENGTH];
uint8_t uart_response[UART_RESPONSE_LENGTH];

extern volatile uint8_t uart_rcv_done;

void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma section text .BOOT
void main(void)
{
	e_packet_handle_t packet_type;

    R_MAIN_UserInit();

    /* Prime the UART to receive a packet */
    R_UART0_Receive(uart_rcv_buffer, UART_PACKET_LENGTH);

    /* Turn on LED for this version */
#ifdef USE_LED_0
    led_control(LED0, LED_ON);
#else
    led_control(LED1, LED_ON);
#endif

    while (1U)
    {
    	/* TODO: Create FULL Custom Application */

    	/*  */
    	if(uart_rcv_done)
    	{
    	    /* Prime the UART to receive a new packet */
    	    R_UART0_Receive(uart_rcv_buffer, UART_PACKET_LENGTH);
    	    /* Verify packet CRC */
			/* Run the CRC on the entire packet, including the crc. It should return a crc of 0 */
			if(general_crc(uart_rcv_buffer, UART_PACKET_LENGTH, CRC_ZERO_SEED) == 0)
			{
    			/* Get our packet type */
    			packet_type = get_packet_type(uart_rcv_buffer[0]);

    			if(packet_type == PACKET_VERSION)
    			{
    				/* Get the version */
    				get_version(uart_response);

					/* Send the current version */
					send_response(COMMAND_SEND_VERSION, uart_response);
    			}
    			if(packet_type == PACKET_SWAP_AND_RESET)
    			{
    				flash_boot_swap_reset();
    			}
			}
		}
    }

}

#pragma section

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
	led_init();
	init_fcl();
	R_UART0_Start();

    EI();
}

