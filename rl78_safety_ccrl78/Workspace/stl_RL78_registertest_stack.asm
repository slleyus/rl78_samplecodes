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
;* File Name : stl_RL78_registertest_stack.asm
;* Version : 2.00
;* Device(s) : RL78/G14
;* Tool-Chain : RL78 CC-RL V1.03.00
;* OS : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library Stack pointer test assembler module
;                The Stack pointer register is tested and restored
;                by this test routine.
;
;* Limitations : It is assumed, that the register test routine is entered with
;                Register bank 0 (RB0) selected.
;                Any dependency of the user application in that it might be
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


    .PUBLIC  _stl_RL78_registertest_stack      ; Global Name Declaration

    .DSEG  SBSS


;*******************************************************************************
; Private variables and functions
;*******************************************************************************
;* Define error variable workspace and Flag bit
errorindicator:   .DS   1

    .CSEG


;******************************************************************************
;* Outline : stl_RL78_registertest_stack RL78 IEC Stack Register test module
;* Include : none
;* Declaration : stl_RL78_registertest_stack
;* Function Name: stl_RL78_registertest_stack
;* Description : IEC Stack Register test code module
;* Argument : none
;* Return Value : Register A - Result status (Pass = 0x00, Fail = 0x01)
;* Calling Functions : main.c
;******************************************************************************
;* Start of stl_RL78_registertest_stack.asm code
_stl_RL78_registertest_stack:
        PUSH    BC                                  ; save BC
        MOV     errorindicator,#0x00
        MOVW    BC, SP
	    MOVW    AX, #0x5555                         ; save SP content in BC before SP test
	    MOVW    SP, AX
        MOVW    AX, SP
        CMPW    AX, #0x5554                         ; bit one always fixed to Zero
        BZ      $sp_1
        set1    errorindicator.0                    ; if not equal continue @ Error

sp_1:   MOVW    SP, #0xAAAA
        MOVW    AX, SP
        CMPW    AX, #0xAAAA
        BZ      $test_finished
        set1    errorindicator.0                    ; if not equal continue @ Error
test_finished:
        MOVW    AX, BC
        MOVW    SP, AX                              ; restore SP register content
        POP     BC                                  ; restore BC
        BT      errorindicator.0, $out_error
        MOV     A, #0x00                            ; return no error (0x00)
        ret
out_error:
        MOV     A, #0x01                            ; return error (0x01)
        ret
