
`timescale 1ns/100ps
module tb_dff_en;
reg clk, tb_d, en;
wire tb_q;

_dff_en U0(clk, tb_d, tb_q, en); // tb인자들을 dff_en 모듈에 적용 

parameter temp = 10;

always #(temp/2) clk = ~clk; // 반주기마다 클럭 반전 
initial 
begin
	clk =0; en = 0; tb_d=0; // 초기값 설정 
	#20 tb_d =1;				// d를 1로 함.-> en이 0이므로 prev값(0)이 들어감 
	#10 en = 1;					// en을 1로 함. -> DFF에 입력으로 d가 들어감.
	#20 tb_d =1;				// d = 1
	#10 tb_d =0;				// d = 0
	#10 $stop;
	end
	endmodule
	