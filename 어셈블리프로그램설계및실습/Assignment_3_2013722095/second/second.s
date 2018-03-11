	AREA ARMex,CODE,READONLY
		ENTRY

start
	LDR r4,temp
	mov r3, #1				 ; 1!
	STR r3, [r4], #4
	mov r0, r3, LSL #1     ; 2!
	STR r0, [r4], #4
	ADD r1, r0,r0, LSL #1 ; 3!
	STR r1, [r4], #4
	MOV r0, r1, LSL #2	 ; 4!
	STR r0, [r4], #4
	ADD r1, r0,r0, LSL #2 ; 5!
	STR r1, [r4], #4
	MOV r2, r1, LSL #1	 ; 2*5!
	ADD r0, r2, r1, LSL #2 ; 2*5! + 4*5!  ->6!
	STR r0, [r4], #4
	RSB r1, r0, r0, LSL #3; 7!
	STR r1, [r4], #4
	MOV r0, r1, LSL #3     ; 8!
	STR r0, [r4], #4
	ADD r1, r0, r0, LSL #3 ; 9!
	STR r1, [r4], #4
	MOV r2, r1, LSL #1;    ; 2*9!
	ADD r0, r2, r1, LSL #3;; 8*9! + 2*9! ->10!
	STR r0, [r4], #4
	mov pc,lr
temp DCD &40000
	END