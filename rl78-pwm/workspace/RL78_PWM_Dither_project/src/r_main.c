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
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G14 V2.04.04.03 [19 Aug 2016]
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 3/15/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_it.h"
#include "r_cg_dtc.h"
#include "pwm_dithering.h"
#include "pwmout_to_input_adc.h"
#include "iodefine.h"
#include "r_cg_userdefine.h"


/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/

#pragma address uart_tx_data = 0xF9F00U
#pragma address pwm_snd_data = 0x0FE000U

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

uint16_t __near pwm_snd_data[16];   /* Send data for DTC RAM to PWM TDR01 compare register */
uint16_t g_target_pwm12_value;
uint8_t g_dither_value_no;
uint8_t g_current_pwm_base_value;   /*upper 8bits of 12bit target pwm value */
uint8_t g_current_dither_value;		/*lower 4bits of 12bit target pwm value */
uint8_t g_closed_loop;
uint8_t g_pwmout_test_flag;


extern uint8_t g_adc_countdown16_flag;	/*from r_cg_adc_user.c */

void pwmout_test_to_adc_input(void);
void set_target_dithering_value(uint16_t target_pwm12_value);
void pwm_dithering_start(uint16_t target_pwm12_value, uint8_t enable_print);

extern void pwmout_test_to_adc_input(void);	/*in pwmout_to_input_adc.c*/
extern void pwm_dithering_processing_and_print(uint8_t enable_print);
extern void pwm_dtc_modulation_test(void); /* for pwm dtc test */


void R_MAIN_UserInit(uint16_t target_pwm12_value);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
	uint8_t enable_print;
	uint8_t pwmout_dtc_test_flag;  /*  for pwm dtc test */
	uint16_t target_pwm12_value;
	uint8_t k1 = 0;
	uint16_t n1 = 0;


	/* */
	target_pwm12_value = 0x0D1F; 	/* set target pwm 12bit effective value here, with 16bit unsigned integer in range of 0x0000 to 0x0FF0 inclusive */
	/* */


	R_MAIN_UserInit(target_pwm12_value);

    PM2 = 0xFF;
    PM15 = 0xFF;
	ADCEN = 1U;
	ADM2 = 0x60;
	PM8 = 0x00;  /*Make Port8 all outputs */
    PM1_bit.no0 = 0U;

	for (n1 = 0; n1 < 65535; n1++)  /*wait for more than 50mSEC */
		{
			for (k1 = 0; k1<32; k1++)
			{
				NOP();
			}
		}


	g_pwmout_test_flag = 0;					/*to test all 8-bit pwm codes into 10-bit ADC, set flag to 1 */
	pwmout_dtc_test_flag = 0; 				/*to test 16 different pwm codes using DTC, set flag to 1, output PWM duty cycles from  */


/*PWMout test to ADC input, without pwm dithering*/
	if (g_pwmout_test_flag)
	{
		pwmout_test_to_adc_input();    /*stays in test function forever after scan of all 8bit pwm output values */
	}
/* end pwmout test check*/

/*PWMout using DTC modulation test, without pwm dithering*/
	if (pwmout_dtc_test_flag)
	{
	pwm_dtc_modulation_test();
	}
/* end PWM out using DTC modulation test check*/



	g_closed_loop = 1U;		/*for Closed loop with ADC feedback and dithering correction, set g_closed_loop = 1U.  For open loop (no dithering
								correction, set g_closed_loop = 0U.  */
	enable_print =  1U;		/*set to 1U enables printing on UART1. To disable printing of pwm Dithering monitor, set to 0U. */


	set_target_dithering_value(target_pwm12_value);
	pwm_dithering_start(target_pwm12_value, enable_print);

	for (n1 = 0; n1 < 65535; n1++)  /*wait for more than 50mSEC */
		{
			;
		}


    while (1U)				/*main processing loop for pwm dithering and User code */
    {
    	/* */
		/* put main user code flow here */
    	/* */

		if(g_adc_countdown16_flag)		/*Execute pwm_dithering when flag = 1, which means 16 consecutive ADC conversion are done (every 9.766mSEC) */
			{
				pwm_dithering_processing_and_print(enable_print); /*background code to update pwm dither values, must be completed within next
																	9.766mSEC (takes 30-40uSEC to complete @32MHZ) */
			}

    }

} /* End of main() */


/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(uint16_t target_pwm12_value)
{

	R_UART1_Start();				/*Start UART */
	R_ADC_Set_OperationOn();		/*Turn on ADC comparator enable (draws 75uA continuously) */
    R_IT_Start();					/*Start 12bit Interval timer at 610.4uSEC (1638.4HZ) interrupt rate for ADC Hardware trigger mode */


    set_target_dithering_value(target_pwm12_value);  /*set up 16 words of DTC values for pwm dithering */

	/*DTC enabled */
    R_DTCD0_Start();				/*Start DTC transfers (16 words to PWM TDR01 compare register, repeating every 256uSEC) */


	/*TAU00 enabled */
    R_TAU0_Channel0_Start();		/*Start TAU0, channel0 constant interrupt rate of 16uSEC (used for DTC transfers to update dithered PWM duty cycle ever PWM period) */
    EI();

} /* End of function R_MAIN_UserInit() */


