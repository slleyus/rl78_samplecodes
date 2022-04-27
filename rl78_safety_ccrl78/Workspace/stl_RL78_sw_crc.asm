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
;      AX     - contains the CRC start value
;      B, DE  - Stores the range from the CRC structure
;      ES HL  - Pointer to the start address & range values in the CRC structure
;
;      AX contains the return value of new calculated crc_value
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
;* 15.11.2013 V1.10 Fixed comments
;* 07.11.2016 V2.00 CC-RL Tool-Chain
;* 08.02.2018 V2.00 VDE Released
;*******************************************************************************

    .PUBLIC  _stl_RL78_sw_crc_asm

    .DSEG SBSS
;******************************************************************************
;* Exported global variables and functions (to be accessed by other files)
;******************************************************************************
crc_input:   .DS   2

    .CSEG CONST
;******************************************************************************
;* Macro definitions
;******************************************************************************
; CRC16 CCITT Table

crc16_table_asm:
        .DB2 0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,0x8108
        .DB2 0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,0x1231,0x0210
        .DB2 0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,0x9339,0x8318,0xb37b
        .DB2 0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,0x2462,0x3443,0x0420,0x1401
        .DB2 0x64e6,0x74c7,0x44a4,0x5485,0xa56a,0xb54b,0x8528,0x9509,0xe5ee
        .DB2 0xf5cf,0xc5ac,0xd58d,0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6
        .DB2 0x5695,0x46b4,0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d
        .DB2 0xc7bc,0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823
        .DB2 0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,0x5af5
        .DB2 0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,0xdbfd,0xcbdc
        .DB2 0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,0x6ca6,0x7c87,0x4ce4
        .DB2 0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,0xedae,0xfd8f,0xcdec,0xddcd
        .DB2 0xad2a,0xbd0b,0x8d68,0x9d49,0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13
        .DB2 0x2e32,0x1e51,0x0e70,0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a
        .DB2 0x9f59,0x8f78,0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e
        .DB2 0xe16f,0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067
        .DB2 0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,0x02b1
        .DB2 0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,0xb5ea,0xa5cb
        .DB2 0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,0x34e2,0x24c3,0x14a0
        .DB2 0x0481,0x7466,0x6447,0x5424,0x4405,0xa7db,0xb7fa,0x8799,0x97b8
        .DB2 0xe75f,0xf77e,0xc71d,0xd73c,0x26d3,0x36f2,0x0691,0x16b0,0x6657
        .DB2 0x7676,0x4615,0x5634,0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9
        .DB2 0xb98a,0xa9ab,0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882
        .DB2 0x28a3,0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a
        .DB2 0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,0xfd2e
        .DB2 0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,0x7c26,0x6c07
        .DB2 0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,0xef1f,0xff3e,0xcf5d
        .DB2 0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,0x6e17,0x7e36,0x4e55,0x5e74
        .DB2 0x2e93,0x3eb2,0x0ed1,0x1ef0


    .CSEG
;******************************************************************************
;* Outline : stl_RL78_sw_crc_asm - RL78 IEC Software CRC test module
;* Include : none
;* Declaration : stl_RL78_sw_crc_asm:
;* Function Name: stl_RL78_sw_crc_asm
;* Description : IEC Software CRC test code module
;* Argument : Register BC - Stores CRC calculation range from CRC structure
;*            Register AX - Starting CRC value
;* Return Value : Register AX - CRC Result 
;* Calling Functions : main.c
;******************************************************************************
; Start of software CRC code
_stl_RL78_sw_crc_asm:

; sort input parameter
        PUSH    HL                      ; save base pointer
        MOVW    crc_input , AX          ; move 1st argument ( i )
        MOVW    AX,BC
        MOVW    HL,AX                   ; arg2
	
	    MOV     C, #0x00
        MOVW    AX, [HL]                ; lower 16 bit of size parameter
        SUBW    AX, #0x0001             ; n-1 effect --- length        
        MOVW    DE, AX                    
        MOVW    AX, [HL+2]              ; upper 16 bit of size parameter 
        XCH     A,X
                               
;check_length not_zero:        
        SUBC    A, #0x00                ; n-1 effect --- length
        MOV     B,A                     ; bit 17->20 of size parameter  
        CMP     A, #0xFF                ; check bit 17->20
        BNZ     $length_not_zero
        MOVW    AX,DE
        CMPW    AX,#0xFFFF              ; check bit 00->16
        BZ      $exit                   ; test length = 0

; start crc test
length_not_zero:
        MOVW    AX, [HL+6]              ; bit 17->20 of start address into AX
        XCH     A,  X                   ; bit 17->20 of start address into A
        MOV     ES, A                   ; bit 17->20 of Start address into ES       
        MOVW    AX, [HL+4]              ; start Address into BC (lower 16 bit of address)
        MOVW    HL, AX                  ; lower 16 bit of start address into HL  
        CLR1    CY

next_rom:
        MOVW    AX, crc_input           ; after finished parameter sorting load crc input value again into AX
        XCH     A, X                    ; crc>>8 is now in register X
        MOV     A, ES:[HL]              ; load data of ROM/Flash area for new crc calculation into A
        XOR     X, A                    ; result of XOR is address value to catch correct table value
        XCH     A, X                    ; maske in A
        MOV     X, #0x00                ; clear register X
        CLR1    CY                      ; Clear Carry Flag
        ROLC    A, 1                    ; multiplication * 2 of address offset value
        XCH     A, X                    ; table address offset value into X of AX register
        ROLC    A, 1                    ; multiplication * 2 of higher part of Address offset value
        ADDW    AX, #crc16_table_asm    ; add onto Table start address calculated offset
        PUSH    HL
        MOVW    HL, AX                  ; calculated Table address into HL register
        
; load AX register with calculated table value
        MOV     A, [HL]                 ; move first byte ( low byte ) of crc16_table into A
        MOV     X, A                    ; move it into register X ( low byte )
        MOV     A, [HL+1]               ; move second byte ( high byte ) of crc16_table into A
        XCHW    AX, HL                  ; move crc16 table value into HL
        MOVW    AX, crc_input           ; reload input crc value for this calculation step into register AX
        MOV     A, #0x00                ; clear A, to prepare 8 times left shift of AX which means register X cleared
        XCH     A, X                    ; is equal 8 times left shift of AX registers
        
; XOR calculated crc16_table value with 8 times left shifted input crc value
        XOR     H, A                    ; XOR high bytes
        MOVW    AX, HL                  ; new CRC value into AX
        MOVW    crc_input, AX           ; update crc input value to prepare next calculation step if not finished        
        POP     HL
        
; check if all specified Memory is tested
        MOVW    AX, DE                  ; move memory_test length indicator into AX
        SUBW    AX, #0x0001             ; decrement length indicator
        MOVW    DE, AX                  ; restore decremented length indicator in DE
        BNC     $not_finished           ; continue if no underflow of lower 16bit of length indicator 
        MOV     A,  B                 
        SUB     A,  #0x01               ; if underflow occured, decrement bit 17->20 of length indicator
        MOV     B,  A
        BC      $exit                   ; if underflow of bit 17->20 of length indicator, calculation finished
        
not_finished:
        MOVW    AX, HL
        ADDW    AX, #0x0001             ; next address of checksum calculation ( increment HL )
        MOVW    HL, AX
        BNC     $next_rom
	    MOV 	A, ES
        INC     A                       ; if overflow HL, incemnt ES register
	    MOV 	ES, A
        BR      $next_rom

; Memory test finished, prepare return value
exit:   MOVW  AX, crc_input                 
	    POP HL
        ret
