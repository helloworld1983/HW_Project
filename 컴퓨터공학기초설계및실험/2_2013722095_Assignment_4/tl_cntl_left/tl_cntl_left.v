module tl_cntl_left(clk,reset,Ta,Tb,Tal,Tbl,La,Lb);
	input Ta,Tb,Tal,Tbl,clk,reset;
	output[1:0]La,Lb;
	
	wire [2:0] w0, w1;
	
	ns_logic U0_ns_logic (.d(w0), .q(w1), .Ta(Ta), .Tal(Tal), .Tb(Tb), .Tbl(Tbl)); // register에서의 Q를 입력으로 받고 D를 출력
	_register3_r U1_register3_r (.clk(clk), .d(w0), .q(w1), .reset(reset)); // ns_logic의 출력인 D를 입력으로 받고 Q를 출력 
	o_logic U2_o_logic (.q(w1), .La(La), .Lb(Lb)); // register에서 출력된 Q를 입력으로 하여 TRAFFIC LIGHT를 출력 


endmodule
