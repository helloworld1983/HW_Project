`timescale 1ns/100ps

module tb_ha;
reg tb_a,tb_b;
wire tb_co,tb_s;

_ha U0_ha(.a(tb_a), .b(tb_b), .co(tb_co), .s(tb_s));
//Half Adder module에 Test Bench의 register와 wire를 적용시킴
// input을 모두 검증하는 Exhaustive Verification으로 Tb를 진행.
initial
begin
			tb_a=0; tb_b=0;
	#10	tb_b=1;
	#10 	tb_a=1; tb_b=0;
	#10	tb_b=1;
	#10 	$stop;
end
endmodule
