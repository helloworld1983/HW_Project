	AREA	ARMex, CODE, READONLY
		ENTRY
start
  	ldr r0,=value1 ; r0에 value1의 메모리값 저장
	ldr r4, TEMP
 ; r2는 count값, r3는 value 바이트값
	
loop							; r3에 r0의 값을 넣어서 이게 0과 같은지 비교함
	ldrb r3, [r0], #1	; r0번지의 메모리에 저장된 값을 r3에 읽어들임
	CMP r3, r5			; r5(0)인지 비교
	ADDNE r2, r2, #1	; 0이 아니라면 count(r2)를 1 증가
	BNE loop				; 0이 아니라면loop 로 점프, 0이면 종료

	STRH r2, [r4]
	mov pc, lr
	
value1 DCB "Helloword"
TEMP DCD &40000000
	END