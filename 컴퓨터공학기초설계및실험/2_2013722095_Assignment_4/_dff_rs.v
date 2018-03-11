module _dff_rs(clk, d, q, set, reset); // reset과 set을 통해서 출력을 control하는 dff_rs module
input clk, d, set, reset;
output q;
wire w0, w1,w2;
_inv U1(.a(reset), .y(w0)); // active low에서 작동하도록 reset을 반전시킴 
_or2 U2(.a(d), .b(set), .y(w1)); // set을 d와 or 시킴 
_and2 U3(.a(w0), .b(w1), .y(w2));// or gate의 출력값과  reset_n을 and gate의 input으로 넣어줌 
_dff U4(.clk(clk), .d(w2), .q(q)); // 최종적인 입력값인 wire w2와 clk를 입력으로 하여 q를 연산 

endmodule
