CR equ 0x0d  ; CR�� 0xd�� ����
	
	AREA strcpy, CODE, READONLY
		ENTRY
main
  	ldr r0, =Arr1	; arr1�� �ּҸ� ����
	ldr r1, =Arr2	; arr2�� �ּҸ� ����
	adr r7, loop	; loop lable�� �ּҰ��� r7�� ����
	
	mov r3 , #1	; arr2�� ���̰��� 1�� �ʱ�ȭ
	strb r3, k

	mov lr, pc				; ����ġ ����
	b copy_arr_wo_space	; copy �Լ� ȣ��
	
 	strb r5,  k	; addr2�� ���̸� k�� ����
	mov pc, #0

copy_arr_wo_space
	stmfa sp!, {r0-r5} ; r0~r5�� ���ÿ� ����
	mov r4, #0		; i = 0
	mov r5, #0		; j = 0
loop
	ldrb r3, [r0, r4]	; arr1[i]�� r3�� ȣ��
	
	cmp r3, #CR ; ARR1�� ���ΰ�?
	beq DONE		; �׷��ٸ� done�� �̵�
	cmp r3, #0x20 ; ������ ���� space�ΰ�?
	addeq r5, r5, #1 ; ���� ��ġ ���Ҹ� ���ؼ� ���� �� ����
	addeq r4, r4, #1
	moveq pc, r7		   ; �׷��ٸ� �ݺ����� ó������ ���ư�
	
	sub r6, r4, r5	; i-j
	strb r3, [r1, r6]	; arr2[i-j] ��ġ�� r3���� ����
	add r4, r4, #1		; arr1�� index����
	add r9, r9, #1 		; arr2�� length

	mov pc,r7 ; r7(loop�� �ּ�)�� pc�� �ű�
	
	
DONE
	strb r9, [sp]		; arr2�� ���̰��� sp ��ġ(r5)�� ����
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
	dcb 0				; ������ ����
	
	end