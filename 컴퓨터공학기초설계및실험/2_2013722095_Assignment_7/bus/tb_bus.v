`timescale 1ns/100ps // timescalse
module tb_bus;				

reg clk, reset_n, M0_req, M0_wr, M1_req, M1_wr;
reg [7:0]M0_address, M1_address;
reg [31:0]M0_dout, M1_dout, S0_dout, S1_dout;
wire M0_grant, M1_grant, S0_sel, S1_sel, S_wr;
wire [7:0] S_address;
wire [31:0] M_din, S_din;
// bus module에 test bench 인자들을 적용 
bus U0_bus(clk, reset_n, M0_req,M0_wr, M0_address, M0_dout, M1_req, M1_wr, M1_address, M1_dout, 
		S0_dout, S1_dout, M0_grant, M1_grant, M_din, S0_sel, S1_sel, S_address, S_wr, S_din);
parameter temp = 10;
always#(temp/2) clk = ~clk; // 5ns마다 clk 를 반전

initial
begin// 초기값 setting
	clk =0; reset_n =0; M0_req =0; M0_wr =0; M0_address = 8'h0; M0_dout = 32'h0; M1_address = 8'h0;M1_req =0;M1_wr=0;M1_dout = 32'h0; S0_dout = 32'ha; S1_dout = 32'hb;
	#3 reset_n =1;
	#10 M0_req =1;  M0_wr =1; M0_address = 8'h01; M0_dout = 32'h2;
	#10 M0_address = 8'h03; M0_dout = 32'h6;
	#10 M0_address = 8'h20; M0_dout = 32'h20;
	#10 M0_address = 8'ha0; M0_dout = 32'hff;
	#10 M0_req =0;  M1_req = 1; M0_wr =0; M1_wr =1; M1_address = 8'h01; M1_dout = 32'h2;
	#10 M1_address = 8'h05; M1_dout = 32'h11;
	#10 M1_address = 8'h06; M1_dout = 32'h22;
	#10 M1_address = 8'h3c; M1_dout = 32'h33;
	#30 $stop;
end
endmodule
