module tl_cntr(clk,reset,Ta,Tb,La,Lb);
	input Ta,Tb,clk,reset;
	output[1:0]La,Lb;
	
	wire [1:0] w0, w1;
	
	ns_logic U0(.d(w0), .q(w1), .Ta(Ta), .Tb(Tb)); // register에서의 Q를 입력으로 받고 D를 출력
	_register2_r_async U1(.clk(clk), .d(w0), .q(w1), .reset(reset)); // ns_logic의 출력인 D를 입력으로 받고 Q를 출력 
	o_logic U2(.q(w1), .La(La), .Lb(Lb)); // register에서 출력된 Q를 입력으로 하여 TRAFFIC LIGHT를 출력 


endmodule
