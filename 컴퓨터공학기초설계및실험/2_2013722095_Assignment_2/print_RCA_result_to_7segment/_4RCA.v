module _4RCA(a,b,ci,co,s); // 4bit ripple carry adder
input [3:0] a,b;
input ci;
output [3:0] s;
output co;
wire [2:0]w;

_full U0_full(.a(a[0]), .b(b[0]), .ci(ci), .co(w[0]), .s(s[0]));
_full U1_full(.a(a[1]), .b(b[1]), .ci(w[0]), .co(w[1]), .s(s[1]));
_full U2_full(.a(a[2]), .b(b[2]), .ci(w[1]), .co(w[2]), .s(s[2]));
_full U3_full(.a(a[3]), .b(b[3]), .ci(w[2]), .co(co), .s(s[3]));
			
endmodule
