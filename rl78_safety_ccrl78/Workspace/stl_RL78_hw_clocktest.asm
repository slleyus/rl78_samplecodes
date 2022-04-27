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
;* File Name  : stl_RL78_hw_clocktest.asm
;* Version    : 2.00
;* Device(s)  : RL78/G14
;* Tool-Chain : RL78 CC-RL V1.03.00
;* OS         : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library hardware system clock test module
;
;     This module tests the operation and accuracy of the sytem clock
;     An external reference clock is input to TAU channel 5. 
;     The timer is clocked with the system clock
;     The measured value is compared to defined reference limits
;     Register "C" returns 0x01 if a fault was detected, otherwise
;     0x00 is returned
;
;     A "No reference clock" detection is provided to ensure a return
;     If no reference signal is detected the return value is 0x02
;     
;     The reference compare valuea are all defined in the stl.h header file
;
;* Limitations : The measured and reference values need to be adjusted to 
;                match the following
;                - Frequency and accuracy of the reference clock
;                - Frequency of the system clock
;*******************************************************************************
;* History :
;* 30.05.2011 V1.00 First Release
;* 15.11.2013 V1.10 Fixed comments and line 95
;* 07.11.2016 V2.00 CC-RL Tool-Chain
;* 08.02.2018 V2.00 VDE Released
;*******************************************************************************


        .PUBLIC  _stl_RL78_hw_clocktest

;*******************************************************************************
; Includes <System Includes> , "Project Includes"
;*******************************************************************************
$include (stl_clocktest.h)

        .CSEG

;******************************************************************************
;* Outline : stl_RL78_hw_clocktest - Initial RL78 IEC March_x test module
;* Include : stl_clocktest.h, ior5f100le.h, ior5f100le_ext.h
;* Declaration : stl_RL78_hw_clocktest:
;* Function Name: stl_RL78_hw_clocktest
;* Description : Initial stl_RL78_hw_clocktest code module
;* Argument : #swMINTIME Minimum allowable measurement value
;*            #swMAXTIME Maximum allowable measurement value
;* Return Value : Register A - Result staus (Pass = 0x00, Fail = 0x01, 
;*                No reference clock = 0x02)
;* Calling Functions : main.c
;******************************************************************************
;* Start of Hardware clocktest module
_stl_RL78_hw_clocktest:
        PUSH    PSW                       ; save interrupt state
        MOVW    AX, #0x0000               ; clear AX
        CLR1    CAPTURE_interrupt_FLAG    ; clear capture flag ( TMIF01)

; Syncronise to Capture
wait_for_first_capture: 
        INCW    AX                        ; No signal detection counter
        CMPW    AX, #0xFFFF
        BZ      $clock_error1                                      ; Over flow no signal detected rturn with error
        BF      CAPTURE_interrupt_FLAG, $wait_for_first_capture    ; wait for low signal
        CLR1    CAPTURE_interrupt_FLAG                             ; clear capture flag (TMIF01)
        MOVW    AX, #0x0000                                        ; clear AX
;

; Synchronised capture time value for measurement
wait_for_second_capture: 
        BF      CAPTURE_interrupt_FLAG, $wait_for_second_capture   ; wait for high signal
        MOVW    AX, TDR01                                          ; Read Captured value
        CLR1    CAPTURE_interrupt_FLAG                             ; clear capture flag (TMIF01)


;check lower frequency limit
        CMPW    AX, #hwMINTIME            ; check lower limit value
        BNH     $clock_error2             ; Branch if error

;check higher frequency limit
        CMPW    AX, #hwMAXTIME            ; check maximum limit value
        BH      $clock_error2
        MOV     A, #0x00                  ; test status TRUE, counter in range
        BR      $finished

;set test result if error occured
clock_error1:
        MOV     A, #0x02                  ; Set NO reference clock source detected
        BR      $finished

clock_error2:
        MOV     A, #0x01                  ; Set System clock timng errro

finished:
        POP     PSW                       ; restore interrupt state
        ret
