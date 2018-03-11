`timescale 1ns/100ps

module tb_cla_clk;

reg [31:0] tb_a, tb_b;
reg tb_ci, clock;
wire [31:0] tb_s_cla;
wire tb_co_cla;

parameter STEP =10;
//test bench의 변수들을 cla_clk module에 적용 
cla_clk U0_cla_clk(.a(tb_a), .b(tb_b), .ci(tb_ci), .clock(clock), .s_cla(tb_s_cla), .co_cla(tb_co_cla));

always#(STEP/2) clock = ~clock;	// 1주기가 10ns인데 그 반주기마다 clock의 값이 반전됨.(rising / falling 을 5ns마다 반복)

initial 
begin
 clock = 1'b 1; tb_a = 32'h 0; tb_b = 32'h 0; tb_ci = 1'b 0;
 #(STEP) tb_a = 32'h ffff_ffff; tb_b = 32'h 0; tb_ci = 1'b 1;
 #(STEP) tb_a = 32'h 0000_ffff; tb_b = 32'h ffff_0000; tb_ci = 1'b 0;
 #(STEP) tb_a = 32'h 135f_a562; tb_b = 32'h 3561_4642; tb_ci = 1'b 0;
 #(STEP*2) $stop;
end
endmodule
 
	