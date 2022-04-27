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
* File Name	: utility.c
* Version 	: 1.00
* Device 	: R5F100LEAFB
* Tool Chain 	: RL Family C Compiler (IAR ICCRL78)
* H/W Platform	: RSKRL78G13
* Description   : Contains functions for converting an integer to decimal or 
*		  hexadecimal string.
*******************************************************************************/
/*******************************************************************************
* History 	: 21.06.2011 Ver. 1.00 First Release
*******************************************************************************/


/*******************************************************************************
User Includes (Project level includes)
*******************************************************************************/
/* Includes integer conversion function prototypes */
#include "utility.h"



/*******************************************************************************
System Definitions & Global Variables
*******************************************************************************/


/*******************************************************************************
* Function Name: uint16_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*		 which is inserted into the character array passed as an 
*		 argument.
* Argument     : uint8_t * string	- Pointer to array to store BCD string 
*		 uint8_t pos		- Array element to start BCD string from.
*		 uint16_t number	- 16bit integer to convert.
* Return value : none
* Note		   : No input validation is used, so output data can overflow
*		     the array passed.
*******************************************************************************/
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
		if((buffer_string[digit] != 0x30) | (!leading_zero))
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
/*******************************************************************************
* End of function uint16_ToBCDString
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_ToString
* Description  : Function converts a 16 bit integer into a character string, 
*		 inserts it into the array via the pointer passed at execution.
* Argument     : uint8_t* output_string : Pointer to uint8_t array that will hold 
*		 character string.
*		 uint8_t pos : uint8_t number, element number to begin inserting 
			       the character string from (offset).
*		 uint16_t input_number : 16 bit integer to convert into a string.
* Return value : none
* Note	       : No input validation is used, so output data can overflow the
*		 array passed.
*******************************************************************************/
void uint16_ToString(uint8_t* output_string, uint8_t pos, uint16_t input_number)
{
#if 0U

	/* Declare temporary charcter storage variable, and bit_shift variable */	
	uint8_t a = 0x00, bit_shift = 12u;	

	/* Declare 16bit mask variable */
	uint16_t mask = 0xF000;	

	/* Loop through until each hex digit is converted to an ASCII character */
	while(bit_shift < 30u)
	{
		/* Mask and shift the hex digit, and store in temporary variable, a */ 
		a = (uint8_t)((input_number & mask) >> bit_shift);	

		/* Convert the hex digit into an ASCII character, and store in output
		   string */
		output_string[pos] = (a < 0x0A) ? (a + 0x30) : (a + 0x37);		

		/* Shift the bit mask 4 bits to the right, to convert the next digit */
		mask = (mask >> 4u);

		/* Decrement the bit_shift counter by 4 (bits in a each digit) */
		bit_shift -= 4u;

		/* Increment the output string location */
		pos++;
	}
        
#endif   
        
        char revertedstr[9] ;
//        char str[9]; 
int length=0;
 
while (input_number > 0)
{
   int a = input_number % 10;
revertedstr[length++] = a | '0';
 input_number /= 10;
}
 
 length--;
 int rev = 0;
 while (length >= 0)
 {
   output_string[rev++] = revertedstr[length--];
 }
 
 output_string[rev] = '\0';
}
/*******************************************************************************
* End of function uint16_ToString
*******************************************************************************/
void Convert_16BitNumber_ToString(unsigned int c, char* buffer)
{
	char a;

	buffer[0] = '=';
	buffer[1] = 'H';
  	buffer[2] = '\'';
	a = (char)((c & 0x0F00)>> 8);
	buffer[3] = (a < 0x0A) ? (a+0x30):(a+0x37);
	a = (char)((c & 0x00F0)>> 4);
	buffer[4] = (a < 0x0A) ? (a+0x30):(a+0x37);
	a = (char)(c & 0x000F);
	buffer[5] = (a < 0x0A) ? (a+0x30):(a+0x37);

	/*	Add Spaces and a string terminating character.	*/
	buffer[6] = ' ';
	buffer[7] = ' ';
	buffer[8] = '\0';
}