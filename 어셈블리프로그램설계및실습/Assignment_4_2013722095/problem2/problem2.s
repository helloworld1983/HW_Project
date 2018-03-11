	AREA	ARMex, CODE, READONLY
		ENTRY
start
 	ldr r10, temp ; r10에 메모리 주소 저장
main
	mov r0, #10 ; 아래와 같이 reg들에 값을 저장
	mov r1, #11
	mov r2, #12
	mov r3, #13
	mov r4, #14
	mov r5, #15
	mov r6, #16
	mov r7, #17

	
	; r11 은 return 으로 함.
	
	stmea sp, {r0-r7} ; reg값들을 stack에 복사
	mov lr, pc ; 돌아올 위치를 lr에 저장
	B doRegister ; doRegister함수 호출
	

	mov r9, r11 ; r9에 r11값 저장
	
 	cmp r12, r11, lsl #31 ; r11의 1의자리 비트값이 0인가?(짝/홀 비교)
	beq even ; 짝수면 even으로 점프


	sub r9, r1	; index가 홀수인 값들을 빼줌
	sub r9, r3
	sub r9, r5
	sub r9, r7
	str r9, [r10]	; 빼준 값을 0x40000에 저장
	mov pc, #0
even
	sub r9, r0 ; index가 짝수인 값들을 빼줌
	sub r9, r2
	sub r9, r4
	sub r9, r6
	str r9, [r10]	; 빼준 값을 0x40000에 저장
	mov pc, #0

	
doRegister
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp 가 가리키는 값을 r9에 복사
	mul r9, r8, r9 ; 복사해둔 reg값과 index의 곱을 저장
	add r11, r9, r11 ; 그 곱을 return에 더함.
	
	
	mov pc, lr ; index가 7이면 doRegister를 호출했던 곳으로 이동
temp & &40000
	END
		