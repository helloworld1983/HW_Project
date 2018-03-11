module _half(input a,b,output cout,sum); // 두 입력의 합과 올림값을 출력하는 module
_and U0_and(.a(a), .b(b), .y(cout));
_xor U0_xor(.a(a), .b(b), .y(sum));
endmodule
