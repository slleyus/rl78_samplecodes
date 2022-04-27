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
Global variables and functions
***********************************************************************************************************************/
uint8_t uart_rcv_buffer[UART_PACKET_LENGTH];
uint8_t uart_response[UART_RESPONSE_LENGTH];

uint8_t flash_read_buffer[BYTES_PER_WRITE];

uint32_t address;

uint16_t running_crc_from_read = 0;
uint16_t current_crc_from_read;

extern volatile uint8_t uart_rcv_done;


void R_MAIN_UserInit(void);
void process_packet(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
	uint8_t err;

    R_MAIN_UserInit();


    /* Erase the application area */
    err = flash_erase_app_area();

    /* If erasure failed */
    while(err != FSL_OK)
    {
    	/* Insert  */
    	;
    }

    address = 0x1000;

    /* Prime the UART to receive a packet */
    R_UART0_Receive(uart_rcv_buffer, UART_PACKET_LENGTH);

    /* Notify PC that we are ready to begin flashing */
    send_response(COMMAND_BEGIN_TRANSMISSION, uart_response);

    while (1U)
    {
    	if(uart_rcv_done)
    	{
    	    /* Prime the UART to receive a new packet */
    	    R_UART0_Receive(uart_rcv_buffer, UART_PACKET_LENGTH);

    	    /* Process the packet that was sent */
    	    process_packet();
    	}
    }
}



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

void process_packet(void)
{
	uint8_t err;
	e_packet_handle_t packet_type;

	/* Verify packet CRC */
	/* Run the CRC on the entire packet, including the crc. It should return a crc of 0 */
	if(general_crc(uart_rcv_buffer, UART_PACKET_LENGTH, CRC_ZERO_SEED) == CRC_OK)
	{
		/* Get our packet type */
		packet_type = get_packet_type(uart_rcv_buffer[0]);

		/* For each packet type:  */
		if(packet_type == PACKET_DATA)
		{
			/* Write next 64 bytes */
			if(flash_write_packet_data(address, &uart_rcv_buffer[3], BYTES_PER_WRITE) == FSL_OK)
			{
				/* Read back the data and CRC the bytes to ensure the write happened correctly */
				flash_read_packet_data(address, flash_read_buffer, BYTES_PER_WRITE);

				/* Calculate data and running CRCs from what was read from memory */
				current_crc_from_read = general_crc(flash_read_buffer, BYTES_PER_WRITE, CRC_ZERO_SEED);
				running_crc_from_read = general_crc(flash_read_buffer, BYTES_PER_WRITE, running_crc_from_read);


				/* Data packet number and CRC to appended to the ACK */
				uart_response[1] = uart_rcv_buffer[1];
				uart_response[2] = uart_rcv_buffer[2];
			    uart_response[3] = (uint8_t) (current_crc_from_read & 0xFF);
			    uart_response[4] = (uint8_t) ((current_crc_from_read >> 8) & 0xFF);

				/* ACK to server and increment address value */
				send_response(COMMAND_ACK_PACKET, uart_response);

				address += BYTES_PER_WRITE;
			}
			else
			{
				/* Transfer data packet number to the NAK transfer */
				uart_response[1] = uart_rcv_buffer[1];
				uart_response[2] = uart_rcv_buffer[2];

				/* ACK to server and increment address value */
				send_response(COMMAND_NAK_PACKET, uart_response);
			}
		}
		else if(packet_type == PACKET_TRANSFER_COMPLETE)
		{
			uint16_t image_crc;
	    	/* If the program download is done check the programs CRC */
		    image_crc = verify_application_crc();

			/* Data packet number and CRC to appended to the ACK */
		    uart_response[1] = (uint8_t) (image_crc & 0xFF);
		    uart_response[2] = (uint8_t) ((image_crc >> 8) & 0xFF);
		    uart_response[3] = (uint8_t) (running_crc_from_read & 0xFF);
		    uart_response[4] = (uint8_t) ((running_crc_from_read >> 8) & 0xFF);

		    if(image_crc == CRC_OK)
		    {
				/* ACK to server that program passed CRC */
				send_response(COMMAND_ACK_PROGRAM, uart_response);
		    }
		    else
		    {
		    	/* Reset address pointer to prepare for re-download */
		        address = 0x1000;

		        /* Erase faulty application */
				err = flash_erase_app_area();
				while(err != FSL_OK)
				{
					;
				}

				/* Reset our running CRC */
				running_crc_from_read = 0;

				/* NAK to server that program failed CRC */
				send_response(COMMAND_NAK_PROGRAM, uart_response);
		    }
		}
		else if(packet_type == PACKET_SWAP_AND_RESET)
		{
			flash_boot_swap_reset();
		}
	}
	else
	{
		/* Packet CRC failed */

		/* Transfer data packet number to the NAK transfer */
		uart_response[1] = uart_rcv_buffer[1];
		uart_response[2] = uart_rcv_buffer[2];

		/* ACK to server and increment address value */
		send_response(COMMAND_NAK_CRC, uart_response);
	}
}

