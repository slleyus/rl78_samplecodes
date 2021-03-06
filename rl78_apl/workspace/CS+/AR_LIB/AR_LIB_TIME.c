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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************
* File Name    : AR_LIB_TIME.c
* Version      : V1.00
* Device(s)    : R5F1000LE
* Tool-Chain   : CCRL
* Description  : This file implements timer function using TAU module.
* Creation Date: 2020/01/01
************************************************************************/

/***********************************************************************
Includes
************************************************************************/
#include "r_cg_macrodriver.h"
//#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "AR_LIB_TIME.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************
Pragma directive
************************************************************************/
#ifdef   CS_ENV
#pragma interrupt r_tau1_channel2_interrupt(vect=INTTM12,bank=RB2,enable=true)
#pragma interrupt r_tau1_channel3_interrupt(vect=INTTM13,bank=RB3,enable=true)
#endif
/***********************************************************************
Global variables and functions
************************************************************************/
volatile    uint32_t    g_millis = 0;   /* 32bit milli sec counter      */
volatile    uint32_t    g_millis_b = 0; /* 32bit milli sec buffer       */

volatile    uint32_t    g_micros;       /* 32bit micro sec counter      */
volatile    uint32_t    g_micros_b;     /* 32bit micro sec buffer       */

volatile    uint8_t     milli_f = 0;    /* milli sec. delay flag        */
volatile    uint8_t     micro_f = 0;    /* micro sec. delay flag        */

extern volatile uint8_t ie_flag;


void wait_timeup(void);                 /* wait for timeup              */

/***********************************************************************
* Function Name: micros
* Description  : This function get run time(micro second unit)
* Arguments    : none
* Return Value : run time by micro second
* Remark       : 
************************************************************************/
uint32_t micros(void)
{
    uint8_t flag;                       /* for interrupt request flag   */
    uint16_t work;                      /* for TM13 count data          */
    uint32_t time_work;                 /* for count data               */

/*--------------------------------------
    for exclusive control
--------------------------------------*/
    TMMK13 = 1;                         /* mask and disable INTTM13     */
    work = TCR13;                       /* read TM13 count data         */
    flag = TMIF13;                      /* read interrupt request flag  */
    time_work = g_micros;               /* read run time data           */

    TMMK13 = 0;                         /* enable INTTM13 again         */

    if (( flag != 0 ) && ( work >= 500 )) /* check TM13 is underflow    */
    {

/*--------------------------------------
    underflow occurs at TM13
--------------------------------------*/

        time_work += ( 2000UL - work ); /* adjust when underflow        */
    }
    else
    {

/*--------------------------------------
    no underflow
--------------------------------------*/

        time_work += ( 1000UL - work );
    }

    return ( time_work );               /* return with runtime (micros) */

}

/***********************************************************************
* Function Name: millis
* Description  : This function get run time(milli second unit)
* Arguments    : none
* Return Value : run time by milli second
* Remark       : 
************************************************************************/
uint32_t millis(void)
{
    return ( g_millis );                /* return with runtime(millis)  */
}

/***********************************************************************
* Function Name: delay
* Description  : This function wait for tha time( milli scond )
* Arguments    : wait time 
* Return Value : none
* Remark       : 
************************************************************************/
void delay(uint32_t time)
{

/*--------------------------------------
    prepare for delay count
--------------------------------------*/

    TMMK13 = 1;                         /* for exclusive control        */
    TDR12 = 1000 - TCR13;               /* set micros runtime to TM12   */
    g_millis_b = time;                  /* get wait time(milli sec.)    */
    milli_f = 1;                        /* set delay count flag         */
    TMMK13 = 0;                         /* enable INTTM13               */

/*======================================
    wait for specfied time
======================================*/

    wait_timeup();

/*--------------------------------------
    timeup!
--------------------------------------*/

}


/***********************************************************************
* Function Name: delayMicroseconds
* Description  : This function wait for tha time( micro scond )
* Arguments    : wait time 
* Return Value : none
* Remark       : 
************************************************************************/
void delayMicroseconds(uint32_t time)
{
    uint16_t time_micros;
    uint16_t work;
    uint32_t time_millis;

    time_millis = time / 1000UL;        /* get milli sec. value     */
    time_micros = ( time % 1000 );      /* get remaining micro sec. */

/*--------------------------------------
    check specified time
--------------------------------------*/

    if ( time <= 1 )
    {
        return;                         /* exit if delay less than 1us  */

    }

    if ( time_millis < 10 )
    {

/*--------------------------------------
    longer than 2 micro and less than 10000
--------------------------------------*/

        time_micros += TCR13;           /* get TM13 count data          */
        TDR12 = ( time_micros - 1 );    /* set TM12 count data          */
        micro_f = 1;                    /* set micro delay count flag   */
        TMIF12 = 0;                     /* clear INTTM12 interrupt flag */
        TMMK12 = 0;                     /* enable INTTM12 interrupt     */
        TS1 |= 0x04;                    /* start TM12(micro interval)   */
        while ( micro_f == 1 )
        {                               /* wait for delay time          */
            HALT();
        }
        
        TMMK12 = 1;                     /* disable INTTM12 interrupt    */
        return;

    }
    else
    {

/*--------------------------------------
    longer than 10000 micro sec.
--------------------------------------*/

        TMMK13 = 1;                     /* for exclusive control        */
        work = TCR13;                   /* read TM13 count data         */
        milli_f = 1;                    /* set milli delay count flag   */
        g_millis_b = ( time_millis - 1 );   /* set milli sec unit time  */
        if (( TMIF13 == 1 ) && ( work > 950 ))
        {

/*--------------------------------------
    adjust when interrupt conflict
--------------------------------------*/

            time_micros += 1000;
        }
        TMMK13 = 0;                     /* end of exclusive control     */
        time_micros += ( 999 - work );
        TDR12 = time_micros;            /* set micro sec unit time      */
        micro_f = 1;
        

/*--------------------------------------
    wait for delay time
--------------------------------------*/

        wait_timeup();                  /* wait timeup by HALT mode     */

    }
}

/***********************************************************************
* Function Name: wait_timeup
* Description  : This function wait for timeup
* Arguments    : none 
* Return Value : none
* Remark       : wait until both micro_f and milli_f become 0
************************************************************************/
void wait_timeup(void)
{

    while ( milli_f == 1 )
    {

/*--------------------------------------
    wait for timeup of milli seconds
--------------------------------------*/

        HALT();
    }

/*--------------------------------------
    wait for remaining micro seconds
--------------------------------------*/

    while ( micro_f == 1 )
    {
        HALT();
    }

/*--------------------------------------
    timeup!
--------------------------------------*/

}

/*********************************************************************************
* Function Name: r_tau1_channel2_interrupt
* Description  : This function is interrupt(INTTM12) service routine.
* Arguments    : None
* Return Value : None
* Remark       : 
*********************************************************************************/
#ifdef  CS_ENV
static void __near r_tau1_channel2_interrupt(void)
#endif
#ifdef  IAR_ENV
#pragma vector = INTTM12
__interrupt void r_tau1_channel2_interrupt(void)
#endif
{

    micro_f = 0;                /* clear delay counting flag    */
    TMMK12 = 1;                 /* disable INTTM12 interrupt    */

}

/*********************************************************************************
* Function Name: r_tau1_channel3_interrupt
* Description  : This function is INTTM13 interrupt service routine.(1ms)
* Arguments    : None
* Return Value : None
*********************************************************************************/
#ifdef  CS_ENV
static void __near r_tau1_channel3_interrupt(void)
#endif
#ifdef  IAR_ENV
#pragma vector = INTTM13
__interrupt void r_tau1_channel3_interrupt(void)
#endif
{

    g_micros += 1000;                   /* count up 1000 micro          */
    g_millis++;                         /* count up milli sec counter   */


    if ( milli_f  != 0 )                /* check delay counting flag    */
    {

/*======================================
    delay function is operating
======================================*/

        g_millis_b--;                   /* count down remaining time    */

        if ( g_millis_b != 0x00000000 )

/*--------------------------------------
    remaining time is more than 1ms
--------------------------------------*/

        {
            /* do nothing   */          /* exit with remaining time     */
        }
        else
        {

/*--------------------------------------
    remaining time is less than 1ms, then
    start TM12 to count remaining time
--------------------------------------*/

            milli_f = 0;                /* clear delay counting flag    */
            micro_f = 1;                /* set micro delay count flag   */
            TMIF12 = 0;                 /* clear INTTM12 interrupt flag */
            TMMK12 = 0;                 /* enable INTTM12 interrupt     */
            TS1 |= 0x04;                /* start TM12(micro interval)   */
        }

    }
    else
    {
        /* do nothing   */
    }

}

