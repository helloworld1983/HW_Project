`timescale 1ns/100ps
module tb_tl_cntr;
reg tb_Ta, tb_Tb;
reg clk, reset;

wire [1:0] tb_La, tb_Lb;
parameter STEP = 10;

tl_cntr U0_tl_cntr (.clk(clk),.reset(reset),.Ta(tb_Ta),.Tb(tb_Tb),.La(tb_La),.Lb(tb_Lb)); // 선언된 TB인자들을 TOP MODULE에 적용 

always #(STEP/2) clk = ~clk; // 클럭이 반주기인 5NS마다 반전됨 
	initial
	begin
	#0; clk = 0 ; reset = 0 ; tb_Ta= 0 ; tb_Tb = 0 ; // 모든 초기상태 설정 
	#10; reset = 1; // RESET값을 NEGATIVE로 함.
	#40; tb_Ta = 1; // A길에 차량이 존재
	#20; tb_Ta = 0;	// A길에 차량 없음
	#40; tb_Tb = 1;	// B길에 차량 존재
	#60; tb_Tb = 0;	// B길에 차량 없음 
	#40; $stop;
	end
endmodule
