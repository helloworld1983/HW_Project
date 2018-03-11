`timescale 1ns/100ps

module tb_alu4; // testbench module
	reg	[3:0]		tb_a, tb_b; // input register
	reg	[2:0]		tb_op;		 // input register
	wire	[3:0]		tb_result;		// result wire
	wire 				tb_c, tb_n, tb_z, tb_v; // flag wire
	
	alu4 U0_alu4	(.a(tb_a), .b(tb_b), .op(tb_op), .result(tb_result), .c(tb_c), .n(tb_n), .z(tb_z), .v(tb_v)); // alu4 module 에 tb인자들을 적용 
	
	initial
	begin
			tb_a = 4'b0000; tb_b = 4'b0000; tb_op = 3'b000;
	#10;	tb_a = 4'b1100;
	#10;  tb_b = 4'b0011; 					  tb_op = 3'b001;
	#10;	tb_a = 4'b0101; tb_b = 4'b1001; tb_op = 3'b010;
	#10;	tb_a = 4'b0101; tb_b = 4'b1010; tb_op = 3'b011;
	#10;	tb_a = 4'b0011; tb_b = 4'b0101; tb_op = 3'b100;
	#10;							    				tb_op = 3'b101;
	#10;	tb_a = 4'b0000; tb_b = 4'b0000; tb_op = 3'b110;
	#10;	tb_a = 4'hf; tb_b = 4'hf;
	#10;	tb_a = 4'ha; tb_b = 4'h3;
	#10;	tb_a = 4'b0001; tb_b = 4'b1000;
	#10;	tb_a = 4'h7; tb_b = 4'h7;
	#10;	tb_a = 4'h3; tb_b = 4'h3;
	#10;	tb_a = 4'hf; tb_b = 4'h5; tb_op = 3'b111;
	#10;	tb_a = 4'h5; tb_b = 4'h7;
	#10;	tb_a = 4'ha; tb_b = 4'ha;
	#10;	tb_a = 4'h7; tb_b = 4'h9;
	#10;	$stop;
	end
endmodule
