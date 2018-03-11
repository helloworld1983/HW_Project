`timescale 1ns/100ps

module tb_rca;
reg [3:0] tb_a, tb_b;
reg tb_ci;
wire [3:0] tb_s;
wire tb_co;
wire [4:0] tb_result;

_rca U0_rca(.a(tb_a), .b(tb_b), .ci(tb_ci), .s(tb_s), .co(tb_co));
//Ripple Carry Adder module에 Test Bench의 register와 wire를 적용시킴
initial
begin
			tb_a = 4'b 0000; tb_b = 4'b 0000; tb_ci = 0;//초기값 설정
	#10	tb_a = 4'b 0011; tb_b = 4'b 1100;//올림수가 발생하지 않는 경우
	#10	tb_a = 4'b 1001; tb_b = 4'b 1011;//1의 자리 비트에서 발생한 올림수가 2번째 3번째자리 까지 올림수를 방생시키는 경우
	#10	tb_a = 4'b 1010; tb_b = 4'b 0011;//2의 자리 비트에서 올림수가 발생한경우
	#10	tb_a = 4'b 0111; tb_b = 4'b 0100;//4의 자리 비트에서 올림수가 발생한 경우 
	#10	tb_a = 4'b 0101; tb_b = 4'b 0011;//1의 자리 비트합에서 발생한 올림수가 지속적으로 올림수를 발생시키는 경우
	#10	tb_a = 4'b 0101; tb_b = 4'b 1011;//1의 자리 비트합에서 발생한 올림수가 Co값을 발생시키는 경우.
	#10	$stop;

end
assign tb_result = {tb_co, tb_s};
endmodule
