`timescale 1ns/100ps // timescalse
module tb;
reg clk, cen, wen;
reg [7:0] addr;
reg [31:0] din;
wire [31:0] dout;

memory U0(clk, cen, wen, addr, din, dout); // memory module에 tb인자들을 적용 

always#(5) clk = ~clk;	// 5ns마다 clk를 반전

initial
begin
	clk =0; cen=0; wen =0; addr = 8'b0; din = 32'b0; // 초기값 설정 
	#10; cen =1; wen =1; addr = 8'h01; din= 32'h123 ; // 메모리에 값을 저장하는 경우 
	#10 addr = 8'h02 ; din= 32'h456 ;
	#10 addr = 8'h03 ; din= 32'h798 ;
	#10 addr = 8'h04 ; din= 32'h4;
	#10 addr = 8'h05 ; din= 32'h5 ;
	#10 addr = 8'h01 ; wen =0; // 메모리에 저장된 값을 읽어들이는 경우
	#10 addr = 8'h02 ;
	#10 addr = 8'h03 ;
	#10 addr = 8'h04 ;
	#10 addr = 8'h05 ;
	#10 addr = 8'h06 ;
	#10 addr = 8'h07 ;
	#10 addr = 8'h08 ;
	#10 $stop;
end
endmodule
