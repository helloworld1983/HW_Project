module mx8_32bits(a,b,c,d,e,f,g,h, s2,s1,s0,y); // 32비트 입력 두개에 대한 연산의 결과들을 선택적으로 출력해주는 module
input [31:0] a,b,c,d,e,f,g,h;
input 		 s2, s1, s0; // 1bit opcode 3개를 이용해 8가지 선택지를 만듬 
output [31:0] y;

wire	[31:0] w0, w1, w2, w3, w4, w5;
/* 다음과 같은 opcode값에 의해 결과가 출력된다.
s2 s1 s0 y
0   0  0 a
0   0  1 b
0   1  0 c
0   1  1 d
1   0  0 e
1   0  1 f
1   1  0 g
1   1  1 h
*/
mx2_32bits U0_mx2_32bits(.d0(a), .d1(b), .s(s0), .y(w0));
mx2_32bits U1_mx2_32bits(.d0(c), .d1(d), .s(s0), .y(w1));
mx2_32bits U2_mx2_32bits(.d0(e), .d1(f), .s(s0), .y(w2));
mx2_32bits U3_mx2_32bits(.d0(g), .d1(h), .s(s0), .y(w3));
mx2_32bits U4_mx2_32bits(.d0(w0), .d1(w1), .s(s1), .y(w4));
mx2_32bits U5_mx2_32bits(.d0(w2), .d1(w3), .s(s1), .y(w5));
mx2_32bits U6_mx2_32bits(.d0(w4), .d1(w5), .s(s2), .y(y));
endmodule 
