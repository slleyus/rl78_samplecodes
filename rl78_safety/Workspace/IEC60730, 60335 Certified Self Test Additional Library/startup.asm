; Copyright(C) NEC Electronics Corporation 2006,2007
; NEC ELECTRONICS CONFIDENTIAL AND PROPRIETARY
; All rights reserved by NEC Electronics Corporation.
; This program must be used solely for the purpose for which
; it was furnished by NEC Electronics Corporation. No part of this
; program may be reproduced or disclosed to others, in any
; form, without the prior written permission of NEC Electronics
; Corporation. Use of copyright notice does not evidence
; publication of the program.
;-------------------------------------
;	W-1	cstart
;
;-------------------------------------
	NAME	@cstart

$INCLUDE (def.inc)
$INCLUDE (macro.inc)

;-----------------------------------------------------------------------------
; declaration of symbol
;
; attention):	change to EQU value 1 -> 0  if necessary
;-----------------------------------------------------------------------------
;
BRKSW	 EQU	1	;brk,sbrk,calloc,free,malloc,realloc function use
EXITSW	 EQU	1	;exit,atexit function use
RANDSW	 EQU	1	;rand,srand  function use
DIVSW	 EQU	1	;div	     function use
LDIVSW	 EQU	1	;ldiv 	     function use
FLOATSW	 EQU	1	;floating point variables use
STRTOKSW EQU	1	;strtok      function use

	PUBLIC	_@cstart,_@cend

$_IF(BRKSW)
	PUBLIC	_@BRKADR,_@MEMTOP,_@MEMBTM
$ENDIF
$_IF(EXITSW)
	PUBLIC	_@FNCTBL,_@FNCENT
$ENDIF
$_IF(RANDSW)
	PUBLIC	_@SEED
$ENDIF
$_IF(DIVSW)
	PUBLIC	_@DIVR
$ENDIF
$_IF(LDIVSW)
	PUBLIC	_@LDIVR
$ENDIF
$_IF(STRTOKSW)
	PUBLIC	_@TOKPTR
$ENDIF
$_IF(BRKSW OR FLOATSW)
	PUBLIC	_errno
$ENDIF

;-----------------------------------------------------------------------------
; external declaration of symbol for stack area
;
; _@STBEG has value of the end address +1 of compiler's stack area.
; _@STBEG is created by linker with -S option.
; Accordingly, specify the -S option when linking.
;
; _@MAA has value of the mirror area.
;-----------------------------------------------------------------------------
	EXTRN	_main,_@STBEG,_hdwinit,_low_level_init

$_IF(EXITSW)
	EXTRN	_exit
$ENDIF
$_IF(_MIRROR)
	EXTRN	_@MAA
$ENDIF

;-----------------------------------------------------------------------------
; external declaration of label for ROMable
;-----------------------------------------------------------------------------
	EXTRN	_?R_INIT,_?RLINIT,_?R_INIS,_?DATA,_?DATAL,_?DATS

;-----------------------------------------------------------------------------
; allocation area which library uses
;
; _@FNCTBL	top address of area used in atexit function
; _@FNCENT	total number of functions registered by the atexit function
; _@SEED	sequence of pseudo-random numbers
; _@DIVR	a result of div library
; _@LDIVR	a result of ldiv library
; _@TOKPTR	pointer which strtok function uses
; _errno	errno number code
; _@MEMTOP	top address of area which memory management functions use
; _@MEMBTM	bottom address of area which memory management functions use
; _@BRKADR	break value
;-----------------------------------------------------------------------------
@@DATA	DSEG	BASEP	;near

$_IF(EXITSW)
_@FNCTBL:	DS	4*32
_@FNCENT:	DS	2
$ENDIF
$_IF(RANDSW)
_@SEED:		DS	4
$ENDIF
$_IF(DIVSW)
_@DIVR:		DS	4
$ENDIF
$_IF(LDIVSW)
_@LDIVR:	DS	8
$ENDIF
$_IF(STRTOKSW)
$_IF(_DATAFAR)
_@TOKPTR:	DS	4
$ELSE
_@TOKPTR:	DS	2
$ENDIF
$ENDIF
$_IF(BRKSW OR FLOATSW)
_errno:		DS	2
$ENDIF
$_IF(BRKSW)
_@BRKADR:	DS	2
_@MEMTOP:	DS	32
_@MEMBTM:
$ENDIF


@@VECT00	CSEG	AT	0
	DW	_@cstart




@@LCODE	CSEG	BASE
_@cstart:
;-----------------------------------------------------------------------------
; setting the register bank RB0 as work register set
;-----------------------------------------------------------------------------
;	NOP
;	SEL	RB0
;-----------------------------------------------------------------------------
; setting the mirror area
;-----------------------------------------------------------------------------
$_IF(_MIRROR)
	MOV	A,#_@MAA
	MOV1	CY,A.0
	MOV1	MAA,CY
$ENDIF
;-----------------------------------------------------------------------------
; setting the stack pointer
;
; _@STBEG is created by linker with -S option.
;-----------------------------------------------------------------------------
	MOVW	SP,#LOWW _@STBEG	;SP <- stack begin address
	CALL	!!_hdwinit
;-----------------------------------------------------------------------------
; errno and _@FNCENT are initialized to 0
;
; The positive error number will be set by several libraries at called them.
;-----------------------------------------------------------------------------
$_IF(BRKSW OR EXITSW OR RANDSW OR FLOATSW)
	CLRW	AX
$ENDIF
$_IF(BRKSW OR FLOATSW)
	MOVW	!_errno,AX	;errno <- 0
$ENDIF
$_IF(EXITSW)
	MOVW	!_@FNCENT,AX	;FNCENT <- 0
$ENDIF
;-----------------------------------------------------------------------------
; initializing _@SEED as 1
;
; Pseudo-random sequence is decided by _@SEED value. This value can be set by
; srand function. If rand is called before srand, the random sequence will
; be the same as when srand is called with a _@SEED value as 1 at first.
;-----------------------------------------------------------------------------
$_IF(RANDSW)
	MOVW	!_@SEED+2,AX
	ONEW	AX
	MOVW	!_@SEED,AX	;SEED <- 1
$ENDIF
;-----------------------------------------------------------------------------
; setting _@MEMTOP address to _@BRKADR
;-----------------------------------------------------------------------------
$_IF(BRKSW)
	MOVW	AX,#_@MEMTOP
	MOVW	!_@BRKADR,AX	;BRKADR <- #MEMTOP
$ENDIF

;#########################################################################################
;  from here included for 60730 test stuff 
  
;  end of inclusion for 60730 test stuff
;#########################################################################################


;######################################################################################
; included to display test result on eva board for march-c initial     
	CALL	!!_low_level_init
;  end of inclusion for 60730 test stuff
;######################################################################################

;-----------------------------------------------------------------------------
; clear saddr
;-----------------------------------------------------------------------------
	MOV	B,#0FEDFH-0FE20H+1
	CLRW	AX
LSADR1:
	DEC	B
	DEC	B
	MOVW	0FE20H[B],AX
	BNZ	$LSADR1

;-----------------------------------------------------------------------------
; ROM data copy
;-----------------------------------------------------------------------------

; copy external variables having initial value
$_IF(_ESCOPY)
	MOV	ES,#HIGHW _@R_INIT
$ENDIF
	MOVW	HL,#LOWW _@R_INIT
	MOVW	DE,#LOWW _@INIT
	BR	$LINIT2
LINIT1:
$_IF(_ESCOPY)
	MOV	A,ES:[HL]
$ELSE
	MOV	A,[HL]
$ENDIF
	MOV	[DE],A
	INCW	HL
	INCW	DE
LINIT2:
	MOVW	AX,HL
	CMPW	AX,#LOWW _?R_INIT
	BNZ	$LINIT1
$_IF(_DATAFAR)
; copy external variables having initial value
	MOVW	HL,#LOWW _@RLINIT
	MOVW	DE,#LOWW _@INITL
	BR	$LINIT4
LINIT3:
	MOV	ES,#HIGHW _@RLINIT
	MOV	A,ES:[HL]
	MOV	ES,#HIGHW _@INITL
	MOV	ES:[DE],A
	INCW	HL
	INCW	DE
LINIT4:
	MOVW	AX,HL
	CMPW	AX,#LOWW _?RLINIT
	BNZ	$LINIT3
$ENDIF
; copy external variables which doesn't have initial value
	MOVW	HL,#LOWW _@DATA
	MOVW	AX,#LOWW _?DATA
	BR	$LDATA2
LDATA1:
	MOV	[HL+0],#0
	INCW	HL
LDATA2:
	CMPW	AX,HL
	BNZ	$LDATA1
$_IF(_DATAFAR)
; copy external variables which doesn't have initial value
	MOV	ES,#HIGHW _@DATAL
	MOVW	HL,#LOWW _@DATAL
	MOVW	AX,#LOWW _?DATAL
	BR	$LDATA4
LDATA3:
	MOV	ES:[HL+0],#0
	INCW	HL
LDATA4:
	CMPW	AX,HL
	BNZ	$LDATA3
$ENDIF
; copy sreg variables having initial value
$_IF(_ESCOPY)
	MOV	ES,#HIGHW _@R_INIS
$ENDIF
	MOVW	HL,#LOWW _@R_INIS
	MOVW	DE,#LOWW _@INIS
	BR	$LINIS2
LINIS1:
$_IF(_ESCOPY)
	MOV	A,ES:[HL]
$ELSE
	MOV	A,[HL]
$ENDIF
	MOV	[DE],A
	INCW	HL
	INCW	DE
LINIS2:
	MOVW	AX,HL
	CMPW	AX,#LOWW _?R_INIS
	BNZ	$LINIS1

; copy sreg variables which doesn't have initial value
	MOVW	HL,#LOWW _@DATS
	MOVW	AX,#LOWW _?DATS
	BR	$LDATS2
LDATS1:
	MOV	[HL+0],#0
	INCW	HL
LDATS2:
	CMPW	AX,HL
	BNZ	$LDATS1

;-----------------------------------------------------------------------------
; call main function
;-----------------------------------------------------------------------------
	CALL	!!_main		;main();
;-----------------------------------------------------------------------------
; call exit function
;-----------------------------------------------------------------------------
$_IF(EXITSW)
	CLRW	AX
	CALL	!!_exit		;exit(0);
$ENDIF
	BR	$$
;
_@cend:
;-----------------------------------------------------------------------------
; define segment and label used by ROMable processing
;-----------------------------------------------------------------------------
@@R_INIT	CSEG	UNIT64KP
_@R_INIT:
$_IF(_DATAFAR)
@@RLINIT	CSEG	UNIT64KP
_@RLINIT:
$ENDIF
@@R_INIS	CSEG	UNIT64KP
_@R_INIS:
@@INIT	DSEG	BASEP
_@INIT:
$_IF(_DATAFAR)
@@INITL	DSEG	UNIT64KP
_@INITL:
$ENDIF
@@DATA	DSEG	BASEP
_@DATA:
$_IF(_DATAFAR)
@@DATAL	DSEG	UNIT64KP
_@DATAL:
$ENDIF
@@INIS	DSEG	SADDRP
_@INIS:
@@DATS	DSEG	SADDRP
_@DATS:
@@CALT	CSEG	CALLT0
@@CNST	CSEG	MIRRORP
@@CNSTL	CSEG	PAGE64KP
@@BITS	BSEG
;
	END
