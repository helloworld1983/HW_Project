module _dff_r_async(clk, reset_n, d, q); // reset신호를 받으면 모두 0이 되는 D-flip flop
	input		clk, reset_n, d;		// Input clock, Reset, D
	output	q;							// Output Q
	reg		q;							// Register Q
	
	always	@	(posedge clk or negedge reset_n)		// When each clock's Rising edge or Reset = 0
	begin
		if(reset_n == 0)		q <= 1'b0;					// reset = 0 -> Q = 0
		else						q <= d;						// else Q = D
	
	end
endmodule

