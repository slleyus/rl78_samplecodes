/*****************************************************************************
 DISCLAIMER
 This software is supplied by Renesas Electronics Corporation and is only
 intended for use with Renesas products. No other uses are authorized. This
 software is owned by Renesas Electronics Corporation and is protected under
 all applicable laws, including copyright laws.
 THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 Renesas reserves the right, without notice, to make changes to this software
 and to discontinue the availability of this software. By using this software,
 you agree to the additional terms and conditions found by accessing the
 following link:
 http://www.renesas.com/disclaimer
 Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/
/*******************************************************************************
* File Name : main.h
* Version : 2.00
* Device(s) : RL78/G14
* Tool-Chain : RL78 CC-RL V1.03.00
* OS : None
* H/W Platform : None
* Description : RL78 IEC Self Test Library Global data example module Header file
* Operation :
* Limitations : None
********************************************************************************
* History :
* 30.05.2011 V1.00 First Release
* 15.11.2013 V1.10 Fixed some comments
* 07.11.2016 V2.00 CC-RL Tool-Chain
*******************************************************************************/

/*******************************************************************************
Macro Definitions
*******************************************************************************/
#ifndef STL_GLOBAL_DATA_EXAMPLE_H
#define STL_GLOBAL_DATA_EXAMPLE_H


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/



/******************************************************************************
Imported global variables and functions (to be accessed by other files)
******************************************************************************/

/* General Imported References for suport functions */
extern char g_resultBool;

/* Imported References used for RAM Tests */
extern uint8_t __far *g_pMC_StAdr;        /* Start Address pointer */
extern uint16_t g_RAM_MarchC_lgth;        /* Address range variable */

extern uint8_t __far *g_pMX_StAdr;        /* Start Address pointer */
extern uint16_t g_RAM_MarchX_lgth;        /* Address range variable */

/* Imported References used for CRC Tests */
extern uint16_t g_crc;                    /* CRC start/ result value */
extern uint16_t g_reference_crc;          /* reference CRC result value */
extern uint16_t g_crcASM;                 /* Software CRC result value */
extern uint8_t __far *g_pStAdr;           /* Start Address pointer */
extern uint32_t g_crc_length;             /* Address range variable */

#endif /* STL_GLOBAL_DATA_EXAMPLE_H */