module cla4_ov(a,b,ci,s,c3,co); // 32bit CLA에서 마지막 모듈에서 co_prev를 넘겨줘야 하므로 cla4_ov module을 새로 선언.
input  [3:0] a,b;
input ci;
output [3:0] s;
output c3, co;

wire c1, c2;

fa_v2 U0_fa	(.a(a[0]), .b(b[0]), .ci(ci), .s(s[0]));
fa_v2 U1_fa	(.a(a[1]), .b(b[1]), .ci(c1), .s(s[1]));
fa_v2 U2_fa	(.a(a[2]), .b(b[2]), .ci(c2), .s(s[2]));
fa_v2 U3_fa	(.a(a[3]), .b(b[3]), .ci(c3), .s(s[3]));

clb4 U4_clb4	(.a(a), .b(b), .ci(ci), .c1(c1), .c2(c2), .c3(c3), .co(co)); // clb4에서 co와 co_prev를 출력한다.
endmodule
