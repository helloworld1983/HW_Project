	AREA	ARMex, CODE, READONLY
		ENTRY
start
	MOV r0,#16  		;insert each data into each register
	MOV r1,#10  
	MOV r2,#08 
	MOV r3,#34  
	
	LDR r4, TEMPADDR1 ; insert TEMPADDR1's address into r4
	STRB r0, [r4], #2 	 ; insert memory(1000)'s value into r0
	STRB r1, [r4], #2		  ; insert memory(1002)'s value into r1
	STRB r2, [r4], #2		  ; insert memory(1004)'s value into r2
	STRB r3, [r4]			  ; insert memory(10006's value into r3
	
	mov pc,lr
TEMPADDR1 & &00001000

	END
		