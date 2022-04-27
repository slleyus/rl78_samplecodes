/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2011, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/*******************************************************************************
* File Name    : AR_LIB_PORT.h
* Version      : Rev. 1
* Device(s)    : R5F104ML
* Tool-Chain   : CCRL
* Description  : This file implements Arduino library.
* Creation Date: 2019/12/30
********************************************************************************/
#define     OUTPUT  ( 0x00 )        /* digital output mode      */
#define     INPUT   ( 0x02 )        /* digital input mode       */
#define     INPUT_PULLUP ( 0x03 )   /* digital input with pullup */

#define     HIGH    ( 1 )           /* value HIGH is 1          */
#define     LOW     ( 0 )           /* value LOW is 0           */

/*******************************************************************************
port definition G14( all of RL78 is basically same )
********************************************************************************/

#define     BIT0    ( 0x01 )        /* data of bit0             */
#define     BIT1    ( 0x02 )        /* data of bit1             */
#define     BIT2    ( 0x04 )        /* data of bit2             */
#define     BIT3    ( 0x08 )        /* data of bit3             */
#define     BIT4    ( 0x10 )        /* data of bit4             */
#define     BIT5    ( 0x20 )        /* data of bit5             */
#define     BIT6    ( 0x40 )        /* data of bit6             */
#define     BIT7    ( 0x80 )        /* data of bit7             */


void pinMode(uint8_t ,uint8_t );    /* set pin mode(function)   */
void digitalWrite(uint8_t , uint8_t );  /* data output to pin   */
uint8_t digitalRead(uint8_t );      /* read data from pin       */


