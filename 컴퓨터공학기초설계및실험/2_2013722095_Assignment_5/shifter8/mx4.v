module mx4(d0, d1, d2, d3, s ,y);
input d0, d1, d2, d3;
input [1:0] s;
output y;
wire w0,w1;
// 4가지 선택지 중 하나를 선택해서 출력하는 multiplexer4
mx2 U0(d0,d1,s[0],w0); 
mx2 U1(d2,d3,s[0],w1);
mx2 U2(w0,w1,s[1],y);
endmodule
