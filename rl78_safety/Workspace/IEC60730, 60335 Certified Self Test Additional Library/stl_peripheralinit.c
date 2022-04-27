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
* File Name : stl_peripheralinit.c
* Version : 1.00
* Device(s) : RL78/G13
* Tool-Chain : RL78 IAR Embedded Workbench V1.10.1
* OS : None
* H/W Platform : RL78/G13 Target Board
* Description : RL78 IEC Self Test Library global peripheral initialisation for 
*               for test harness module and IEC tests
* Operation : This module provides the device initialisation for the IEC60730/60335 
*             test modules. via the low level initialisation function
*             This includes 
*               - Option byte settings 
*               - Oscillator Settings
*               - Target Board LED Port initialisation
*               - Timer Array Unit for Clock test module
* Limitations : None
********************************************************************************
* History :
* 20.03.2011 V1.00 Pre Release
*******************************************************************************/
#include "globaldefines.h"

#pragma sfr
#pragma NOP

extern void R_INTC_Create(void);
extern void R_PORT_Create(void);
extern void R_ADC_Create(void);
extern void R_SAU0_Create(void);

/******************************************************************************
* Outline : __low_level_init
* Include : intrinics.h, ior5f100le.h, ior5f100le_ext.h, stdbool.h, stdint.h
* Declaration : int __low_level_init (void)
* Function Name: low level initialisation
* Description : This provides the basic start up and initialisation functions 
                for the IEC test harness and self test modules. It is called by
                the cstartup module on power on / reset and replaces the 
                standard IAR version

* Argument : none
* Return Value : ENABLE_VAR_INIT - Flag to continue memory initialisation and main call 
* Calling Functions : cstartup.s87
******************************************************************************/      
int low_level_init (void)
{

/* Oscillator Settings */
      CMC = 0x53U;            /* X1/X2 as oscillator,  XT1/XT2 as oscillator, AMPH for >10MHz */
      CKC = 0U;
      CSC = 0x80U;            /* X1 external oscillator stopped */
                              /* HS internal oscillator running (Set for 32MHz) */
                              /* XT1 Sub clock operating (32KHz) */

/* Port Settings for status LEDs on application/target board */
#ifdef IDBU_TB

     P6.2 = 1U;         /* Led1 Off */
     P6.3 = 1U;         /* Led2 Off */
     PM6.2 = 0U;        /* Led 1 output */
     PM6.3 = 0U;        /* Led 2 output */
#else

     PM5   = 0x00;            /* set into output mode */

#endif


/* Clock Output used as reference clock for software clock measurement 
   Uses 32KHz sub clock output connected to port test input pin  */
 
      PCLOE0 = 0U;	      /* disable PCLBUZ0 operation */
      CKS0 &= (unsigned char)~0x0F;
      CKS0 |= 0x08U;          /* Set Clock output to 32KHz */

      P14 &= 0xFEU;           /* Set PCLBUZ0 pin to output */
      PM14 &= 0xFEU;
     
    /* Initialise input port for software clock test */
      PM0.2 = 1U;       /* set port input pin to input */
      PMC0.2 = 0U;      /* Set port I/O mode to digital input */
      P0.2 = 0U;        
      CKS0  |= 0x80U;         /* Enable Clock Output (Port 14 pin 0) (32KHz) */
     

    /* Initialise Interval Timer
       Interval timer tused for LED on/off timing delay. Uses 32KHz sub clock */      

      RTCEN = 1U;	      /* supply RTC clock */
      OSMC  = 0U;             /* Sub system clock selected */
      ITMC  = 0U;             /* disable ITMC operation */
      ITMK  = 1U;	      /* disable INTIT interrupt */
      ITIF  = 0U;	      /* clear INTIT interrupt flag */
      ITMC = 0x0FFFU;         /* Set interval time to */

// interrupt test
#ifdef INTERRUPTS_test
	R_INTC_Create();
#endif

// port test
#ifdef PORT_test
	R_PORT_Create();
#endif

//ADC test
#ifdef ADC_test
	R_ADC_Create();
	
#endif

#ifdef UART0_test
	R_SAU0_Create();
#endif
   return(1);
}
