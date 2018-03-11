module LSR8(d_in, shamt, d_out); // 8bit를 오른쪽으로 최대 3bit shift하는 LSR module
input [7:0] d_in;
input [1:0] shamt;
output [7:0] d_out;

mx4 U0(.d0(d_in[7]), .d1(1'b0), 		.d2(1'b0), 		.d3(1'b0), 		.s(shamt),	.y(d_out[7]));
mx4 U1(.d0(d_in[6]), .d1(d_in[7]),  .d2(1'b0),		.d3(1'b0),		 .s(shamt), .y(d_out[6]));
mx4 U2(.d0(d_in[5]), .d1(d_in[6]),  .d2(d_in[7]),  .d3(1'b0),		 .s(shamt),	.y(d_out[5]));
mx4 U3(.d0(d_in[4]), .d1(d_in[5]),  .d2(d_in[6]),  .d3(d_in[7]), .s(shamt), 	.y(d_out[4]));
mx4 U4(.d0(d_in[3]), .d1(d_in[4]),  .d2(d_in[5]),  .d3(d_in[6]), .s(shamt),	.y(d_out[3]));
mx4 U5(.d0(d_in[2]), .d1(d_in[3]),  .d2(d_in[4]),  .d3(d_in[5]), .s(shamt), 	.y(d_out[2]));
mx4 U6(.d0(d_in[1]), .d1(d_in[2]),  .d2(d_in[3]),  .d3(d_in[4]), .s(shamt), 	.y(d_out[1]));
mx4 U7(.d0(d_in[0]), .d1(d_in[1]),  .d2(d_in[2]),  .d3(d_in[3]), .s(shamt), 	.y(d_out[0]));
endmodule
