	AREA	ARMex, CODE, READONLY
		ENTRY
start
	MOV r0, #1 ; r0 =1
	LDR r5, TEMP
loop
	ADD r1,r0,#1 ;r1 = r0 +1
	CMP r0, #10 ; r0가 10인가?
	ADDNE	r0, r0,#1 ; 아니면 r0에 1을 더함
	BNE	loop				; 아니면 loop로 점프
	
	MUL r2, r0, r1 ; 맞다면 r2 = r0*r1
	MOV r2, r2, LSR #1	; r2 =r2/2 (비트를 오른쪽으로 한칸 옮김으로써 2로 나눠진 값을 반환)
	STR r2, [r5]
	
	mov pc,lr
TEMP DCD &40000000
	END
	