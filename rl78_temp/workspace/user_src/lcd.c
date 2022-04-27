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
* File Name	: lcd.c
* Version 	: 1.00
* Device 	: R5F100LEAFB
* Tool Chain 	: RL Family C Compiler (IAR ICCRL78)
* H/W Platform	: RSKRL78G13
* Description   : LCD Module utility functions.
*		  Written for KS0066u compatible LCD Module.
*	    	  (8 characters by 2 lines)
*******************************************************************************/
/*******************************************************************************
* History 	: 21.06.2011 Ver. 1.00 First Release
*******************************************************************************/


/*******************************************************************************
User Includes (Project level includes)
*******************************************************************************/
/* Defines used within the project scope */
#include "rskrl78g13def.h"
/* Includes LCD defines */
#include "lcd.h"


/*******************************************************************************
Local Function Prototypes
*******************************************************************************/



/*******************************************************************************
* Function name	: InitialiseLCD
* Description 	: Initialises the LCD display. 
* Argument 	: none
* Return value  : none
*******************************************************************************/
void InitialiseDisplay(void)
{
	/* Power Up Delay for LCD Module */
	EN_PIN = SET_BIT_HIGH;
	DisplayDelay(7000);
	EN_PIN = SET_BIT_LOW;

	/* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
	to set to 4 bit mode. */
	/* Function Set */
	LCD_nibble_write(CTRL_WR,0x03);
	LCD_nibble_write(CTRL_WR,0x03);
	DisplayDelay(39);
 
	/* Configure display */
	LCD_nibble_write(CTRL_WR,0x03);
	LCD_nibble_write(CTRL_WR,0x02);
	LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
	LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
	DisplayDelay(39);

	/* Display ON/OFF control */
	LCD_write(CTRL_WR,LCD_CURSOR_OFF);
	DisplayDelay(39);

	/* Display Clear */
	LCD_write(CTRL_WR,LCD_CLEAR);
	DisplayDelay(1530);

	/* Entry Mode Set */
	LCD_write(CTRL_WR,0x06);
	LCD_write(CTRL_WR,LCD_HOME_L1);
}
/*******************************************************************************
End of function InitialiseDisplay
*******************************************************************************/

/*******************************************************************************
* Function name	: DisplayString
* Description 	: Clear the LCD.
* Argument  	: (uint8_t) position 
*		  (uint8_t *) string
* Return value  : none.
*******************************************************************************/
void DisplayString(uint8_t position, uint8_t * string)
{
	static uint8_t next_pos = 0xFF;

	/* Set line position if needed. We don't want to if we don't need 
	   to because LCD control operations take longer than LCD data
	   operations. */
	if( next_pos != position)
	{
		if(position < LCD_LINE2)
		{
			/* Display on Line 1 */
		  	LCD_write(CTRL_WR, ((int8_t)(LCD_HOME_L1 + position)));
		}
		else
		{
			/* Display on Line 2 */
		  	LCD_write(CTRL_WR, ((int8_t)((LCD_HOME_L2 + position) - LCD_LINE2)));
		}
		/* set position index to known value */
		next_pos = position;		
	}

	do
	{
		LCD_write(DATA_WR,*string++);
		
		/* increment position index */
		next_pos++;				
	} 
	while(*string);
}
/******************************************************************************
End of function DisplayString
******************************************************************************/

/******************************************************************************
* Function name	: LCD_write
* Description 	: This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*		  to specify the starting position.
*		  For example, to start at the 2nd position on line 1...
*			DisplayString(LCD_LINE1 + 1, "Hello") 
* Argument  	: (uint8_t)value - the value to write
*		  (uint8_t) data_or_ctrl - To write value 
*					as DATA or CONTROL
*						1 = DATA
*						0 = CONTROL
* Return value  : none
*******************************************************************************/
void LCD_write(uint8_t data_or_ctrl, int8_t value)
{
	/* Write upper nibble first */
	LCD_nibble_write(data_or_ctrl, (int8_t)(value & 0xF0) >> 4);
	
	/* Write lower nibble second */
	LCD_nibble_write(data_or_ctrl, (value & 0x0F));
}
/******************************************************************************
End of function LCD_write
******************************************************************************/

/*******************************************************************************
* Function name	: LCD_nibble_write
* Description 	: Writes data to display. Sends command to display. 
* Argument 	: (uint8_t) value - the value to write
*		  (uint8_t) data_or_ctrl - To write value  
*				    as DATA or CONTROL
*						1 = DATA
*						0 = CONTROL
* Return value  : none
*******************************************************************************/
void LCD_nibble_write(uint8_t data_or_ctrl, int8_t value)
{
	int8_t ucStore;
	
	/* Set Register Select pin high for Data */
	if (data_or_ctrl == DATA_WR)
	{
		RS_PIN = SET_BIT_HIGH;
	}
	else
	{
		RS_PIN = SET_BIT_LOW;
	}
	
	/* There must be 40ns between RS write and EN write */
  	DisplayDelay(1);					
	
  	/* EN enable chip (HIGH) */
	EN_PIN = SET_BIT_HIGH;
	
	/* Tiny delay */  			
  	DisplayDelay(1);
	
	/* Clear port bits used */	
	ucStore = DATA_PORT;
	ucStore &= (int8_t) ~DATA_PORT_MASK;
	
	/* OR in data */	
	ucStore |= (int8_t) ((value << DATA_PORT_SHIFT) & DATA_PORT_MASK );
	
	/* Write data to port */	
	DATA_PORT = ucStore;
	
	/* write delay while En High */	            
	DisplayDelay(20);
	
	/* Latch data by dropping EN */					
    	EN_PIN = SET_BIT_LOW;
	
	/* Data hold delay */				
	DisplayDelay(20);					
	
	if (data_or_ctrl == CTRL_WR)
	{
		/* Extra delay needed for control writes */
		DisplayDelay(0x7FF);
	}				
}
/*******************************************************************************
End of function LCD_nibble_write
*******************************************************************************/

/*******************************************************************************
* Function name	: DisplayDelay
* Description 	: Delay routine for LCD display.
* Argument 	: (uint32_t) units 
* Return value  : none
*******************************************************************************/
void DisplayDelay(uint32_t units)
{
	uint32_t counter = units * DELAY_TIMING;
	
	while(counter--)
	{
		/* Delay Loop	*/
		__no_operation();
	}
}
/*******************************************************************************
End of function DisplayDelay
*******************************************************************************/


