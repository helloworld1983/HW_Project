`timescale 1ns/100ps
module tb_dff_r;
reg tb_d, reset, clk;
wire tb_q;

_dff_r U0(tb_d, reset, clk ,tb_q );

parameter temp = 10;

always #(temp/2) clk = ~clk;
initial 
begin 
		clk =0; reset =0; tb_d =0;
	#10 tb_d =1;
	#10 tb_d =0;
	#10 reset =1 ; tb_d =1 ;
	#10 tb_d =0;
	#10 $stop;
end
endmodule 