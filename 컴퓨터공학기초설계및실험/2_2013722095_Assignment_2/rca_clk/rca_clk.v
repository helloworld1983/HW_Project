module rca_clk(clock, a, b, ci, s_rca, co_rca); // Flip-flop을 통해 clock의 rise timing에 계산이 되도록 함.

	input 			clock;
	input 	[31:0] a, b;
	input 			ci;
	output 	[31:0] s_rca;
	output 			co_rca;
	
	
	reg		[31:0] reg_a, reg_b;
	reg				 reg_ci;
	reg		[31:0] reg_s_rca;
	reg				 reg_co_rca;
	
	wire 		[31:0] wire_s_rca;
	wire				 wire_co_rca;
	
	
	always @ (posedge clock) // clock이 positive Edge(rise timing)일때 작동하게함.
	begin
	// 각각의 Flip flop에 전달값 연결 
		reg_a			<= a;
		reg_b			<= b;
		reg_ci		<= ci;
		reg_s_rca	<=	wire_s_rca;
		reg_co_rca	<= wire_co_rca;
		
	end
	//Flip flop에서 나오는 값들을 32bit 연산 Ripple carry adder에 연결
	rca32 U0_rca(.a(reg_a), .b(reg_b), .ci(reg_ci), .s(wire_s_rca), .co(wire_co_rca));
	
	// Ripple carry Adder에서 연산된후  flip flop에서 나온 결과값들을 module의 합, carry out 변수에 이어줌 
	assign s_rca  = reg_s_rca;
	assign co_rca = reg_co_rca;
	
endmodule
