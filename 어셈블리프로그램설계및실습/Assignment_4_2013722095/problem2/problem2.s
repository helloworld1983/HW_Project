	AREA	ARMex, CODE, READONLY
		ENTRY
start
 	ldr r10, temp ; r10�� �޸� �ּ� ����
main
	mov r0, #10 ; �Ʒ��� ���� reg�鿡 ���� ����
	mov r1, #11
	mov r2, #12
	mov r3, #13
	mov r4, #14
	mov r5, #15
	mov r6, #16
	mov r7, #17

	
	; r11 �� return ���� ��.
	
	stmea sp, {r0-r7} ; reg������ stack�� ����
	mov lr, pc ; ���ƿ� ��ġ�� lr�� ����
	B doRegister ; doRegister�Լ� ȣ��
	

	mov r9, r11 ; r9�� r11�� ����
	
 	cmp r12, r11, lsl #31 ; r11�� 1���ڸ� ��Ʈ���� 0�ΰ�?(¦/Ȧ ��)
	beq even ; ¦���� even���� ����


	sub r9, r1	; index�� Ȧ���� ������ ����
	sub r9, r3
	sub r9, r5
	sub r9, r7
	str r9, [r10]	; ���� ���� 0x40000�� ����
	mov pc, #0
even
	sub r9, r0 ; index�� ¦���� ������ ����
	sub r9, r2
	sub r9, r4
	sub r9, r6
	str r9, [r10]	; ���� ���� 0x40000�� ����
	mov pc, #0

	
doRegister
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	add r8, r8, #1 ; index +1
	
	ldmfd sp!, {r9} ; sp �� ����Ű�� ���� r9�� ����
	mul r9, r8, r9 ; �����ص� reg���� index�� ���� ����
	add r11, r9, r11 ; �� ���� return�� ����.
	
	
	mov pc, lr ; index�� 7�̸� doRegister�� ȣ���ߴ� ������ �̵�
temp & &40000
	END
		