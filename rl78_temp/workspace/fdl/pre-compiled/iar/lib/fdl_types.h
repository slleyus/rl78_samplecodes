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
* File Name : fdl_types.h
* Version : 1.0
* Device(s) : RL78/G13 (R5F100LE)
* Tool-Chain : RL78 IAR Embedded Workbench V1.xx
* OS : None
* H/W Platform : RL78/G13 Stick (YRPBRL78G13)
* Description : Flash Data Access Library (T01) - Definition of types used by the library
* Operation :
* Limitations : None
********************************************************************************
* History :
* 10.03.2011 V1.00 First Release
*******************************************************************************/


#ifndef __FDL_TYPES_H_INCLUDED
#define __FDL_TYPES_H_INCLUDED



/*==============================================================================================*/
/* unsigned type definitions                                                                    */
/*==============================================================================================*/
typedef unsigned char                       fal_u08;
typedef unsigned int                        fal_u16;
typedef unsigned long int                   fal_u32;


/*==============================================================================================*/
/* global constant definitions                                                                  */
/*==============================================================================================*/



/*==============================================================================================*/
/* global type definitions                                                                      */
/*==============================================================================================*/


/* FAL command code set */
typedef   enum  {                                               /* ---------------------------------------------  */
                  FAL_CMD_UNDEFINED           = (0x00),         /* 0x00, undefined command code (default)         */
                  FAL_CMD_BLANKCHECK_WORD     = (0x00 | 0x01),  /* 0x01, WCMD, checks if one flash word is blank  */
                  FAL_CMD_IVERIFY_WORD        = (0x00 | 0x02),  /* 0x02, WCMD, verifies data of one flash word    */
                  FAL_CMD_READ_WORD           = (0x00 | 0x03),  /* 0x03, WCMD, reads content of one flash word    */
                  FAL_CMD_WRITE_WORD          = (0x00 | 0x04),  /* 0x04, WCMD, writes data into one flash word    */
                  FAL_CMD_ERASE_BLOCK         = (0x00 | 0x05)   /* 0x05, BCMD, erases one flash block             */
                } fal_command_t;                                /* ---------------------------------------------  */



/* FAL command error set */
typedef   enum  {
                  /* operation related status               */  /* ---------------------------------------------- */
                  FAL_OK                      = (0x00),         /* 0x00, no problems                              */
                  FAL_BUSY                    = (0x00 | 0x01),  /* 0x01, command is being processed               */
                  /* run-time error related status          */  /* ---------------------------------------------- */
                  FAL_ERR_PROTECTION          = (0x10 | 0x00),  /* 0x10, protection error (access right conflict) */
                  FAL_ERR_BLANKCHECK          = (0x10 | 0x01),  /* 0x11, blankcheck error (MRG12)                 */
                  FAL_ERR_VERIFY              = (0x10 | 0x02),  /* 0x12, internal verify error (MRF01/MRG12)      */
                  FAL_ERR_WRITE               = (0x10 | 0x03),  /* 0x13, write error                              */
                  FAL_ERR_ERASE               = (0x10 | 0x04),  /* 0x14, erase error                              */
                  /* configuration error related status     */  /* ---------------------------------------------- */
                  FAL_ERR_PARAMETER           = (0x20 | 0x00),  /* 0x20, parameter error                          */
                  FAL_ERR_CONFIGURATION       = (0x20 | 0x01),  /* 0x21, pool or frequency configuration wrong    */
                  FAL_ERR_INITIALIZATION      = (0x20 | 0x02),  /* 0x22, FAL_Init() missing                       */
                  FAL_ERR_COMMAND             = (0x20 | 0x03),  /* 0x23, wrong command code detected              */
                  FAL_ERR_REJECTED            = (0x20 | 0x04)   /* 0x24, rejected --> driver active               */
                } fal_status_t;                                 /* ---------------------------------------------- */


/* FAL request type (base type for any FAL access) */
typedef struct {                                                /* ---------------------------------------------- */
                  fal_u32             data_u32;                 /* 4, W/R, 1 word data buffer                     */
                  fal_u16             index_u16;                /* 2, W,virt. word/block index inside FAL-pool    */
                  fal_command_t       command_enu;              /* 1, W, command to be processed                  */
                  fal_status_t        status_enu;               /* 1, R, status code                              */
                } fal_request_t;                                /*------------------------------------------------*/
                                                                /* 8 bytes in total                               */
                                                                /*------------------------------------------------*/


/* FAL descriptor type */
typedef struct {
                  fal_u32     fal_pool_first_addr_u32;          /* 4, first physical address of the FAL pool      */
                  fal_u32     eel_pool_first_addr_u32;          /* 4, first physical address of the EEL pool      */
                  fal_u32     user_pool_first_addr_u32;         /* 4, first physical address of the USER pool     */

                  fal_u32     fal_pool_last_addr_u32;           /* 4, last physical address of the FAL pool       */
                  fal_u32     eel_pool_last_addr_u32;           /* 4, last physical address of the EEL pool       */
                  fal_u32     user_pool_last_addr_u32;          /* 4, last physical address of the USR pool       */

                  fal_u16     fal_pool_first_block_u16;         /* 2, first physical block of the FAL pool        */
                  fal_u16     eel_pool_first_block_u16;         /* 2, first physical block of the EEL pool        */
                  fal_u16     user_pool_first_block_u16;        /* 2, first physical block of the USER pool       */

                  fal_u16     fal_pool_last_block_u16;          /* 2, last physical block of the FAL pool         */
                  fal_u16     eel_pool_last_block_u16;          /* 2, last physical block of the EEL pool         */
                  fal_u16     user_pool_last_block_u16;         /* 2, last physical block of the USER pool        */

                  fal_u16     fal_first_widx_u16;               /* 2, first word-index of the FAL pool            */
                  fal_u16     eel_first_widx_u16;               /* 2, first word-index of the EEL pool            */
                  fal_u16     user_first_widx_u16;              /* 2, first word-index of the USER pool           */

                  fal_u16     fal_last_widx_u16;                /* 2, last word-index of the FAL pool             */
                  fal_u16     eel_last_widx_u16;                /* 2, last word-index of the EEL pool             */
                  fal_u16     user_last_widx_u16;               /* 2, last word-index of the USER pool            */

                  fal_u16     fal_pool_wsize_u16;               /* 2, size of the FAL pool expressed in words     */
                  fal_u16     eel_pool_wsize_u16;               /* 2, size of the FAL pool expressed in words     */
                  fal_u16     user_pool_wsize_u16;              /* 2, size of the FAL pool expressed in words     */

                  fal_u16     block_size_u16;                   /* 2, block size expressed in bytes               */
                  fal_u16     block_wsize_u16;                  /* 2, block size expressed in words               */

                  fal_u08     fal_pool_size_u08;                /* 1, size of the FAL pool expressed in blocks    */
                  fal_u08     eel_pool_size_u08;                /* 1, size of the FAL pool expressed in blocks    */
                  fal_u08     user_pool_size_u08;               /* 1, size of the FAL pool expressed in blocks    */

                  fal_u08     fx_MHz_u08;                       /* 1, system frequency expressed in MHz           */
                  fal_u08     wide_voltage_mode_u08;            /* 1, programming voltage mode                    */
                                                                /*    (full/wide voltage mode)                    */
                } fal_descriptor_t;                             /*------------------------------------------------*/
                                                                /* 63 bytes in total                              */


#endif
