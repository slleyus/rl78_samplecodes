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
* File Name    : pwm_dithering.c
* Version      : CodeGenerator for RL78/G14 V2.04.04.03 [19 Aug 2016]
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file sets up the DTC for 16 values to be loaded into pwm TDR01 compare register
* Creation Date: 3/15/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
#include "r_cg_userdefine.h"
#include "r_cg_serial.h"
#include "r_cg_dtc.h"
#include "r_cg_timer.h"
#include "utility.h"


/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/



/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

uint8_t g_interim_dither_16[16];	/* for 8-bit pwm dither table before being converted to 16bit for TDR01 */
uint16_t g_pwm_interim_dither_16[16];
uint8_t g_adc_countdown256 = 16;

extern uint8_t g_current_pwm_base_value;   	/*upper 8bits of 12bit target pwm value, from r_main.c */
extern uint8_t g_current_dither_value;		/*lower 4bits of 12bit target pwm value,  from r_main.c */

extern uint16_t g_target_pwm12_value;	/*from r_main.c */
extern uint16_t g_adc_256sum_avg_old;	/*from r_cg_adc_user.c */
uint8_t g_adjust_pwm_value_flag;		/*from pwm_dithering.c */

extern uint8_t g_adc_countdown16;   	/*from r_cg_adc_user.c */
extern uint8_t g_adc_countdown16_flag;  /*from r_cg_adc_user.c */

extern uint8_t g_uart_tx_data[5];    	/*from pwmout_to_input_adc.c */
extern uint16_t __near pwm_snd_data[16];   	/*from r_main.c  */

extern uint8_t g_adc_sum16_count;    	/*one adc_sum16_count per 16 ADC conversion sums, from r_main.c */

void pwm_dithering_increase(void);
void pwm_dithering_decrease(void);
void pwm_dithering_zero(void);
void pwm_dithering_prepare(uint8_t pwm_base_value, uint8_t dither_value_no);
void pwm_dithering_copy(void);
void dither_difference_adjust(void);
void pwm_dithering_full_magnitude_test(uint8_t dither_value_no);
void set_target_dithering_value(uint16_t target_pwm12_value);
void pwm_dithering_start(uint16_t target_pwm12_value, uint8_t enable_print);
void pwm_dtc_modulation_test(void);

extern void R_IT_Stop(void);

/* this dither test table is used to create varying TAU0 PWM (TO01) output to test PWM with repeating 16cycle DTC updates */
const uint8_t pwm_dtc_test_table[16] = {0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8};

/* this dither table is used to add to an 8bit pwm base value, to create 16byte RAM entries used for DTC output pwm dithering */
const uint8_t pwm_dither_table[256] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
			0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
			0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
			0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01,
			0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01,
			0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
			0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
			0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};


/* this dither test table is used to create a full magnitude (0x00-0xFF) pwm output to test ripple amplitude on RC filter output */
const uint8_t pwm_dither_test_table[256] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
			0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
			0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
			0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
			0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
			0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF,
			0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
			0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF,
			0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF,
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


/***********************************************************************************************************************
* Function Name: pwm_dithering_increase
* Description  : This function operates on temporary pwm_dithering 16byte DTC RAM entry to increase the dither value.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void pwm_dithering_increase(void)
{
	if (g_current_pwm_base_value < 0xFF)		/*if pwm base value is already max, can't increase value  */
	{
		if (g_current_dither_value == 15U)
		{
			g_current_dither_value = 0; 		/*increment dither value from 15 to 0 */
			g_current_pwm_base_value++ ;		/*for pwm base values 0x00 to 0xFE, can be increased by one */
			pwm_dithering_zero();				/*set the dither entries to all the same pwm output, no dithering */
		}
		else
		{
			g_current_dither_value++ ;				/*only increase the dithering factor */
			pwm_dithering_prepare(g_current_pwm_base_value, g_current_dither_value);
		}
	}
	/* else if g_current_pwm_base_value == 0xFF, do nothing as 0x0FF0 is maximum 12bit effective pwm value */
} /* End of function pwm_dithering_increase() */



/***********************************************************************************************************************
* Function Name: pwm_dithering_decrease
* Description  : This function operates on temporary pwm_dithering 16byte DTC RAM entry to decrease the value.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void pwm_dithering_decrease(void)
{
	if (g_current_dither_value == 0U)
	{

		if (g_current_pwm_base_value > 0)      /* Can only go to lower pwm base value if previously was 0x01 to 0xFF.
		 	 	 	 	 	 	 	 	 	 	 Can't decrease if previous	pwm base value was 0x00 */
		{
			g_current_dither_value = 15;
			g_current_pwm_base_value-- ;		/*Decrease pwm base value */
		}
		pwm_dithering_prepare(g_current_pwm_base_value, g_current_dither_value);
	}
	else      /* if g_current_dither_value = 1 to 15 */
	{
		g_current_dither_value-- ;
		if (g_current_dither_value == 0)
			{
				pwm_dithering_zero();		/*set the dither entries to all the same pwm output, no dithering */
			}
		else
			{
				pwm_dithering_prepare(g_current_pwm_base_value, g_current_dither_value);
			}
	}

} /* End of function pwm_dithering_decrease() */



/***********************************************************************************************************************
* Function Name: pwm_dithering_zero
* Description  : This function operates on pwm_dithering table to zero out the table (base pwm value with no dithering).
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void pwm_dithering_zero(void)
{
	uint8_t k1;
	for (k1 = 0; k1< 16; k1++)
	{
		g_pwm_interim_dither_16[k1] = (uint16_t) g_current_pwm_base_value;  /*initialize 16bit data buffer elements 0-15 to pwm base value with no offset */
	}
} /* End of function pwm_dithering_zero() */



/***********************************************************************************************************************
* Function Name: pwm_dithering_prepare
* Description  : This function prepares a 16 entry set of values for dithering, This is a temporary version to be copied
* 				into DTC RAM later
* Arguments    : pwm_base_value and dither_value_no
* Return Value : None
***********************************************************************************************************************/
void pwm_dithering_prepare(uint8_t pwm_base_value, uint8_t dither_value_no)
{
	uint8_t n1;

	for (n1=0; n1<16; n1++)
		{
		g_pwm_interim_dither_16 [n1] = (uint16_t) (pwm_base_value + pwm_dither_table[(dither_value_no * 16) + n1]);   	/*changed to g_dither_value_no */
		}
} /* End of function pwm_dithering_prepare() */



/***********************************************************************************************************************
* Function Name: pwm_dithering_copy
* Description  : This function copies a 16 dither entry set to DTC RAM for dithering
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void pwm_dithering_copy(void)
{
	uint8_t n1 = 0;

	for (n1=0; n1<16; n1++)
		{
		pwm_snd_data[n1] = (uint16_t) g_pwm_interim_dither_16 [n1];   /*copy 16 bytes over to DTC RAM */
		}
} /* End of function pwm_dithering_copy() */



/***********************************************************************************************************************
* Function Name: dither_difference_adjust
* Description  : This function takes the difference between g_target_pwm12_value (target 12bit pwm dithering value) and
* 				g_adc_256sum_avg_old (most recent 12bit effective value measure by ADC in 256 samples, scaled down
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void dither_difference_adjust(void)
{
 	int16_t dither_diff;

	dither_diff = g_target_pwm12_value - g_adc_256sum_avg_old;
	if (dither_diff > 0)
		{
			pwm_dithering_increase();
			g_adjust_pwm_value_flag = 1;  /*flag set when there is difference between target pwm value and actual */
		}

	if (dither_diff < 0)
		{
			pwm_dithering_decrease();
			g_adjust_pwm_value_flag = 1;	/*flag set when there is difference between target pwm value and actual */
		}

	NOP();    /* if dither_diff = 0, then no change needed */
} /* End of function dither_difference_adjust() */




/***********************************************************************************************************************
* Function Name: pwm_dtc_modulation_test
* Description  : This function prepares a 16 entry set of values for DTC PWM modulation dithering test.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void pwm_dtc_modulation_test(void)  /* for PWM test */
{
	uint8_t n1;
	R_DTCD0_Stop();
	R_IT_Stop();
	R_ADC_Stop();

	for (n1=0; n1<16; n1++)
		{
		NOP();
		pwm_snd_data[n1] = (uint16_t) pwm_dtc_test_table[n1];   	/* 16-bit data for PWM */
		}


    R_DTCD0_Start();
    R_TAU0_Channel0_Start();


    while(1)
    {
    	P8_bit.no4 ^= 1U;           /*monitor while(1) */
    	NOP();
    }

} /* End of function pwm_dtc_modulation_test() */



/***********************************************************************************************************************
* Function Name: set_target_dithering_value
* Description  : This function initializes pwm base value and a 16 entry set of values for dithering
* Arguments    : target_pwm12_value
* Return Value : None
***********************************************************************************************************************/
void set_target_dithering_value(uint16_t target_pwm12_value)
{

	uint8_t n1 = 0;


/*Target pwm12 value is 12bits effective, but limited from 0x000 to 0xFF0 inclusive */
	if (target_pwm12_value > 0x0FF0)
	{
		target_pwm12_value = 0x0FF0;	/*Saturate value to 0xFF0 maximum */
	}

	g_target_pwm12_value = target_pwm12_value;
	initialize_adc16_data_buffer(g_target_pwm12_value);  	/*pre-fill adc sample buffers */

	g_current_dither_value = g_target_pwm12_value & 0x0F;	/*dither value is lowest 4 bits of target, 0-15 decimal */
	g_current_pwm_base_value = (uint8_t) (g_target_pwm12_value>>4);		/*pwm base value is 0x00 to 0xFF (0 to 255 decimal) */

	for (n1=0; n1<16; n1++)
		{
		pwm_snd_data [n1] = (uint16_t) (g_current_pwm_base_value + pwm_dither_table[(g_current_dither_value * 16) + n1]);      	/* 16-bit data for DTC buffer */
		/*write dither values to DTC 16 word buffer */
		}
} /* End of function set_target_dithering_value() */



/***********************************************************************************************************************
* Function Name: pwm_dithering_start
* Description  : This function starts the 10ADC conversion on PWM dithering output to RC filter input to ADC.  Also,
*				(if enable_print = 1), outputs the target_pwm12_value with Line Feed before 10-bit ADC measurement
*				values are captured
* Arguments    : target_pwm12_value and enable_print
* Return Value : None
***********************************************************************************************************************/
void pwm_dithering_start(uint16_t target_pwm12_value, uint8_t enable_print)
{

	volatile MD_STATUS    status = 0U;        /* UART Status */
	uint16_t n1;


	R_UART1_Start();						/*Start UART */
	ADS = _0A_AD_INPUT_CHANNEL_10;			/*RC filtered pwm output is on channel ANI10 */
	g_adc_countdown16 = 16;					/*Initialize for 16 consecutive ADC conversions */
	g_adc_countdown16_flag = 0;             /*flag set to 1 after 16 consecutive ADC conversions */
	g_adc_countdown256 = 16;

	R_ADC_Start();

	g_adc_sum16_count = 0;

	if (enable_print)
		{
		bit12_to_ascii(target_pwm12_value, 16);  /*Send out g_target_pwm12_value with Line Feed */

		status = R_UART1_Send(g_uart_tx_data, (uint16_t) 4u);       /*Send out target_pwm12_value (3 data characters), with Line feed on UART1 */
		}

	for (n1 = 0; n1 < 65535; n1++)  /*wait for more than 2mSEC */
		{
			;
		}
} /* End of function pwm_dithering_start() */
