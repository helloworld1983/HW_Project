	AREA	ARMex, CODE, READONLY
			ENTRY
start
	MOV r0, #10 	; r0~r1���� ���� 10, 5, 6, 2�� ����(������ F)
	MOV r1, #5
	MOV r2, #6
	MOV r3, #2
	
	SUB r0,r0,r1		; r0�� r0-r1�� ����(10-5)
	MUL r1, r3, r3 	; r1�� r3*r3�� ����(2*2)
	ADD r2, r2, r1 	; r2�� r2+r1�� ����(6+4)
	ADD r0, r0, r2	; r0�� r0+r2�� ����(5+10)
	
	LDR r1, TEMP	; r1�� �ּҰ� ����
	STR r0,[r1]	; r1�� ���� ���� �ּҿ� r0�� ����
TEMP DCD &40000000
	
	mov pc,lr
	END