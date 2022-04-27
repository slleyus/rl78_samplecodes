; Copyright(C) NEC Electronics Corporation 2006,2007,2008
; NEC ELECTRONICS CONFIDENTIAL AND PROPRIETARY
; All rights reserved by NEC Electronics Corporation.
; This program must be used solely for the purpose for which
; it was furnished by NEC Electronics Corporation. No part of this
; program may be reproduced or disclosed to others, in any
; form, without the prior written permission of NEC Electronics
; Corporation. Use of copyright notice does not evidence
; publication of the program.
;-------------------------------------
;
;-------------------------------------
_STATIC		EQU	0
_PASCAL		EQU	0
$IF (SAVERSVD)
_SAVERSVD	EQU	1
$ELSE
_SAVERSVD	EQU	0
$ENDIF
_OLDFUNC	EQU	0

$IF (XMULDIV)
_XMULDIV	EQU	1
_XMUL		EQU	1
$ELSE
_XMULDIV	EQU	0
 $IF (XMUL)
_XMUL		EQU	1
 $ELSE
_XMUL		EQU	0
 $ENDIF
$ENDIF

$IF (FLASH)
_FLASH		EQU	1
$ELSE
_FLASH		EQU	0
$ENDIF

$IF (MIRROR)
_MIRROR		EQU	1
$ELSE
_MIRROR		EQU	0
$ENDIF

$IF (NOESCOPY)
_ESCOPY		EQU	0
$ELSE
_ESCOPY		EQU	1
$ENDIF

_LIBCNSTFAR	EQU	1

; NEAR/FAR SWITCH
$IF (NEARFARSW)
_ORGFUNC	EQU	0
; MEMORY MODEL
$IF (LARGE)
_CODEFAR	EQU	1
_DATAFAR	EQU	0
$ELSE
 $IF (COMPACT)
_CODEFAR	EQU	1
_DATAFAR	EQU	0
  $ELSE
  $IF (MEDIUM)
_CODEFAR	EQU	1
_DATAFAR	EQU	1
   $ELSE ; SMALL
_CODEFAR	EQU	1
_DATAFAR	EQU	1
  $ENDIF
 $ENDIF
$ENDIF
$ELSE	; NORMAL
_ORGFUNC	EQU	1
; MEMORY MODEL
$IF (LARGE)
_CODEFAR	EQU	1
_DATAFAR	EQU	1
$ELSE
 $IF (COMPACT)
_CODEFAR	EQU	1
_DATAFAR	EQU	1
  $ELSE
  $IF (MEDIUM)
_CODEFAR	EQU	1
_DATAFAR	EQU	0
   $ELSE ; SMALL
_CODEFAR	EQU	1
_DATAFAR	EQU	0
  $ENDIF
 $ENDIF
$ENDIF
$ENDIF

$IF (DHRYSTONE)
_DHRYSTONE		EQU	1
$ELSE
_DHRYSTONE		EQU	0
$ENDIF

_PATCH01	EQU	1	;for 1st device
