	AREA	ARMex, CODE, READONLY
			ENTRY
start
  	ldr r0, value1		; 입력값 1
	ldr r1, value2		; 입력값 2
	ldr r12, temp		; 최종값 저장할 주소
	mov r10, 0x10000000
	mov r11, 0x80000000	;	절대값 비교를 위한 기준
	mov lr, pc
	b func
	
	str r9, [r12]
	mov pc, #0

func ; 실 연산 함수
	mov r2, r0, lsr #31; sign bit 추출
	mov r3, r1, lsr #31
	cmp r2, r3			; 두 입력값의 부호가 서로 같은가?
	movne r11, #1		; 서로 부호가 다르면 1
	moveq r11, #0
	mov r4, r0, lsl #1	; exponent값 추출하고 오른쪽끝으로 밀어둠
	mov r4, r4, lsr #24
	mov r5, r1, lsl #1	
	mov r5, r5, lsr #24
	mov r6, r0, lsl #9 	; mantissa 추출하고 1을 더할 자리 마련
	mov r6, r6, lsr #4
	mov r7, r1, lsl #9
	mov r7, r7, lsr #4

	add r6, r6, r10 	; mantissa에 1을 더해줌
	add r7, r7, r10 	; mantissa에 1을 더해줌
	
	cmp r4, r5			; exponent값을 비교해서 nomalizing
	sublt r8, r5, r4		; 두 exponent의 곱셈차를 계산.
	subgt r8, r4, r5
	movlt r4, r5
	movlt r6, r6, lsr r8	; mantissa의 소수점 이동
	movgt r7, r7, lsr r8
	
	
	mov r0, r0, lsl #1; 절대값으로 변환
	mov r1, r1, lsl #1
	mov r0, r0, lsr #1; 
	mov r1, r1, lsr#1
	
	cmp r0, r1		; 절대값이 큰 수의 sign비트가 최종비트로 사용됨
	movgt r9, r2
	movlt r9, r3

	cmp r11, #0 			; 두 입력값의 부호가 서로 같은가?
	beq addition


subtraction

	cmp r0, r1		; 절대값이 큰 수의 sign비트가 최종비트로 사용됨
	subgt r8, r6, r7	; 절대값이 큰 값에서 작은 값의 mantissa를 뺌
	sublt r8, r7, r6
	mov r10, r8, lsr #28	; 뺄셈한 이후에 1을 더했던 자리가 0인가를 확인
	cmp r10, #0				
	subeq r4, r4, #1		; 그렇다면 곱셈값에서 1을 빼고
	moveq r8, r8, lsl #1  ; 소수점 이동
	b next
	

addition
	
	add r8, r6, r7		; 두 mantissa의 합
	mov r10, r8, lsr #29 ; 덧셈결과로 carry가 생겼는가 확인
	cmp r10, #0				; r10이 0인가?
	addne r4,  r4, #1	;아니면 곱셈값에 1을 더함.
	movne r8, r8, lsr #1; 소수점 자리 이동
	

next

	mov r9,r9, lsl #8 ; exponent들어갈 자리 마련
	add r9, r9, r4
	mov r9, r9, lsl #23
	mov r8, r8, lsl #4 ; 더했던 1 제거
	mov r8, r8, lsr #9; 비트 자리 맞춤
	add r9, r9, r8
	mov pc, lr
value1 DCD 0x410C0000
value2 DCD 0xC0600000
temp DCD 0x40000
	
	end