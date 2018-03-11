	AREA	ARMex, CODE, READONLY
		ENTRY
start
	MOV r0, #1
	LDR r2, TEMP
loop
	
	CMP r0, #11  		; r0가 11인가?
	ADDNE r1, r1, r0	; 아니라면 r1(합)에 r0를 더함
	ADDNE r0,r0,#1 	; 아니라면 r0에 1을 더함
	BNE	loop				; 아니라면 loop로 점프
	
	STRH r1, [r2]
	MOV pc,lr
TEMP DCD &40000000
	
END