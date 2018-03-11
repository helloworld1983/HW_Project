	AREA	ARMex, CODE, READONLY
			ENTRY
start
	mov r0, #1; r0 =1
	ldr r2, TEMP
	ADD r1, r1, r0 ; r1 =1
	ADD r0, r0, #1; r0 =2
	ADD r1, r1, r0 ; r1 =3
	ADD r0, r0, #1; r0 =3
	ADD r1, r1, r0 ; r1 =6
	ADD r0, r0, #1; r0 =4
	ADD r1, r1, r0 ; r1 =10
	ADD r0, r0, #1; r0 =5
	ADD r1, r1, r0 ; r1 =15
	ADD r0, r0, #1; r0 =6
	ADD r1, r1, r0 ; r1 =21
	ADD r0, r0, #1; r0 =7
	ADD r1, r1, r0 ; r1 =28
	ADD r0, r0, #1; r0 =8
	ADD r1, r1, r0 ; r1 =36
	ADD r0, r0, #1; r0 =9
	ADD r1, r1, r0 ; r1 =45
	ADD r0, r0, #1; r0 =10
	ADD r1, r1, r0 ; r1 =55
	
	STRH r1, [r2]
	mov pc, lr
TEMP DCD &40000000
	END