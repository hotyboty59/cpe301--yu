/*
 * Design Assignment1.asm
 *
 *  Created: 2/17/2014 8:23:24 PM
 *   Author: Tingyu
 */ 

 /* Macro to set the stack pointer to end of ram */
 .MACRO SET_STACK
	LDI	R16, HIGH(RAMEND)
	OUT SPH, R16
	LDI R16, LOW(RAMEND)
	OUT	SPL, R16
.ENDMACRO

SET_STACK

LDI	R18, 0x32	;load 50 to R16
LDI R19, 0x14	;load 20 to R19 to be use as a counter
LDI R20, 0
LDI R21, 0
LDI R23, 0
LDI R24, 0

L1: PUSH R18  ;loop to push 20 value to a stack		
	INC R18
	DEC R19
	BRNE L1

LDI R19, 0x15
LDI R30, 0
LDI R27, 0x3
LDI R28, 0x5

NEXT:
	DEC R19		;parse throught the 20 numbers
	CP R19, R30
	BREQ END	 
	POP R15	;pop a number from the stack
	LDI R25, 0x1B   ;load 27 to R25	
	LDI R26, 0x19	;load 25 to R26

L3:
	CP R25, R15   ;compare the number with R25 if it's greater or equal to, jump to the next number
	BRGE NEXT
	ADD R25, R27   ;add 3 to R25
	ADD R26, R28	;add 5 to R26
	CP R25, R15		;compare R25 with the number if equal
	BREQ SUM3	;branch to add to the sum of all number divisible by 3
L5:
	CP R26, R15		;compare R26 with the number if equal
	BREQ SUM5		;branch to add to the sum of all number divisible by 5
	RJMP L3
SUM3:
	ADD R21, R15   ;add the number divisible by 3 and store in R20:21
	ADC R20, R30
	RJMP L5
SUM5:
	ADD R24, R15	;add the number divisible by 5 and store in R23:24
	ADC R23, R30
	CP R19, R30     
	BRNE L3

END:
	RET
		
	
		
	
