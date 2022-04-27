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
* File Name : fdl.h
* Version : 1.0
* Device(s) : RL78/G13 (R5F100LE)
* Tool-Chain : RL78 IAR Embedded Workbench V1.xx
* OS : None
* H/W Platform : RL78/G13 Stick (YRPBRL78G13)
* Description : Flash Data Access Library (T01) - Contains the API definition of the Flash Access Library (data flash only)
* Operation :
* Limitations : None
********************************************************************************
* History :
* 10.03.2011 V1.00 First Release
*******************************************************************************/


#ifndef __FDL_H_INCLUDED
#define __FDL_H_INCLUDED

/*==============================================================================================*/
/* include files list                                                                           */
/*==============================================================================================*/
#include "fdl_types.h"



/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Enable data flash access.                                                                       */
/*                Pre-condition: The initialization of the library must be done by the FAL_Init function.         */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  __far_func void   FAL_Open(void);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Disable data flash access.                                                                      */
/*                Pre-condition: The initialization of the library must be done by the FAL_Init function.         */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  __far_func void   FAL_Close(void);



/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Initialization of the Flash Access Library.                                                     */
/*                Based on the descriptor data several checks are performed:                                      */
/*                - plausibility check of the FAL pool configuration                                              */
/*                - initialization of all internal variables                                                      */
/*                After initialization the FAL remains passive.                                                   */
/*                FAL_Open() has to be executed to open access to the FAL pool                                    */
/*  Input:        &descriptor_pstr far pointer to the descriptor                                                  */
/*  Output:       -                                                                                               */
/*  Return:       status: configuration related error code                                                        */
/*                - FAL_OK:                 initialization done without problems                                  */
/*                - FAL_ERR_CONFIGURATION:  FAL pool-configuration error                                          */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  __far_func fal_status_t   FAL_Init(const __far fal_descriptor_t __far* descriptor_pstr);



/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Initiating execution of the FAL request described in the passed request reference.              */
/*                The corresponding request variable has to be defined before starting the execution.             */
/*                                                                                                                */
/*                                                 WCMD (word command)            BCMD (block command)            */
/*                                                 -------------------------------------------------------------- */
/*                my_request->data_u32         :   read/write data                no meaning                      */
/*                my_request->index_u16        :   word-index within pool         block index within pool         */
/*                my_request->command_enu      :   command code                   command code                    */
/*                my_request->status_enu       :   request status                 request status                  */
/*                                                                                                                */
/*                Each client who is using FAL_Execute() has to use two private variables of initiating requests. */
/*                One of them is exclusively reserved for suspendable block-oriented commands (BCMD).             */
/*                The other one defines the non-suspendable word-oriented commands (WCMD).                        */
/*                Please refer to the users manual for details of the suspend-/resume-mechanism.                  */
/*                                                                                                                */
/*  Input:        &my_request_pstr - pointer to privat request variable  (WCMD or BCMD)                           */
/*  Output:       status inside the request variable                                                              */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  __far_func void   FAL_Execute(__near fal_request_t __near* request_pstr);



/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Continuing of not finished commands.                                                            */
/*                                                                                                                */
/*  Input:        -                                                                                               */
/*  Output:       status inside the request variable for command executed via FAL_Execute                         */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  __far_func void   FAL_Handler(void);



/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Return the start address of the library version string                                          */
/*                                                                                                                */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       starting pointer to the zero-terminated version string                                          */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  __far_func fal_u08 __far*   FAL_GetVersionString(void);


#endif
