CR equ 0x0d  ; CR을 0xd로 정의
	
	AREA strcpy, CODE, READONLY
		ENTRY
main
  	ldr r0, =Arr1	; arr1의 주소를 저장
	ldr r1, =Arr2	; arr2의 주소를 저장
	adr r7, loop	; loop lable의 주소값을 r7에 저장
	
	mov r3 , #1	; arr2의 길이값을 1로 초기화
	strb r3, k

	mov lr, pc				; 현위치 저장
	b copy_arr_wo_space	; copy 함수 호출
	
 	strb r5,  k	; addr2의 길이를 k에 저장
	mov pc, #0

copy_arr_wo_space
	stmfa sp!, {r0-r5} ; r0~r5를 스택에 저장
	mov r4, #0		; i = 0
	mov r5, #0		; j = 0
loop
	ldrb r3, [r0, r4]	; arr1[i]을 r3에 호출
	
	cmp r3, #CR ; ARR1의 끝인가?
	beq DONE		; 그렇다면 done로 이동
	cmp r3, #0x20 ; 복사할 값이 space인가?
	addeq r5, r5, #1 ; 저장 위치 감소를 위해서 빼줄 값 증가
	addeq r4, r4, #1
	moveq pc, r7		   ; 그렇다면 반복문의 처음으로 돌아감
	
	sub r6, r4, r5	; i-j
	strb r3, [r1, r6]	; arr2[i-j] 위치에 r3값을 저장
	add r4, r4, #1		; arr1의 index증가
	add r9, r9, #1 		; arr2의 length

	mov pc,r7 ; r7(loop의 주소)로 pc를 옮김
	
	
DONE
	strb r9, [sp]		; arr2의 길이값을 sp 위치(r5)에 저장
	ldr 	r5, [sp]; pop
	mov pc, lr
	
;===========dataarray area
	AREA dataArray, DATA
		
k
	dcb 0
	
Arr1
	dcb "Hello, World", CR
	ALIGN
	
	
Arr2
	dcb 0				; 복사할 공간
	
	end