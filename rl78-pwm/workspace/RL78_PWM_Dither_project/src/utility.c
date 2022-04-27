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
* File Name		: utility.c
* Device(s)    	: R5F104PJ
* Tool-Chain   	: GNURL78 v12.01
* H/W Platform	: RSKRL78G14
* Description   : Contains functions for converting an integer to decimal or hexadecimal string.
* Creation Date	: 02/05/2012
***********************************************************************************************************************/

/***********************************************************************************************************************
User Includes (Project level includes)
***********************************************************************************************************************/
/* Includes integer conversion function prototypes */
#include "utility.h"


/***********************************************************************************************************************
* Function Name: uint16_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*		 		 which is inserted into the character array passed as an
*		 		 argument.
* Argument     : uint8_t * string	- Pointer to array to store BCD string 
*		 		 uint8_t pos		- Array element to start BCD string from.
*		 		 uint16_t number	- 16bit integer to convert.
* Return value : none
* Note		   : No input validation is used, so output data can overflow
*		     	 the array passed.
***********************************************************************************************************************/
void uint16_ToBCDString(uint8_t * output_string, uint8_t pos, uint16_t number)
{
	/* Local digit position variable */
	uint8_t digit = 5u;
	
	/* Digit is a leading zero indicator variable */
	uint8_t leading_zero = 1;
	
	/* Local string buffer variable */
	uint8_t buffer_string[] = "00000";
	
	/* Decrement digit each iteration, until digit is zero */
	while(digit--)
	{
		/* Write BCD value to string element */
		buffer_string[digit] = (uint8_t) (0x30 + (number % 10));
		
		/* Move to the next decimal digit */
		number /= 10u;
	}
	
	/* Set digit position to zero */
	digit = 0u;
	
	/* Loop through each digit, and replace leading zeros with a space
	   character */
	while(digit < 5u)
	{
		/* Check if digit is not a zero character, or if leading_zero indicator is
		   0 */
		if((buffer_string[digit] != 0x30) || (!leading_zero))
		{
			/* Copy digit from buffer string to output string */
			output_string[digit + pos] = buffer_string[digit];
			
			/* Set the leading zero indicator variable to 0 */
			leading_zero = 0;	
		}
		
		/* Increment the loop counter */
		digit++;
	}
}
/***********************************************************************************************************************
* End of function uint16_ToBCDString
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: uint16_ToString
* Description  : Function converts a 16 bit integer into a character string, 
*		 		 inserts it into the array via the pointer passed at execution.
* Argument     : uint8_t* output_string : Pointer to uint8_t array that will hold 
*		 		 						  character string.
*		 		 uint8_t pos : uint8_t number, element number to begin inserting
			       	   	   	   the character string from (offset).
*		 		 uint16_t input_number : 16 bit integer to convert into a string.
* Return value : none
* Note	       : No input validation is used, so output data can overflow the
*		 		 array passed.
***********************************************************************************************************************/
void uint16_ToString(uint8_t* output_string, uint8_t pos, uint16_t input_number)
{

	/* Declare temporary character storage variable, and bit_shift variable */
	uint8_t a = 0x00;	
	uint8_t bit_shift = 12u;

	/* Declare 16bit mask variable */
	uint16_t mask = 0xF000;	

	/* Loop through until each hex digit is converted to an ASCII character */
	while(bit_shift < 30u)
	{
		/* Mask and shift the hex digit, and store in temporary variable, a */ 
		a = (uint8_t)((input_number & mask) >> bit_shift);	

		/* Convert the hex digit into an ASCII character, and store in output
		   string */
		output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

		/* Shift the bit mask 4 bits to the right, to convert the next digit */
		mask = (uint16_t)(mask >> 4u);

		/* Decrement the bit_shift counter by 4 (bits in a each digit)
		   (By default the GNURL78 compiler assigns macros and magic number defines as
	        signed integer variable and outputs a warning when converting between different
	        types. The result of this operation is intended and a warning expected)*/
		bit_shift -= (uint8_t) 4u;

		/* Increment the output string location */
		pos++;
	}
}
/***********************************************************************************************************************
* End of function uint16_ToString

***********************************************************************************************************************/



/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "iodefine.h"

extern uint8_t g_uart_tx_data[5];   /*from pwmout_to_input_adc.c */

/***********************************************************************************************************************
* Function Name: bit12_to_ascii
* Description  : This function produces 4 ASCII characters consisting of 3digit hex number from 12bit binary + a comma
* 				character (each value is sum of 16 consecutive 10bit ADC conversions.  After 16 sets of data, adds a
* 				line feed.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void bit12_to_ascii(uint16_t bit12_value, uint8_t data_number)      /*prepares 4 ASCII Characters (3 for data, and 4th is either Comma or Line Feed character) */
{
		uint8_t a;

		if (bit12_value > 0x0FFF)		/*bit12_value should NOT be > 0x0FFF */
		{
			bit12_value = 0x0FFF;  		/* saturate value at 12bits maximum */
		}

		a = (uint8_t)((bit12_value & 0x0F00) >> 8);
		g_uart_tx_data[0] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
		a = (uint8_t)((bit12_value & 0x00F0) >> 4);
		g_uart_tx_data[1] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
		a = (uint8_t)(bit12_value & 0x000F);
		g_uart_tx_data[2] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));

		if (data_number == 16)
			{
			g_uart_tx_data[3] = 0x0A;  /*if the data is 16th value, add HEX code for ASCII character LF (Line Feed) */


			}
		else
			{
			g_uart_tx_data[3] = 0x2C;  /*if data number is 01-15, add HEX code for ASCII character "," (comma) */
			NOP();
			}

}

