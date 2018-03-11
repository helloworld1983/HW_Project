module _dff_rs_async(clk, set_n, reset_n, d, q);
input clk, set_n, reset_n, d;
output q;
reg q;

always@(posedge clk or negedge set_n or negedge reset_n) // clk에 상관없이 set_n, reset_n이 negative edge일때도 다음의 내용을 진행함.
begin 
	if(reset_n == 0)	q<= 1'b0; // reset_n  = 0이면 q =0
	else if(set_n ==0) q<= 1'b1;// reset_n =1, set= 0이면 q = 1
	else 						q<=d;	// reset_n =1, set= 1이면 q = d
end

endmodule
