	AREA	ARMex, CODE, READONLY
		ENTRY
start
	mov r2, #1  ; �ǰ�����
	mov r1, #1 ; ������
	ldr r3, temp ; �޸� ���� �ּ�
loop
	
	MUL r2, r1,r2 ; r2�� r2*r1�� ����
	STR r2, [r3],#4 ; r2���� r3������ �����ϰ� r3�� 4 ������.
	CMP r1,#10   ; r1�� 10�ΰ�?
	ADDNE r1,#1 ; �ƴϸ� r1�� 1������Ű��
	BNE loop      ; �ƴϸ� loop�� ����
	mov pc,lr
temp DCD 0x40000
END