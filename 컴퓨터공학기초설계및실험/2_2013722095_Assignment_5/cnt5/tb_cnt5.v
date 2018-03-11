`timescale 1ns/100ps

module tb_cnt5;

reg clk, tb_inc, reset_n;
wire [2:0] tb_cnt;
parameter temp = 10;
cnt5 U0_cnt5(tb_cnt, clk, reset_n, tb_inc); // tb인자들을 cnt5 module에 적용 

always #(temp/2) clk = ~clk;
initial
begin
	clk = 0; reset_n =0; tb_inc =1; // 초기값 설정 , 순방향 진행 
	#10;	reset_n =1; // reset switch off
	#50; tb_inc =0;	// 역방향 진행 
	#50; $stop;
end
	
endmodule
