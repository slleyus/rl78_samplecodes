/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2010, 2011 Renesas Electronics Corporation. All rights reserved.
* History : DD.MM.YYYY Version Description
*         : 21.05.2012 1.00    First Release
*         : 06.06.2011 1.10    Modified float calculation g_measured_tempf
*         
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : Applilet3 for RL78/G13 V1.03.00 [15 Sep 2011]
* Device(s)    : R5F101LE
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements main function.
* Creation Date: 12/16/2011
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
#include "lcd.h"
#include "utility.h"
#include "r_cg_userdefine.h"
#include "fdl_types.h"
#include "fdl.h"
#include "fdl_descriptor.h"
/* End user code. Do not edit comment generated here */


/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0xEFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0xFFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xE8U;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x84U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] = 
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

/* Start user code for global. Do not edit comment generated here */
uint16_t  g_adc_Temperature; 
uint16_t  g_adc_IntRef;
float g_measured_tempf;
uint16_t g_measured_temp_avg;
uint16_t g_measured_tempi;
int16_t  g_tempv_int;
uint16_t  g_cal_temp_avg;
int8_t    g_cal_offset;
int16_t  g_temp_cal_data;
uint32_t tmp1;
int16_t tmp2;
extern uint8_t g_adc_ConvertFlag;

#define T_CAL_INDEX (0)
#define SENSOR_REF_TEMP (25)
#define INT_REF_V (1.45f)
#define INT_REF_TEMP (1.05f)
#define TEMP_SENSOR_GAIN (-0.0036f)
#define SENSOR_REF_TEMP_SCALED (250)
#define INT_REF_V_SCALED (145000L)
#define INT_REF_TEMP_SCALED (105000L)
#define TEMP_SENSOR_GAIN_SCALED (36) 
#define ERASED_VALUE (0XFFFFFFFF)

static void get_temperature(void); 
static void display_temperature(uint16_t);
extern fal_u32 ReadDataFlash(fal_u16 index);
extern void WriteDataFlash(fal_u32 data, fal_u16 index);
extern void EraseDataFlash(fal_u16 index);
void do_temp_cal (void);

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void  main(void)
{
    /* Start user code. Do not edit comment generated here */
    fal_u32 temp_cal_flash;
    DFLEN = 1;       // Enable data flash access
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;
    
    /* Start RTC, Turn on ADC and Initialise LCD Display.  RTC and ADC were
    configured in r_systeminit.c */
    R_RTC_Start(); 
    R_ADC_Set_OperationOn();
    InitialiseDisplay();
    /* Check if there is calibration data in the data flash is erased (0XFFFF). 
    If data is 0Xffff jump to "do_temp_cal" routine.  If not copy contents
    to the global temperature calibration variable.  Note the data flash routine
    uses an unsigned value to get the data, however, the actual stored data is 
    signed*/
    temp_cal_flash = ReadDataFlash(T_CAL_INDEX);
    if (ERASED_VALUE == temp_cal_flash)
    {
        do_temp_cal ();
        temp_cal_flash = ReadDataFlash(T_CAL_INDEX);
    }
      
    /*  Allow user to force a re-calibration by pushing switches 3 and 2 at the
    same time coming out of Reset*/
    if (!SW3 && !SW2)
    {
     
        EraseDataFlash(T_CAL_INDEX); 
        do_temp_cal ();
        temp_cal_flash = ReadDataFlash(T_CAL_INDEX);
    }    
    /* update temperature calibration variable with flash calibration data */
    g_temp_cal_data = (int32_t) temp_cal_flash;
        
     
    DisplayString(LCD_LINE1, "Cal Temp" ); 
    while (1U)
    {
        if (g_adc_ConvertFlag)
        {
        get_temperature();
        display_temperature(g_cal_temp_avg);
        }
    }
        
}     
        
/***********************************************************************************************************************
* Function Name: get_temperature
* Description : This function reads the internal temperature and voltage sensor
                and converts the ADC readings to a temperature reading. 
                The 10 bit internal voltage and temperature sensors are each 
                read 4 times then that result is divided by 2 to provide an
                11 bit value.  The function also provides a single point
                calibration adjustment and a running average filter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void get_temperature()
{
    uint8_t i;
    g_adc_Temperature = 0;
    g_adc_IntRef = 0;


/* setup to measure internal voltage reference */
        
    for (i = 0; i<16;i++)
    {
         ADS = _81_AD_INPUT_INTERREFVOLT; 
        R_ADC_Start();
        /* Wait for the A/D conversion to complete */
        while(ADIF == 0);
        /* Clear ADC interrupt flag */
        ADIF = 0;
        g_adc_IntRef += (ADCR>>6) ;
     }

    /*measure internal temperature sensor */
     for (i = 0; i<16;i++)
     {
        /* Start an A/D conversion */
         ADS = _80_AD_INPUT_TEMPERSENSOR_0;
         R_ADC_Start();
     
        /* Wait for the A/D conversion to complete */
        while(ADIF == 0);
      
        /* Clear ADC interrupt flag */
        ADIF = 0;
      
        /* Shift the ADC result contained in the 16-bit ADCR register since data
           is left justified after conversion*/
          g_adc_Temperature += (ADCR>>6) ;
     }     
      
     /* decimate the accumulated ADC readings to equivalent 12 bit data */ 
     g_adc_IntRef= g_adc_IntRef>>2;
     g_adc_Temperature = g_adc_Temperature >>2;
  
     /* convert the ADC readings to a temperature using float math.  */
//     g_measured_tempf = ((INT_REF_V  * (float) g_adc_Temperature / g_adc_IntRef) - INT_REF_TEMP) / TEMP_SENSOR_GAIN;
//     g_measured_tempf += SENSOR_REF_TEMP;
//     g_measured_tempi = (uint16_t)(10 *g_measured_tempf);
  
    /* convert the ADC readings to a temperature using integer math. The internal constants 
      Result is temperature * 10 to allow storing an int so the 25C reference is 250 in the second line below */
      g_tempv_int = (int16_t)(((INT_REF_V_SCALED) *g_adc_Temperature / g_adc_IntRef)-(INT_REF_TEMP_SCALED)) ;
      g_measured_tempi =  (uint16_t)( -(g_tempv_int/ (TEMP_SENSOR_GAIN_SCALED)) + SENSOR_REF_TEMP_SCALED);

     /* provide a running average filter on the temperature measurement.  The "if" code checks the filtered value
       and fills the filter result with the initial reading to make the filter converge faster on startup.
       It can be eliminated if ramp up is not an issue */
    if (!g_measured_temp_avg) 
    {
        g_measured_temp_avg = g_measured_tempi;
    }
    else
    {
        g_measured_temp_avg = (9*g_measured_temp_avg + g_measured_tempi)/10;
    }
 
    g_cal_temp_avg =g_measured_temp_avg + g_temp_cal_data; 
    LED2 ^=1;    
    g_adc_ConvertFlag = 0;

  
}

    /* End user code. Do not edit comment generated here */


/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: display_temperature
* Description : This function displays the temperature on the LCD screen
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void display_temperature(uint16_t temperature)
{
    uint8_t disp_temp[7];
    uint8_t i;
    uint8_t j;

    for (i=0; i<6;i++)
    {
        disp_temp[i] = 32;
    }
    uint16_ToBCDString(disp_temp, 0,temperature);
    for (j= 1;j<4;j++)
    {
       disp_temp[j-1] =  disp_temp[j];
    }
    disp_temp[3] = 0x2E;
    disp_temp[6] = 0x43;
    DisplayString(LCD_LINE2 + 1, disp_temp);
      
    
    __no_operation();
}

/***********************************************************************************************************************
* Function Name: do_temp_cal
* Description  : Prompts user to perform calibration step on device.  SW3 increments, SW2 decrements actual temperature.
*                SW1 indicates the user is done inputting actual temperature.  The data is then stored 
*                to the data flash
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void do_temp_cal (void)
{
    uint16_t actual_temp;
    uint32_t t_cal_offset;
    uint8_t i;
   /* run get_temperature routine to stabilize the average temperature value*/
    for (i= 0;i<16;i++)
    {
        get_temperature();
    }
    actual_temp = 220;
    DisplayString(LCD_LINE1, "Cal Mode " );
    display_temperature(actual_temp);
    while (SW1)
    {
        if (!SW2)
        {
            actual_temp--;
            display_temperature(actual_temp);
            while (!SW2);   // wait for switch to be released
        }   
        if (!SW3)
        {
            actual_temp++;
            display_temperature(actual_temp);
            while (!SW3);   // wait for switch to be released
        } 
        
    }
    t_cal_offset = (uint32_t) (actual_temp - g_measured_temp_avg);
    WriteDataFlash( t_cal_offset,T_CAL_INDEX);

}

/* End user code. Do not edit comment generated here */




