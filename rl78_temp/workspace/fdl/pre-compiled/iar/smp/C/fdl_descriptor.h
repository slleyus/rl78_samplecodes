/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name : fdl_descriptor.h
* Version : 1.0
* Device(s) : RL78/G13 (R5F100LE)
* Tool-Chain : RL78 IAR Embedded Workbench V1.xx
* OS : None
* H/W Platform : RL78/G13 Stick (YRPBRL78G13)
* Description : Flash Data Access Library (T01) - Contains user configuration of the FAL pool
* Operation :
* Limitations : None
********************************************************************************
* History :
* 10.03.2011 V1.00 First Release
*******************************************************************************/


#ifndef __FAL_DESCRIPTOR_H_INCLUDED
#define __FAL_DESCRIPTOR_H_INCLUDED

#include "fdl_types.h"

/* specify the CPU frequency in [Hz]                                        */
#define   FDL_SYSTEM_FREQUENCY      32000000

/* programming voltage mode                                                 */
/* #define   FDL_WIDE_VOLTAGE_MODE  */

/* specify the size of the FAL pool expressed in number of blocks           */
#define   FAL_POOL_SIZE               4

/* specify the size of the EEL pool inside the FAL pool expressed in blocks */
#define   EEL_POOL_SIZE               0

/* external reference to descriptor */
extern    __far const fal_descriptor_t  fal_descriptor_str;

#endif
