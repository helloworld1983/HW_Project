module _rca(a,b,ci,s,co);//입력된 두 값의 합을 계산해주는 ripple carry adder
input [3:0] a,b;	//각각 4비트 짜리 입력값
input ci;			//이전 모듈에서 넘겨져오는 올림값.
output [3:0] s;	//올림값을 넘기고 나서의 실질적인 비트 합값
output co;			//다음 모듈에 넘겨줄 올림값
wire [2:0]c;		//올림값을 다음 모듈의 ci로 넘겨줄 wire

_fa U0_fa(.a(a[0]), .b(b[0]), .ci(ci), .s(s[0]), .co(c[0]));//1의 자리 비트 합 및 올림값 계산
_fa U1_fa(.a(a[1]), .b(b[1]), .ci(c[0]), .s(s[1]), .co(c[1]));//1의 자리에서의 올림값을 carry in 으로 주고 2의 자리 비트 합 및 올림값 계산
_fa U2_fa(.a(a[2]), .b(b[2]), .ci(c[1]), .s(s[2]), .co(c[2]));//2의 자리에서의 올림값을 carry in 으로 주고 4의 자리 비트 합 및 올림값 계산
_fa U3_fa(.a(a[3]), .b(b[3]), .ci(c[2]), .s(s[3]), .co(co));//4의 자리에서의 올림값을 carry in으로 주고 8의 자리 비트 합 및 올림값 계산
			
endmodule
