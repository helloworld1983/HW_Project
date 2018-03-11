module _dlatch(clk, d, q, q_bar);
	input clk, d;
	output q, q_bar;
	wire d_bar, r, s;
	
	_inv U0_inv (.a(d), .y(d_bar));
	_and2 U1_and2 (.a(d_bar), .b(clk), .y(r)); // clk과 d_bar가 두 1이 되면 reset을 1로 함
	_and2 U2_and2 (.a(d), .b(clk), .y(s));			// d와 clk가 모두 1이 되면 set을 1로 함 
	_srlatch U3_srlatch (.s(s), .r(r), .q(q), .q_bar(q_bar)); // set, reset을 sraltch에 넣어서 Q와 ~Q를 출력함.
endmodule
