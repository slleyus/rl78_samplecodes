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
;* File Name : stl_RL78_registertest_psw.asm
;* Version : 2.00
;* Device(s) : RL78 (RL78/G14)
;* Tool-Chain : RL78 CC-RL V1.03.00
;* OS : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library PSW register test module
;                The Processor Status Word (PSW) Register is tested and restored
;                by this test routine.
;
;* Limitations : Any dependency of the user application in that it might be
;                necessary to disable interrupts while testing, because the
;                Register banks are switched and tested in turn, with the original
;                Register content are buffered on Stack
;*******************************************************************************
;* History :
;* 30.05.2011 V1.00 First Release
;* 15.11.2013 V1.10 Fixed comments
;* 07.11.2016 V2.00 CC-RL Tool-Chain
;* 08.02.2018 V2.00 VDE Released
;*******************************************************************************

    .PUBLIC  _stl_RL78_registertest_psw      ; Global Name Declaration

    .DSEG  SBSS


;******************************************************************************
; Private variables and functions
;******************************************************************************
;* Define error variable workspace and Flag bit
errorindicator:   .DS   1


	.CSEG

;******************************************************************************
;* Outline : stl_RL78_registertest_psw - RL78 IEC PSW Register test module
;* Include : none
;* Declaration : stl_RL78_registertest_psw
;* Function Name: stl_RL78_registertest_psw
;* Description : IEC PSW Register test code module
;* Argument : none
;* Return Value : Register A - Result status (Pass = 0x00, Fail = 0x01)
;* Calling Functions : main.c
;******************************************************************************
;* Start of stl_RL78_registertest_psw.asm module
_stl_RL78_registertest_psw:
        MOV     errorindicator,#0x00            ; Clear error flag
        PUSH    PSW                             ; save PSW content before test

psw_temp:
        MOV     PSW, #0x55                      ; load PSW with 0x55
        MOV     A, PSW
        CMP     A, #0x55                        ; compare AX with 0x55
        BZ      $psw_1                          ; if equal no error
        set1    errorindicator.0                ; set error indicator

psw_1:
        MOV     PSW, #0xAA                      ; load PSW with 0xAA ( we are now on RB03 )
        PUSH    AX                              ; store AX of RB03
        MOV     A, PSW
        CMP     A, #0xAA                        ; compare AX with 0xAA
        POP     AX                              ; restore AX of RB03
        BZ      $test_finished                  ; if equal no error
        set1    errorindicator.0                ; set error indicator

test_finished:
        POP     PSW                             ; restore PSW register content
        BT      errorindicator.0, $out_error    ; Test for error
        MOV     A, #0x00                        ; Return no error (0x00) to calling function
        ret
out_error:
        MOV     A, #0x01                        ; Return error (0x01) to calling function
        ret
