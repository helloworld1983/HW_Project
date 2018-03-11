	AREA	ARMex, CODE, READONLY
			ENTRY
start
	MOV r0, #10 	; r0~r1까지 각각 10, 5, 6, 2를 저장(예상결과 F)
	MOV r1, #5
	MOV r2, #6
	MOV r3, #2
	
	SUB r0,r0,r1		; r0에 r0-r1값 저장(10-5)
	MUL r1, r3, r3 	; r1에 r3*r3값 저장(2*2)
	ADD r2, r2, r1 	; r2에 r2+r1값 저장(6+4)
	ADD r0, r0, r2	; r0에 r0+r2값 저장(5+10)
	
	LDR r1, TEMP	; r1에 주소값 저장
	STR r0,[r1]	; r1이 가진 값의 주소에 r0를 저장
TEMP DCD &40000000
	
	mov pc,lr
	END