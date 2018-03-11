module o_logic(q, La, Lb); // 2bit 입력을 받아서 Traffic right를 조절해주는 output logic

input  [2:0] q;
output [1:0] La, Lb;
/*신호등 표현 bit들의 setting*/

wire 	not_q0, not_q2;
wire w0,w1;
_inv U0(q[0], not_q0);
_inv U1(q[2], not_q2);
// La[1] setting
_and2 U2(q[1], not_q0, w0);
_or2	U3(w0, q[2], La[1]);
//La[0] setting
_or2 	U4(q[0], q[2], La[0]);
//Lb[1] setting
_and2 U5(q[1], not_q0, w1);
_or2	U6(not_q2, w1, Lb[1]);
// Lb[0] setting
_or2	U7(not_q2, q[0], Lb[0]);

endmodule
