/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name	: utility.h
* Version 	: 1.00
* Device 	: R5F100LEAFB
* Tool Chain 	: RL Family C Compiler (IAR ICCRL78)
* H/W Platform	: RSKRL78G13
* Description   : Defines utility.c file's function prototypes. 
*******************************************************************************/
/*******************************************************************************
* History 	: 21.06.2011 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project level includes)
*******************************************************************************/
#include "r_cg_macrodriver.h"
/* Defines used within the project scope */
#include "rskrl78g13def.h"

/* Multi inclusion prevention macro */
#ifndef _UTILITY_H
#define _UTILITY_H

/* Integer to binary coded string function prototype */
void uint16_ToBCDString(uint8_t *, uint8_t, uint16_t);

/* Integer to ASCII string function prototype */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);

#endif
