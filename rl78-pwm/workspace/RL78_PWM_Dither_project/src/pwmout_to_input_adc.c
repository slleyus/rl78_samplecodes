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
* File Name    : pwmout_to_input_adc.c
* Version      :
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file contains a function to measure all 256 output codes of 8-bit pwm into 10bit ADC and print
* 				resulting values (16 sums where each sum is 16 consecutive ADC conversions of filtered pwm output. Outputs
* 				16 values (value from 0x0000 to 0x3FF0 max) to UART1, in one line per pwm output value.
*
* Creation Date: April 2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/



/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "pwmout_to_input_adc.h"
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"
#include "r_cg_dtc.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Following header file provides common defines for widely used items. */
#include "rskrl78g14def.h"


/* Declare a variable for A/D results */
uint16_t gADC_Result = 0;

extern uint16_t g_adc_sum_16;			/*from r_cg_adc_user.c */
extern uint8_t g_adc_countdown16;		/*from r_cg_adc_user.c */
uint8_t g_uart_tx_data[5];				/*from pwmout_to_input_adc.c */
extern uint8_t g_conversion_done_flag;	/*from r_cg_adc_user.c */
extern uint16_t g_adc_sum_16_latest;	/*from r_cg_adc_user.c */

extern void R_UART1_Start(void);
extern MD_STATUS R_UART1_Send(uint8_t * const tx_buf, uint16_t tx_num);


/***********************************************************************************************************************
* Function Name: pwmout_test_to_adc_input
* Description  : This function goes thru all 256 pwm output combinations, performing 256ADC conversions per pwm output.
*				outputs 81 ASCII characters for each line of 16values of 16bits each (each value is sum of 16 ADC
*				conversions.)
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void pwmout_test_to_adc_input(void)
{

	uint16_t n1;
	uint16_t n2;
	uint8_t a;
	uint8_t volatile k1;
	uint16_t volatile pwm_index16 = 0;
	uint8_t volatile pwm_index8 = 0;
	uint8_t volatile scan_done = 0;

	volatile MD_STATUS    status = 0U;        /* UART Status */

	R_DTCD0_Stop();
	TDR01 = 0x0000;
	for (n1 = 0; n1 < 8000; n1++)  /*wait for 2mSEC or so @32MHZ */
		{

		for (n2 = 0; n2 < 100; n2++)  /*wait for 2mSEC or so @32MHZ */
				{
				NOP();
				}

		NOP();
		}

    R_UART1_Start();
	ADS = _0A_AD_INPUT_CHANNEL_10;    /*input for PWM RC filter output to ADC input */

   for (pwm_index16 = 0; pwm_index16 < 256; pwm_index16++)
   {

		NOP();
	    TDR01 = pwm_index16;

		g_adc_sum_16 = 0U;
		g_adc_countdown16 = 16;
		k1 = 0U;
		g_conversion_done_flag = 0;
		scan_done = 0;    /*checking for scan of all 256 values of 8-bit pwm */


		for (n1 = 0; n1 < 8000; n1++)  /*wait for 2mSEC or so @32MHZ */
			{

			for (n2 = 0; n2 < 10; n2++)  /*wait for 2mSEC or so @32MHZ */
					{
					NOP();
					}

			NOP();
			}

		R_ADC_Start();

		while (scan_done == 0)
		{
			while (g_conversion_done_flag == 0 )
			{
				;
			}

			g_conversion_done_flag = 0;
			if (g_adc_countdown16 == 0)
			{

				g_adc_countdown16 = 16;

				/* Convert ADC result into a character string, and store in the local */
				a = (uint8_t)((g_adc_sum_16_latest & 0xF000) >> 12);
				g_uart_tx_data[0] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
				a = (uint8_t)((g_adc_sum_16_latest & 0x0F00) >> 8);
				g_uart_tx_data[1] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
				a = (uint8_t)((g_adc_sum_16_latest & 0x00F0) >> 4);
				g_uart_tx_data[2] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
				a = (uint8_t)(g_adc_sum_16_latest & 0x000F);
				g_uart_tx_data[3] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));

				g_uart_tx_data[4] = 0x2C;  /*HEX code for ASCII character "," (comma) */

				g_adc_sum_16 = 0;
				k1 ++;

				if (k1 == 16)
				{
					k1 = 0;
					R_ADC_Stop();
					g_uart_tx_data[4] = 0x0A;  /*HEX code for ASCII character LF (Line Feed) */
					scan_done = 1;
				}
				status = R_UART1_Send(g_uart_tx_data, (uint16_t) 5u);		/*print out 4 data characters + either commma or line feed character */
			}

			NOP();

		}


		for (n1 = 0; n1 < 10000; n1++)  /*wait for 0.5mSEC or so @32MHZ */
			{
				NOP();
			}

    }

   while(1)
   {
	   ;    /*wait forever here */
   }
} /* End of function pwmout_test_to_adc_input() */




