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
* File Name : Main.c
* Version : 1.00
* Device(s) : RL78 (Test device used RL78/G13)
* Tool-Chain : RL78 IAR Embedded Workbench V1.10.1
* OS : None
* H/W Platform : RL78/G13 Target Board
* Description : RL78 IEC Self Test Library Main test harness API module
* Operation : This module provides the IEC60730/60335 API test harness for the
*             test modules. The appropriate test is selected by the options 
*             set in the IAR project set-up, pre-processor options menu
*
* Limitations : None
********************************************************************************
* History :
* 30.05.2011 V1.00 First Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , “Project Includes”
*******************************************************************************/
#include "main.h"
#include "globaldefines.h"

#pragma sfr
#pragma NOP
#pragma DI
#pragma EI
#pragma brk

/* General Variables definitions used by support functions */
unsigned int i;
unsigned int g_I;
char g_resultBool;

void indicate_test_result (char);
void LED_Display_Time_Delay(char);
extern void R_INTC0_Start(void);	
extern void R_INTC0_Stop(void);
extern void R_ADC_Start(void);
extern void UART_test(void);

/*******************************************************************************
Typedef definitions
*******************************************************************************/
/******************************************************************************
* Outline : Main RL78 IEC test harness module
* Include : stdbool.h, intrinsics.h, or5f100le.h, ior5f100le_ext.h, stl.h, main.h,
	    stdint.h, stl_gobal_data_example.h
* Declaration : void main(void)
* Function Name: main.c  
* Description : Main function providing the test harness for the IEC code modules
* Argument : none
* Return Value : None
* Calling Functions : cstartup.s87
******************************************************************************/
void main( void )
{

	DI();

  
#ifdef IDBU_TB   /* Use RL78/G13 Target board as test hardware */

    /* Start Up - Flash both RL78 target board LEDs */
    P6.2 = 0U;                     /* Turn LED1 On */
    P6.3 = 0U;                     /* Turn LED2 On */


    P6.2 = 1U;                     /* Turn LED1 Off */
    P6.3 = 1U;                     /* Turn LED2 Off */


#else    /* Use application board as test hardware */

    P6 = 0x00;                          /* white LED on */
    LED_Display_Time_Delay(3U);
    P6 = 0xFF;                          /* white LED off */
    LED_Display_Time_Delay(4U);

#endif


  while (1U)
  {

#ifdef INTERRUPTS_test
	EI();

	R_INTC0_Start();
	
#endif

#ifdef PORT_test
	P6 = 0x00;
	P6 = 0x0F;
#endif

#ifdef ADC_test
	EI();

	R_ADC_Start();

#endif

#ifdef UART0_test
	EI();

	UART_test();
	DI();
#endif
  }  

}
