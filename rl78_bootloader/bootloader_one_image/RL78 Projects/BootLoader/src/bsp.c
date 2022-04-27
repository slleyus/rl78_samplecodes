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
* File Name    : bsp.c
* Version      : CodeGenerator for RL78/G14 V2.05.04.02 [20 Nov 2019]
* Device(s)    : R5F104ML
* Tool-Chain   : CCRL
* Description  : This file implements device driver for INTC module.
* Creation Date: 6/21/2020
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <r_cg_macrodriver.h>
#include <r_cg_userdefine.h>
#include "bsp.h"


/***********************************************************************************************************************
* Function Name: led_init
* Description  : This function enables the operation of LED0 and LED1 on the user board.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void led_init(void)
{
	PM4 &= PM_43_44_MODE_OUTPUT;

	led_control(LED0, LED_OFF);
	led_control(LED1, LED_OFF);
}

/***********************************************************************************************************************
* Function Name: led_control
* Description  : This function enables the control of LED0 and LED1 on the user board.
* Arguments    : leds_t led
* 				 led_state_t state
* Return Value : None
***********************************************************************************************************************/
void led_control(leds_t led, led_state_t state)
{
	if(led == LED0)
	{
		switch (state)
		{
		    case LED_ON:
			    P4 &= ~LED0_PIN;
			    break;

		    case LED_OFF:
				P4 |= LED0_PIN;
		    	break;

		    case LED_TOGGLE:
		    	P4 ^= LED0_PIN;
		    	break;

		    default:
		    	break;
		}
	}
	else
	{
		switch (state)
		{
		    case LED_ON:
			    P4 &= ~LED1_PIN;
			    break;

		    case LED_OFF:
				P4 |= LED1_PIN;
		    	break;

		    case LED_TOGGLE:
		    	P4 ^= LED1_PIN;
		    	break;

		    default:
		    	break;
		}
	}
}


