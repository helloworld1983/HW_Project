// 1bit 2 to 1 multiplexer
module mux2(d0, d1, s, y);// Selectively choose two values by sel
input d0, d1;
input s;
output reg y;

always@(s, d0,d1)
begin
	if (s == 0)
		y <= d0;
	else
		y<= d1;
end
endmodule
// 8bit 2 to 1 multiplexer
module mux2_8bit(d0, d1, s, y);// Selectively choose two values by sel
input [7:0]d0, d1;
input s;
output reg [7:0]y;

always@(s, d0,d1)
begin
	if (s == 0)
		y <= d0;
	else
		y<= d1;
end
endmodule
// 32bit 2 to 1 multiplexer
module mux2_32bit(d0, d1, s, y); // Selectively choose two values by sel
input [31:0]d0, d1;
input s;
output reg [31:0]y;

always@(s, d0,d1)
begin
	if (s == 0)
		y <= d0;
	else
		y<= d1;
end
endmodule
// 32bit 6 to 1 multiplexer
module mux6_32bit(d0, d1, d2, d3, d4, s, y); // Selectively choose 8 values by sel
input [31:0] d0,d1,d2,d3,d4;
input [4:0]s;
output reg [31:0]y;

always@(s,d0,d1,d2,d3,d4)
begin
	if (s == 5'b00000) 
		y <= 32'h0;
	else if(s == 5'b10000)
		y <= d0;
	else if(s == 5'b01000)
		y <= d1;
	else if(s == 5'b00100)
		y <= d2;
	else if(s == 5'b00010)
		y <= d3;
	else if(s == 5'b00001)
		y <= d4;
	else
		y <=32'bx;
end
endmodule

// D-filp flop with reset signal
module dff_r(clk, reset_n, d, q);
input clk, reset_n; 
input [4:0]d;
output reg [4:0]q;

always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 0) q<= 5'b0;
	else 					q <= d;
end
endmodule
