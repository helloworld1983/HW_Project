module fa_v2(a,b,ci,s); // 두 개의 입력값의 합을 출력하는 full adder 
input a,b,ci;
output s;
wire w0;

_xor2 U0_xor2(.a(a), .b(b), .y(w0));
_xor2 U1_xor2(.a(w0), .b(ci), .y(s));

endmodule
