module _dff_r(clk, reset_n, d, q);
input clk, reset_n, d;
output reg q;

always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 0)  q <= 1'b0; // reset_n이 0이면 출력값을 0으로 
	else					q <= d;		// 그 이외의 경우 입력값을 출력값으로 함. 
end
endmodule
