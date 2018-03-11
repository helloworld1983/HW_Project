module ns_logic(q, d, Ta, Tal, Tb, Tbl); // 

input Ta, Tb, Tal, Tbl;
input [2:0] q;
output [2:0] d;
wire Ta_bar, Tb_bar, Tal_bar, Tbl_bar;
wire w_q_0, w_q_1, w_q_2;
wire w0, w1,w2, w3,w4,w5, w6,w7,w8,w9;
/* input들의 inverted vaule*/
_inv U0_inv(.a(Ta), .y(Ta_bar));
_inv U1_inv(.a(Tb), .y(Tb_bar));
_inv U2_inv(.a(q[2]), .y(w_q_2));
_inv U3_inv(.a(q[1]), .y(w_q_1));
_inv U4_inv(.a(q[0]), .y(w_q_0));
_inv U5(.a(Tal), .y(Tal_bar));
_inv U6(.a(Tbl), .y(Tbl_bar));

//d2 = ~q2*q1*q0 + q2*q1 + q2*q1*~q0
_and3 U7(w_q_2, q[1], q[0], w0);
_and2	U8(q[2], w_q_1, w1);
_and3 U9(q[2], q[1], w_q_0, w2);
_or3  U10(w0,w1,w2,d[2]);

_and3 U11(w_q_2, w_q_1, q[0], w3);
_and2 U12(q[1], w_q_0, w4);
_and3 U13(q[2], w_q_1, q[0], w5);
_or3	U14(w3,w4,w5, d[1]);

_and4 U15(w_q_2, w_q_1, w_q_0, Ta_bar, w6);
_and4 U16(w_q_2, q[1], w_q_0, Tal_bar, w7);
_and4 U17(q[2], w_q_1, w_q_0, Tb_bar, w8);
_and4 U18(q[2], q[1], w_q_0, Tbl_bar, w9);
_or4	U19(w6,w7,w8,w9,d[0]);
endmodule
