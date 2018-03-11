module ns_logic(q, d, Ta, Tb); // 

input Ta, Tb;
input [1:0] q;
output [1:0] d;
wire Ta_bar, Tb_bar;
wire w_q_0, w_q_1;
wire w0, w1;
/* input들의 inverted vaule*/
_inv U0_inv(.a(Ta), .y(Ta_bar));
_inv U1_inv(.a(Tb), .y(Tb_bar));
_inv U2_inv(.a(q[1]), .y(w_q_1));
_inv U3_inv(.a(q[0]), .y(w_q_0));

_xor2 U4_xor2 (.a(q[1]), .b(q[0]), .y(d[1])); // d[1]은 q[1]^q[0]
_and3 U5_and3 (.a(w_q_1), .b(w_q_0), .c(Ta_bar), .y(w0)); // ~q[1]과 ~q[0], Ta_bar를 and 시켜서 wire w0에 연결 
_and3 U6_and3 (.a(q[1]), .b(w_q_0), .c(Tb_bar), .y(w1));  // q[1]과 ~q[0], Tb_barfmf and 시켜서 wire w1에 연결 
_or2	U7_or2	  (.a(w0), .b(w1), .y(d[0])); // d[0]는 w1|w0

endmodule
