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
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name 	: r_macrodriver.h
* Version 		: 1.0
* Device(s) 	: R5F10PMFK
* H/W Platform 	: QB-R5F10PMFK-TB
* Description 	: This file implements general head file.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 21.12.2012 1.00 First Release
******************************************************************************/

#ifndef _MDSTATUS_
#define _MDSTATUS_
/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#pragma sfr
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#ifndef __TYPEDEF__
typedef signed char         s8;
typedef unsigned char       u8;
typedef signed short        s16;
typedef unsigned short      u16;
typedef signed long         s32;
typedef unsigned long       u32;
typedef	unsigned short		MD_STATUS;

/******************************************************************************
Macro definitions
******************************************************************************/
/* Type test */
#define UartFunc_Transmit	0x00U	/* check ransmission */
#define UartFunc_Receive	0x01U	/* check reception 	 */
#define UartFunc_Error		0x02U	/* check reception error */

/* Error detection function */
#define ErrorFunc_BE		0x01U	/* check Bit error */
#define ErrorFunc_OE		0x02U	/* check Overrun error */
#define ErrorFunc_FE		0x03U	/* check Framing error */
#define ErrorFunc_PE		0x04U	/* check Parity  error */
#define ErrorFunc_EXB		0x05U	/* check Expansion bit detect */


/* Status list definition */
#define MD_STATUSBASE		0x00U
#define MD_OK			(MD_STATUSBASE + 0x00U)	/* register setting OK */
#define MD_SPT			(MD_STATUSBASE + 0x01U)	/* IIC stop */
#define MD_NACK			(MD_STATUSBASE + 0x02U)	/* IIC no ACK */
#define MD_BUSY1		(MD_STATUSBASE + 0x03U)	/* busy 1 */
#define MD_BUSY2		(MD_STATUSBASE + 0x04U)	/* busy 2 */

/* Error list definition */
#define MD_ERRORBASE		0x80U
#define MD_ERROR		(MD_ERRORBASE + 0x00U)	/* error */
#define MD_ARGERROR		(MD_ERRORBASE + 0x01U)	/* error agrument input error */
#define MD_ERROR1		(MD_ERRORBASE + 0x02U)	/* error 1 */
#define MD_ERROR2		(MD_ERRORBASE + 0x03U)	/* error 2 */
#define MD_ERROR3		(MD_ERRORBASE + 0x04U)	/* error 3 */
#define MD_ERROR4		(MD_ERRORBASE + 0x05U)	/* error 4 */
#define __TYPEDEF__
#endif

typedef union U_W_8X2 {		// int data union for two byte data
	struct S_W_8X2 {			// int data struct for two byte data
		unsigned charL: 8;	// byte lower address
		unsigned charH : 8;	// byte upper address
	}byte;
	unsigned short all;
}W_8X2;

typedef union U_W_8X4 {		// long data union for four byte data
	struct S_W_8X4 {			// long data struct for four byte data
		unsigned charLL : 8;// byte data
		unsigned charLH : 8;// byte data
		unsigned charHL : 8;// byte data
		unsigned charHH : 8;// byte data
	}byte;
	struct S_W_8X4_W {		// long data struct for four byte data
		unsigned shortL :16;// word data
		unsigned shortH :16;// word data
	}word;
	unsigned long all;
}W_8X4;						// long data union for four byte data


#endif