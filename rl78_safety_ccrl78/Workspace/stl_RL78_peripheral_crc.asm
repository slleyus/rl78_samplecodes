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
;* File Name : stl_RL78_sw_crc.asm
;* Version : 2.00
;* Device(s) : RL78/G14
;* Tool-Chain : RL78 CC-RL V1.03.00
;* OS : None
;* H/W Platform : RL78/G14 Target Board
;* Description : RL78 IEC Self Test Library CRC calculation module
;
;      AX     - CRC start value
;      B, DE  - Stores the range contained in the crc structure
;      ES, HL - Pointer to the start address & range conatined in the CRC structure
;
;      AX contains the return value of calculated crc_value
;
;* Limitations : Any dependancy of the user application in that it might be
;                necessary to dissable interrupts while testing
;                 
;                The module does not perform any result checking. This is left
;                to the application code to confirm that the result is correct
;                and take any necessary corrective action
;*******************************************************************************
;* History :
;* 30.05.2011 V1.00 First Release
;* 15.11.2013 V1.10 Fixed comments. (Target address and the size are fixed in main.c)
;* 07.11.2016 V2.00 CC-RL Tool-Chain
;* 08.02.2018 V2.00 VDE Released
;*******************************************************************************

    .PUBLIC  _stl_RL78_peripheral_crc


;*******************************************************************************
; Includes <System Includes> , "Project Includes"
;*******************************************************************************


	.CSEG

;******************************************************************************
;* Outline : stl_RL78_peripheral_crc RL78 IEC Peripheral CRC test module
;* Include : ior5f100le.h, ior5f100le_ext.h
;* Declaration : stl_RL78_peripheral_crc:
;* Function Name: stl_RL78_peripheral_crc
;* Description : IEC Peripheral CRC test code module
;* Argument : Register BC - Stores CRC calculation range from CRC structure
;*            Register AX - Starting CRC value
;* Return Value : Register AX - CRC Result 
;* Calling Functions : main.c
;******************************************************************************
; Start of stl_RL78_peripheral_crc module
_stl_RL78_peripheral_crc:

; sort input parameter
        PUSH    HL                  ; save base pointer
        MOVW    !CRCD , AX          ; move 1st argument ( i )
        MOVW    AX,BC
        MOVW    HL,AX
	
    	MOV     C, #00
        MOVW    AX, [HL]             ; lower 16 bit of size parameter
        SUBW    AX, #0001            ; n-1 effect --- length        
        MOVW    DE, AX                    
        MOVW    AX, [HL+2]           ; upper 16 bit of size parameter 
        XCH     A,X                            
;check length_not_zero:        
        SUBC    A, #000              ; n-1 effect --- length
        MOV     B,A                  ; bit 17->20 of size parameter  
        CMP     A, #0xFF             ; check bit 17->20
        BNZ     $length_not_zero
        MOVW    AX,DE
        CMPW    AX,#0xFFFF            ; check bit 00->16
        BZ      $exit                 ; test length = 0

; start crc test
length_not_zero:
        MOVW    AX, [HL+6]           ; bit 17->20 of start address into AX
        XCH     A,  X                ; bit 17->20 of start address into A
        MOV     ES, A                ; bit 17->20 of Start address into ES       
        MOVW    AX, [HL+4]           ; start Address into BC (lower 16 bit of address)
        MOVW    HL, AX               ; lower 16 bit of start address into HL  
        CLR1    CY

next_rom:
        MOV     A, ES:[HL]           ; load data of ROM/Flash area for new crc calculation into A
        MOV     CRCIN,A
        NOP
        NOP
        MOVW    AX,!CRCD
       
        
; check if all specified Memory is tested
        MOVW    AX, DE               ; move memory_test length indicator into AX
        SUBW    AX, #00001           ; decrement length indicator
        MOVW    DE, AX               ; restore decremented length indicator in DE
        BNC     $not_finished        ; continue if no underflow of lower 16bit of length indicator 
        MOV     A,  B                 
        SUB     A,  #001             ; if underflow occurred, decrement bit 17->20 of length indicator
        MOV     B,  A
        BC      $exit                ; if underflow of bit 17->20 of length indicator, calculation finished
not_finished:
        MOVW    AX, HL
        ADDW    AX, #00001           ; next address of checksum calculation ( increment HL )
        MOVW    HL, AX
        BNC     $next_rom
	    MOV 	A, ES
        INC     A
	    MOV 	ES, A                ; if overflow HL, increment ES register
        BR      $next_rom


; Memory test finished, prepare return value
exit:   MOVW  AX, !CRCD                 
	    POP HL
        ret    
