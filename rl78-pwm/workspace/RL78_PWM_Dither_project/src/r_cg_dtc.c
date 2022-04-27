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
* File Name    : r_cg_dtc.c
* Version      : CodeGenerator for RL78/G14 V2.04.04.03 [19 Aug 2016]
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DTC module.
* Creation Date: 3/15/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_dtc.h"
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/


/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
#pragma address dtc_vectortable = 0x0FFD00U  /* pwm */
uint8_t __near dtc_vectortable[40U];

#pragma address dtc_controldata_0 = 0x0FFD40U	/* pwm */
st_dtc_data __near dtc_controldata_0;


/***********************************************************************************************************************
* Function Name: R_DTC_Create
* Description  : This function initializes the DTC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTC_Create(void)
{
    /* Enable input clock supply */
    DTCEN = 1U;
    /* Disable all DTC channels operation */
    DTCEN0 = 0x00U;
    DTCEN1 = 0x00U;
    DTCEN2 = 0x00U;
    DTCEN3 = 0x00U;
    DTCEN4 = 0x00U;
    /* Set base address */
    DTCBAR = 0xFDU;  /* pwm */
    /* Set DTCD0 */
    dtc_vectortable[19] = 0x40U;

    dtc_controldata_0.dtccr = _00_DTC_TRANSFER_MODE_NORMAL | _40_DTC_DATA_SIZE_16BITS | _04_DTC_SOURCE_ADDR_INCREMENTED |
                              _00_DTC_DEST_ADDR_FIXED | _00_DTC_CHAIN_TRANSFER_DISABLE;  /* 16bit transfer for pwm */

    dtc_controldata_0.dtbls = _01_DTCD0_TRANSFER_BLOCKSIZE;
    dtc_controldata_0.dtcct = _10_DTCD0_TRANSFER_BYTE;
    dtc_controldata_0.dtrld = 0x00U;
    dtc_controldata_0.dtsar = _E000_DTCD0_SRC_ADDRESS;
    dtc_controldata_0.dtdar = _FF1A_DTCD0_DEST_ADDRESS;   /*FF1A is address of TDR01, for PWM period */
}

/***********************************************************************************************************************
* Function Name: R_DTCD0_Start
* Description  : This function enables DTCD0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD0_Start(void)
{
    DTCEN2 |= _10_DTC_TAU00_ACTIVATION_ENABLE;
}

/***********************************************************************************************************************
* Function Name: R_DTCD0_Stop
* Description  : This function disables DTCD0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD0_Stop(void)
{
    DTCEN2 &= (uint8_t)~_10_DTC_TAU00_ACTIVATION_ENABLE;
}


