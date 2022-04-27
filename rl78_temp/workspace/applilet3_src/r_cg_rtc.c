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
* File Name    : r_cg_rtc.c
* Version      : Applilet3 for RL78/G13 V1.03.00 [15 Sep 2011]
* Device(s)    : R5F101LE
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for RTC module.
* Creation Date: 12/16/2011
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RTC_Create
* Description  : This function initializes the real-time clock module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Create(void)
{
    RTCEN = 1U;    /* supply RTC clock */
    RTCE = 0U;     /* disable RTC clock operation */
    RTCMK = 1U;    /* disable INTRTC interrupt */
    RTCIF = 0U;    /* clear INTRTC interrupt flag */
    /* Set INTRTC low priority */
    RTCPR1 = 1U;
    RTCPR0 = 1U;
    RTCC0 = _00_RTC_RTC1HZ_DISABLE | _00_RTC_12HOUR_SYSTEM | _02_RTC_INTRTC_CLOCK_1;
}

/***********************************************************************************************************************
* Function Name: R_RTC_Start
* Description  : This function enables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Start(void)
{
    RTCIF = 0U;    /* clear INTRTC interrupt flag */
    RTCMK = 0U;    /* enable INTRTC interrupt */
    RTCE = 1U;     /* enable RTC clock operation */
}

/***********************************************************************************************************************
* Function Name: R_RTC_Stop
* Description  : This function disables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Stop(void)
{
    RTCE = 0U;    /* disable RTC clock operation */
    RTCMK = 1U;   /* disable INTRTC interrupt */
    RTCIF = 0U;   /* clear INTRTC interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_RTC_Get_CounterValue
* Description  : This function reads the results of real-time clock and store them in the variables.
* Arguments    : counter_read_val -
*                    the current real-time clock value(BCD code)
* Return Value : status -
*                    MD_OK, MD_BUSY1 or MD_BUSY2
***********************************************************************************************************************/
MD_STATUS R_RTC_Get_CounterValue(rtc_counter_value_t * const counter_read_val)
{
    MD_STATUS status = MD_OK;
    uint32_t  w_count;
    
    RTCC1 |= _01_RTC_COUNTER_PAUSE;

    /* Wait 10 us */
    for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
    {
        NOP();
    }

    if (0U == RWST)
    {
        status = MD_BUSY1;
    }
    else
    {
        counter_read_val->sec = SEC;
        counter_read_val->min = MIN;
        counter_read_val->hour = HOUR;
        counter_read_val->week = WEEK;
        counter_read_val->day = DAY;
        counter_read_val->month = MONTH;
        counter_read_val->year = YEAR;

        RTCC1 &= (uint8_t)~_01_RTC_COUNTER_PAUSE;

        /* Wait 10 us */
        for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
        {
            NOP();
        }

        if (1U == RWST)
        {
            status = MD_BUSY2;
        }
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_CounterValue
* Description  : This function changes the real-time clock value.
* Arguments    : counter_write_val -
*                    the expected real-time clock value(BCD code)
* Return Value : status -
*                    MD_OK, MD_BUSY1 or MD_BUSY2
***********************************************************************************************************************/
MD_STATUS R_RTC_Set_CounterValue(rtc_counter_value_t counter_write_val)
{
    MD_STATUS status = MD_OK;
    uint32_t  w_count;
    
    RTCC1 |= _01_RTC_COUNTER_PAUSE;

    /* Wait 10 us */
    for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
    {
        NOP();
    }

    if (0U == RWST)
    {
        status = MD_BUSY1;
    }
    else
    {
        SEC = counter_write_val.sec;
        MIN = counter_write_val.min;
        HOUR = counter_write_val.hour;
        WEEK = counter_write_val.week;
        DAY = counter_write_val.day;
        MONTH = counter_write_val.month;
        YEAR = counter_write_val.year;
        RTCC1 &= (uint8_t)~_01_RTC_COUNTER_PAUSE;

        /* Wait 10 us */
        for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
        {
            NOP();
        }

        if (1U == RWST)
        {
            status = MD_BUSY2;
        }
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOn
* Description  : This function enables constant-period interrupt.
* Arguments    : period -
*                    the constant period of INTRTC
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period)
{
    MD_STATUS status = MD_OK;

    if ((period < HALFSEC) || (period > ONEMONTH))
    {
        status = MD_ARGERROR;
    }
    else
    {
        RTCMK = 1U;    /* disable INTRTC */
        RTCC0 = (RTCC0 & _F8_RTC_INTRTC_CLEAR) | period;
        RTCC1 &= (uint8_t)~_08_RTC_INTC_GENERATE_FLAG;
        RTCIF = 0U;    /* clear INTRTC interrupt flag */
        RTCMK = 0U;    /* enable INTRTC interrupt */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOff
* Description  : This function disables constant-period interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ConstPeriodInterruptOff(void)
{
    RTCC0 &= _F8_RTC_INTRTC_CLEAR;
    RTCC1 &= (uint8_t)~_08_RTC_INTC_GENERATE_FLAG;
    RTCIF = 0U;        /* clear INTRTC interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
