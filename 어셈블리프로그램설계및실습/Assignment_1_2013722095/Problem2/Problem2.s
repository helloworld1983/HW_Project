	AREA	ARMex, CODE, READONLY
			ENTRY
start
		mov r0,#1 		;insert each value into each register
		mov r1,#2
		mov r2,#3
		mov r3,#4
		
		ldr r4,TEMP 		; insert memory's starting address into register 4
		mov r7,r4			; save register4's value into register 7
		
		STRB r0, [r4], #1 ; save r0~r3's value into memory
		STRB r1, [r4], #1
		STRB r2, [r4], #1
		STRB r3, [r4]
		
		ldr r5, [r7] ; little - endian type 
		
		mov r4, r7			;initializing starting address
		STRB r3, [r4], #1		; save r0~r3's value into memory
		STRB r2, [r4], #1
		STRB r1, [r4], #1
		STRB r0, [r4]
		ldr r6, [r7] ; big - endian type 
TEMP	&	&00001000
		mov pc,lr
		end