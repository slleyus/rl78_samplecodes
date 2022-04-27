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
* Copyright (C) 2010, 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_rtc.h
* Version      : Applilet3 for RL78/G13 V1.03.00 [15 Sep 2011]
* Device(s)    : R5F101LE
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for RTC module.
* Creation Date: 12/16/2011
***********************************************************************************************************************/

#ifndef RTC_H
#define RTC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Peripheral enable register 0 (PER0)
*/
/* Control of real-time clock (RTC) input clock (RTCEN) */
#define _00_RTC_CLOCK_STOP            (0x00U)    /* stops supply of input clock */
#define _01_RTC_CLOCK_SUPPLY          (0x01U)    /* supplies input clock */

/*
    Real-time clock register 0 (RTCC0)
*/
/* Real-time clock operation control (RTCE) */
#define _00_RTC_COUNTER_STOP          (0x00U)    /* stops clock operation */
#define _80_RTC_COUNTER_START         (0x80U)    /* starts clock operation */
/* Selection of 12-/24-hour system (AMPM) */
#define _08_RTC_HOURSYSTEM_CLEAR      (0x08U)    /* clear the AMPM bit */
#define _08_RTC_RTCC0_AMPM            (0x08U)    /* AMPM bit status detect */
#define _00_RTC_12HOUR_SYSTEM         (0x00U)    /* 12-hour system (a.m. and p.m.. are displayed.) */
#define _08_RTC_24HOUR_SYSTEM         (0x08U)    /* 24-hour system */
/* Interrupt (INTRTC) selection (CT2 - CT0) */
#define _F8_RTC_INTRTC_CLEAR          (0xF8U)    /* clear constant-period interrupt function */
#define _00_RTC_INTRTC_NOT_GENERATE   (0x00U)    /* does not use constant-period interrupt function */
#define _01_RTC_INTRTC_CLOCK_0        (0x01U)    /* once per 0.5s */
#define _02_RTC_INTRTC_CLOCK_1        (0x02U)    /* once per 1 s */
#define _03_RTC_INTRTC_CLOCK_2        (0x03U)    /* once per 1 minute */
#define _04_RTC_INTRTC_CLOCK_3        (0x04U)    /* once per 1 hour */
#define _05_RTC_INTRTC_CLOCK_4        (0x05U)    /* once per 1 day */
#define _06_RTC_INTRTC_CLOCK_5        (0x06U)    /* once per 1 month */
/* RTC1HZ pin output control (CLOE1) */
#define _00_RTC_RTC1HZ_DISABLE        (0x00U)    /* disables output of RTC1HZ pin (1 kHz) */
#define _20_RTC_RTC1HZ_ENABLE         (0x20U)    /* enable output of RTC1HZ pin (1 kHz) */

/*
    Real-time clock control register 1 (RTCC1)
*/
/* Alarm operation control (WALE) */
#define _00_RTC_ALARM_DISABLE         (0x00U)    /* alarm match operation is invalid */
#define _80_RTC_ALARM_ENABLE          (0x80U)    /* alarm match operation is valid */
/* Control of alarm interrupt function operation (WALIE) */
#define _00_RTC_ALARM_INT_DISABLE     (0x00U)    /* does not generate interrupt on matching of alarm */
#define _40_RTC_ALARM_INT_ENABLE      (0x40U)    /* generates interrupt on matching of alarm */
/* Alarm detection status flag (WAFG) */
#define _00_RTC_ALARM_MISMATCH        (0x00U)    /* alarm mismatch */
#define _10_RTC_ALARM_MATCH           (0x10U)    /* detection of matching of alarm */
/* Constant-period interrupt status flag (RIFG) */
#define _00_RTC_INTC_NOTGENERATE_FALG (0x00U)    /* constant-period interrupt is not generated */
#define _08_RTC_INTC_GENERATE_FLAG    (0x08U)    /* generated constant-period interrupt */
/* Wait status flag of real-time clock (RWST) */
#define _00_RTC_COUNTER_OPERATING     (0x00U)    /* clock is operating */
#define _02_RTC_COUNTER_STOP          (0x02U)    /* mode to read or write clock value */
/* Wait control of real-time clock (RWAIT) */
#define _00_RTC_COUNTER_SET           (0x00U)   /* sets clock operation */
#define _01_RTC_COUNTER_PAUSE         (0x01U)   /* stops SEC to YEAR counters. Mode to read or write counter value */

/*
    Watch error correction register (SUBCUD)
*/
/* Setting of watch error correction timing (DEV) */
#define _00_RTC_CORRECTION_EVERY20S   (0x00U)   /* corrects watch error when the second digits are at 00, 20, or 40 */
#define _80_RTC_CORRECTION_EVERY60S   (0x80U)   /* corrects watch error when the second digits are at 00 */
/* Setting of watch error correction method (F6) */
#define _00_RTC_CORRECTION_INCREASE   (0x00U)   /* increase */
#define _40_RTC_CORRECTION_DECREASE   (0x40U)   /* decrease */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define RTC_WAITTIME                  (320U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct 
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t week;
    uint8_t month;
    uint8_t year;
} rtc_counter_value_t;
typedef struct
{
    uint8_t alarmwm;
    uint8_t alarmwh;
    uint8_t alarmww;
} rtc_alarm_value_t;
typedef enum
{ 
    HOUR12,
    HOUR24
}rtc_hour_system_t;
typedef enum 
{
    HALFSEC = 1U,
    ONESEC,
    ONEMIN,
    ONEHOUR,
    ONEDAY,
    ONEMONTH
}rtc_int_period_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_RTC_Create(void);
void R_RTC_Start(void);
void R_RTC_Stop(void);
MD_STATUS R_RTC_Get_CounterValue(rtc_counter_value_t * const counter_read_val);
MD_STATUS R_RTC_Set_CounterValue(rtc_counter_value_t counter_write_val);
MD_STATUS R_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period);
void R_RTC_Set_ConstPeriodInterruptOff(void);
static void R_RTC_CallbackConstPeriod(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
