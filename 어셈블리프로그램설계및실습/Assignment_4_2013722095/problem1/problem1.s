	AREA	ARMex, CODE, READONLY
		ENTRY
start
	mov r0, #0 	;	�Ʒ��� ���� ������ r0-r7�� ����
	mov r1, #1
	mov r2, #2
	mov r3, #3
	mov r4, #4
	mov r5, #5
	mov r6, #6
	mov r7, #7
	
	stmfa sp!, {r2}			;r2�� ���ÿ� ����
	stmfa sp!, {r0, r3}	; r0, r3�� ���ÿ� ����
	stmfa sp!, {r5-r7}	; r5-r7�� ���ÿ� ����
	stmfa sp!, {r1, r4}	; r1, r4�� ���ÿ� ����
	
	ldmfa sp, {r0-r7}		; ���ÿ� ������ִ� ������ register�� ����
	mov pc, #0
	end
	