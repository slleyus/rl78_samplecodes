/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2011, 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G14 V2.04.04.03 [19 Aug 2016]
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for TAU module.
* Creation Date: 3/15/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
#include "r_cg_dtc.h"
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_tau0_channel0_interrupt(vect=INTTM00)


/***********************************************************************************************************************
Global variables and functions

***********************************************************************************************************************/

uint8_t g_dither_change_wait;

extern uint8_t g_pwmout_test_flag;		/*from r_main.c */
extern uint8_t g_adjust_pwm_value_flag;	/*from pwm_dithering.c */
extern uint8_t g_closed_loop; 			/*from r_main.c */

extern st_dtc_data dtc_controldata_0;

extern void pwm_dithering_copy(void);


/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel0_interrupt(void)
{

	P8_bit.no7 ^= 1U;           /*P8.7 used to monitor constant 256uSEC DTC, 16 byte transfers completed */

	/*start of section used for closed loop */

	if (g_closed_loop)
		{
		if (g_adjust_pwm_value_flag == 1)		/*close the loop, adjust the pwm dither to reduce offset error */
			{
				pwm_dithering_copy();				/*if pwm dithering value needs updating, do it now at end of 16 DTC cycles */
				g_adjust_pwm_value_flag = 0;		/*clear flag */
				g_dither_change_wait = 1U;
			}
		}
	/*end of section used for closed loop */


	if (1 != g_pwmout_test_flag)				/*if pwm output to ADC input test is running (no dithering), keep DTC disabled */
	{

	    /* don't re-enable dtc if using pwmout_test */
		dtc_controldata_0.dtcct = _10_DTCD0_TRANSFER_BYTE;
	    dtc_controldata_0.dtsar = _E000_DTCD0_SRC_ADDRESS;
	    DTCEN2 |= _10_DTC_TAU00_ACTIVATION_ENABLE;


	}



}


