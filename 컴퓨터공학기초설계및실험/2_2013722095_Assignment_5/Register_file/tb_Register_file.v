`timescale 1ns/100ps 
module tb_Register_file;
reg clk, reset_n, we;
reg [2:0] wAddr, rAddr;
reg [31:0] wData;
wire [31:0] rData;

Register_file U0_Register_file(clk, reset_n, wAddr, wData, we, rAddr, rData);

parameter temp = 10;

always #(temp/2) clk = ~clk;
initial
	begin
			clk =0; reset_n =0; we =0; wData = 32'h00000000;
	#5;	reset_n =1; we =1;
	#5;	wAddr = 3'b000; wData = 32'h00000001;
	#10;	wAddr = 3'b001; wData = 32'h00000002;
	#10;	wAddr = 3'b010; wData = 32'h00000003;
	#10;	wAddr = 3'b011; wData = 32'h00000004;
	#10;	wAddr = 3'b100; wData = 32'h00000005;
	#10;	wAddr = 3'b101; wData = 32'h00000006;
	#10;	wAddr = 3'b110; wData = 32'h00000007;
	#10;	wAddr = 3'b111; wData = 32'h00000008;
	#10; 	rAddr = 3'b000;
	#10; 	rAddr = 3'b001;
	#10; 	rAddr = 3'b010;
	#10; 	rAddr = 3'b011;
	#10; 	rAddr = 3'b100;
	#10; 	rAddr = 3'b101;
	#10; 	rAddr = 3'b110;
	#10; 	rAddr = 3'b111;
	#10; 	$stop;
	end
endmodule
