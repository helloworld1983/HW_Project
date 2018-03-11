`timescale 1ns/100ps

module tb_dff;
	reg clk, tb_d;
	wire tb_q, tb_q_bar;
	
	_dff U0(clk, tb_d, tb_q, tb_q_bar); // tb인자들을 DFF 모듈에 적용 
	
	parameter temp = 10;
	
	always #(temp/2) clk = ~clk; // 반 주기 마다 clk를 반전시킴 
	initial
	begin
		clk =0; tb_d =0; // 초기값 설정 
	#10 tb_d =1; // d=1일때 
	#5  tb_d =0; // d=0일때 
	#15 tb_d =1; // d=1일때 
	#15 tb_d =0; // d=0일때 
	#10 $stop;
	end
	endmodule
	