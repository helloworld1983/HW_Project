	AREA	ARMex, CODE, READONLY
		ENTRY
start
	mov r0, #0 	;	아래와 같은 값들을 r0-r7에 저장
	mov r1, #1
	mov r2, #2
	mov r3, #3
	mov r4, #4
	mov r5, #5
	mov r6, #6
	mov r7, #7
	
	stmfa sp!, {r2}			;r2를 스택에 저장
	stmfa sp!, {r0, r3}	; r0, r3를 스택에 저장
	stmfa sp!, {r5-r7}	; r5-r7을 스택에 저장
	stmfa sp!, {r1, r4}	; r1, r4를 스택에 저장
	
	ldmfa sp, {r0-r7}		; 스택에 저장되있는 값들을 register로 복사
	mov pc, #0
	end
	