module tb_ram;

reg clk, cen, wen;
reg [7:0] addr;
reg [31:0] din;
wire [31:0] dout;
ram ram(clk, cen, wen, addr, din, dout);


always#(5) clk = ~clk;

initial
begin
			clk = 0; cen = 0; wen = 0; addr = 0; din = 0;
		
		#10 cen = 1; wen = 1; addr = 8'h11; din = 32'h50;
		#10 addr = 8'h12 ; din = 32'h60 ;
		#10 addr = 8'h13 ; din = 32'h70 ;
		#10 addr = 8'h14 ; din = 32'h80 ;
		#10 cen = 1; wen = 0; addr = 8'h11;
		#10  addr = 8'h12;
		#10  addr = 8'h13;
		#10  addr = 8'h14;
		#10  addr = 8'h15;
		#10 $stop;
end

endmodule	