module _ha(input a,b,output co,s); 
assign co = a&b; // 올림값은 a와 b의 and gate 결과로 정의
assign s = a^b;	// 합 값은 올림값이 발생하는 경우를 가정해 exclusive or gate의 결과로 정의

endmodule
//입력된 두개의 이진수 값의 합과 올림수를 산출해주는 Half Adder

