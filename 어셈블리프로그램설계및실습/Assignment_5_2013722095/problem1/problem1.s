	AREA	ARMex, CODE, READONLY
			ENTRY
start
  	ldr r0, value1		; �Է°� 1
	ldr r1, value2		; �Է°� 2
	ldr r12, temp		; ������ ������ �ּ�
	mov r10, 0x10000000
	mov r11, 0x80000000	;	���밪 �񱳸� ���� ����
	mov lr, pc
	b func
	
	str r9, [r12]
	mov pc, #0

func ; �� ���� �Լ�
	mov r2, r0, lsr #31; sign bit ����
	mov r3, r1, lsr #31
	cmp r2, r3			; �� �Է°��� ��ȣ�� ���� ������?
	movne r11, #1		; ���� ��ȣ�� �ٸ��� 1
	moveq r11, #0
	mov r4, r0, lsl #1	; exponent�� �����ϰ� �����ʳ����� �о��
	mov r4, r4, lsr #24
	mov r5, r1, lsl #1	
	mov r5, r5, lsr #24
	mov r6, r0, lsl #9 	; mantissa �����ϰ� 1�� ���� �ڸ� ����
	mov r6, r6, lsr #4
	mov r7, r1, lsl #9
	mov r7, r7, lsr #4

	add r6, r6, r10 	; mantissa�� 1�� ������
	add r7, r7, r10 	; mantissa�� 1�� ������
	
	cmp r4, r5			; exponent���� ���ؼ� nomalizing
	sublt r8, r5, r4		; �� exponent�� �������� ���.
	subgt r8, r4, r5
	movlt r4, r5
	movlt r6, r6, lsr r8	; mantissa�� �Ҽ��� �̵�
	movgt r7, r7, lsr r8
	
	
	mov r0, r0, lsl #1; ���밪���� ��ȯ
	mov r1, r1, lsl #1
	mov r0, r0, lsr #1; 
	mov r1, r1, lsr#1
	
	cmp r0, r1		; ���밪�� ū ���� sign��Ʈ�� ������Ʈ�� ����
	movgt r9, r2
	movlt r9, r3

	cmp r11, #0 			; �� �Է°��� ��ȣ�� ���� ������?
	beq addition


subtraction

	cmp r0, r1		; ���밪�� ū ���� sign��Ʈ�� ������Ʈ�� ����
	subgt r8, r6, r7	; ���밪�� ū ������ ���� ���� mantissa�� ��
	sublt r8, r7, r6
	mov r10, r8, lsr #28	; ������ ���Ŀ� 1�� ���ߴ� �ڸ��� 0�ΰ��� Ȯ��
	cmp r10, #0				
	subeq r4, r4, #1		; �׷��ٸ� ���������� 1�� ����
	moveq r8, r8, lsl #1  ; �Ҽ��� �̵�
	b next
	

addition
	
	add r8, r6, r7		; �� mantissa�� ��
	mov r10, r8, lsr #29 ; ��������� carry�� ����°� Ȯ��
	cmp r10, #0				; r10�� 0�ΰ�?
	addne r4,  r4, #1	;�ƴϸ� �������� 1�� ����.
	movne r8, r8, lsr #1; �Ҽ��� �ڸ� �̵�
	

next

	mov r9,r9, lsl #8 ; exponent�� �ڸ� ����
	add r9, r9, r4
	mov r9, r9, lsl #23
	mov r8, r8, lsl #4 ; ���ߴ� 1 ����
	mov r8, r8, lsr #9; ��Ʈ �ڸ� ����
	add r9, r9, r8
	mov pc, lr
value1 DCD 0x410C0000
value2 DCD 0xC0600000
temp DCD 0x40000
	
	end