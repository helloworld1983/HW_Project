	AREA	ARMex, CODE, READONLY
		ENTRY
start
	MOV r0, #1 ; r0 =1
	LDR r5, TEMP
loop
	ADD r1,r0,#1 ;r1 = r0 +1
	CMP r0, #10 ; r0�� 10�ΰ�?
	ADDNE	r0, r0,#1 ; �ƴϸ� r0�� 1�� ����
	BNE	loop				; �ƴϸ� loop�� ����
	
	MUL r2, r0, r1 ; �´ٸ� r2 = r0*r1
	MOV r2, r2, LSR #1	; r2 =r2/2 (��Ʈ�� ���������� ��ĭ �ű����ν� 2�� ������ ���� ��ȯ)
	STR r2, [r5]
	
	mov pc,lr
TEMP DCD &40000000
	END
	