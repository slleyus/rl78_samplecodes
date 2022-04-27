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
* File Name    : r_cg_adc_user.c
* Version      : CodeGenerator for RL78/G14 V2.04.04.03 [19 Aug 2016]
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ADC module.
* Creation Date: 3/15/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"
#include "utility.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_adc_interrupt(vect=INTAD)



/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

uint16_t g_adc_sum_16;				/*sum of 16 consecutive ADC 10bit conversions */
uint16_t g_adc_sum_16_new;			/*temp storage of g_adc_sum_16 */
uint8_t g_adc_countdown16;			/*counts down for 16 sums of 16 consecutive ADC conversions */
uint8_t g_conversion_done_flag;		/*Set to one on each ADC conversion interrupt */
uint16_t g_adc_16_data[16];			/*using for running sum, last 16 bins, each bin is sum of 16 ADC conversions */
uint32_t adc_256_sum;				/*sum of last 256 (sum of g_adc_16_data elements 1-16) ADC data conversions on ANI14, total max is 18bits */
uint16_t g_adc_256sum_avg;			/*average of last 256 ADC conversion, scaled down to 12bits */
uint16_t g_adc_256sum_avg_old;
uint8_t g_adc_countdown16_flag;		/*flag set when each set of 16 ADC conversions is obtained */
uint16_t g_adc_sum_16_latest;		/*saves latest sum of 16 consecutive ADC conversions so new sum can proceed */

uint8_t g_adc_sum16_count;          /*one adc_sum16_count per 16 ADC conversion sums */

extern uint8_t g_dither_change_wait;   	/*from r_cg_timer_user.c */
extern uint8_t g_uart_tx_data[5];		/*from pwmout_test_to_adc_input.c */
extern uint8_t g_closed_loop; 			/*from r_main.c */


void adc_256pwm_running_avg(void);
void initialize_adc16_data_buffer(uint16_t volatile target_dc_value);
void adc_16_sample(void);

void pwm_dithering_processing_and_print(uint8_t enable_print);

extern void dither_difference_adjust(void);			/*from pwm_dithering.c */
extern MD_STATUS R_UART1_Send(uint8_t * const tx_buf, uint16_t tx_num);   /*from r_cg_serial.c */



/***********************************************************************************************************************
* Function Name: r_adc_interrupt
* Description  : This function is INTAD interrupt service routine and sums 16 consecutive 10bit ADC conversions
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_adc_interrupt(void)
{

	g_adc_sum_16 += (ADCR>>6);
	g_adc_countdown16 --;		/*starts at 16 and counts down by one on every ADC conversion */
	if (g_adc_countdown16 == 0)
		{
		g_adc_countdown16_flag = 1;
		g_adc_sum_16_latest = g_adc_sum_16;   /*save latest sum of 16 consecutive ADC conversions */
		g_adc_sum_16 = 0;					/* zero out sum for next 16 consecutive ADC conversions */
		}
	g_conversion_done_flag = 1;
	P8_bit.no5 ^= 1U;		/*P8.5 used to monitor constant 61.04uSEC ADC conversion completion interrupts */

} /* End of function r_adc_interrupt() */



/***********************************************************************************************************************
* Function Name: initialize_adc16_data_buffer
* Description  : This function copies target_pwm12_value (g_target_pwm12_value) scaled up by factor of 4 into all
* 				 16 elements of g_adc_16_data buffer.  This buffer is used to find 256 consecutive ADC convers values
* 				 from the PWM dithering RC filter output
* Arguments    : target_dc_value (g_target_dc_value)
* Return Value : None
***********************************************************************************************************************/
void initialize_adc16_data_buffer(uint16_t volatile target_dc_value)
{

	uint8_t k1;
	uint16_t scaled_target_dc_value;
	scaled_target_dc_value = target_dc_value << 2;    /*scale 12bit value to 14bits, equivalent to 10bit (ADC) + 4bits (16 samples) */

	g_adc_256sum_avg =  target_dc_value;

	for (k1 = 0; k1< 16; k1++)
	{
		g_adc_16_data[k1] = scaled_target_dc_value;  /*initialize data buffer elements 1-15 (element 0 is always written from element 1) */
	}

} /* End of function initialize_adc16_data_buffer() */




/***********************************************************************************************************************
* Function Name: adc_256pwm_running_avg
* Description  : This function deletes the oldest ADC sum of 16samples, adds in the newest, and sums all 256 ADC samples.
* 				This 18bit value is scaled down by 6 bits and rounded to make a 12bit effective pwm comparison value.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void adc_256pwm_running_avg(void)
{

	uint8_t k1;
	adc_256_sum = 0;


	for (k1 = 0; k1< 15; k1++)
	{
		g_adc_16_data[k1] = g_adc_16_data[k1 + 1];  /*shift last 15 ADC summed array values 1-15  into array items 0-14 */
													/*each array value is sum of 16 ADC results */
		adc_256_sum = adc_256_sum + g_adc_16_data[k1]; /*sum the 15 array sums */
	}

	g_adc_16_data[15] = g_adc_sum_16_new;  			/*put newest 16 ADC sum into array item 16 */
	adc_256_sum = adc_256_sum + g_adc_16_data[15];   /* max adc_256sum is 256 x 1023 = 261888decimal = 3FF00 hex (18bit range) */

	g_adc_256sum_avg = (uint16_t) (adc_256_sum >> 5);  /*shift right 5 bits to scale to 13bit result */

	if ((g_adc_256sum_avg && 0x0001) == 1)   /*check for lsb = 1 */
	{
		g_adc_256sum_avg++;       /*if lsb = 1, then round up */
	}

	g_adc_256sum_avg = g_adc_256sum_avg >>1;    /*shift right once more to yield 12bit result */

	NOP();


} /* End of function adc_256pwm_running_avg() */




/***********************************************************************************************************************
* Function Name: pwm_dithering_processing_and_print
* Description  : This function procsesses the pwm dithering RC filtered output to monitor the 12bit effective pwm value
* 				and print (if enable_print = 1) 12bit value on UART1 every 9.766mSEC
* Arguments    : enable_print
* Return Value : None
***********************************************************************************************************************/
void pwm_dithering_processing_and_print(uint8_t enable_print)
{

	volatile MD_STATUS    status = 0U;        /* UART Status */

	g_adc_sum_16_new = g_adc_sum_16_latest;        /*Save temp copy of g_adc_sum_16, in case next adc conversion interrupt occurs */
	g_adc_countdown16 = 16;     	/*reset counter for 16 more ADC samples */
	g_adc_countdown16_flag = 0;   	/*clear flag for next 16 consecutive ADC samples */

	P8_bit.no6 ^= 1U;			/*used to monitor constant adc_16_sample (16 ADC samples) at 102.4HZ (9.766mSEC) rate */

	adc_256pwm_running_avg();
	g_adc_256sum_avg_old = g_adc_256sum_avg;

	g_adc_sum16_count++;

	if (g_adc_sum16_count == 16)
		{

		/*start of section used for closed loop with ADC feedback */
		if (g_closed_loop)
			{
			if (g_dither_change_wait > 0)
				{
				dither_difference_adjust();
				}
			else
				{
				g_dither_change_wait--;
				}
			}
		/*end of section used for closed loop */
		}



	if (enable_print)
	{
	bit12_to_ascii(g_adc_256sum_avg_old, g_adc_sum16_count);
	status = R_UART1_Send(g_uart_tx_data, (uint16_t) 4u);		/*print out 3 data characters + either commma or line feed character */
	}

	if (g_adc_sum16_count == 16)
		{
		g_adc_sum16_count = 0;
		}

} /* End of function pwm_dithering_processing_and_print() */

