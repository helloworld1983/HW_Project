	AREA	ARMex, CODE, READONLY
		ENTRY
start
	MOV r0, #1
	LDR r2, TEMP
loop
	
	CMP r0, #11  		; r0�� 11�ΰ�?
	ADDNE r1, r1, r0	; �ƴ϶�� r1(��)�� r0�� ����
	ADDNE r0,r0,#1 	; �ƴ϶�� r0�� 1�� ����
	BNE	loop				; �ƴ϶�� loop�� ����
	
	STRH r1, [r2]
	MOV pc,lr
TEMP DCD &40000000
	
END