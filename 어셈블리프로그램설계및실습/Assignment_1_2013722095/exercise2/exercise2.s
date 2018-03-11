	AREA	ARMex, CODE, READONLY
	ENTRY
start
	MOV 	r0,#1		;insert each value into each register
	MOV		r1,#-1
	MOV		r2,#8

	
	CMP		r0,#0				;compare(r0-0)
	MOVPL r3, r0				;if r0 > 0 mov r0 into r3
	
	CMP		r1,#0
	ADDMI r3,r3,r1				;if r1 > 0 mov add r1 into r3
	
		
	CMP 	r2,#7				;r2 - r4(8-7) 
	ADDLE	r3,r3,r2			 	;if r2<=7, add r2 into r3
	
	TST		r3, #0				;compare(r3 and 0)
	MOVEQ r3,r0;				;if r3==0, mov r0 into r3
	
	mov pc,lr 
	END