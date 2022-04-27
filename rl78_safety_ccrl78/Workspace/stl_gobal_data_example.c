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
* File Name : stl_global_data_example.c
* Version : 2.00
* Device(s) : RL78/G14
* Tool-Chain : RL78 CC-RL V1.03.00
* OS : None
* H/W Platform : RL78/G14 Target Board
* Description : RL78 IEC Self Test Library global variable declaration for API 
*               test harness module
* Operation : This module provides the global variables for the IEC60730/60335 
*             test harness (main.c). Variables are supplied for general use, 
*             the CRC test, the March C and March X ram tests.
*             
* Limitations : None
********************************************************************************
* History :
* 20.03.2011 V0.10 Pre Release
* 15.11.2013 V1.10 Fixed comments
* 07.11.2016 V2.00 CC-RL Tool-Chain
*******************************************************************************/


/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/



/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/




/* Variable definitions used for CRC Tests modules */
unsigned short g_crc;
unsigned short g_reference_crc;
unsigned short g_crcASM;
unsigned char __far *g_pStAdr;
unsigned int g_crc_length;

