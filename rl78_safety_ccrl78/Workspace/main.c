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
*******************************************************************************/
/*******************************************************************************
* File Name : main.c
* Version : 2.00
* Device(s) : RL78 (Test device used RL78/G14)
* Tool-Chain : RL78 CC-RL V1.03.00
* OS : None
* H/W Platform : RL78/G14 Target Board
* Description : RL78 IEC Self Test Library Main test harness API module
* Operation : This module provides the IEC60730/60335 API test harness for the
*             test modules. The appropriate test is selected by the options 
*             set in the Cubesuite project set-up, pre-processor options menu
*
* Limitations : None
********************************************************************************
* History :
* 30.05.2011 V1.00 First Release
* 15.11.2013 V1.10 Second Release
* 07.11.2016 V2.00 CC-RL Tool-Chain
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "iodefine.h" 
#include "globaldefines.h"
#include "stl.h"
#include "main.h"
#include "RL78_gpp.h"


/* General Variables definitions used by support functions */
char g_resultBool;

/* Variable definitions used for RAM Test modules */
unsigned char __far *g_pMC_StAdr;
unsigned short g_RAM_MarchC_lgth;

unsigned char __far *g_pMX_StAdr;
unsigned short g_RAM_MarchX_lgth;

/* Variable definitions used for CRC Tests modules */
extern unsigned short g_crc;
extern unsigned short g_reference_crc;
extern unsigned short g_crcASM;
extern unsigned char __far *g_pStAdr;
extern unsigned int g_crc_length;
extern unsigned char g_referenceCalc_Done;

/* CRC test range variable structure */ 
	
static CHECKSUM_CRC_TEST_AREA checksum_crc;



    
void indicate_test_result (char resultBool);

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
* Calling Functions : startup.asm
******************************************************************************/
  void 	main( void )
{


	unsigned short MyChecksum;

	__DI();

    /* Define crc reference location for hardware background CRC execution (Defined by ????? */  

 
    g_referenceCalc_Done  = 0U;          /* Set reference CRC calculation not done */


    
#ifdef IDBU_TB   /* Use RL78/G14 Target board as test hardware */

    /* Start Up - Flash both RL78 target board LEDs */
	P6_bit.no2 = 0U;                     /* Turn LED1 On */
	P6_bit.no3 = 0U;                     /* Turn LED2 On */

	P6_bit.no2 = 1U;                     /* Turn LED1 Off */
	P6_bit.no3 = 1U;                     /* Turn LED2 Off */


#else    /* Use application board as test hardware */

    P6 = 0x00;                          /* white LED on */
    LED_Display_Time_Delay(3U);
    P6 = 0xFF;                          /* white LED off */
    LED_Display_Time_Delay(4U);

#endif


  while (1U)
  {

#ifdef hw_clock_test_enabled
/*******************************************************************************
  DESCRIPTION: stl_RL78_hw_clocktest
      Example of a system clock test using the on chip hardware (TAU ch5). 
      The system clock (32MHz) is measured during the period of the target board
      32KHz sub clock. The timer channel of the TAU is configured to measure the 
      Sub clock period and the capture value is compared against a reference window
      values, defined in stl_clocktest.h

  RETURN: 0 if CPU clock is operating within limits, 1 otherwise
*******************************************************************************/
    /* Execute clock test */
    g_resultBool = stl_RL78_hw_clocktest();     

    /* indicate result on RL78 target board LEDs */
    indicate_test_result( g_resultBool );     
    
    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U); 
 

#endif
    
    
#ifdef sw_clock_test_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_sw_clocktest
      Example of a system clock test. 
      The system clock (32MHz) is measured during the period of the target board
      32KHz sub clock. The Sub clock is output from the Clock/Buzzer output pin
	  and connected to the "TESTPORT" input pin defined in the stl_clocktest.h 
	  header file. The 32KHz sub clock period is measured by sampling the input
	  port pin with a software counter The result is compared against reference
	  window values, defined in stl_clocktest.h
	  
  RETURN: 0 if CPU clock is operating within limits, 1 otherwise
*******************************************************************************/
    /* Execute clock test */
    g_resultBool = stl_RL78_sw_clocktest();     

    /* indicate result on RL78 target board LEDs */
    indicate_test_result( g_resultBool );     
    
    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);  
    
#endif
#endif
#endif
#endif
    

#ifdef sw_crc_test_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_sw_crc_asm
    This is a software implementation of the CRC 16 (CCITT) polynomial
    The function calculates the CRC of data starting at "start_address" over
	the range set in "length ". "g_crc" is the starting CRC value. The module will 
	return the new CRC of the specified memory area for verification against a 
	reference value.

  INPUT: start address (checksum_crc.start_address)
         Test end address (length) (checksum_crc.length )
		 
  RETURN: CRC result over the specified Flash area
*******************************************************************************/
    IAWCTL  |= 0x80U;                             /* Illegal access detection setting */
                                                  /* Bit 7 = 0 Detection Disabled */ 
                                                  /* Bit 7 = 1 Detection enabled */

    g_crc = 0x0000U;                             /* Set CRC start value */

    checksum_crc.start_address = 0x00003000U;    /* CRC start address */
    checksum_crc.length        = 0x0000C7FEU;    /* Set CRC end address (Length of calculation + 1) */
//    checksum_crc.length        = 0x0000FFFFU;    /* Set CRC end address (Length of calculation + 1) */
 // checksum_crc.length        = 0x00000000U;    /* Set CRC end address (Length of calculation + 1) */
    /* calculate CRC */
    g_crcASM = stl_RL78_sw_crc_asm(g_crc, &checksum_crc);  

   /* Check CRC result against calculated by Cubesuite */
    MyChecksum = 0xcc63; /* enter the externally calculated result here */

    if (g_crcASM  != MyChecksum)
	{
      g_resultBool = 1U;                          /* Fail CRC value does not match reference value calculated by Cubesuite */
    }
    else
    {
      g_resultBool = 0U;                          /* Pass CRC value matches reference value calculated by Cubesuite */
    }

    /* indicate result on RL78 target board LEDs */
    indicate_test_result( g_resultBool );

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U); 


#endif
#endif
#endif
#endif

    
#ifdef hw_peripheral_crc_test_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_peripheral_crc.asm
    This function calculates the cyclic redundancy check (CRC) of data
    starting at the address in "start_address" over the range set in "length". 
	The starting CRC value is set in "g_crc" The final CRC result is stored in 
	the "CRCD" register is returned for verification where it is compared 
	against a reference value.
  
  INPUT: start address (checksum_crc.start_address)
         Test end address (length) (checksum_crc.length )
  
  RETURN: CRC result over the specified Flash area
*******************************************************************************/
    IAWCTL  |= 0x80U;                             /* Illegal access detection setting */
                                                  /* Bit 7 = 0 Detection Disabled */ 
                                                  /* Bit 7 = 1 Detection enabled */

    g_crc   = 0x0000U;                            /* Clear CRC result variable */
    checksum_crc.start_address = 0x00003000U;     /* CRC start address */
//    checksum_crc.length        = 0x00002000U;     /* Set CRC end address (Length of calculation - 1) */
//   checksum_crc.length        = 0x00000000U;     /* Set CRC end address (Length of calculation - 1) */
    checksum_crc.length        = 0x00000FFFU;     /* Set CRC end address (Length of calculation - 1) */
    /* Execute peripheral CRC calculation */
    g_crcASM = stl_RL78_peripheral_crc(g_crc, &checksum_crc);

    /* calculate reference peripheral CRC  !! Do this only once !! */
    if(g_referenceCalc_Done == 0U)                /* Do if reference CRC not already calculated */
    {      
        g_crc = 0x0U;
        g_pStAdr =  (unsigned char __far *)checksum_crc.start_address;
        g_crc_length = checksum_crc.length;

        g_reference_crc  = reference_crc_calculation(g_crc, g_crc_length, g_pStAdr);
    }
    
    /* Check CRC result against calculated reference */    
    if (g_crcASM  != g_reference_crc)
    {
        g_resultBool = 1U;                        /* Fail CRC value does not match reference value calculated by Cubesuite */
    }
    else
    {
        g_resultBool = 0U;                        /* Pass CRC value matches reference value calculated by reference module */
    }

    /* indicate result on RL78 target board LEDs */
    indicate_test_result( g_resultBool );

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);


#endif
#endif
#endif
#endif   
        
    
#ifdef march_c_test_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_march_c
      stl_RL78_march_c test performs a March C test on the memory
      starting at address "g_pMC_StAdr" with the memory range set in "g_RAM_MarchC_lgth".
      This test is destructive, the memory content is not preserved, 
    
  INPUT: RAM start address (g_pMC_StAdr)
         RAM Test end address (test range) (g_RAM_MarchC_lgth)
    
  RETURN: 0 if no memory fault was detected, 1 otherwise
*******************************************************************************/
    g_pMC_StAdr = (unsigned char __far *) 0x000FF9D0;   /* Set RAM start address */
    g_RAM_MarchC_lgth = 0x310U;                               /* Set RAM test length (= end adress - start address */

	
    /* Execute March C test */
    g_resultBool = stl_RL78_march_c((unsigned char *)g_pMC_StAdr, (unsigned short)g_RAM_MarchC_lgth);

    /* indicate result on RL78 target board LEDs */
    indicate_test_result (g_resultBool);

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);

#endif
#endif
#endif
#endif 

#ifdef  march_x_test_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_march_x
      stl_RL78_march_x test performs a March X test on the memory
      starting at address "g_pMX_StAdr" with the range "g_RAM_MarchX_lgth". this test
      is destructive, the memory content is not preserved, 

  INPUT: RAM start address (g_pMX_StAdr)
         RAM Test end address (test range) (g_RAM_MarchX_lgth)

  RETURN: 0 if no memory fault was detected, 1 otherwise
*******************************************************************************/
    g_pMX_StAdr = (unsigned char __far *)0xFF9D0;       /* Set RAM start address */
    g_RAM_MarchX_lgth = 0x310U;//0x0500U;     /* Set RAM test length (= end adress - start address */
	

    /* Execute March X test */
    g_resultBool = stl_RL78_march_x((unsigned char *)g_pMX_StAdr, g_RAM_MarchX_lgth);

	
    /* indicate result on RL78 target board LEDs */
    indicate_test_result (g_resultBool);

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);

   
#endif
#endif
#endif
#endif 

#ifdef register_test_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_cpu_registertest
      performs a functional test of the CPU core register
      ( AX, BC, DE, HL - all four banks ). The test is non destructive,
      all registers are saved and restored on completion of the test function.

  RETURN: 0 if no memory fault was detected, 1 otherwise
*******************************************************************************/
    
	/* Execute Register test */
    g_resultBool = stl_RL78_registertest();
	

    /* indicate result on RL78 target board LEDs */
    indicate_test_result (g_resultBool);

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);



#endif
#endif
#endif
#endif 

#ifdef register_test_psw_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_cpu_registertest_psw
      performs a functional test of the PSW,  CPU core register .
      The test is non destructive, The PSW register content is saved and restored 
	  on the completion of the test function.

  RETURN: 0 if no memory fault was detected, 1 otherwise
*******************************************************************************/
    /* Execute PSW Register test */
    g_resultBool = stl_RL78_registertest_psw();
	

    /* indicate result on RL78 target board LEDs */
    indicate_test_result (g_resultBool);

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);

#endif
#endif
#endif
#endif 


#ifdef register_test_stack_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_cpu_registertest_stack
      performs a functional test of the SP, CPU core register .
      The test is non destructive, the stack register content is saved and 
	  restored on completion of the test function.

  RETURN: 0 if no memory fault was detected, 1 otherwise
*******************************************************************************/
      /* Execute Stack Register test */
    g_resultBool = stl_RL78_registertest_stack();
	

    /* indicate result on RL78 target board LEDs */
    indicate_test_result (g_resultBool);

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);



#endif
#endif
#endif
#endif 

  
#ifdef register_test_cs_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_cpu_registertest_cs
      Performs a functional test of the CS CPU core register .
      The test is non destructive, the CS register content is saved and 
      restored on completion of the test function.

  RETURN: 0 if no memory fault was detected, 1 otherwise
*******************************************************************************/
    /* Execute Stack Register test */
    g_resultBool = stl_RL78_registertest_cs();
	

    /* indicate result on RL78 target board LEDs */
    indicate_test_result (g_resultBool);

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);



#endif
#endif
#endif
#endif 

  

#ifdef register_test_es_enabled
#ifdef variant_G14
#ifdef flash_64
#ifdef pins_44
/*******************************************************************************
  DESCRIPTION: stl_RL78_cpu_registertest_es
      Performs a functional test of the ES CPU core register .
      The test is non destructive, the ES register content is saved and 
      restored on completion of the test function.

  RETURN: 0 if no memory fault was detected, 1 otherwise
*******************************************************************************/
    /* Execute Stack Register test */
    g_resultBool = stl_RL78_registertest_es();
	

    /* indicate result on RL78 target board LEDs */
    indicate_test_result (g_resultBool);

    /* Target Board LED on time delay */
    LED_Display_Time_Delay(3U);


#endif
#endif
#endif
#endif

  }  

}

