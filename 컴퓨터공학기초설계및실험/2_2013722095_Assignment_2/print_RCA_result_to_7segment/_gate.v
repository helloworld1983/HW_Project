module _xor(input a,b, output y); // 값이 서로 다를 때 1이 출력되는 gate
wire w0, w1;
_or U0_or(.a(a), .b(b), .y(w0));
_nand U0_nand(.a(a), .b(b), .y(w1));
_and U0_and(.a(w0), .b(w1), .y(y));
endmodule



module _nand(input a,b, output y); //입력이 모두 1일때만 0을 출력하는 gate

wire w0;
_and U0_and(.a(a), .b(b), .y(w0));
_inv U0_inv(.a(w0), .y(y));
endmodule





module _and(input a,b, output y); // 두 입력이 모두 1일때 1을 출력
assign y=a&b;
endmodule


module _or(input a,b, output y); // 두 입력이 모두 0일때만 0을 출력
assign y = a|b;
endmodule


module _inv(input a, output y); // 값을 반전시켜서 출력
assign y=~a;
endmodule
