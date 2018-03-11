	AREA	ARMex, CODE, READONLY
		ENTRY
start
	mov r2, #1  ; 피곱셈값
	mov r1, #1 ; 곱셈값
	ldr r3, temp ; 메모리 접근 주소
loop
	
	MUL r2, r1,r2 ; r2에 r2*r1을 저장
	STR r2, [r3],#4 ; r2값을 r3번지에 저장하고 r3를 4 증가함.
	CMP r1,#10   ; r1이 10인가?
	ADDNE r1,#1 ; 아니면 r1을 1증가시키고
	BNE loop      ; 아니면 loop로 점프
	mov pc,lr
temp DCD 0x40000
END