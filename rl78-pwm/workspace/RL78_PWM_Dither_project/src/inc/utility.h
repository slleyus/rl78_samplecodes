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
* File Name		: utility.h
* Device(s)    	: R5F104PJ
* Tool-Chain   	: GNURL78 v12.01
* H/W Platform	: RSKRL78G14
* Description   : Defines utility.c file's function prototypes. 
* Creation Date	: 02/05/2012
***********************************************************************************************************************/

/* Multi inclusion prevention macro */
#ifndef _UTILITY_H
#define _UTILITY_H

/*******************************************************************************
User Includes (Project level includes)
*******************************************************************************/
/* Defines C99 variable types */
#include "r_cg_macrodriver.h"

/* Defines used within the project scope */
#include "rskrl78g14def.h"

/***********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/
/* Integer to binary coded string function prototype */
void uint16_ToBCDString(uint8_t *, uint8_t, uint16_t);

/* Integer to ASCII string function prototype */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);

#define BIT_MAX_12   	(0x0FFF)

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void bit12_to_ascii(uint16_t bit12_value, uint8_t data_number);

#endif
