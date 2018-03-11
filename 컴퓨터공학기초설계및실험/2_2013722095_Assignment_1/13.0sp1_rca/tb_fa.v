`timescale 1ns/100ps

module tb_fa;
reg tb_a,tb_b,tb_ci;
wire tb_co,tb_s;

_fa U0_fa(.a(tb_a), .b(tb_b), .ci(tb_ci), .s(tb_s), .co(tb_co));
//Full Adder module에 Test Bench의 register와 wire를 적용시킴
initial 
begin
			tb_a=0; tb_b=0; tb_ci=0;//Carry in 값이 0인 case
	#10	tb_b=1;						
	#10	tb_a=1; tb_b=0;
	#10	tb_b=1;
	#10	tb_a=0; tb_b=0; tb_ci=1;//Carry in 값이 1인 case
	#10	tb_b=1;
	#10	tb_a=1; tb_b=0;
	#10	tb_b=1;
	#10 	$stop;
end
endmodule
