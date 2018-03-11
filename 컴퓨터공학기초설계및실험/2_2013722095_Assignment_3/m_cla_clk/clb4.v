module clb4(a,b,ci,co); // 4bit 입력값 두개의 올림값을 미리 계산해주는 module
input [3:0] a,b;
input ci;
output co;

wire [3:0] g,p;
wire w0_co, w1_co, w2_co, w3_co;

//Generate
_and2 U0_and2	(.a(a[0]), .b(b[0]), .y(g[0]));
_and2 U1_and2	(.a(a[1]), .b(b[1]), .y(g[1]));
_and2 U2_and2	(.a(a[2]), .b(b[2]), .y(g[2]));
_and2 U3_and2	(.a(a[3]), .b(b[3]), .y(g[3]));

//Propagate
_or2 U4_or2	(.a(a[0]), .b(b[0]), .y(p[0]));
_or2 U5_or2	(.a(a[1]), .b(b[1]), .y(p[1]));
_or2 U6_or2	(.a(a[2]), .b(b[2]), .y(p[2]));
_or2 U7_or2	(.a(a[3]), .b(b[3]), .y(p[3]));

// 최종적인 올림값만 계산해주면 되기 때문에 아래의 연산만 실행함.
//co = G3 + (P3*G2) + (P3*P2*G1) + (P3*P2*P1*G0) + (P3*P2*P1*P0*CI)
_and2 U17_and2	(.a(p[3]), .b(g[2]), .y(w0_co));
_and3 U18_and3	(.a(p[3]), .b(p[2]), .c(g[1]), .y(w1_co));
_and4 U19_and4	(.a(p[3]), .b(p[2]), .c(p[1]), .d(g[0]), .y(w2_co));
_and5 U20_and5	(.a(p[3]), .b(p[2]), .c(p[1]), .d(p[0]), .e(ci), .y(w3_co));
_or5  U21_or5	(.a(g[3]), .b(w0_co), .c(w1_co), .d(w2_co), .e(w3_co), .y(co));

endmodule
