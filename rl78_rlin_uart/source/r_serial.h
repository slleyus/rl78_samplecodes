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
* File Name 	: r_serial.h
* Version 		: 1.0
* Device(s) 	: R5F10PMFK
* H/W Platform 	: QB-R5F10PMFK-TB
* Description 	: This file implements device driver for Serial module.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 21.12.2012 1.00 First Release
******************************************************************************/

#ifndef _MDSERIAL_
#define _MDSERIAL_
/******************************************************************************
Macro definitions
******************************************************************************/
/*
	LIN Wake-up Baud Rate Selector Register n (LWBRn)
*/
/*  */
#define _000_LWBRn_LPRS_1		0x00U	/* set 1/1   prescaler clock */
#define _001_LWBRn_LPRS_2		0x02U	/* set 1/2   prescaler clock */
#define _010_LWBRn_LPRS_4		0x04U	/* set 1/4   prescaler clock */
#define _011_LWBRn_LPRS_8		0x06U	/* set 1/8   prescaler clock */
#define _100_LWBRn_LPRS_16		0x08U	/* set 1/16  prescaler clock */
#define _101_LWBRn_LPRS_32		0x0AU	/* set 1/32  prescaler clock */
#define _110_LWBRn_LPRS_64		0x0CU	/* set 1/64  prescaler clock */
#define _111_LWBRn_LPRS_128		0x0EU	/* set 1/128 prescaler clock */
#define _00_LWBRn_NSPB_16		0x00U	/* 16 samples per bit*/
#define _30_LWBRn_NSPB_4		0x30U	/* 4  samples per bit*/
#define _50_LWBRn_NSPB_6		0x50U	/* 6  samples per bit*/
#define _60_LWBRn_NSPB_7		0x60U	/* 7  samples per bit*/
#define _70_LWBRn_NSPB_8		0x70U	/* 8  samples per bit*/
#define _80_LWBRn_NSPB_9		0x80U	/* 9  samples per bit*/
#define _90_LWBRn_NSPB_10		0x90U	/* 10 samples per bit*/
#define _A0_LWBRn_NSPB_11		0xA0U	/* 11 samples per bit*/
#define _B0_LWBRn_NSPB_12		0xB0U	/* 12 samples per bit*/
#define _C0_LWBRn_NSPB_13		0xC0U	/* 13 samples per bit*/
#define _D0_LWBRn_NSPB_14		0xD0U	/* 14 samples per bit*/
#define _E0_LWBRn_NSPB_15		0xE0U	/* 15 samples per bit*/
#define _F0_LWBRn_NSPB_16		0xF0U	/* 16 samples per bit*/


/*
	UART Standby Control Register (LUSC)
*/
/**/
#define _00_LUSC_UWC			0x00U	/* Disable the start-up of reception in the STOP mode */
#define _01_LUSC_UWC			0x01U	/* Enable the start-up of reception in the STOP mode */
#define _00_LUSC_USEC			0x00U	/* Enable the generation of error interrupts */
#define _02_LUSC_USEC			0x02U	/* Disable the generation of error interrupts */
#define _00_LUSC_URDCC			0x00U	/* Disable comparison of reception data with ID register in SNOOZE mode */
#define _04_LUSC_URDCC			0x04U	/* Enable comparison of reception data with ID register in SNOOZE mode */


/*
	LIN/UART Mode Register n (LMDn)
*/
/**/
#define _01_LMDn_LMD			0x01U	/* UART mode select */
#define _00_LMDn_LMD			0x00U	/* LIN Master mode select */
#define _00_LMDn_LRDNFS			0x00U	/* use Noise Filter */
#define _20_LMDn_LRDNFS			0x20U	/* not use Noise Filter */


/*
	UART Configuration Register n (LBFCn)
*/
/**/
#define _00_LBFCn_UBLS_8bit			0x00U	/* UART 8-bit communication */
#define _01_LBFCn_UBLS_7bit			0x01U	/* UART 7-bit communication */
#define _00_LBFCn_UBOS_LSB			0x00U	/* LSB first */
#define _02_LBFCn_UBOS_MSB			0x02U	/* MSB first */
#define _00_LBFCn_USBLS_1bit		0x00U	/* Stop Bit 1 bit */
#define _04_LBFCn_USBLS_2bit		0x04U	/* Stop Bit 2 bits */
#define _00_LBFCn_UPS_NOT_USE		0x00U	/* Parity Disabled */
#define _08_LBFCn_UPS_Even			0x08U	/* Even Parity */
#define _10_LBFCn_UPS_0				0x10U	/* 0 Parity */
#define _18_LBFCn_UPS_Odd			0x18U	/* Odd Parity */
#define _00_LBFCn_URPS_R_Normal		0x00U	/* without inversion */
#define _20_LBFCn_URPS_R_Inversion	0x20U	/* with inversion */
#define _00_LBFCn_UTPS_T_Normal		0x00U	/* without inversion */
#define _40_LBFCn_UTPS_T_Inversion	0x40U	/* with inversion */

/*
	UART Error Detection Enable Register n (LEDEn)
*/
/**/
#define	_00_LEDEn_BERE_Disable	0x00U	/* Bit Error Detection Disabled */
#define _01_LEDEn_BERE_Enable	0x01U	/* Bit Error Detection Enabled */
#define _00_LEDEn_OERE_Disable	0x00U	/* Overrun Error Detection Disabled */
#define _04_LEDEn_OERE_Enable	0x04U	/* Overrun Error Detection Enabled */
#define _00_LEDEn_FERE_Disable	0x00U	/* Framing Error Detection Disabled */
#define _08_LEDEn_FERE_Enable	0x08U	/* Framing Error Detection Enabled */


/*
	LIN Interrupt Enable Register n (LIEn)
*/
/**/
#define	_00_LIEn_ERRIE_Disable	0x00U	/* Error Detection Interrupt Disabled */
#define	_04_LIEn_ERRIE_Enable	0x04U	/* Error Detection Interrupt Enabled */

/*
	UART Control Register n (LCUCn)
*/
/**/
#define _00_LCUCn_OM0_Active	0x00U	/* SW Reset request is active */
#define _01_LCUCn_OM0_Inactive	0x01U	/* SW Reset request is inactive */
#define _00_LCUCn_OM1_Wake_UP	0x00U	/* LIN Wake-up mode enabled */
#define _02_LCUCn_OM1_Normal	0x02U	/* LIN Normal Communication mode enabled */


/*
	UART Option Register n (LUORn1)
*/
/**/
#define _00_LUORn1_UEBE_Disable		0x00U	/* Disables expansion bit operation */
#define _01_LUORn1_UEBE_Enable		0x01U	/* Enables expansion bit operation */
#define _00_LUORn1_UEBDL_Value_0	0x00U	/* Selects expansion bit value "0" as expansion bit detecition level */
#define _02_LUORn1_UEBDL_Value_1	0x02U	/* Selects expansion bit value "1" as expansion bit detecition level */
#define _00_LUORn1_UTIGTS_Start		0x00U	/* Outputs transmission interrupt request upon transmission start */
#define _08_LUORn1_UTIGTS_Complet	0x08U	/* Outputs transmission interrupt request upon transmission completion */
#define _00_LUORn1_UEBE_Enable		0x00U	/* Expansion bit comparison enable */
#define _10_LUORn1_UEBE_Disable		0x10U	/* Expansion bit comparison disable */


/*
	UART Transmission Control Register n(LTRCn)
*/
/**/
#define _00_LTRCn_RTS_Stop		0x00U	/* UART buffer mode transmission is stopped */
#define _02_LTRCn_RTS_Start		0x02U	/* UART buffer mode start is enabled */

/*
	UART Status Register n (LSTn)
*/
/**/
#define _00_LSTn_FTC_NOT_Complet	0x00U	/* Frame transmission not completed */
#define _01_LSTn_FTC_Complet		0x01U	/* Frame transmitted completed */
#define _00_LSTn_ERR_NOT_Detected	0x00U	/* No changes in Error status detected in UART mode */
#define _04_LSTn_ERR_Detection		0x04U	/* Change in Error status detected in UART mode */
#define _00_LSTn_UTS_NOT_Operate	0x00U	/* A transmit operation is not in progress */
#define _10_LSTn_UTS_Operation		0x10U	/* A transmit operation is in progress */
#define _00_LSTn_RTS_NOT_Operate	0x00U	/* A receive operation is not in progress */
#define _10_LSTn_RTS_Operation		0x10U	/* A receive operation is in progress */

/*
	UART Error Status Register n (LESTn)
*/
/**/
#define _00_LESTn_BER_NOT_Detected	0x00U	/* Bit error not detected */
#define _01_LESTn_BER_Detection		0x01U	/* Bit error detected */
#define _00_LESTn_OER_NOT_Detected	0x00U	/* UART Overrun error not detected */
#define _04_LESTn_OER_Detection		0x04U	/* UART Overrun error detected */
#define _00_LESTn_FER_NOT_Detected	0x00U	/* UART Framing error not detected */
#define _08_LESTn_FER_Detection		0x08U	/* UART Framing error detected */
#define _00_LESTn_EXBT_NOT_Detected	0x00U	/* Expansion bit is not detected */
#define _10_LESTn_EXBT_Detection	0x10U	/* Expansion bit is detected */
#define _00_LESTn_UPER_NOT_Detected	0x00U	/* UART Parity Error not detected */
#define _40_LESTn_UPER_Detection	0x40U	/* UART Parity Error detected */

/*
	UART Operation Enable Register n (LUOER)
*/
/**/
#define	_00_LUOERn_UTOE_Disable		0x00U	/* Stops transmission operation */
#define	_01_LUOERn_UTOE_Enable		0x01U	/* Enables transmission operation */
#define	_00_LUOERn_UROE_Disable		0x00U	/* Stops reception operation */
#define	_02_LUOERn_UROE_Enable		0x02U	/* Enables reception operation */

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void R_LIN0_Init(void);
void R_UART_Init(void);
void R_UART_Start(void);
void R_UART_Stop(void);
MD_STATUS R_UART_Receive(u8 *rxbuf, u8 rxnum);
MD_STATUS R_UART_Send(u8 *txbuf, u8 txnum);

void R_UART_Callback_ReceiveEnd(void);
void R_UART_Callback_SendEnd(void);
void R_UART_Callback_Error(u8 err_type);
#endif
