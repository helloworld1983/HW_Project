`timescale 1ns/100ps

module tb_register32;
reg clk;
reg [31:0] tb_d;
wire [31:0] tb_q;

register32 U0(clk, tb_d, tb_q); // module에 tb인자 적용 

parameter temp = 10;

always#(temp/2) clk = ~clk; // 반주기마다 클럭 반전 
initial 
begin
 clk =0; tb_d = 32'h 0;
 #10 tb_d = 32'h00ff;
 #10 tb_d = 32'hff00;
 #10 tb_d = 32'h1234;
 #10 $stop;
 end
 endmodule
 