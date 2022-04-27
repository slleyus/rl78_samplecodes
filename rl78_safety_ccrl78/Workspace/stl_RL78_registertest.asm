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
;* File Name : stl_RL78_registertest.asm
;* Version : 2.00
;* Device(s) : RL78/G14
;* Tool-Chain : RL78 CC-RL V1.03.00
;* OS : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library Register Bank test module
;                Register Banks 0, 1, 2, 3, are tested and restored
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


    .PUBLIC  _stl_RL78_registertest     ; Global Name Declaration

    .DSEG   SBSS
;******************************************************************************
; Private variables and functions
;******************************************************************************
; Define error variable workspace and Flag bit
errorindicator:     .DS     1


 	.CSEG

;******************************************************************************
;* Outline : stl_RL78_registertest - RL78 IEC Register Bank test module
;* Include : none
;* Declaration : stl_RL78_registertest
;* Function Name: stl_RL78_registertest
;* Description : IEC Register Bank test code module
;* Argument : none
;* Return Value : Register A - Result status (Pass = 0x00, Fail = 0x01)
;* Calling Functions : main.c
;******************************************************************************
; Start of stl_RL78_registertest.asm code
_stl_RL78_registertest:
        MOV     errorindicator,#0x00
        PUSH    AX                    ; move on stack AX register content of RB0
        PUSH    BC                    ; move on stack BC register content of RB0
        PUSH    DE                    ; move on stack DE register content of RB0
        PUSH    HL                    ; move on stack HL register content of RB0
        BR      $go_tst               ; go to test routine

loop_rb1:
        SEL     RB1                   ; Select register bank 1
        PUSH    AX                    ; move on stack AX register content of RB1
        PUSH    BC                    ; move on stack BC register content of RB1
        PUSH    DE                    ; move on stack DE register content of RB1
        PUSH    HL                    ; move on stack HL register content of RB1
        BR      $go_tst               ; go to test routine
loop_rb2:
        POP     HL                    ; restore HL register content of RB1
        POP     DE                    ; restore DE register content of RB1
        POP     BC                    ; restore BC register content of RB1
        POP     AX                    ; restore AX register content of RB1
        SEL     RB2                   ; Select register bank 2
        PUSH    AX                    ; move on stack AX register content of RB2
        PUSH    BC                    ; move on stack BC register content of RB2
        PUSH    DE                    ; move on stack DE register content of RB2
        PUSH    HL                    ; move on stack HL register content of RB2
        BR      $go_tst               ; go to test routine
loop_rb3:
        POP     HL                    ; restore HL register content of RB2
        POP     DE                    ; restore DE register content of RB2
        POP     BC                    ; restore BC register content of RB2
        POP     AX                    ; restore AX register content of RB2
        SEL     RB3                   ; Select register bank 3
        PUSH    AX                    ; move on stack AX register content of RB3
        PUSH    BC                    ; move on stack BC register content of RB3
        PUSH    DE                    ; move on stack DE register content of RB3
        PUSH    HL                    ; move on stack HL register content of RB3
	    BR      $go_tst               ; go to test routine

ret_temp:
	BR      $loop_rb1

ret_temp1:
	BR      $loop_rb2

ret_temp2:
	BR      $loop_rb3

; start register test routine
go_tst:
        MOVW    AX, #0xAAAA           ; load AX with 0xAAAA
        CMPW    AX, #0xAAAA           ; compare AX with 0xAAAA
        BZ      $nxt_1                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error
nxt_1:  MOVW    AX, #0x5555           ; load AX with 0x5555
        CMPW    AX, #0x5555           ; compare AX with 0x5555
        BZ      $nxt_2                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error
nxt_2:  MOVW    BC, #0xAAAA           ; load BC with 0xAAAA
        MOVW    AX, BC                ; move content of BC into AX, compare operation can only be done with AX
        CMPW    AX, #0xAAAA           ; compare AX ( BC ) with 0xAAAA
        BZ      $nxt_3                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error
nxt_3:  MOVW    BC, #0x5555           ; load BC with 0x5555
        MOVW    AX, BC                ; move content of BC into AX, compare operation can only be done with AX
        CMPW    AX, #0x5555           ; compare AX ( BC ) with 0x5555
        BZ      $nxt_4                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error
nxt_4:  MOVW    DE, #0xAAAA           ; load DE with 0xAAAA
        MOVW    AX, DE                ; move content of DE into AX, compare operation can only be done with AX
        CMPW    AX, #0xAAAA           ; compare AX ( DE ) with 0xAAAA
        BZ      $nxt_5                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error
nxt_5:  MOVW    DE, #0x5555           ; load DE with 0x5555
        MOVW    AX, DE                ; move content of DE into AX, compare operation can only be done with AX
        CMPW    AX, #0x5555           ; compare AX ( DE ) with 0x5555
        BZ      $nxt_6                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error
nxt_6:  MOVW    HL, #0xAAAA           ; load HL with 0xAAAA
        MOVW    AX, HL                ; move content of HL into AX, compare operation can only be done with AX
        CMPW    AX, #0xAAAA           ; compare AX ( HL ) with 0x5555
        BZ      $nxt_7                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error
nxt_7:  MOVW    HL, #0x5555           ; load HL with 0x5555
        MOVW    AX, HL                ; move content of HL into AX, compare operation can only be done with AX
        CMPW    AX, #0x5555           ; compare AX ( HL ) with 0x5555
        BZ      $nxt_8                ; if not equal continue @ Error
        set1    errorindicator.0      ; if not equal continue @ Error

; check current Register Bank and select next one
nxt_8:  MOV     A, PSW                ; test and select next register bank
        AND     A, #0b00101000        ; isolate RBS0 and RBS1
        CMP     A, #0b00000000        ; check if current RB is RB0
        SKNZ
        BR      $ret_temp             ; if yes ( zero ) select RB1 and continue test
        CMP     A, #0b00001000        ; check if current RB is RB1
        SKNZ
        BR      $ret_temp1            ; if yes ( zero ) select RB2 and continue test
        CMP     A, #0b00100000        ; check if current RB is RB2
        BZ      $ret_temp2              ; if yes ( zero ) select RB3 and continue test
                                      ; if no ( not zero ) selected RB is RB3 and test finished
        POP     HL                    ; restore HL register content of RB3
        POP     DE                    ; restore DE register content of RB3
        POP     BC                    ; restore BC register content of RB3
        POP     AX                    ; restore AX register content of RB3

; restore original register bank values
        SEL     RB0                   ; select register bank 0
        POP     HL                    ; restore HL register content of RB0
        POP     DE                    ; restore DE register content of RB0
        POP     BC                    ; restore BC register content of RB0
        POP     AX                    ; restore AX register content of RB0

; test result status and return status code value
        BT      errorindicator.0, $out_error
        MOV     A, #0x00              ; Return 00 (no error) to calling function
        ret
out_error:
        MOV     A, #0x01              ; Return 01 (error)to calling function
        ret
