
/*32bit data를 기억하고 있는 register를 8개 instance한 module*/
module register32_8(clk, reset_n, en, d_in, d_out0, d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7);
input clk, reset_n;
input [7:0] en;
input [31:0] d_in;
output [31:0] d_out0, d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7;
// 8bit로 된 en의 값에 따라서 입력값을 어떤 output에 연결해줄지 선택한다.
register32_r_en U0_register32_r_en(clk, reset_n, d_in, d_out0, en[0]);
register32_r_en U1_register32_r_en(clk, reset_n, d_in, d_out1, en[1]);
register32_r_en U2_register32_r_en(clk, reset_n, d_in, d_out2, en[2]);
register32_r_en U3_register32_r_en(clk, reset_n, d_in, d_out3, en[3]);
register32_r_en U4_register32_r_en(clk, reset_n, d_in, d_out4, en[4]);
register32_r_en U5_register32_r_en(clk, reset_n, d_in, d_out5, en[5]);
register32_r_en U6_register32_r_en(clk, reset_n, d_in, d_out6, en[6]);
register32_r_en U7_register32_r_en(clk, reset_n, d_in, d_out7, en[7]);

endmodule 

// 32bit를 기억할 수 있도록 설계된 module.  8bit register 4개를 instance하여 구성 
module register32_r_en(clk, reset_n, d_in, d_out, en);
input clk, reset_n, en;
input [31:0] d_in;
output [31:0] d_out;
register8_r_en U0_register8_r_en(clk, reset_n, d_in[7:0], d_out[7:0], en);
register8_r_en U1_register8_r_en(clk, reset_n, d_in[15:8], d_out[15:8], en);
register8_r_en U2_register8_r_en(clk, reset_n, d_in[23:16], d_out[23:16], en);
register8_r_en U3_register8_r_en(clk, reset_n, d_in[31:24], d_out[31:24], en);

endmodule


// 1bit기억소자인 DFF 8개를 instance하여 구성 
module register8_r_en(clk, reset_n, d_in, d_out, en);
input clk, reset_n, en;
input [7:0] d_in;
output [7:0] d_out;

_dff_r_en U0_dff_r_en(clk, reset_n,en,d_in[0], d_out[0]);
_dff_r_en U1_dff_r_en(clk, reset_n,en,d_in[1], d_out[1]);
_dff_r_en U2_dff_r_en(clk, reset_n,en,d_in[2], d_out[2]);
_dff_r_en U3_dff_r_en(clk, reset_n,en,d_in[3], d_out[3]);
_dff_r_en U4_dff_r_en(clk, reset_n,en,d_in[4], d_out[4]);
_dff_r_en U5_dff_r_en(clk, reset_n,en,d_in[5], d_out[5]);
_dff_r_en U6_dff_r_en(clk, reset_n,en,d_in[6], d_out[6]);
_dff_r_en U7_dff_r_en(clk, reset_n,en,d_in[7], d_out[7]);

endmodule

module register4_r_en(clk, reset_n, d_in, d_out, en);
input clk, reset_n, en;
input [3:0] d_in;
output [3:0] d_out;

_dff_r_en U0_dff_r_en(clk, reset_n,en,d_in[0], d_out[0]);
_dff_r_en U1_dff_r_en(clk, reset_n,en,d_in[1], d_out[1]);
_dff_r_en U2_dff_r_en(clk, reset_n,en,d_in[2], d_out[2]);
_dff_r_en U3_dff_r_en(clk, reset_n,en,d_in[3], d_out[3]);

endmodule

// 3bit 기억소자
module register3_r_en(clk, reset_n, d_in, d_out, en);
input clk, reset_n, en;
input [2:0] d_in;
output [2:0] d_out;

_dff_r_en U0_dff_r_en(clk, reset_n,en,d_in[0], d_out[0]);
_dff_r_en U1_dff_r_en(clk, reset_n,en,d_in[1], d_out[1]);
_dff_r_en U2_dff_r_en(clk, reset_n,en,d_in[2], d_out[2]);

endmodule

// 1bit 기억소자인 D-flip flop
module _dff_r_en(clk, reset_n, en, d, q);
input clk, reset_n, en, d;
output reg q;

always @(posedge clk or negedge reset_n)
begin
	if (~reset_n) q<= 1'b0;
	else if (en)	q<=d;
	else				q<=q;
end
endmodule
