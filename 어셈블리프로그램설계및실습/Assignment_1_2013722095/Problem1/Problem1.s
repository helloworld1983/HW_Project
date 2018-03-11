	AREA	ARMex, CODE, READONLY
			ENTRY
start
		mov r0, #5			;save each value to each register
		mov r1, #10			
		mov r2, #15			
		mov r3, #10			
		
		ldr r4, TEMP			; save memory's adress into register 4
		mov r9, r4				; save resigster 4's value into register 9
		STRB r0, [r4], #1	; save resigsters' value into memory
		STRB r1, [r4], #1
		STRB r2, [r4], #1
		STRB r3, [r4], #1
		
		LDRB	r5, [r9], #1 ; load memory's value into each register
		LDRB	r6, [r9], #1
		LDRB	r7, [r9], #1
		LDRB	r3, [r9]
		
		CMP r3,r2				; compare / r3-r2
		MOVMI r5,#1		; if r3<r2, insert 1 into register 5(1004)
		MOVGT r5,#2		
		MOVEQ r5,#3
		STRB r5, [r4], #1
		
		CMP r3,r0				; compare / r3-r0
		MOVMI r5,#1		; 
		MOVGT r5,#2		
		MOVEQ r5,#3		; 
		STRB r5, [r4], #1
		
		CMP r3,r1				;compare / r3-r0
		MOVMI r5,#1		; if r3<r2, insert 1 into register 5(1004)
		MOVGT r5,#2		
		MOVEQ r5,#3	
		STRB r5, [r4]
		
		MOV pc,lr
TEMP	&	&00001000

		END