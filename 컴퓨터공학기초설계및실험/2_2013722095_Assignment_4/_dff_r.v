module _dff_r(d, reset, clk ,q ); // reset값에 의해 입력이 초기화 되는 _dff_r module
input d, reset, clk;
output q;
wire w0, w1;
	_inv U0(.a(reset) , .y(w1)); // active low에서 동작하게 하기 위해 reset을 반전시킴 
	_and2 U1(.a(d), .b(w1), .y(w0)); // reset_n값과 input인 d를 and gate의 input으로 넣어 입력을 control 한다 
	_dff U2(.clk(clk), .d(w0), .q(q)); // and gate의 출력값과 clk를 입력으로 하여  DFF 연산을 한다. 
endmodule
