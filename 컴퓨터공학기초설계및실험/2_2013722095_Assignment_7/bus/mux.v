// 1bit 2 to 1 multiplexer
module mux2(d0, d1, s, y); // 두개의 입력을 s의 값에 따라 조건적으로 출력해주는 mux 모듈 
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
module mux2_8bit(d0, d1, s, y); // 두개의 입력을 s의 값에 따라 조건적으로 출력해주는 mux 모듈 
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
module mux2_32bit(d0, d1, s, y); // 두개의 입력을 s의 값에 따라 조건적으로 출력해주는 mux 모듈 
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
// 32bit 3 to 1 multiplexer
module mux3_32bit(d0, d1, s, y); // 두개의 입력을 s의 값에 따라 조건적으로 출력해주는 mux 모듈 
input [31:0]d0, d1;
input [1:0]s;
output reg [31:0]y;

always@(s,d0,d1)
begin
	if (s == 2'b00) 
		y <= 32'h00;
	else if(s == 2'b10)
		y<= d0;
	else if (s == 2'b01)
		y <= d1;
	else ;
end
endmodule
// D-filp flop with reset signal
module dff_r(clk, reset_n, d, q);
input clk, reset_n; 
input [1:0]d;
output reg [1:0]q;

always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 0) q<= 2'b00;
	else 					q <= d;
end
endmodule
