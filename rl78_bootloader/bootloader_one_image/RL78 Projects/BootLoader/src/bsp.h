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
* File Name    : bsp.h
* Version      : CodeGenerator for RL78/G14 V2.05.04.02 [20 Nov 2019]
* Device(s)    : R5F104ML
* Tool-Chain   : CCRL
* Description  : This file implements device driver for BSP module.
* Creation Date: 6/21/2020
***********************************************************************************************************************/

#ifndef BSP_H
#define BSP_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define KEY_WAITTIME                    (8U)      /* change the waiting time according to the system */

#define PORT4_OUTPUT	        (0x00U)
#define PM_43_44_MODE_OUTPUT    (0xE7U)

#define LED0_PIN				(0x08U)
#define LED1_PIN				(0x10U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
	LED0,
	LED1
} leds_t;

typedef enum
{
	LED_ON = 0,
	LED_OFF = 1,
	LED_TOGGLE
} led_state_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void led_init(void);
void led_control(leds_t led, led_state_t state);


#endif





















