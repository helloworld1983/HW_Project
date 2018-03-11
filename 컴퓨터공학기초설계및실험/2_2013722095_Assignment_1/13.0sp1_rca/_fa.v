module _fa(input a,b,ci, output s,co);
wire w0, w1, w3;

_ha U0(.a(b), .b(ci), .co(w0), .s(w1)); // 입력값 하나와 전열에서 올라온 올림값을 half adder에 넣어 이에 대한 올림값, 합계를 계산하고 
_ha U1(.a(a), .b(w1), .co(w3), .s(s));  // 이 합과 다른 입력값과의 합 및 올림값을 계산해서
_or U2(.a(w3), .b(w0), .y(co));			 // 최종적인 올림값 계산.

endmodule
//올림수를 받아 입력된 두 값의 계산에 적용하는 Full Adder