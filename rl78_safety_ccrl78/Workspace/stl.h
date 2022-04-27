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
* File Name : stl.h
* Version : 2.00
* Device(s) : RL78/G14
* Tool-Chain : RL78 CC-RL V1.03.00
* OS : None
* H/W Platform : RL78 Target Board
* Description : RL78 IEC Self Test Library Header File
* Operation :
* Limitations : None
********************************************************************************
* History :
* 30.05.2011 V1.00 First Release
* 15.11.2013 V1.10 Fixed some comments.
* 07.11.2016 V2.00 CC-RL Tool-Chain
*******************************************************************************/
#ifndef STL_H_INCLUDED
#define STL_H_INCLUDED

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
/*******************************************************************************
Macro Definitions
*******************************************************************************/


/*******************************************************************************
Typedef definitions
*******************************************************************************/

/* CRC test range variable structure */ 
typedef struct {
   unsigned long  length;
   unsigned long start_address;
} CHECKSUM_CRC_TEST_AREA;


/*******************************************************************************
Imported Function Prototypes
*******************************************************************************/

/* Register tests */
extern char stl_RL78_registertest (void);
extern char stl_RL78_registertest_psw (void);
extern char stl_RL78_registertest_stack (void);
extern char stl_RL78_registertest_cs (void);
extern char stl_RL78_registertest_es (void);


/* Clock tests */
extern char stl_RL78_hw_clocktest (void);
extern char stl_RL78_sw_clocktest (void);

/* RAM tests */
extern char stl_RL78_march_c(unsigned char *addr, unsigned short num);
extern char stl_RL78_march_x(unsigned char *addr, unsigned short num);

/* Software CRC test */
#ifdef sw_crc_test_enabled
extern unsigned short stl_RL78_sw_crc_asm (unsigned short crc, CHECKSUM_CRC_TEST_AREA *p);
#endif

/* Hardware Peripheral CRC test */
#ifdef hw_peripheral_crc_test_enabled
extern unsigned short stl_RL78_peripheral_crc(unsigned short gcrc, CHECKSUM_CRC_TEST_AREA *p);
unsigned short reference_crc_calculation(unsigned short crc_input, unsigned int length, unsigned char __far *p);
#endif

/* Support functions Software delay */
extern void LED_Display_Time_Delay (unsigned char num);

#endif /* STL_H_INCLUDED      */
