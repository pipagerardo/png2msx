;-----------------------------------------------------------
;| ------------------------------------------------------- |
;| |                    I N I C I O                      | |
;| ------------------------------------------------------- |
;-----------------------------------------------------------
DEPLETER_VRAM:
; Pletter v0.5c MSX unpacker - XL2S Entertainment 2008
; Copyright (c) 2002-2003 Team Bomba.
; Versión modificada para volcar directamente a VRAM.
; Entrada:	HL = RAM/ROM source
; Salida:	DE = RAM desination
; Registros:	Todos
; Requisitos:	Desactivar interrupciones y la pantalla
; PLETTER_LENGTHINDATA=1
	IFDEF PLETTER_LENGTHINDATA
	INC	HL
	INC	HL
	ENDIF
; ------------------------------------------------
	LD	A, E			; T4
	OUT	[VDP_CMD], A	; T11
	LD	A, D			; T4
	AND	$3F			; T7
	OR	$40			; T7	
	OUT	[VDP_CMD], A	; T11	T29
	LD	A, [HL]		; INICIALIZACIÓN
	INC	HL
	EXX
	LD	DE, 0
	ADD	A, A
	INC	A
	RL	E
	ADD	A, A
	RL	E
	ADD 	A, A
	RL	E
	RL	E
	LD	HL, PLETTER_VRAM_modes
	ADD	HL, DE
	LD	E, [HL]
	LD	IXL, E
	INC	hl
	LD	E, [HL]
	LD	IXH, E
	LD	E, 1
	EXX
	LD	IY, PLETTER_VRAM_loop
PLETTER_VRAM_literal:		; MAIN DEPACK LOOP
	LD	C, VDP_DATA	; T7		$98
	OUTI				; T16
	INC	DE			; T6
PLETTER_VRAM_loop:
	ADD	A,  A
	CALL	Z,  PLETTER_VRAM_getbit
	JR	NC, PLETTER_VRAM_literal
	EXX			; COMPRESSED DATA
	LD	H, D
	LD	L, E
PLETTER_VRAM_getlen:
	ADD	A,  A
	CALL	Z,  PLETTER_VRAM_getbitexx
	JR	NC, @@PLETTER_VRAM_lenok
@@PLETTER_VRAM_lus:
	ADD	A,  A
	CALL	Z,  PLETTER_VRAM_getbitexx
	ADC	HL, HL
	RET	C
	ADD	A,  A
	CALL	Z,  PLETTER_VRAM_getbitexx
	JR	NC, @@PLETTER_VRAM_lenok
	ADD	A,  A
	CALL	Z,  PLETTER_VRAM_getbitexx
	ADC	HL, HL
	RET	C
	ADD	A, A
	CALL	Z, PLETTER_VRAM_getbitexx
	JP	C, @@PLETTER_VRAM_lus
@@PLETTER_VRAM_lenok:
	INC	HL
	EXX
	LD	C, [HL]
	INC	HL
	LD	B, 0
	BIT	7, C
	JP	Z, PLETTER_VRAM_offsok
	JP	IX
PLETTER_VRAM_mode6:
	ADD	A, A
	CALL	Z, PLETTER_VRAM_getbit
	RL	B
PLETTER_VRAM_mode5:
	ADD	A, A
	CALL	Z, PLETTER_VRAM_getbit
	RL	B
PLETTER_VRAM_mode4:
	ADD	A, A
	CALL	Z, PLETTER_VRAM_getbit
	RL	B
PLETTER_VRAM_mode3:
	ADD	A, A
	CALL	Z, PLETTER_VRAM_getbit
	RL	B
PLETTER_VRAM_mode2:
	ADD	A, A
	CALL	Z, PLETTER_VRAM_getbit
	RL	B
	ADD	A,  A
	CALL	Z,  PLETTER_VRAM_getbit
	JR	NC, PLETTER_VRAM_offsok
	OR	A
	INC	B
	RES	7, C
PLETTER_VRAM_offsok:
	INC	BC
	PUSH	HL
	EXX
	PUSH	HL
	EXX
	LD	L, E
	LD	H, D
	SBC	HL,BC
	POP	BC
	PUSH	AF
.PLETTER_LOOP:
	LD	A, L		; T4
	OUT	[$99], A	; T11	T58
	NOP			; T4
	NOP			; T4
	LD	A, H		; T4
	OUT	[$99], A	; T11	T31
	NOP			; T4
	NOP			; T4
	NOP			; T4
	IN	A, [$98]	; T11	T31
	NOP			; T4
	EX	AF, AF'		; T4
	LD	A, E		; T4
	OUT	[$99], A	; T11	T31
	LD	A, D		; T4
	AND	$3F		; T7
	OR	$40		; T7
	OUT	[$99], A	; T11	T29
	NOP			; T4
	NOP			; T4
	EX	AF, AF'		; T4
	OUT	[$98], A	; T11	T31
	INC	DE		; T6
	CPI			; T16
	JP	PE, .PLETTER_LOOP	; T10
	POP	AF
	POP	HL
	JP	IY
PLETTER_VRAM_getbit:
	LD	A, [HL]
	INC	HL
	RLA
	RET
PLETTER_VRAM_getbitexx:
	EXX
	LD	A, [HL]
	INC	HL
	EXX
	RLA
	RET
PLETTER_VRAM_modes:
	DW	PLETTER_VRAM_offsok
	DW	PLETTER_VRAM_mode2
	DW	PLETTER_VRAM_mode3
	DW	PLETTER_VRAM_mode4
	DW	PLETTER_VRAM_mode5
	DW	PLETTER_VRAM_mode6

;-----------------------------------------------------------
;| ------------------------------------------------------- |
;| |                      F I N                          | |
;| ------------------------------------------------------- |
;-----------------------------------------------------------
