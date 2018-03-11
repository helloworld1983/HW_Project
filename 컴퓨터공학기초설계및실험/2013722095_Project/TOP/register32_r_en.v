/*module which can save 8 inputs by instance 8 register32*/
module register32_8(clk, reset_n, en, d_in, d_out0, d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7);
input clk, reset_n;
input [7:0] en;
input [31:0] d_in;
output [31:0] d_out0, d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7;
// choose output accoding to 8bit en 
register32_r_en U0_register32_r_en(clk, reset_n, d_in, d_out0, en[0]);
register32_r_en U1_register32_r_en(clk, reset_n, d_in, d_out1, en[1]);
register32_r_en U2_register32_r_en(clk, reset_n, d_in, d_out2, en[2]);
register32_r_en U3_register32_r_en(clk, reset_n, d_in, d_out3, en[3]);
register32_r_en U4_register32_r_en(clk, reset_n, d_in, d_out4, en[4]);
register32_r_en U5_register32_r_en(clk, reset_n, d_in, d_out5, en[5]);
register32_r_en U6_register32_r_en(clk, reset_n, d_in, d_out6, en[6]);
register32_r_en U7_register32_r_en(clk, reset_n, d_in, d_out7, en[7]);

endmodule 




/*module which can save 15 inputs by instance 15 register32*/
module register32_15(clk, reset_n, en, d_in, d_out0, d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7, d_out8, d_out9, d_out10, d_out11, d_out12, d_out13,
							d_out14, d_out15);
input clk, reset_n;
input [15:0] en;
input [31:0] d_in;
output [31:0] d_out0, d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7, d_out8, d_out9, d_out10, d_out11, d_out12, d_out13,
							d_out14, d_out15;
// 8bit로 된 en의 값에 따라서 입력값을 어떤 output에 연결해줄지 선택한다.
register32_r_en U0_register32_r_en(clk, reset_n, d_in, d_out0, en[0]);
register32_r_en U1_register32_r_en(clk, reset_n, d_in, d_out1, en[1]);
register32_r_en U2_register32_r_en(clk, reset_n, d_in, d_out2, en[2]);
register32_r_en U3_register32_r_en(clk, reset_n, d_in, d_out3, en[3]);
register32_r_en U4_register32_r_en(clk, reset_n, d_in, d_out4, en[4]);
register32_r_en U5_register32_r_en(clk, reset_n, d_in, d_out5, en[5]);
register32_r_en U6_register32_r_en(clk, reset_n, d_in, d_out6, en[6]);
register32_r_en U7_register32_r_en(clk, reset_n, d_in, d_out7, en[7]);
register32_r_en U8_register32_r_en(clk, reset_n, d_in, d_out8, en[8]);
register32_r_en U9_register32_r_en(clk, reset_n, d_in, d_out9, en[9]);
register32_r_en U10_register32_r_en(clk, reset_n, d_in, d_out10,en[10]);
register32_r_en U11_register32_r_en(clk, reset_n, d_in, d_out11,en[11]);
register32_r_en U12_register32_r_en(clk, reset_n, d_in, d_out12,en[12]);
register32_r_en U13_register32_r_en(clk, reset_n, d_in, d_out13,en[13]);
register32_r_en U14_register32_r_en(clk, reset_n, d_in, d_out14,en[14]);
register32_r_en U15_register32_r_en(clk, reset_n, d_in, d_out15,en[15]);
endmodule 

/*module which can save 32bits input by instance 4 register8*/
module register32_r_en(clk, reset_n, d_in, d_out, en);
input clk, reset_n, en;
input [31:0] d_in;
output [31:0] d_out;
register8_r_en U0_register8_r_en(clk, reset_n, d_in[7:0], d_out[7:0], en);
register8_r_en U1_register8_r_en(clk, reset_n, d_in[15:8], d_out[15:8], en);
register8_r_en U2_register8_r_en(clk, reset_n, d_in[23:16], d_out[23:16], en);
register8_r_en U3_register8_r_en(clk, reset_n, d_in[31:24], d_out[31:24], en);

endmodule


/*module which can save 8bits input by instance 8 dff*/
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

/*module which can save 4bits input by instance 4 dff*/
module register4_r_en(clk, reset_n, d_in, d_out, en);
input clk, reset_n, en;
input [3:0] d_in;
output [3:0] d_out;

_dff_r_en U0_dff_r_en(clk, reset_n,en,d_in[0], d_out[0]);
_dff_r_en U1_dff_r_en(clk, reset_n,en,d_in[1], d_out[1]);
_dff_r_en U2_dff_r_en(clk, reset_n,en,d_in[2], d_out[2]);
_dff_r_en U3_dff_r_en(clk, reset_n,en,d_in[3], d_out[3]);

endmodule

/*module which can save 3bits input by instance 3 dff*/
module register3_r_en(clk, reset_n, d_in, d_out, en);
input clk, reset_n, en;
input [2:0] d_in;
output [2:0] d_out;

_dff_r_en U0_dff_r_en(clk, reset_n,en,d_in[0], d_out[0]);
_dff_r_en U1_dff_r_en(clk, reset_n,en,d_in[1], d_out[1]);
_dff_r_en U2_dff_r_en(clk, reset_n,en,d_in[2], d_out[2]);

endmodule

//  D-flip flop which can remember 1bit input
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
