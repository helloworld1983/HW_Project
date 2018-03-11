module mx2_32bits(d0,d1,s,y); // MUX2 module
input [31:0] d0, d1;
input 		 s;
output [31:0] y;

assign y=(s==0) ? d0:d1; // 3항 연산자를 이용해서 s=0이면 d0를 아니라면 d1을 출력한다.
endmodule
