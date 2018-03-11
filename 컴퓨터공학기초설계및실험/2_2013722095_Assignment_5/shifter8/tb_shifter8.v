`timescale 1ns/100ps

module tb_shifter8;
reg clk, reset_n;
reg [2:0] op;
reg [1:0] shamt;
reg [7:0] d_in;
wire [7:0] d_out;

shifter8 U0(clk, reset_n, op, shamt, d_in, d_out); // topmodule 에 tb변수들을 적용 

parameter temp = 10;
always #(temp/2) clk = ~clk; // 반주기마다 clk를 반전시킴 
	initial
	begin
			clk =0; reset_n=0; op = 3'b000; shamt = 2'b10; d_in = 8'b00101010; // 초기값 설정 input : 11001010 / Shift amount : 2
	#10;	reset_n =1; op = 3'b001; // load
	#10;	op = 3'b010; // lsl
	#10;	op = 3'b000; // 현재값 유지 
	#10;	op = 3'b100; // asr
	#10;	op = 3'b011; // lsr
	#10;	$stop;
	end
endmodule
