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
* File Name : fdl_descriptor.c
* Version : 1.0
* Device(s) : RL78/G13 (R5F100LE)
* Tool-Chain : RL78 IAR Embedded Workbench V1.xx
* OS : None
* H/W Platform : RL78/G13 Stick (YRPBRL78G13)
* Description : Flash Data Access Library (T01) - Contains FAL pool descriptor
* Operation :
* Limitations : Please don't change this file!!!!
********************************************************************************
* History :
* 10.03.2011 V1.00 First Release
*******************************************************************************/


/*==============================================================================================*/
/* compiler settings                                                                            */
/*==============================================================================================*/
#pragma language = extended

/*==============================================================================================*/
/* include files list                                                                           */
/*==============================================================================================*/
#include "fdl_types.h"
#include "fdl_descriptor.h"

/* ----------------------------------------------------------------------------------------------------- */
/* ------                                                                                          ----- */
/* ------     B E G I N    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A       ----- */
/* ------                                                                                          ----- */
/* ----------------------------------------------------------------------------------------------------- */

/* constant segment definition */
 #pragma location="FAL_CNST"

#if (EEL_POOL_SIZE>FAL_POOL_SIZE)
  #error "EEL pool exceeds the FAL pool! Please adapt EEL_POOL_SIZE definition in fdl_descriptor.h"
#endif

/* specify the flash block size expressed in bytes */
#define   FAL_BLOCK_SIZE                (1*1024)

/* data flash start and end address                */
#define   FAL_DATA_FLASH_START_ADDR     0x000F1000
#define   FAL_DATA_FLASH_END_ADDR       0x000F1FFF



/* FAL pool configuration */

#define FAL_POOL_FIRST_BLOCK            0

#if (FAL_POOL_SIZE>0)
  #define FAL_POOL_LAST_BLOCK           (FAL_POOL_SIZE - 1)
  #define FAL_POOL_FIRST_ADDRESS        FAL_DATA_FLASH_START_ADDR
  #define FAL_POOL_LAST_ADDRESS         ( FAL_POOL_FIRST_ADDRESS + (FAL_BLOCK_SIZE*FAL_POOL_LAST_BLOCK) + (FAL_BLOCK_SIZE-1) )
  #define FAL_POOL_WSIZE                (FAL_POOL_SIZE*FAL_BLOCK_SIZE/4)
  #define FAL_POOL_FIRST_WIDX           0
  #define FAL_POOL_LAST_WIDX            (FAL_POOL_WSIZE - 1)
  #define FAL_BLOCK_WSIZE               (FAL_BLOCK_SIZE/4)
#else
  #define FAL_POOL_LAST_BLOCK           0
  #define FAL_POOL_FIRST_ADDRESS        0
  #define FAL_POOL_LAST_ADDRESS         0
  #define FAL_POOL_WSIZE                0
  #define FAL_POOL_FIRST_WIDX           0
  #define FAL_POOL_LAST_WIDX            0
  #define FAL_BLOCK_WSIZE               0
#endif



/* EEL pool configuration */

#define EEL_POOL_FIRST_BLOCK            0

#if (EEL_POOL_SIZE>0)
  #define EEL_POOL_LAST_BLOCK           (EEL_POOL_FIRST_BLOCK + EEL_POOL_SIZE - 1)
  #define EEL_POOL_FIRST_ADDRESS        FAL_POOL_FIRST_ADDRESS
  #define EEL_POOL_LAST_ADDRESS         ( FAL_POOL_FIRST_ADDRESS + ( FAL_BLOCK_SIZE*EEL_POOL_LAST_BLOCK ) + (FAL_BLOCK_SIZE-1) )
  #define EEL_POOL_WSIZE                (EEL_POOL_SIZE*FAL_BLOCK_SIZE/4)
  #define EEL_POOL_FIRST_WIDX           0
  #define EEL_POOL_LAST_WIDX            (EEL_POOL_WSIZE - 1)
#else
  #define EEL_POOL_LAST_BLOCK           0
  #define EEL_POOL_FIRST_ADDRESS        0
  #define EEL_POOL_LAST_ADDRESS         0
  #define EEL_POOL_WSIZE                0
  #define EEL_POOL_FIRST_WIDX           0
  #define EEL_POOL_LAST_WIDX            0
#endif


/* USER pool configuration */

#define USER_POOL_SIZE                  (FAL_POOL_SIZE - EEL_POOL_SIZE)

#if (USER_POOL_SIZE>0)
  #define USER_POOL_FIRST_BLOCK         0
  #define USER_POOL_LAST_BLOCK          (USER_POOL_FIRST_BLOCK + USER_POOL_SIZE - 1)

  #if (EEL_POOL_SIZE>0)
    #define USER_POOL_FIRST_ADDRESS     (EEL_POOL_LAST_ADDRESS + 1)
  #else
    #define USER_POOL_FIRST_ADDRESS     FAL_POOL_FIRST_ADDRESS
  #endif

  #define USER_POOL_LAST_ADDRESS        ( USER_POOL_FIRST_ADDRESS + ( FAL_BLOCK_SIZE*USER_POOL_LAST_BLOCK ) + (FAL_BLOCK_SIZE-1) )
  #define USER_POOL_WSIZE               (USER_POOL_SIZE*FAL_BLOCK_SIZE/4)
  #define USER_POOL_FIRST_WIDX          0
  #define USER_POOL_LAST_WIDX           (USER_POOL_WSIZE - 1)
#else
  #define USER_POOL_FIRST_BLOCK         0
  #define USER_POOL_LAST_BLOCK          0
  #define USER_POOL_FIRST_ADDRESS       0
  #define USER_POOL_LAST_ADDRESS        0
  #define USER_POOL_WSIZE               0
  #define USER_POOL_FIRST_WIDX          0
  #define USER_POOL_LAST_WIDX           0
#endif

/* calculate f_MHz = round-up(FDL_SYSTEM_FREQUENCY)   */
#define FDL_FX_MHZ                      ((FDL_SYSTEM_FREQUENCY+999999)/1000000)

#ifdef FDL_WIDE_VOLTAGE_MODE
  #define FDL_VOLTAGE_MODE  0x01
#else
  #define FDL_VOLTAGE_MODE  0x00
#endif

__far const fal_descriptor_t  fal_descriptor_str = {                              \
                                                (fal_u32)FAL_POOL_FIRST_ADDRESS,  \
                                                (fal_u32)EEL_POOL_FIRST_ADDRESS,  \
                                                (fal_u32)USER_POOL_FIRST_ADDRESS, \

                                                (fal_u32)FAL_POOL_LAST_ADDRESS,   \
                                                (fal_u32)EEL_POOL_LAST_ADDRESS,   \
                                                (fal_u32)USER_POOL_LAST_ADDRESS,  \

                                                (fal_u16)FAL_POOL_FIRST_BLOCK,    \
                                                (fal_u16)EEL_POOL_FIRST_BLOCK,    \
                                                (fal_u16)USER_POOL_FIRST_BLOCK,   \

                                                (fal_u16)FAL_POOL_LAST_BLOCK,     \
                                                (fal_u16)EEL_POOL_LAST_BLOCK,     \
                                                (fal_u16)USER_POOL_LAST_BLOCK,    \

                                                (fal_u16)FAL_POOL_FIRST_WIDX,     \
                                                (fal_u16)EEL_POOL_FIRST_WIDX,     \
                                                (fal_u16)USER_POOL_FIRST_WIDX,    \

                                                (fal_u16)FAL_POOL_LAST_WIDX,      \
                                                (fal_u16)EEL_POOL_LAST_WIDX,      \
                                                (fal_u16)USER_POOL_LAST_WIDX,     \

                                                (fal_u16)FAL_POOL_WSIZE,          \
                                                (fal_u16)EEL_POOL_WSIZE,          \
                                                (fal_u16)USER_POOL_WSIZE,         \

                                                (fal_u16)FAL_BLOCK_SIZE,          \
                                                (fal_u16)FAL_BLOCK_WSIZE,         \

                                                (fal_u08)FAL_POOL_SIZE,           \
                                                (fal_u08)EEL_POOL_SIZE,           \
                                                (fal_u08)USER_POOL_SIZE,          \

                                                (fal_u08)FDL_FX_MHZ,              \
                                                (fal_u08)FDL_VOLTAGE_MODE,        \
                                       };

/* ----------------------------------------------------------------------------------------------------- */
/* ------                                                                                          ----- */
/* ------     E N D    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A           ----- */
/* ------                                                                                          ----- */
/* ----------------------------------------------------------------------------------------------------- */
