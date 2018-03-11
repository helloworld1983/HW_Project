module _dff(clk, d, q, q_bar); // clk에 따라서 q와 ~q를 출력해주는 D-flip flop

	input clk, d;
	output q, q_bar;
	wire clk_bar, w_q;
	
	_inv U0_inv (.a(clk), .y(clk_bar)); // ~clk 값을 clk_bar로 연결 
	_dlatch U1_dlatch (.clk(clk_bar), .d(d), .q(w_q)); // ~clk와 D를 입력으로 하여 w_q를 출력 
	_dlatch U2_dlatch (.clk(clk), .d(w_q), .q(q), .q_bar(q_bar)); // clk와 wire w_q를 입력으로 하여 output q, q_bar를 출력 
	
endmodule
