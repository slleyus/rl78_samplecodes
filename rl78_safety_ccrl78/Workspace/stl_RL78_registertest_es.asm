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
;* File Name : stl_RL78_registertest_es.asm
;* Version : 2.00
;* Device(s) : RL78 (RL78/G14)
;* Tool-Chain : RL78 CC-RL V1.03.00
;* OS : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library ES register test module
;                The data extension Register (ES) is tested and restored by this
;                self test module. The original register content are buffered 
;                on the Stack and restored on completion of the test
;
;* Limitations : Any dependency of the user application in that it might be
;                necessary to disable interrupts while testing.
;                The top 4 bits of the es register are fixed to "0"
;*******************************************************************************
;* History :
;* 30.05.2011 V1.00 First Release
;* 15.11.2013 V1.10 Second Release
;* 07.11.2016 V2.00 CC-RL Tool-Chain
;* 08.02.2018 V2.00 VDE Released
;*******************************************************************************

    .PUBLIC  _stl_RL78_registertest_es      ; Global Name Declaration

  	.DSEG SBSS
;******************************************************************************
; Private variables and functions
;******************************************************************************
;* Define error variable workspace and Flag bit
errorindicator:   .DS   1


        .CSEG

;******************************************************************************
;* Outline : stl_RL78_registertest_es - RL78 IEC ES Register test module
;* Include : none
;* Declaration : stl_RL78_registertest_es
;* Function Name: stl_RL78_registertest_es
;* Description : RL78 IEC ES Register test code module
;* Argument : none
;* Return Value : Register A - Result status (Pass = 0x00, Fail = 0x01)
;* Calling Functions : main.c
;******************************************************************************
;* Start of stl_RL78_registertest_es.asm module
_stl_RL78_registertest_es:
        MOV     errorindicator,#0x00            ; Clear error flag
        MOV     A, ES
        PUSH    AX                              ; save es register content before test

es_temp:
        MOV     ES, #0x05                       ; load es with 0x05
        MOV     A, ES
        CMP     A, #0x05                        ; compare A with 0x05
        BZ      $es_1                           ; if equal no error
        set1    errorindicator.0                ; set error indicator

es_1:	
        MOV     ES, #0x0A                       ; load es with 0x0A
        MOV     A, ES
        CMP     A, #0x0A                        ; compare A with 0x0A
        BZ      $test_finished                  ; if equal no error
        set1    errorindicator.0                ; set error indicator

test_finished:
        POP     AX
        MOV     ES, A                           ; restore es register content
        BT      errorindicator.0, $out_error    ; Test for error
        MOV     A, #0x00                        ; Return no error (0x00) to calling function
        ret
out_error:
        MOV     A, #0x01                        ; Return error (0x01) to calling function
        ret
