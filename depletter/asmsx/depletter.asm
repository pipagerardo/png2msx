;-----------------------------------------------------------
;| ------------------------------------------------------- |
;| |                    I N I C I O                      | |
;| ------------------------------------------------------- |
;-----------------------------------------------------------
DEPLETER:
; Pletter v0.5c MSX unpacker - XL2S Entertainment 2008
; Copyright (c) 2002-2003 Team Bomba.
; Entrada:	HL = RAM/ROM source
; Salida:	DE = RAM desination
; Registros:	Todos
; Requisitos:
; PLETTER_LENGTHINDATA=1
	IFDEF PLETTER_LENGTHINDATA
	INC	HL
	INC	HL
	ENDIF
; ------------------------------------------------
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
	LD	HL, PLETTER_modes
	ADD	HL, DE
	LD	E, [HL]
	LD	IXL, E
	INC	hl
	LD	E, [HL]
	LD	IXH, E
	LD	E, 1
	EXX
	LD	IY, PLETTER_loop
PLETTER_literal:		; MAIN DEPACK LOOP
	LDI
PLETTER_loop:
	ADD	A,  A
	CALL	Z,  PLETTER_getbit
	JR	NC, PLETTER_literal
	EXX			; COMPRESSED DATA
	LD	H, D
	LD	L, E
PLETTER_getlen:
	ADD	A,  A
	CALL	Z,  PLETTER_getbitexx
	JR	NC, @@PLETTER_lenok
@@PLETTER_lus:
	ADD	A,  A
	CALL	Z,  PLETTER_getbitexx
	ADC	HL, HL
	RET	C
	ADD	A,  A
	CALL	Z,  PLETTER_getbitexx
	JR	NC, @@PLETTER_lenok
	ADD	A,  A
	CALL	Z,  PLETTER_getbitexx
	ADC	HL, HL
	RET	C
	ADD	A, A
	CALL	Z, PLETTER_getbitexx
	JP	C, @@PLETTER_lus
@@PLETTER_lenok:
	INC	HL
	EXX
	LD	C, [HL]
	INC	HL
	LD	B, 0
	BIT	7, C
	JP	Z, PLETTER_offsok
	JP	IX
PLETTER_mode6:
	ADD	A, A
	CALL	Z, PLETTER_getbit
	RL	B
PLETTER_mode5:
	ADD	A, A
	CALL	Z, PLETTER_getbit
	RL	B
PLETTER_mode4:
	ADD	A, A
	CALL	Z, PLETTER_getbit
	RL	B
PLETTER_mode3:
	ADD	A, A
	CALL	Z, PLETTER_getbit
	RL	B
PLETTER_mode2:
	ADD	A, A
	CALL	Z, PLETTER_getbit
	RL	B
	ADD	A,  A
	CALL	Z,  PLETTER_getbit
	JR	NC, PLETTER_offsok
	OR	A
	INC	B
	RES	7, C
PLETTER_offsok:
	INC	BC
	PUSH	HL
	EXX
	PUSH	HL
	EXX
	LD	L, E
	LD	H, D
	SBC	HL,BC
	POP	BC
	LDIR
	POP	HL
	JP	IY
PLETTER_getbit:
	LD	A, [HL]
	INC	HL
	RLA
	RET
PLETTER_getbitexx:
	EXX
	LD	A, [HL]
	INC	HL
	EXX
	RLA
	RET
PLETTER_modes:
	DW	PLETTER_offsok
	DW	PLETTER_mode2
	DW	PLETTER_mode3
	DW	PLETTER_mode4
	DW	PLETTER_mode5
	DW	PLETTER_mode6

;-----------------------------------------------------------
;| ------------------------------------------------------- |
;| |                      F I N                          | |
;| ------------------------------------------------------- |
;-----------------------------------------------------------