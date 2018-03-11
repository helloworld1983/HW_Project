module _register8_r(clk, reset_n, d, q); // 8bit 입력값을 기억하는 소자 module
input clk, reset_n;
input [7:0] d;
output [7:0] q;

_dff_r U0(clk, reset_n, d[0], q[0]);
_dff_r U1(clk, reset_n, d[1], q[1]);
_dff_r U2(clk, reset_n, d[2], q[2]);
_dff_r U3(clk, reset_n, d[3], q[3]);
_dff_r U4(clk, reset_n, d[4], q[4]);
_dff_r U5(clk, reset_n, d[5], q[5]);
_dff_r U6(clk, reset_n, d[6], q[6]);
_dff_r U7(clk, reset_n, d[7], q[7]);

endmodule