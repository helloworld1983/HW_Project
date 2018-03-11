`timescale 1ns/100ps
module tb_Multiplier_Top;
reg clk, reset_n, S_sel, S_wr;
reg [7:0] S_address;
reg [31:0] S_din;

wire [31:0] S_dout;
wire m_interrupt;

Multiplier_Top U0_top(clk, reset_n, S_sel, S_wr, S_address, S_din, S_dout, m_interrupt); 

always#(5) clk = ~clk;

initial
begin
			clk =0; reset_n =0; S_sel =0; S_wr =0; S_address = 0; S_din =0;
	#10 reset_n = 1; S_sel = 1; S_wr = 1; S_din = 32'h10;
	#10 S_address = 8'h1; S_din = 32'h22222222;
	#10 S_address = 8'h0; S_din = 32'h40;
	#10 S_address = 8'h1; S_din = 32'h50;
	#10  S_address = 8'h4; S_din = 32'h01;// start
	#780 S_wr = 0; S_address = 8'h2;	// result
	#60 S_address = 8'h6;				// done
	#10 S_address = 8'h3;  S_wr = 1;		S_din = 32'h1;// interrupt enable
	#10 S_address = 3'h5; // clear
	#30 $stop;
end
endmodule 