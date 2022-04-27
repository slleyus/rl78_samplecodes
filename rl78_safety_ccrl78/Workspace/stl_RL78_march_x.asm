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
;* File Name : stl_RL78_march_x.asm
;* Version : 2.10
;* Device(s) : RL78/G14
;* Tool-Chain : RL78 Cubesuite Embedded Workbench V1.10.1
;* OS : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library March X test assembler module
;* Argument: Register "AX" contains the start address of the RAM area to be tested
;*           Register "BC" contains the number of bytes(length) of the RAM area
;      Registers AX, BC, DE of Bank1 are used to buffer start, stop, length,
;      step indicator values of march_x test
;
;     Register "A" returns 0x01 if a memory fault was detected, otherwise
;     0x00 is returned

;* Limitations : It is assumed, that the register test routine is entered with
;                Register bank 0 (RB0) selected.
;                Any dependency of the user application in that it might be
;                necessary to disable interrupts while testing, due to the use
;                of the register banks
;*******************************************************************************
;* History :
;* 30.05.2011 V1.00 First Release
;* 15.11.2013 V1.10 Fixed comments
;* 12.06.2014 V1.20 Fixed cord part to be completely identical to V1.01
;* 07.11.2016 V2.00 CC-RL Tool-Chain
;* 08.02.2018 V2.00 VDE Released
;* 21.12.2018 V2.10 Fixed Step 3 of March X
;*******************************************************************************

    .PUBLIC  _stl_RL78_march_x            ; Global Name Declaration


	.CSEG


;******************************************************************************
;* Outline : stl_RL78_march_x - RL78 IEC March_x test module
;* Include : none
;* Declaration : stl_RL78_march_x:
;* Function Name: stl_RL78_march_x
;* Description : IEC March_x test code module
;* Argument: Register "BC" contains the address range of the RAM area to be tested
;*           Register "AX" contains the start address of the RAM area to be tested
;*           Registers AX, BC, DE of Bank1 are used to buffer 
;            start address, end address, test range, step indicator values
;* Return Value : Register A - Result status (Pass = 0x00, Fail = 0x01)
;* Calling Functions : main.c
;******************************************************************************
;* Start of stl_RL78_march_x.asm code
_stl_RL78_march_x:

	    PUSH    HL		;save HL on stack
	    PUSH    AX		;save AX on stack (arg1)
	    PUSH    BC		;save BC on stack 
        SEL     RB1		;select Reg. Bank 1
        PUSH    AX		;save AX (RB1) on stack 
        PUSH    BC   		;save BC (RB1) on stack
        PUSH    DE		;save DE (RB1) on stack
        SEL     RB0		;select Reg. Bank 0
	    MOVW    AX , SP         ; copy stack pointer
        MOVW    HL , AX		    ; save SP in HL
        MOV     A , [ HL + 0x08]; get arg1(LSB)from stack
        XCH     A , X		    ; save LSB of arg1 in X 
        MOV     A , [ HL + 0x09 ]; get arg2(MSB)from stack
	    MOVW    0xFFEF2, AX     ; store start address of marchc test into 0xFEF2 ( BC [Bank1] )            
        
	    MOV     A , [ HL + 0x06] ; get arg2(LSB)from stack
        XCH     A , X		     ; save LSB of arg1 in X 
        MOV     A , [ HL + 0x07 ]; get arg2(MSB)from stack
	    MOVW    BC, AX           ; test length into AX
        MOVW    0xFFEF0, AX      ; store test length into 0xFEF0 ( AX [Bank1] )  

;* test if length is zero
        SUBW  AX, #0x0001       ; decrement test_length_value
        BNC   $length_not_zero  ; if test_length_value was zero, c flag is set
        CLRB  A                 ; return value true - no error
	    MOV   C, A
        BR    $zero_length_return

;* start march_x test
length_not_zero:
        MOVW  AX, 0xFFEF2       ; store start address of march x test into AX
        ADDW  AX, 0xFFEF0       ; calculate stop address
        MOVW  0xFFEF4, AX       ; store march-x end address into DE [Bank1]

;* clear error indicator (Register D)
        CLRB  A                 ; clear register A
        MOV   D, A              ; clear D, which functions as error indicator

;* step 1 - <>(w0) - clear specified memory area
;* init step 1
        MOVW  HL, 0xFFEF2       ; march-x start address from BC [Bank1] into HL register
        MOVW  AX, 0xFFEF4       ; march-x end address from AX [Bank1] into AX register

;* test step 1
loop_step_1:

        MOV   [HL+0], #0x00     ; clear by HL addressed memory cell
        INCW  HL                ; increment RAM address register
        CMPW  AX, HL            ; check if all specified memory is cleared
        BZ    $march_x_step_2   ; check if all RAM is cleared ( check length counter )
        BR    $loop_step_1      ; next cycle

;* step 2 - >(r0,w1) - read bitwise if zero is in RAM, then write 1 into it, from lower address
;* init step 2
march_x_step_2:
        MOVW  AX, 0xFFEF4       ; march-x end- address from AX [Bank1] into AX register
        MOVW  HL, 0xFFEF2       ; march-x start address from BC [Bank1] into HL register
;* test step 2
loop_step_2:
        BR    $r0_w1            ; branch to by [HL] addressed byte test and do r0 > w1
ret_step_2:
        MOVW  AX, 0xFFEF4       ; march-x end address from AX [Bank1] into AX register
        INCW  HL                ; increment RAM address register
        CMPW  AX, HL            ; check if all specified memory is written one to
        BZ    $march_x_step_3   ; check if all memory has been addressed
        BR    $loop_step_2      ; not finished, next cycle

;* step 3 - <(r1,w0) - read bitwise if one is in RAM, then write zero into it from upper address
; init step 3
march_x_step_3:
        MOVW  AX, 0xFFEF2       ; march-x start address from BC [Bank1] into AX register
        MOVW  HL, 0xFFEF4       ; march-x end address from AX [Bank1] into HL register
;* test step 3
loop_step_3:
        DECW  HL
        BR    $r1_w0            ; branch to by [HL] addressed byte test and do r1 > w0
ret_step_3:
        MOVW  AX, 0xFFEF2
        CMPW  AX, HL
        BZ    $march_x_step_4   ; check if all RAM is cleared ( check length counter )
        BR    $loop_step_3      ; next cycle

;* step 4 - <>(r0) - check if specified memory area is all zero
;* init step 4
march_x_step_4:
        MOVW  AX, 0xFFEF4       ; march-x end address from AX [Bank1] into AX register
        MOVW  HL, 0xFFEF2       ; march-x start address from BC [Bank1] into HL register

;* test step 4
loop_step_4:
        MOV   A, [HL]           ; load RAM cell addressed by HL
        CMP0  A                 ; check if RAM content is zero
        BZ    $is_zero
        MOV   D, #0x01          ; D functions as error indicator
is_zero:
        MOVW  AX, 0xFFEF4       ; march-x end address from AX [Bank1] into AX register
        INCW  HL                ; increment RAM address register
        CMPW  AX, HL
        BZ    $out_4            ; check if all RAM is cleared ( check length counter )
        BR    $loop_step_4      ; next cycle

;* move test result into reg A and return it to calling function
out_4:
        MOV   A, D              ; return test result and restore register to original contents
        MOV   L,A
zero_length_return:             ; restore register contents directly is test length was set to zero

        SEL   RB1		;select Reg. Bank 1
        POP  DE		    ;save DE (RB1) on stack
        POP  BC   		;save BC (RB1) on stack
        POP  AX		    ;save AX (RB1) on stack 

        SEL   RB0		;select Reg. Bank 0
	    POP  DE		    ;save DE on stack 
	    POP  AX		    ;save AX on stack (arg1)
        MOV  A,L
   	    POP  HL		    ;save HL on stack

        ret             ; test finished; test result in A, 01=error, 00=no error; return to c_startup

ret_temp:
	    BR    $ret_step_2

ret_temp_2:
        BR    $ret_step_3
;* submodule to read by HL addressed ram, check bitwise if content is 0 and set bitwise to 1
r0_w1:
        MOV   A, [HL]           ; move to be tested byte into register A
        CMP   A, #0b00000000    ; compare Register A with reference
        BZ    $S_HL_0           ; if register content A equal reference value, no error - continue with next bit
        MOV   D, #0x01          ; if error occurred Register D carries error information 0x01
S_HL_0: set1  [HL].0
        MOV   A, [HL]
        CMP   A, #0b00000001
        BZ    $S_HL_1
        MOV   D, #0x01
S_HL_1: set1  [HL].1
        MOV   A, [HL]
        CMP   A, #0b00000011
        BZ    $S_HL_2
        MOV   D, #0x01
S_HL_2: set1  [HL].2
        MOV   A, [HL]
        CMP   A, #0b00000111
        BZ    $S_HL_3
        MOV   D, #0x01
S_HL_3: set1  [HL].3
        MOV   A, [HL]
        CMP   A, #0b00001111
        BZ    $S_HL_4
        MOV   D, #0x01
S_HL_4: set1  [HL].4
        MOV   A, [HL]
        CMP   A, #0b00011111
        BZ    $S_HL_5
        MOV   D, #0x01
S_HL_5: set1  [HL].5
        MOV   A, [HL]
        CMP   A, #0b00111111
        BZ    $S_HL_6
        MOV   D, #0x01
S_HL_6: set1  [HL].6
        MOV   A, [HL]
        CMP   A, #0b01111111
        BZ    $S_HL_7
        MOV   D, #0x01
S_HL_7: set1  [HL].7
        MOV   A, [HL]
        CMP   A, #0b11111111
        BZ    $S_HL_out
        MOV   D, #0x01
S_HL_out:
        BR    $ret_temp

ret_temp_1:
	BR	$ret_temp_2
;* submodule to read by HL addressed ram, check bitwise if content is 1 and set bitwise to 0
r1_w0:
        MOV   A, [HL]           ; move to be tested byte into register A
        CMP   A, #0b11111111    ; compare Register A with reference
        BZ    $C_HL_7           ; if register content A equal reference value, no error - continue with next bit
        MOV   D, #0x01          ; if error occurred Register D carries error information 0x01
C_HL_7: clr1  [HL].7
        MOV   A, [HL]
        CMP   A, #0b01111111
        BZ    $C_HL_6
        MOV   D, #0x01
C_HL_6: clr1  [HL].6
        MOV   A, [HL]
        CMP   A, #0b00111111
        BZ    $C_HL_5
        MOV   D, #0x01
C_HL_5: clr1  [HL].5
        MOV   A, [HL]
        CMP   A, #0b00011111
        BZ    $C_HL_4
        MOV   D, #0x01
C_HL_4: clr1  [HL].4
        MOV   A, [HL]
        CMP   A, #0b00001111
        BZ    $C_HL_3
        MOV   D, #0x01
C_HL_3: clr1  [HL].3
        MOV   A, [HL]
        CMP   A, #0b00000111
        BZ    $C_HL_2
        MOV   D, #0x01
C_HL_2: clr1  [HL].2
        MOV   A, [HL]
        CMP   A, #0b00000011
        BZ    $C_HL_1
        MOV   D, #0x01
C_HL_1: clr1  [HL].1
        MOV   A, [HL]
        CMP   A, #0b00000001
        BZ    $C_HL_0
        MOV   D, #0x01
C_HL_0: clr1  [HL].0
        MOV   A, [HL]
        CMP   A, #0b00000000
        BZ    $C_HL_out
        MOV   D, #0x01
C_HL_out:
        BR    $ret_temp_1
