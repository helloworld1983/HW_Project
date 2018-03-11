module cla_clk(clock, a, b, ci, s_cla, co_cla);// 32BIT CLA MODULE에 FLIP-FLOP을 적용시킨 CLA_CLK MODULE
	input			clock;									// Flip-flop을 통해 clock의 rise timing에 계산이 되도록 함.
	input	[31:0]a,b;
	input			ci;
	output[31:0]s_cla;
	output		co_cla;
	
	reg [31:0]	reg_a, reg_b;
	reg			reg_ci;
	reg [31:0]	reg_s_cla;
	reg			reg_co_cla;
	
	wire[31:0]	wire_s_cla;
	wire			wire_co_cla;
	
	always @ (posedge clock) // clock이 positive Edge(rise timing)일때 작동하게함.
	begin
			// 각각의 Flip flop에 전달값 연결 
		reg_a			<= a;
		reg_b			<= b;
		reg_ci		<= ci;
		reg_s_cla	<= wire_s_cla;
		reg_co_cla 	<= wire_co_cla;
		
	end
	//Flip flop에서 나오는 값들을 32bit CLA에 연결
	cla32 U0_cla32(.a(reg_a), .b(reg_b), .ci(reg_ci), .s(wire_s_cla), .co(wire_co_cla));
	// CLA에서 연산된후  flip flop에서 나온 결과값들을 module의 합, carry out 변수에 이어줌 
	assign s_cla = reg_s_cla;
	assign co_cla = reg_co_cla;
	
	endmodule
		