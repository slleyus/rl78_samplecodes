;*******************************************************************************
;* DISCLAIMER
;* This software is supplied by Renesas Electronics Corporation and is only
;* intended for use with Renesas products. No other uses are authorized. This
;* software is owned by Renesas Electronics Corporation and is protected under
;* all applicable laws, including copyright laws.
;* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
;* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
;* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
;* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
;* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
;* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
;* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
;* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
;* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
;* Renesas reserves the right, without notice, to make changes to this software
;* and to discontinue the availability of this software. By using this software,
;* you agree to the additional terms and conditions found by accessing the
;* following link:
;* http://www.renesas.com/disclaimer
;* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
;*******************************************************************************
;*******************************************************************************
;* File Name  : stl_RL78_sw_clocktest.asm
;* Version    : 2.00
;* Device(s)  : RL78/G14
;* Tool-Chain : RL78 CC-RL V1.03.00
;* OS         : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library software system clock test module
;
;     This module tests the operation and accuracy of the system clock
;     An external reference clock is input to the "TESTPORT" input pin.
;     A software counter is used to measure the system clock
;     The measured value is compared to predefined reference limits
;     Register "C" returns 0x01 if a fault was detected, otherwise
;     0x00 is returned
;     A "No reference clock" detection is provided to ensure a return
;     If no reference signal is detected the return value is 0x02
;     
;     The input port pin and reference compare values are all defined in 
;     the stl.h header file
;
;* Limitations : The measured and reference values need to be adjusted to 
;                match the following
;                - Frequency and accuracy of the reference clock
;                - Frequency of the system clock
;*******************************************************************************
;* History :
;* 30.05.2011 V1.00 First Release
;* 15.11.2013 V1.10 Fixed comments
;* 07.11.2016 V2.00 CC-RL Tool-Chain
;* 08.02.2018 V2.00 VDE Released
;*******************************************************************************

    .PUBLIC  _stl_RL78_sw_clocktest


;*******************************************************************************
; Includes <System Includes> , "Project Includes"
;*******************************************************************************
$INCLUDE (stl_clocktest.h)


;******************************************************************************
;* Outline : stl_RL78_sw_clocktest RL78 IEC Software clock test module
;* Include : stl_clocktest.h, ior5f100le.h, ior5f100le_ext.h
;* Declaration : stl_RL78_sw_clocktest:
;* Function Name: stl_RL78_sw_clocktest
;* Description : IEC Software Clock test code module
;* Argument : TESTPORT - Definition of Signal Input Port pin
;*            #swMINTIME Minimum allowable measurement value
;*            #swMAXTIME Maximum allowable measurement value
;* Return Value : Register A - Result status (Pass = 0x00, Fail = 0x01, No reference clock = 0x02)
;* Calling Functions : main.c
;******************************************************************************
;* Start of software clock test module
_stl_RL78_sw_clocktest:
        PUSH    PSW                             ; save interrupt state
        MOVW    AX, #0x0000                     ; clear software counter (AX)

;* Synchronise to reference signal
wait_for_low:
        INCW    AX                              ; No signal detection counter
        CMPW    AX, #0xFFFF
        BZ      $clock_error1                   ; Over flow - no signal detected return with error
        BT      TESTPORT, $wait_for_low         ; wait for low signal
        MOVW    AX, #0x0000                     ; Signal detected, clear measurement counter
        
;* Synchronised now wait for next edge transition of reference signal        
wait_for_high:
        INCW    AX                              ; No signal detection counter
        CMPW    AX, #0xFFFF
        BZ      $clock_error1                   ; Over flow no signal detected return with error
        BF      TESTPORT, $wait_for_high        ; if high signal, start counting
        MOVW    AX, #0x0000                     ; Signal detected, clear measurement counter

;* Measure reference signal high period
measure_high_time:
        INCW    AX
        BT      TESTPORT, $measure_high_time    ; check port status

;check lower frequency limit                                         
        CMPW    AX, #swMINTIME
        BNH     $clock_error2                   ; check lower limit
         
;check upper frequency limit                                               
        CMPW    AX, #swMAXTIME                  ; Maximum limit into AX                          
        BH      $clock_error2
        MOV     A, #0x00                        ; test status counter in range    
        BR      $finished
        
;set test status if error occurred  
clock_error1:
        MOV     A, #0x02                        ;  no reference signal detected 
        BR      $finished
        
clock_error2:                                               
        MOV     A, #0x01                        ; counter result out of range 
        
finished:    
        POP     PSW                             ; restore interrupt state
        ret
