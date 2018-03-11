`timescale 1ns/100ps

module tb_srlatch;
reg tb_r, tb_s;
wire tb_q, tb_q_bar;

_srlatch U0(tb_r, tb_s, tb_q, tb_q_bar); // stlatch module에 tb인자들을 적용시킴 

initial 
begin 
	tb_r=0; tb_s=0;
	#10; tb_r =1;
	#10; tb_s =1; tb_r = 0;
	#10; tb_r =1;
	#10; $stop;
end
endmodule
