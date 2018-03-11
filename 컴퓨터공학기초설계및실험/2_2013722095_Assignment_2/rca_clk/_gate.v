module _xor(input a,b, output y); // 입력된 두 값이 서로 다를 때 1을 출력해주는 exclusive or 게이트
wire w0, w1;
_or U0_or(.a(a), .b(b), .y(w0));	// or게이트는 0111의 진리표값이 나오고
_nand U0_nand(.a(a), .b(b), .y(w1));//nand게이트는 1110의 진리표값이 나오므로
_and U0_and(.a(w0), .b(w1), .y(y));// 이 두 게이트의 출력값을 and게이트의 입력값으로 넣어주면 0110의 xor 게이트 진리표값이 나온다.
endmodule



module _nand(input a,b, output y); // 입력된 두 값이 and 게이트를 통해 출력된 값을 반전시켜 출력해주는 nand게이트

wire w0;
_and U0_and(.a(a), .b(b), .y(w0)); //and gate module에 a,b(nand gate의)를 입력값으로 넣고 그 출력값을 w0에 넣어준다
_inv U0_inv(.a(w0), .y(y));			// w0에 들어간 값을 반전시켜서 y(nand gate의)에 반환
endmodule





module _and(input a,b, output y);// 입력된 두 값이 모두 1일때 1을 출력하고 그 외에는 0을 출력하는 and 게이트 
assign y=a&b;
endmodule


module _or(input a,b, output y); // 입력된 두 값이 모두 0일때 0을 출력하고 그 이외에는 1을 출력하는 or 게이트
assign y = a|b;
endmodule


module _inv(input a, output y);// 입력된 값을 반전시켜서 출력시켜주는 inverter 게이트
assign y=~a;
endmodule
