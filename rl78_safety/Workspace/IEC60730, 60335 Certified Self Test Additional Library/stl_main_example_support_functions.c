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
* File Name : stl_main_example_support_functions.c
* Version : 1.00
* Device(s) : RL78 (Test device used RL78/G13)
* Tool-Chain : RL78 IAR Embedded Workbench V1.10.1
* OS : None
* H/W Platform : RL78/G13 Target Board
* Description : RL78 IEC Self Test Library Main test harness support functions
* Operation : This module provides support function for the main test harness
*             modules (main.c) by providing LED control and delay functions
* Limitations : None
********************************************************************************
* History :
* 30.05.2011 V1.00 First Release
*******************************************************************************/


/*******************************************************************************
Includes <System Includes> , “Project Includes”
*******************************************************************************/
//#include <intrinsics.h>
//#include <ior5f100le.h>
//#include <ior5f100le_ext.h>
//#include <stdbool.h>
//#include <stdint.h>


#pragma SFR
#pragma NOP
/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
******************************************************************************/ 
unsigned int J;
unsigned char g_referenceCalc_Done;

/******************************************************************************
 modul global variables and functions 
******************************************************************************/ 
#if (hw_peripheral_crc_test_enabled==1)
static unsigned short crc_table[256];
static unsigned short poly = 0x1021U;
static void GenerateCRCTable(void);
static unsigned char reverse8(unsigned char value);
static unsigned char reverse16(unsigned short value);
#endif
       
/******************************************************************************
Imported global variables and functions (to be accessed by other files)
******************************************************************************/
extern char g_resultBool;

/*******************************************************************************
Function Prototypes
*******************************************************************************/
void LED_Display_Time_Delay (unsigned char num);
void indicate_test_result (char resultBool);
unsigned short reference_crc_calculation(unsigned short crc_input, unsigned int length, unsigned char __far *p);

/* macros */
#define IDBU_TB

/******************************************************************************
* Outline : LED_Display_Time_Delay Software delay function
* Include : intrinics.h, ior5f100le.h, ior5f100le_ext.h, stdbool.h, stdint.h
* Declaration : void LED_Display_Time_Delay (void)
* Function Name: LED_Display_Time_Delay
* Description : RL78 IEC Target board LED software delay function.
* Argument : none
* Return Value : none 
* Calling Functions : stl_main_example_support_functions.c, main.c, cstartup.s87
******************************************************************************/
void LED_Display_Time_Delay (unsigned char num)
{
	for(J=0; J<65000; J++)
	{
		NOP();
		NOP();
		NOP();
		NOP();
	}
}

/******************************************************************************
* Outline : indicate_test_result (bool g_resultBool)
* Include : intrinics.h, ior5f100le.h, ior5f100le_ext.h, stdbool.h, stdint.h
* Declaration : void LED_Display_Time_Delay (void)
* Function Name: LED_Display_Time_Delay
* Description : General routine used to provide a hardware indication of the 
                test result. Light the appropriate LED depending on the status

     g_resultBool = 0x00 LED1 is lit if the test result is a pass (LED2 is Off)
     g_resultBool = 0x01 LED2 is lit if the test result is a fail (LED1 is Off)
     g_resultBool = 0x02 Both LED's are lit as NO reference clock is detected for 
                    the System clock test modules (Hardware and Software)

* Argument : g_resultBool - Test result status
* Return Value : none 
* Calling Functions : stl_main_example_support_functions.c, main.c, cstartup.s87
******************************************************************************/      
void indicate_test_result (char resultBool)
{

#ifdef IDBU_TB

  unsigned char result;               /* Local variable used for the system clock test */
  
  result = resultBool;          /* Save result values for LED Pass / Fail selection */
  
      if (result == 0U)         /* Switch Test result LED(s)   Pass / Fail */
      {
          P6.2 = 0U;      /* Test Pass Led1 ON */
          P6.3 = 1U;      /* Test Pass Led2 Off */
      }
      else                      /* Test failed */
      {          
        if (result == 1U)       /* Normal Test failure */
        {
            P6.2 = 1U;    /* Test failed Led1 Off */
            P6.3 = 0U;    /* Test failed Led2 On */
        }
        else                    /* Result = 0x02 => No reference signal for Clock test modules */
        {
          P6.2 = 0U;      /* Led1 On */
          P6.3 = 0U;      /* Led2 On */
        }
      }
        
#else /* Application Board */
      
      /* switch Test result LED */
      if (g_resultBool == 0x00)   
      {
          P5 = 0x01;            /* test OK  -  green LED */
      }
      else
      {
          P5 = 0x02;            /* test not OK  -  red LED */
      }
#endif


    LED_Display_Time_Delay (2U);


#ifdef IDBU_TB

        P6.2 = 1U;        /* Turn both LED's off */
        P6.3 = 1U;
#else
        P5  = 0x00;           /* clear Status LED  */
#endif

          g_resultBool = 0x00;  /* clear error indicator */
}



#ifdef hw_peripheral_crc_test_enabled
/******************************************************************************
* Outline : GenerateCRCTable();
* Include : intrinics.h, ior5f100le.h, ior5f100le_ext.h, stdbool.h, stdint.h
* Declaration : void LED_Display_Time_Delay (void)
* Function Name: GenerateCRCTable
* Description : Generates a reference CRC table for use with the Hardware 
                peripheral CRC to the defined polynomial (poly)
* Argument : none
* Return Value : none 
* Calling Functions : stl_main_example_support_functions.c
******************************************************************************/      
static void GenerateCRCTable(void)
{
      /* Temporary Variables */
      unsigned short temp, a;
      
      /* Create table to Polynomial supplied in this file */
      for (unsigned short i = 0U; i < 256U; ++i)
      {
          temp = 0U;
          a = (unsigned short)(i << 8U);
          for (unsigned short j = 0; j < 8; ++j)
          {
              if (((temp ^ a) & 0x8000) != 0U)
              {
                  temp = (uint16_t)((temp << 1U) ^ poly);
              }
              else
              {
                  temp <<= 1U;
              }
              a <<= 1;
          }
          crc_table[i] = temp;      /* Add value to reference CRC table */
      }
}
#endif

#ifdef hw_peripheral_crc_test_enabled
/******************************************************************************
* Outline : reverse16(uint16_t value);
* Include : intrinics.h, ior5f100le.h, ior5f100le_ext.h, stdbool.h, stdint.h
* Declaration : uint16_t reverse16(uint16_t value)
* Function Name: reverse16
* Description : Bit reverse the 16 bit input value 
* Argument : uint16_t value - 16bit input value
* Return Value : uint16_t valueout - Bit swapped 16bit value
* Calling Functions : stl_main_example_support_functions.c
******************************************************************************/      
static unsigned short reverse16(unsigned short value)
{
    /* Temporary Variables */
    unsigned short i, j = 1U, valueout = 0U;

   /* Function to bit swap the 16bit input value */
   for (i = (unsigned short)1 << 15; i != 0U; i >>= 1U)
   {
       if ((value & i) != 0U)
       {
           valueout |= j;
       }
           j <<= 1;
   }
    
   return (valueout);               /* Return bit swapped value */
}
#endif


#ifdef hw_peripheral_crc_test_enabled
/******************************************************************************
* Outline : reverse8(uint8_t value);
* Include : intrinics.h, ior5f100le.h, ior5f100le_ext.h, stdbool.h, stdint.h
* Declaration : uint16_t reverse16(uint16_t value)
* Function Name: reverse16
* Description : Bit reverse the 8 bit input value 
* Argument : uint8_t value - 8 bit input value
* Return Value : uint8_t valueout - Bit swapped 8 bit value
* Calling Functions : stl_main_example_support_functions.c
******************************************************************************/      
static unsigned char reverse8(unsigned char value)
{
   /* Temporary Variables */
   unsigned char i, j = 1U, valueout = 0U;
    

   /* Function to bit swap the 16bit input value */
   for (i = (unsigned short)1 << 7U; i != 0U; i >>= 1U)
   {
        if ((value & i) != 0U)
        {
            valueout |= j;
        }
        j <<= 1;
   }

   return (valueout);               /* Return bit swapped value */
}
#endif

#ifdef hw_peripheral_crc_test_enabled
/******************************************************************************
* Outline : reference_crc_calculation
* Include : intrinics.h, ior5f100le.h, ior5f100le_ext.h, stdbool.h, stdint.h
* Declaration : uint16_t reference_crc_calculation(uint16_t crc_input, uint32_t length, uint8_t __far *p)
* Function Name: reference_crc_calculation
* Description : Calculate reference CRC value of the specified memory area
                The CRC generation is compatible with the RL78 CRC peripheral 
                (Peripheral mode)
* Argument : uint16_t crc_input - Starting CRC Value
             uint32_t length - Length of CRC calculation range 
             uint8_t __far *p - Start address pointer
* Return Value : uint16_t crc_calc - Calculated reference CRC result value
* Calling Functions : main.c
******************************************************************************/      
unsigned short reference_crc_calculation(unsigned short crc_input, unsigned int length, unsigned char __far *p)
{
   /* Temporary Variables */
   unsigned char byte = 0U;
   unsigned int i;
   unsigned short crc_calc;
      
   /* Generate reference table */
   GenerateCRCTable();


   /* Buffer initial CRC start value*/
   crc_calc = crc_input;
      

    /* Calculate CRC */
    for ( i = 0U; i < length; ++i)
    {
         #pragma diag_suppress=Pm088
         byte = *p++;
         #pragma diag_default=Pm088
         byte = reverse8(byte);
         crc_calc = crc_table[(crc_calc >> 8) ^ byte] ^ (crc_calc << 8);
    }
        
    /* Bit Swap result */
    crc_calc = reverse16(crc_calc);

    g_referenceCalc_Done = 1U;      /* Set reference CRC calculation done flag */

    return (crc_calc);              /* Return calculated reference CRC value */
}
#endif

