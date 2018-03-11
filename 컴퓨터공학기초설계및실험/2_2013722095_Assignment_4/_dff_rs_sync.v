module _dff_rs_sync(clk, set_n, reset_n, d , q);
input clk, set_n, reset_n, d;
output q;
reg q;

always @(posedge clk) // clock이 positive edge일때 
begin
	if(reset_n == 0) q<= 1'b0;// reset_n이 0이면 q = 0
	else if(set_n ==0) q<= 1'b1;// reset_n =1, set_n =0이면 q =1;
	else					q<= d;// reset_n =1, set_n =1 이면 q = d
end
endmodule
