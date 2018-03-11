`timescale 1ns/100ps	//timescalse
module tb; // test bench module
reg [64:0] multiplier, multiplicand;
reg	op_start, op_clear, reset_n, clk;
wire op_done;
wire [127:0] result;

Multiplier U0_mul(reset_n, clk, multiplier, multiplicand, op_start, op_clear, op_done, result); // tb인자들을 Multiplier module에 적용 


parameter temp =10;


always#(temp/2) clk = ~clk; // 5ns마다 clk를 반전
initial
begin
			reset_n =0; clk =0; multiplier = 64'b0; multiplicand = 64'b0; op_start=0; op_clear =0; // 초기값 initializing
	#10	reset_n =1; multiplier = 64'h11; multiplicand = -3; // reset_n = 1로 만들고 multiplicand와 multiplier 입력
	#10;	
	#10	op_start = 1; // 연산 시작 
	#10	op_start = 0;
	#105;
	#5;	op_clear = 1; //연산이 끝나기 전에 op_clear를 1로 
	#10;	op_clear = 0; 
	#20; 	multiplier = 64'h11; multiplicand = 64'h43; // 다른값을 재입력 
	#10;	op_start = 1;	// 연산시작 
	#10;	op_start = 0;
	#700;	op_clear = 1;
	#100$stop;
end
endmodule
