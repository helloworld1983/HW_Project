module _ha(input a,b,output co,s); 
_and U0_and(.a(a), .b(b), .y(co));	// 입력된 값의 올림수 출력
_xor U0_xor(.a(a), .b(b), .y(s));	// 입력된 값의 합 출력
endmodule
//입력된 두개의 이진수 값의 합과 올림수를 산출해주는 Half Adder

