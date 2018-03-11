module fa(a,b,ci,s,co); // 두 개의 입력값의 합을 출력하는 full adder 
input a,b,ci;
output s,co;
wire w0, w1, w2;

ha U0_ha(.a(a), .b(b), .s(w2), .co(w0));
ha U1_ha(.a(w2), .b(ci), .s(s), .co(w1));
_or2	U2_or2(.a(w0), .b(w1), .y(co));

endmodule
