;------------------------------------------------------------------------------
; Module       = hwmdu_LibReplacement.s
; Version      = 1.3
;------------------------------------------------------------------------------
;                                  COPYRIGHT
;------------------------------------------------------------------------------
; Copyright (c) 2013 by Renesas Electronics Europe GmbH,
;               a company of the Renesas Electronics Corporation
;------------------------------------------------------------------------------
; Purpose:
;       sample code to use RL78_2 Hardware Multiply Division Unit (HW_MDU)
;
;------------------------------------------------------------------------------
;
; Warranty Disclaimer
;
; Because the Product(s) is licensed free of charge, there is no warranty
; of any kind whatsoever and expressly disclaimed and excluded by Renesas,
; either expressed or implied, including but not limited to those for
; non-infringement of intellectual property, merchantability and/or
; fitness for the particular purpose.
; Renesas shall not have any obligation to maintain, service or provide bug
; fixes for the supplied Product(s) and/or the Application.
;
; Each User is solely responsible for determining the appropriateness of
; using the Product(s) and assumes all risks associated with its exercise
; of rights under this Agreement, including, but not limited to the risks
; and costs of program errors, compliance with applicable laws, damage to
; or loss of data, programs or equipment, and unavailability or
; interruption of operations.
;
; Limitation of Liability
;
; In no event shall Renesas be liable to the User for any incidental,
; consequential, indirect, or punitive damage (including but not limited
; to lost profits) regardless of whether such liability is based on breach
; of contract, tort, strict liability, breach of warranties, failure of
; essential purpose or otherwise and even if advised of the possibility of
; such damages. Renesas shall not be liable for any services or products
; provided by third party vendors, developers or consultants identified or
; referred to the User by Renesas in connection with the Product(s) and/or the
; Application.
;
;------------------------------------------------------------------------------
;
;------------------------------------------------------------------------------
; History: 1.0 Initial version
;          1.1  - Correction of signed division functions 
;                 to achieve interrupt safety
;               - unnecessary function HWMUL_32_32_32 removed
;               - modulo functions added 
;          1.2  - Correction of 16bit signed division and modulo functions 
;                 (preserve of register BC to be compatible to SW-library)
;          1.3 Adapted for Embedded Workbench for RL78 V2.10 and far runtime
;              library calls
;  
;------------------------------------------------------------------------------
;
;---------------------------------CAUTION--------------------------------------
;
; In case of using these functions interrupt handling is delayed!
;
;------------------------------------------------------------------------------
;
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------

#if __CORE__ != __RL78_2__
    #error "Source Code for for RL78_2 devices only"
#endif

;------------------------------------------------------------------------------
;       Division of two unsigned chars
;
;       Input:  A         Operand 1
;               X         Operand 2
;
;       Call:   CALL       HWDIV_8_8_8
;
;       Output: A         Operand1 div Operand2
;               Z-flag
; This function can be used as replacement for the library function ?UC_DIV_L01
; by using the linker option -eHWDIV_8_8_8=?UC_DIV_L01
;------------------------------------------------------------------------------
        MODULE __HWMDU_REPLACEMENTS
        PUBLIC  HWDIV_8_8_8
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWDIV_8_8_8:
        PUSH  DE

        MOVW  DE,AX
        CMP0  X                   ; divison by zero ?
        BZ    LABEL02
        CLRB  A
        XCHW  AX,DE 
        CLRB  X
        XCH   A,X
        DIVHU
        XCH   A,X

LABEL02:
        ; set / clear Z-Flag according to the result
        CMP0  A

        POP   DE       

        RET
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;       Modulo Operation of two unsigned chars
;
;       Input:  A         Operand 1
;               X         Operand 2
;
;       Call:   CALL       HWMOD_8_8_8
;
;       Output: A         Operand1 modulo Operand2
;               Z-flag
; This function can be used as replacement for the library function ?UC_MOD_L01
; by using the linker option -eHWMOD_8_8_8=?UC_MOD_L01
;------------------------------------------------------------------------------
        PUBLIC  HWMOD_8_8_8
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWMOD_8_8_8:
        PUSH  DE

        MOVW  DE,AX
        CMP0  X                   ; divison by zero ?
        XCH   A,X
        BZ    LABEL12
        XCH   A,X
        CLRB  A
        XCHW  AX,DE 
        CLRB  X
        XCH   A,X
        DIVHU
        MOV   A,E

LABEL12:
        ; set / clear Z-Flag according to the result
        CMP0  A

        POP   DE
        
        RET
;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
;       Division of two signed chars
;
;       Input:  A         Operand 1
;               X         Operand 2
;
;       Call:   CALL       HWSDIV_8_8_8
;
;       Output: A          Operand1 div Operand2
;               Z-flag
; This function can be used as replacement for the library function ?SC_DIV_L01
; by using the linker option -eHWSDIV_8_8_8=?SC_DIV_L01
;------------------------------------------------------------------------------
        PUBLIC   HWSDIV_8_8_8
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWSDIV_8_8_8:
        PUSH  BC
        PUSH  DE

        MOVW  DE,AX
        XOR   A, X                
        AND   A, #80H             
        MOV   C, A                ; C = 0x00 = positive,  0x80 = negative
        MOVW  AX, DE              ; restore AX

        CMP0  X                   ; divison by zero ?
        BZ    LABEL22
        CLRB  A
        XCH   A,X
        BF    A.7, LABEL23         ; negative value ?
        SUB   X,A
        CLRB  A
        CMP0  A                   ; just to trigger the next skip instruction
        SKZ

LABEL23:
        XCH   A,X
        XCHW  AX,DE
        CLRB  X
        BF    A.7, LABEL21         ; negative value ?
        SUB   X,A
        CLRB  A
        XCH   A,X

LABEL21:
        XCH   A,X
        DIVHU

        CMP0  C                   
        SKNZ
        XCH   A,X
        SKZ
        SUB   A,X

LABEL22:
        ; set / clear Z-Flag according to the result
        CMP0  A

        POP   DE
        POP   BC

        RET

;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;       Modulo Operation of two signed chars
;
;       Input:  A         Operand 1
;               X         Operand 2
;
;       Call:   CALL       HWSMOD_8_8_8
;
;       Output: A          Operand1 modulo Operand2
;               Z-flag
; This function can be used as replacement for the library function ?SC_MOD_L01
; by using the linker option -eHWSMOD_8_8_8=?SC_MOD_L01
;------------------------------------------------------------------------------
        PUBLIC   HWSMOD_8_8_8
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWSMOD_8_8_8:
        PUSH  BC
        PUSH  DE

        MOVW  DE,AX
        AND   A, #80H            
        MOV   C, A                ; C = 0x00 = positive, 0x80 = negative
        MOVW  AX, DE              ;

        CMP0  X                   ; divison by zero ?
        BZ    LABEL32
        CLRB  A
        XCH   A,X
        BF    A.7, LABEL33         ; negative value ?
        SUB   X,A
        CLRB  A
        CMP0  A
        SKZ
LABEL33:
        XCH   A,X
        XCHW  AX,DE      
        CLRB  X
        BF    A.7, LABEL31         ; negative value ?
        SUB   X,A
        CLRB  A
        XCH   A,X
          
LABEL31:
        XCH   A,X
        DIVHU
        
        CMP0  C                   ; check negative flag
        MOV   A,E
        CLRB  X 
        SKNZ
        XCH   A,X
        SKZ
        SUB   X,A
        
LABEL32:
        XCH   A,X
        ; set / clear Z-Flag according to the result
        CMP0  A
        
        POP   DE
        POP   BC

        RET

;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
;       Division of two signed ints
;
;       Input:  AX         Operand 1
;               BC         Operand 2
;
;       Call:   CALL       HWSDIV_16_16_16
;
;       Output: AX         Operand1 div Operand2
;               Z-flag
; This function can be used as replacement for the library function ?SI_DIV_L02
; by using the linker option -eHWSDIV_16_16_16=?SI_DIV_L02
;------------------------------------------------------------------------------
        PUBLIC   HWSDIV_16_16_16
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWSDIV_16_16_16:
        PUSH  BC
        PUSH  DE
        PUSH  HL                  

        MOVW  DE,AX               
        XOR   A, B                
        AND   A, #80H             
        MOV   L, A                ; L = 0x00 = positive, 0x80 = negative
        MOVW  AX, DE              

        BF    A.7, LABEL41         ; negative value ?
        CLRW AX
        SUB   A,E
        XCH   A,X
        SUBC  A,D
LABEL41:
        XCHW AX, BC
        BF    A.7, LABEL42         ; negative value ?
        MOVW  DE,AX
        CLRW AX
        SUB   A,E
        XCH   A,X
        SUBC  A,D

LABEL42:
        CMPW  AX, #0x0000         ; divisor equal zero
        MOVW  DE, AX
        MOVW  AX, BC
        BZ    LABEL43

        DIVHU

        XCH   A, L                ; read flag value
        CMP0  A                   
        XCH   A, L                
        BZ    LABEL43
        MOVW  DE,AX
        CLRW  AX
        SUB   A,E
        XCH   A,X
        SUBC  A,D
LABEL43:
        ; set / clear Z-Flag according to the result
        CMPW  AX, #0x0000

        POP   HL                  
        POP   DE
        POP   BC
        RET

;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;       Modulo Operation of two signed ints
;
;       Input:  AX         Operand 1
;               BC         Operand 2
;
;       Call:   CALL       HWSMOD_16_16_16
;
;       Output: AX         Operand1 div Operand2
;               Z-flag
; This function can be used as replacement for the library function ?SI_MOD_L02
; by using the linker option -eHWSMOD_16_16_16=?SI_MOD_L02
;------------------------------------------------------------------------------
        PUBLIC   HWSMOD_16_16_16
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWSMOD_16_16_16:
        PUSH  BC
        PUSH  DE
        PUSH  HL

        MOVW  DE,AX               
        AND   A, #80H             
        MOV   L, A                ; L = 0x00 = positive, 0x80 = negative
        MOVW  AX, DE              

        BF    A.7, LABEL51         ; negative value ?
        MOVW  DE,AX
        CLRW  AX
        SUB   A,E
        XCH   A,X
        SUBC  A,D
LABEL51:
        XCHW AX, BC      
        BF    A.7, LABEL52         ; negative value ?
        MOVW  DE,AX
        CLRW  AX
        SUB   A,E
        XCH   A,X
        SUBC  A,D

LABEL52:
        CMPW  AX, #0x0000         ; divisor equal zero
        MOVW  DE, AX
        SKZ
        MOVW  AX, BC
        BZ    LABEL53

        DIVHU

        MOV   A,L
        CMP0  A                  ; check negative flag 
        MOVW  AX,DE
        BZ    LABEL53
        CLRW  AX
        SUB   A,E
        XCH   A,X
        SUBC  A,D
LABEL53:
        ; set / clear Z-Flag according to the result
        CMPW  AX, #0x0000

        POP   HL
        POP   DE
        POP   BC
        RET

;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
;       Division of two signed longs
;
;       Input:  AX, BC     Operand 1
;               Stack      Operand 2
;
;       Call:   CALL       HWSDIV_32_32_32
;
;       Output: AX, BC     Operand1 div Operand2
;               Z-flag
; This function can be used as replacement for the library function ?SL_DIV_L03
; by using the linker option -eHWSDIV_32_32_32=?SL_DIV_L03
;------------------------------------------------------------------------------
        PUBLIC  HWSDIV_32_32_32
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWSDIV_32_32_32:
        PUSH  DE
        PUSH  HL

        XCHW  AX,BC

        PUSH  AX                  ; stack reservation for negative sign
        PUSH  AX                  ; 
        MOVW  HL,SP               ; 
        XOR   A, [HL+15]          ; XOR highest bit from Operand 2
        AND   A, #80H             ; 
        MOV   [HL+2], A           ; store negative flag (0x00 = positive, 0x80 = negative)
        POP   AX                  ; 




        BF    A.7, LABEL65         ; negative value ?
        XCHW  AX,BC
#if defined(__USE_FAR_RT_CALLS__)
        CALL    F:CHGSIGN_32
#else
        CALL  CHGSIGN_32          ; change sign
#endif
        XCHW  AX,BC
LABEL65:
        PUSH  AX
        PUSH  BC
        MOVW  HL,SP
        MOVW  AX,[HL+16]
        CMPW  AX, #0x0000          ; high word zero ?
        BZ    LABEL63

        BF    A.7, LABEL66          ; negative value ?
        XCHW  AX,BC
        MOVW  AX,[HL+14]
#if defined(__USE_FAR_RT_CALLS__)
        CALL    F:CHGSIGN_32
#else
        CALL  CHGSIGN_32          ; change sign
#endif
        BR    LABEL64
LABEL66
        XCHW  AX,BC
        MOVW  AX,[HL+14]
        BR    LABEL64
LABEL63:
        XCHW  AX,BC
        MOVW  AX,[HL+14]
        CMPW  AX, #0x0000          ; low word zero ?
        BNZ   LABEL64

        ; division by zero

        ; clean up stack
        ADDW SP, #0x04

        CLRW  AX
        CLRW  BC
        SET1  PSW.6
        BR    LABEL62

LABEL64:
        ; start division
        MOVW   DE,AX
        XCHW   AX,BC
        MOVW   HL,AX
        POP    AX
        POP    BC

        DIVWU

        ; check negative flag
        PUSH  AX                          
        MOVW  HL,SP
        MOV   A, [HL+2]                   
        CMP0  A                           
        POP   AX                          

        SKZ
#if defined(__USE_FAR_RT_CALLS__)
        CALL    F:CHGSIGN_32
#else
        CALL  CHGSIGN_32          ; change sign
#endif
LABEL68
       ; clean up stack

        ; set / clear Z-Flag according to the result
        XCHW  AX,BC
        CMPW  AX, #0x0000
        BNZ   LABEL61
        XCHW  AX,BC
        CMPW  AX, #0x0000
        BR    LABEL62

LABEL61:
        XCHW  AX,BC

LABEL62:
        POP   HL    
        POP   HL
        POP   DE
        RET
;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
;       Modulo Operation of two signed longs
;
;       Input:  AX, BC     Operand 1
;               Stack      Operand 2
;
;       Call:   CALL       HWSMOD_32_32_32
;
;       Output: AX, BC     Operand1 modulo Operand2
;               Z-flag
; This function can be used as replacement for the library function ?SL_MOD_L03
; by using the linker option -eHWSMOD_32_32_32=?SL_MOD_L03
;------------------------------------------------------------------------------
        PUBLIC  HWSMOD_32_32_32
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
HWSMOD_32_32_32:
        PUSH  DE
        PUSH  HL

        XCHW  AX,BC

        PUSH  AX                  ; stack reservation for negative sign
        PUSH  AX                  ; 
        MOVW  HL,SP               ; 
        AND   A, #80H             ; 
        MOV   [HL+2], A           ; store negative flag (0x00 = positive, 0x80 = negative)
        POP   AX                  ; 

        BF    A.7, LABEL75         ; negative value ?
        XCHW  AX,BC
#if defined(__USE_FAR_RT_CALLS__)
        CALL    F:CHGSIGN_32
#else
        CALL  CHGSIGN_32          ; change sign
#endif
        XCHW  AX,BC
LABEL75:
        PUSH  AX
        PUSH  BC
        MOVW  HL,SP
        MOVW  AX,[HL+16]
        CMPW  AX, #0x0000          ; high word zero ?
        BZ    LABEL73

        BF    A.7, LABEL76          ; negative value ?
        XCHW  AX,BC
        MOVW  AX,[HL+14]
#if defined(__USE_FAR_RT_CALLS__)
        CALL    F:CHGSIGN_32
#else
        CALL  CHGSIGN_32          ; change sign
#endif
        BR    LABEL74
LABEL76:
        XCHW  AX,BC
        MOVW  AX,[HL+14]
        BR    LABEL74
LABEL73:
        XCHW  AX,BC
        MOVW  AX,[HL+14]
        CMPW  AX, #0x0000          ; low word zero ?
        BNZ   LABEL74

        ; division by zero

        MOV   A, [HL+4]                   
        MOV   C,A
        ; clean up stack
        ADDW SP, #0x04

        CMP0  C  
        MOVW  AX,#0xED68
        CLRW  BC
        SKZ   
#if defined(__USE_FAR_RT_CALLS__)
        CALL    F:CHGSIGN_32
#else
        CALL  CHGSIGN_32          ; change sign
#endif
        CLR1  PSW.6
        BR    LABEL72

LABEL74:
        ; start division
        MOVW   DE,AX
        XCHW   AX,BC
        MOVW   HL,AX
        POP    AX
        POP    BC

        DIVWU

        ; check negative flag
        PUSH  HL                          
        MOVW  HL,SP
        MOV   A, [HL+2]                   
        CMP0  A                           
        POP   BC
        MOVW  AX,DE  

        SKZ
#if defined(__USE_FAR_RT_CALLS__)
        CALL    F:CHGSIGN_32
#else
        CALL  CHGSIGN_32          ; change sign
#endif
LABEL78:
       ; clean up stack

        ; set / clear Z-Flag according to the result
        XCHW  AX,BC
        CMPW  AX, #0x0000
        BNZ   LABEL71
        XCHW  AX,BC
        CMPW  AX, #0x0000
        BR    LABEL72

LABEL71:
        XCHW  AX,BC

LABEL72:
        POP   HL    
        POP   HL
        POP   DE
        RET
;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
;       Change sign of long
;
;       Input:  AX, BC     Operand
;
;       Call:   CALL       CHGSIGN_32
;
;       Output: AX, BC     -(Operand)
;------------------------------------------------------------------------------
#ifdef __USE_FAR_RT_CALLS__
        SECTION `.ftext`:CODE:NOROOT(0)
#else
        SECTION `.text`:CODE:NOROOT(0)
#endif        
CHGSIGN_32:
        PUSH HL
        MOVW HL,AX
        CLRW AX
        SUB  A,L
        XCH  A,X
        SUBC A,H

        MOVW HL,AX

        CLRW AX
        SUBC A,C
        XCH  A,X
        SUBC A,B

        XCHW AX,BC
        MOVW AX,HL

        POP  HL
        ret

;------------------------------------------------------------------------------

        END
