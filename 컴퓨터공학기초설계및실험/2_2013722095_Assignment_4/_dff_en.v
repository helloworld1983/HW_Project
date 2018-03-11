module _dff_en(clk, d, q, en); // en값에 따라서 q를 출력해주는 D-flip flop

	input clk, d, en;
	output q;
	wire clk_bar, w_q;
	wire w0;
	mx2 U0_mx(.d0(q), .d1(d), .s(en), .y(w0)); // mux2를 통해서 입력을 조정 0이면 q / 1이면 d
	_dff U1_dff(.clk(clk), .d(w0), .q(q)); // mux2에서 나온 출력값을 입력으로 받아 q를 출력 
endmodule
