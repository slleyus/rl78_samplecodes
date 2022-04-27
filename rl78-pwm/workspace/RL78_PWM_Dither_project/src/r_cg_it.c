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
* File Name    : r_cg_it.c
* Version      : CodeGenerator for RL78/G14 V2.04.04.03 [19 Aug 2016]
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for IT module.
* Creation Date: 3/15/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "inc/r_cg_it.h"

#include "inc/r_cg_macrodriver.h"
#include "inc/r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/


/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_IT_Create
* Description  : This function initializes the IT module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IT_Create(void)
{
    RTCEN = 1U;    /* supply IT clock */
    ITMC = _0000_IT_OPERATION_DISABLE;    /* disable IT operation */
    ITMK = 1U;    /* disable INTIT interrupt */
    ITIF = 0U;    /* clear INTIT interrupt flag */
    /* Set INTIT low priority */
    ITPR1 = 1U;
    ITPR0 = 1U;
    ITMC = _0013_ITMCMP_VALUE;	/* sample rate for pwm with ADC, 610.3uSEC (1638.4HZ) */
} /* End of function R_IT_Create() */


/***********************************************************************************************************************
* Function Name: R_IT_Start
* Description  : This function starts IT module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IT_Start(void)
{
    ITIF = 0U;    /* clear INTIT interrupt flag */
    ITMK = 1U;    /* disable INTIT interrupt */
    ITMC |= _8000_IT_OPERATION_ENABLE;    /* enable IT operation */
} /* End of function R_IT_Start() */


/***********************************************************************************************************************
* Function Name: R_IT_Stop
* Description  : This function stops IT module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IT_Stop(void)
{
    ITMK = 1U;    /* disable INTIT interrupt */
    ITIF = 0U;    /* clear INTIT interrupt flag */
    ITMC &= (uint16_t)~_8000_IT_OPERATION_ENABLE;    /* disable IT operation */
} /* End of function R_IT_Stop() */

