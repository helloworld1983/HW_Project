module alu4(a,b,op, result, c,n,z,v);
	input 	[3:0] a,b; // 4bit 입력 a,b
	input		[2:0] op; // 3bit opcode
	output 	[3:0] result; // 두 입력의 합
	output 			c,n,z,v; // flag
	
	wire 		[3:0] w_not_a, w_not_b, w_and, w_or, w_xor, w_xnor, w_add, w_sub; // 두 입력들의 산술 논리 결과를 이어줄 wire
	wire				c3_add, co_add, c3_sub, co_sub;
	
	_inv_4bits	 	U0_inv_4bits	(.a(a), .y(w_not_a));
	_inv_4bits 		U1_inv_4bits	(.a(b), .y(w_not_b));
	_and2_4bits		U2_and2_4bits	(.a(a), .b(b), .y(w_and));
	_or2_4bits 		U3_or2_4bits	(.a(a), .b(b), .y(w_or));
	_xor2_4bits 	U4_xor2_4bits	(.a(a), .b(b), .y(w_xor));
	_xnor2_4bits	U5_xnor2_4bits	(.a(a), .b(b), .y(w_xnor));
	cla4_ov			U6_add			(.a(a), .b(b), .ci(1'b0), .s(w_add), .c3(c3_add), .co(co_add));
	cla4_ov			U7_sub			(.a(a), .b(w_not_b), .ci(1'b1), .s(w_sub), .c3(c3_sub), .co(co_sub));
	mx8_4bits		U8_mx8_4bits	(.a(w_not_a), .b(w_not_b), .c(w_and), .d(w_or), .e(w_xor), .f(w_xnor), .g(w_add), .h(w_sub), .s2(op[2]), .s1(op[1]), .s0(op[0]), .y(result));
												
	cal_flags4		U9_cal_flags4	(.op(op), .result(result), .co_add(co_add), .c3_add(c3_add), .co_sub(co_sub), .c3_sub(c3_sub), .c(c), .n(n), .z(z), .v(v));
endmodule
