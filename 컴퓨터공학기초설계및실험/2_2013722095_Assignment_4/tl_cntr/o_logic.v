module o_logic(q, La, Lb); // 2bit 입력을 받아서 Traffic right를 조절해주는 output logic

input  [1:0] q;
output [1:0] La, Lb;
wire not_q, w0;
/*신호등 표현 bit들의 setting*/
_inv U0(q[1], not_q);
_inv U1(q[1], w0);
assign La[1] = q[1];				
_and2 U2(not_q,q[0],La[0]);
assign Lb[1] = w0;
_and2 U3(q[1], q[0], Lb[0]);


endmodule
