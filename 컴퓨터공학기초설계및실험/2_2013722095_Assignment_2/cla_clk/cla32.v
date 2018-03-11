module cla32(a,b,ci,s,co); // 4BIT cla 8개를 직렬연결하여 32BIT입력값을 계산함.
input [31:0] a, b;
input ci;
output [31:0] s;
output co;
wire w1,w2,w3,w4,w5,w6,w7;
/* 입력받은 32BIT 입력들을 4BIT씩 나누어 CLA4 MODULE에 적용시킵니다. */
/* 이때 각 모듈들은 직렬로 연결되어있기 때문에 이전모듈의 CARRY OUT을 CARRY IN으로 받습니다.  */
cla4 U0_cla4(.a(a[3:0]), .b(b[3:0]), .ci(ci), .s(s[3:0]), .cout(w1));
cla4 U1_cla4(.a(a[7:4]), .b(b[7:4]), .ci(w1), .s(s[7:4]), .cout(w2));
cla4 U2_cla4(.a(a[11:8]), .b(b[11:8]), .ci(w2), .s(s[11:8]), .cout(w3));
cla4 U3_cla4(.a(a[15:12]), .b(b[15:12]), .ci(w3), .s(s[15:12]), .cout(w4));
cla4 U4_cla4(.a(a[19:16]), .b(b[19:16]), .ci(w4), .s(s[19:16]), .cout(w5));
cla4 U5_cla4(.a(a[23:20]), .b(b[23:20]), .ci(w5), .s(s[23:20]), .cout(w6));
cla4 U6_cla4(.a(a[27:24]), .b(b[27:24]), .ci(w6), .s(s[27:24]), .cout(w7));
cla4 U7_cla4(.a(a[31:28]), .b(b[31:28]), .ci(w7), .s(s[31:28]), .cout(co)); //마지막 MODULE에서는 최종적인 CARRY OUT값이 발생


endmodule
