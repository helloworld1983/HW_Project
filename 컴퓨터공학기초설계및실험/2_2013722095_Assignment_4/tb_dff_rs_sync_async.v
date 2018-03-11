`timescale 1ns/100ps 

module tb_dff_rs_sync_async;
reg clk, set_n, reset_n, tb_d;
wire tb_q_sync, tb_q_async;


_dff_rs_sync_async U0(clk, set_n, reset_n, tb_d, tb_q_sync, tb_q_async); // tb인자들을 module에 적용함. 

parameter temp = 10;

always#(temp/2) clk = ~clk; // 클럭을 반주기마다 반전시킴 

initial
begin
			clk =0; tb_d=0; reset_n =0; set_n =1; // 입력값들을 초기화시킴 
	#10 tb_d =1; // reset=0, d =1 , set=1 이므로 이전의 값 유지 
	#5 reset_n =1; // reset =1, d=1, set=1 이므로 이전의 값 유지 
	#15 tb_d =0;	// d =0
	#10 set_n =0; tb_d=1; // set =0 , 이전의 값을 지속적으로 유지하지 않음 
	#10 tb_d = 0; // d=0
	#10 tb_d=1; // d=1
	#10 $stop;
	end
	endmodule
	