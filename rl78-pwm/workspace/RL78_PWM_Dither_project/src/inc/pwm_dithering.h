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


/*
* pwm_dithering.h
 *
 */

#ifndef PWM_DITHERING_H_
#define PWM_DITHERING_H_

#define PWM_DITHER_MAX   	(0x0FF0)

#include "pwm_dithering.h"
#include "r_cg_macrodriver.h"

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void pwm_dithering_increase(void);
void pwm_dithering_decrease(void);
void pwm_dithering_zero(void);
void pwm_dithering_prepare(uint8_t dac_base_value, uint8_t dither_value_no);
void pwm_dithering_copy(void);
void dither_difference_adjust(void);
void pwm_dithering_full_magnitude_test(uint8_t dither_value_no);
void set_target_dithering_value(uint16_t target_dac12_value);
void pwm_dithering_start(uint16_t target_dac12_value, uint8_t enable_print);

void pwm_dtc_modulation_test(void);

void R_DTCD0_Start(void);

#endif /* PWM_DITHERING_H_ */
