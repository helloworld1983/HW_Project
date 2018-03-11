module _full(input a,b,ci, output s,co); // carry in을 a,b연산에 포함시켜서 올림값과 합을 출력하는 module
wire w0, w1, w3;

_half U0(.a(b), .b(ci), .cout(w0), .sum(w1));
_half U1(.a(a), .b(w1), .cout(w3), .sum(s)); 
_or U2(.a(w3), .b(w0), .y(co));

endmodule
