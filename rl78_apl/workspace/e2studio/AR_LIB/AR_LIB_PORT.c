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

/*******************************************************************************
* File Name    : AR_LIB_PORT.c
* Version      : Rev. 1.00b
* Device(s)    : R5F104ML
* Tool-Chain   : CCRL
* Description  : This file implements Arduino library(digital I/O).
* Creation Date: 2020/01/01
* Modified Date: 2020/01/23 add PMCn control 
********************************************************************************/

/*******************************************************************************
Includes
********************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
#include "r_cg_userdefine.h"
#include "AR_LIB_PORT.h"
/*******************************************************************************
Pragma directive
********************************************************************************/


/*******************************************************************************
Global variables and functions
********************************************************************************/

/*******************************************************************************
port definition for G14_FPB
********************************************************************************/

static uint8_t const P_dif[][4]={
/*   The order of the data is specified by digital pin number from 'D0 pin'.
    Format is {port address offset , bit data,PMCn exist,PUn exist} for each pin.
    Usage of the offset is shown in the following.
     port data register address is &P0 + offset,
     port mode register address is &PM0 + offset,
     pullup resistor address is &PU0 + offset,
     PMCn register address is &PMC0 + offset.
    'Bit data' is bit pattern(only specified bit is set).
    'PMCn exist' is 1 if the port has PMCn bit. Otherwise, 0.
    'PUn exist'  is 1 if the port has PUn bit. Otherwise, 0.
    
     for exsample
     (1)in case of P00(without PMCn bit) is { 0, BIT0, 0, 1 }.
     (2)in case of P17(with PMCn bit) is { 1, BIT7, 1, 1 }.
     
    */

    { 14, BIT3, 0, 1 },     /* D0  is allocated at P143     */
    { 14, BIT4, 0, 1 },     /* D1  is allocated at P144     */
    { 14, BIT2, 0, 1 },     /* D2  is allocated at P142     */
    { 7,  BIT5, 0, 1 },     /* D3  is allocated at P75      */
    { 7,  BIT6, 0, 1 },     /* D4  is allocated at P76      */
    { 7,  BIT7, 0, 1 },     /* D5  is allocated at P77      */
    { 1,  BIT0, 0, 1 },     /* D6  is allocated at P10      */
    { 0,  BIT2, 1, 1 },     /* D7  is allocated at P02      */

    { 0,  BIT3, 1, 1 },     /* D8  is allocated at P03      */
    { 1,  BIT1, 0, 1 },     /* D9  is allocated at P11      */
    { 1,  BIT2, 1, 1 },     /* D10 is allocated at P12      */
    { 5,  BIT2, 0, 1 },     /* D11 is allocated at P52      */
    { 5,  BIT3, 0, 1 },     /* D12 is allocated at P53      */
    { 5,  BIT4, 0, 1 },     /* D13 is allocated at P54      */

    { 6,  BIT1, 0, 0 },     /* D14 is allocated at P61      */
    { 6,  BIT0, 0, 0 },     /* D15 is allocated at P60      */

    { 4,  BIT3, 0, 1 },     /* D16(LED0) is allocated at P43*/
    { 4,  BIT4, 0, 1 },     /* D17(LED1) is allocated at P44*/

    { 13, BIT7, 0, 0 }      /* D18(SW) is allocated at P137 */

}; 



/*******************************************************************************
* Function Name: pinMode
* Description  : This function set pin to digital In/Out.
* Arguments    : pin:pin number( D0 - D18 )
                 mode:pin function(OUTPUT:0x00/INPUT:0x02/INPUT_PULLUP:0x03)
* Return Value : None
********************************************************************************/
void pinMode(uint8_t pin,uint8_t mode)
{

    uint8_t  *pm_addr;                      /* PM register pointer      */
    uint8_t  *pu_addr;                      /* PU register pointer      */
    uint8_t  *pmc_addr;                     /* PMCn register pointer    */
    uint8_t p_offset;                       /* port address offset      */
    uint8_t work;
    uint8_t temp;

/*=======================================
    set Port register pointer
=======================================*/

    p_offset = P_dif[pin][0];


    pm_addr = (uint8_t *)( &PM0 + p_offset );   /* set PM register address  */
    pu_addr = (uint8_t *)( &PU0 + p_offset );   /* set PU register address  */

    work = P_dif[pin][1];                       /* get bit pattern          */

/*--------------------------------------
    set Port mode control register
--------------------------------------*/

    if ( P_dif[pin][2] != 0 )
    {

/*--------------------------------------
    Port mode control register is exist
--------------------------------------*/

        pmc_addr = (uint8_t *)( &PMC0 + p_offset ); /* set PMCn address     */
        *pmc_addr &= ( work ^ 0xFF );           /* clear PMC and set to port*/
    }
    else
    {
        /* do nothing   */
    }

/*--------------------------------------
    set Port mode register
--------------------------------------*/

    temp = *pm_addr;                        /* get PM register value    */

    if (( mode & 0x02 ) == 0 )              /* check OUTPUT mode or not */
    {

/*=======================================
    when set output mode
=======================================*/

        temp &= (work ^ 0xFF);              /* clear PM bit data        */
        *pm_addr = temp;                    /* set PM to output mode    */
    }
    else
    {

/*=======================================
   when set input mode
=======================================*/

        temp = work;                        /* set PM bit data          */
        *pm_addr |= temp;                   /* set PM to input mode     */
        
        if ( P_dif[pin][3] == 1 )           /* check PUn is exist or not*/
        {
            if (( mode & 0x01 ) != 0 )      /* check pullup resistor    */
            {

/*--------------------------------------
    when set input with pullup resistor
--------------------------------------*/

                *pu_addr |= work;           /* set PU register          */
            }
            else 
            {

/*--------------------------------------
    set input without pullup resistor
--------------------------------------*/

                *pu_addr &= ( work ^ 0xFF );  /* reset PU register      */
            }
        }
    }

}

/*******************************************************************************
* Function Name: digitalWrite
* Description  : This function outputs data to specfied pin.
* Arguments    : pin:pin number( D0:0 - D15:15 )
                 value:HIGH:1/LOW:0
* Return Value : None
********************************************************************************/
void digitalWrite(uint8_t pin, uint8_t value)
{

    uint8_t  *p_addr;                   /* Port register pointer        */
    uint8_t  *pm_addr;                  /* PM register pointer          */
    uint8_t  *pu_addr;                  /* PU register pointer          */

    uint8_t p_offset;                   /* port address offset          */
    uint8_t work;


/*=======================================
    set Port register offset and bit
=======================================*/

    p_offset = P_dif[pin][0];           /* set port offset address      */

    p_addr = (uint8_t *)( &P0 + p_offset ); /* set P register address   */
    pm_addr = (uint8_t *)( &PM0 + p_offset ); /* set PM register address  */
    pu_addr = (uint8_t *)( &PU0 + p_offset ); /* set PU register address  */

    work = P_dif[pin][1];               /* get bit value            */

/*=======================================
    confirm Port mode register
=======================================*/

    if ((*pm_addr & work ) == work )    /* check if input mode      */
    {

/*=======================================
    when set as input mode
=======================================*/

        if ( value == 0 )
        {

/*--------------------------------------
    if data is 0 then turn off pullup resistor
--------------------------------------*/

            *pu_addr &= ( work ^ 0xFF );    /* turn off pull up         */
        }
        else
        {

/*--------------------------------------
    if data is 1 then turn on pullup resistor
--------------------------------------*/

            *pu_addr |= work;               /* turn on pull up resistor */
        }
    }
    else
    {

/*=======================================
    when set as output mode
=======================================*/

        if ( value == 0 )               /* check output level       */
        {

/*--------------------------------------
    output LOW level to pin
--------------------------------------*/

            *p_addr &= ( work ^ 0xFF );     /* turn off pin             */
        }
        else
        {

/*--------------------------------------
    output HIGH level to pin
--------------------------------------*/

            *p_addr |= work;                /* turn on pin level        */
        }
    }
}

/*******************************************************************************
* Function Name: digitalRead
* Description  : This function inputs data from specfied pin.
* Arguments    : pin:pin number( D0 - D15 )
* Return Value : pin level:HIGH:1/LOW:0
********************************************************************************/
uint8_t digitalRead(uint8_t pin)
{
    uint8_t  *p_addr;                       /* PM register pointer      */
    uint8_t work;

/*=======================================
    set Port register pointer and bit
=======================================*/

    p_addr = (uint8_t *)(&P0 + P_dif[pin][0]); /* set port offset address  */

/*=======================================
    get pin value
=======================================*/

    work = ( *p_addr & P_dif[pin][1] );     /* get pin data value       */

    if ( work )                             /* check pin level          */
    {

/*--------------------------------------
   pin is HIGH level
--------------------------------------*/

        return( HIGH );                     /* return with HIGH         */
    }
    else
    {

/*--------------------------------------
   pin is LOW level
--------------------------------------*/

        return( LOW );                      /* return with LOW          */
    }
}

