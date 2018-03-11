	AREA	ARMex, CODE, READONLY
		ENTRY

start
		MOV		r0,#10 			; input 10 into register 0
		MOV 	r1,#10			; input 10 into register 1
		CMP		r0,r1 			; compare(r0-r1)
		MOVMI       r2,r0		;if r0<r1 / input r0 into r2
		MOVGT	  r2,r1		;if r0>r1 / input r1 into r2
		ADDEQ		  r0,r0,r1	;if r0 = r1 / input r0+r1 into r0
		MOVEQ 	  r2,r0		; insert register0's value into register 2
		
		MOV pc,lr
		END
		