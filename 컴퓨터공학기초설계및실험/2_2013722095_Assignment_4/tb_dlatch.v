`timescale 1ns/100ps
module tb_dlatch;

reg clk, tb_d;
wire tb_q, tb_q_bar;

_dlatch U0(.clk(clk), .d(tb_d), .q(tb_q), .q_bar(tb_q_bar));

	parameter temp = 10;
always #(temp/2) clk = ~clk; // 반주기마다 clk를 반전시킴 
	initial
	begin
			  clk =0; tb_d =0;
		#10; tb_d =1;
		#20; tb_d =0;
		#20; tb_d =1;
		#10; $stop;
	end
endmodule
