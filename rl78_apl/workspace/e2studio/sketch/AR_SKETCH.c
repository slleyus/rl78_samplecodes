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
* Copyright (C) 2011, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : AR_SKETCH.c
* Version      : V1.00
* Device(s)    : R5F104ML(G14-FPB)
* Tool-Chain   : CCRL
* Description  : This file implements the Arduino sketch.
* Creation Date: 2020/01/01
***********************************************************************************************************************/

/***********************************************************************
Includes
************************************************************************/
#include "r_cg_macrodriver.h"
#include <AR_LIB_PORT.h>
#include <AR_LIB_TIME.h>
#include <AR_SKETCH.h>
#include "r_cg_userdefine.h"

/***********************************************************************
Global variables and functions
************************************************************************/

/*---------------------------------------------------------------------
     Definition area. Write pin definition here.
----------------------------------------------------------------------*/
int segPinA = 0;                // assign D0 pin to segPinA for 7SEG_LED.
int segPinB = 1;                // assign D1 pin to segPinB for 7SEG_LED.
int segPinC = 2;                // assign D2 pin to segPinC for 7SEG_LED.
int segPinD = 3;                // assign D3 pin to segPinD for 7SEG_LED.
int segPinE = 4;                // assign D4 pin to segPinE for 7SEG_LED.
int segPinF = 5;                // assign D5 pin to segPinF for 7SEG_LED.
int segPinG = 6;                // assign D6 pin to segPinG for 7SEG_LED.
int segPinDP = 7;               // assign D7 pin to segPinDP for 7SEG_LED.

int comPin0 = 8;                // assign D8 pin to comPin0 for 7SEG_LED.
int comPin1 = 9;                // assign D9 pin to comPin1 for 7SEG_LED.
int comPin2 = 10;               // assign D10 pin to comPin2 for 7SEG_LED.
int comPin3 = 11;               // assign D11 pin to comPin3 for DOT_LEDs.

int ex_swPin = 13;              // assign D13 pin to ex_swPin for external SW.
int swPin = 18;                 // assign D18 pin to swPin for SW_USER.


const int SEG_TABLE[]=
{                               // segment data table for 7seg_LED
    0x3F,                       // data "0"
    0x06,                       // data "1"
    0x5B,                       // data "2"
    0x4F,                       // data "3"
    0x66,                       // data "4"
    0x6D,                       // data "5"
    0x7D,                       // data "6"
    0x27,                       // data "7"
    0x7F,                       // data "8"
    0x6F,                       // data "9"
    0x00                        // data " "
};

int old_time = 0x0000;          // previous time(milli sec.)

char mini_data = 0;             // minute data 0:1:3:7:F:1F:3F:7F:FF
char sec_data = 0;              // second count data(0 to 59)
char precount1 = 0;             // prescaler for 100milli seconds.(25)
char precount2 = 0;             // prescaler(10) for 1second
char digi_sel = 0;              // digit select

char prescale4sw = 0;           // prescaler for SW check( up to 5:20milli sec.)
int sw_data = 0xFFFF;           // SW data image(history of 20 milli sec. interval)
int time_mode = 0;              // o:stop/1:run

void set_SEG( char );           // set segment data
/***********************************************************************
* Sketch area
* Description  : This area is for sketch of arduino.
* setup        : setup program area executed once only
* loop         : main procedure
************************************************************************/

/*---------------------------------------------------------------------
     Arduino setup area. Write setup program here.
----------------------------------------------------------------------*/

void setup(void){
  // put your setup code here, to run once:
    pinMode(segPinA, OUTPUT);           // set D0pin to output mode
    pinMode(segPinB, OUTPUT);           // set D1pin to output mode
    pinMode(segPinC, OUTPUT);           // set D2pin to output mode
    pinMode(segPinD, OUTPUT);           // set D3pin to output mode
    pinMode(segPinE, OUTPUT);           // set D4pin to output mode
    pinMode(segPinF, OUTPUT);           // set D5pin to output mode
    pinMode(segPinG, OUTPUT);           // set D6pin to output mode
    pinMode(segPinDP, OUTPUT);          // set D7pin to output mode
    pinMode(comPin0, OUTPUT);           // set D8pin to output mode
    pinMode(comPin1, OUTPUT);           // set D9pin to output mode
    pinMode(comPin2, OUTPUT);           // set D10pin to output mode
    pinMode(comPin3, OUTPUT);           // set D11pin to output mode
    pinMode(ex_swPin, INPUT_PULLUP);    // set D13pin to input mode
    pinMode(swPin, INPUT);		        // set D18pin to input mode
}


/*---------------------------------------------------------------------
     Arduino loop area. Write the program in loop function here.
----------------------------------------------------------------------*/

void loop(void){
  // put your main code here, to run repeatedly:

    int time_work;                      // present time buffer
    char com_sel;                       // common select signal
    char seg_data;                      // segment data
    char sw_work;                       // work for switch check

/*--------------------------------------
    wait for 4milli seconds interval.
----------------------------------------*/

    time_work = ( int )( millis() & 0x0FFC ); // read milli sec data

    if ( old_time != time_work )        // check timing of change LED
    {

/*--------------------------------------
    every 4milli seconds timing
    change display digit.
----------------------------------------*/

        old_time = time_work;           // change current time data
        time_work &= 0x00C;             // get comsel data(= 4milli second unit)
    
        switch ( time_work )
        {
            case 0x00:                  // 10second digit timing
                digitalWrite(comPin3,LOW);  // turn off minute digit
                seg_data = SEG_TABLE[(sec_data / 10)]; // get 10second digit data
                set_SEG( seg_data );    // set segment data
                com_sel = comPin0;      // set 10second digit
                break;

            case 0x04:                  // second digit timing
                digitalWrite(comPin0,LOW);  // turn off 10second digit
                seg_data = SEG_TABLE[(sec_data % 10)]; // get second digit data
                set_SEG( ( seg_data + 0x80 ) ); // set segment data
                com_sel = comPin1;      // set second digit
                break;

            case 0x08:                  // second digit timing
                digitalWrite(comPin1,LOW);  // turn off second digit
                seg_data = SEG_TABLE[precount2]; // get second digit data
                set_SEG( seg_data );    // set segment data
                com_sel = comPin2;      // set 100milli second digit
                break;

            default:                    // minute digit timing
                digitalWrite(comPin2,LOW);  // turn off second digit
                seg_data = mini_data;   // get minute display data
                set_SEG( seg_data );    // set segment data
                com_sel = comPin3;      // set minute digit


        }
        digitalWrite(com_sel,HIGH);     // enable display

/*--------------------------------------
    every 4milli seconds timing
    if count enable, then count time up.
----------------------------------------*/

        if (( 25 <= ++precount1 ) && ( 1 == time_mode ))
        {                               // 100 milli seconds passing
            precount1 = 0;              // clear 100milli second counter
            if (  10 <= ++precount2 )   // count for seconds
            {                           // one second passing
                precount2 = 0;          // clear one second counter
                if ( 60 <= ++sec_data )
                {                       // one minute passing
                    sec_data = 0;
                    if ( 0xFF == mini_data )
                    {                   // over flow
                        mini_data = 0;  // clear minute data
                    }
                    else
                    {                   // change minute data
                        mini_data <<= 1; // shift 1bit left and
                        mini_data++;    // set bit0
                    }
                }
            }
        }

/*--------------------------------------
    switch data check timing
----------------------------------------*/

        if ( ++prescale4sw >= 5 )       // check SW check timing or not
        {                               // 20milli seconds passing
            prescale4sw = 0x00;         // clear timing counter
            sw_data <<= 1;              // shift olddata left
            sw_data += ( digitalRead(ex_swPin)&digitalRead(swPin) ); // read sw data
            sw_work = (sw_data & 0x06); // extract check timing data
            if ( 0x04 == sw_work )
            {                           // sw is pushed
                time_mode ^= 0x01;      // change count mode flag
            }
        }
    
    }
}

/*----------------------------------------------
    set segment data function
------------------------------------------------*/
void set_SEG( char data )
{
    char work_data;

    work_data = data;                   // set segment data

    digitalWrite(segPinA,( work_data & 0x01 )); // set segmentA data
    work_data >>= 1;                    // move next bit
    digitalWrite(segPinB,( work_data & 0x01 )); // set segmentB data
    work_data >>= 1;                    // move next bit
    digitalWrite(segPinC,( work_data & 0x01 )); // set segmentC data
    work_data >>= 1;                    // move next bit
    digitalWrite(segPinD,( work_data & 0x01 )); // set segmentD data
    work_data >>= 1;                    // move next bit
    digitalWrite(segPinE,( work_data & 0x01 )); // set segmentE data
    work_data >>= 1;                    // move next bit
    digitalWrite(segPinF,( work_data & 0x01 )); // set segmentF data
    work_data >>= 1;                    // move next bit
    digitalWrite(segPinG,( work_data & 0x01 )); // set segmentG data
    work_data >>= 1;                    // move next bit
    digitalWrite(segPinDP,( work_data & 0x01 ));// set segmentDP data

}
/*---------------------------------------------------------------------
     End of sketch area
----------------------------------------------------------------------*/

