	AREA	ARMex, CODE, READONLY
		ENTRY
start
  	ldr r0,=value1 ; r0�� value1�� �޸𸮰� ����
	ldr r4, TEMP
 ; r2�� count��, r3�� value ����Ʈ��
	
loop							; r3�� r0�� ���� �־ �̰� 0�� ������ ����
	ldrb r3, [r0], #1	; r0������ �޸𸮿� ����� ���� r3�� �о����
	CMP r3, r5			; r5(0)���� ��
	ADDNE r2, r2, #1	; 0�� �ƴ϶�� count(r2)�� 1 ����
	BNE loop				; 0�� �ƴ϶��loop �� ����, 0�̸� ����

	STRH r2, [r4]
	mov pc, lr
	
value1 DCB "Helloword"
TEMP DCD &40000000
	END