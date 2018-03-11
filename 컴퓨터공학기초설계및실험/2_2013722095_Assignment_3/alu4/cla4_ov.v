module cla4_ov(a,b,ci,s,c3,co); // 2개의 4bit 입려력을 받아 그 합과 carry out을 출력하는 모듈 
input  [3:0] a,b;
input ci;
output [3:0] s;
output c3, co;

wire c1, c2;
// 각 비트의 합을 연산하는 Fa
fa_v2 U0_fa	(.a(a[0]), .b(b[0]), .ci(ci), .s(s[0]));
fa_v2 U1_fa	(.a(a[1]), .b(b[1]), .ci(c1), .s(s[1]));
fa_v2 U2_fa	(.a(a[2]), .b(b[2]), .ci(c2), .s(s[2]));
fa_v2 U3_fa	(.a(a[3]), .b(b[3]), .ci(c3), .s(s[3]));

clb4 U4_clb4	(.a(a), .b(b), .ci(ci), .c1(c1), .c2(c2), .c3(c3), .co(co)); // 최종 co값을 연산해주는 clb모듈 
endmodule
