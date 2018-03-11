module cla4(a,b,ci,s,cout); // 4bit 입력 2개의 합과 최종 carry out 값을 계산하는 module

input [3:0] a,b;
input ci;
output [3:0] s;
output cout;

wire c1, c2, c3;

clb4 U0_clb4(.a(a), .b(b), .ci(ci), .c1(c1), .c2(c2), .c3(c3), .co(cout));// 각 입력값을 clb module에 적용함.

/* CLB module에서 나온 입력값과 해당 비트들의 합 계산 */
fa_v2 U1_fa(.a(a[0]), .b(b[0]), .ci(ci), .s(s[0])); 
fa_v2 U2_fa(.a(a[1]), .b(b[1]), .ci(c1), .s(s[1]));
fa_v2 U3_fa(.a(a[2]), .b(b[2]), .ci(c2), .s(s[2]));
fa_v2 U4_fa(.a(a[3]), .b(b[3]), .ci(c3), .s(s[3]));


endmodule

