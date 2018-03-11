module bus_addr(s_address,s0_sel, s1_sel); // address decoder module
input [7:0]s_address;
output reg s0_sel, s1_sel;
// 아래의 조건에 따라 변환된 값을 출력 
always@(s_address)
	begin
	if((s_address >= 8'h00)&&(s_address <8'h20)) // 0x00=<s_address <0x20이면 
		{s0_sel, s1_sel} <= 2'b10;						//s0_sel = 1, s1_sel = 0을 출력
	else if((s_address >= 8'h20)&&(s_address < 8'h40))// 0x20=<s_address <0x40이면 
		{s0_sel, s1_sel} <= 2'b01;							  // s0_sel = 0, s1_sel = 1을 출력 
	else																// 그외의 경우에는 둘다 0을 출력
		{s0_sel, s1_sel} <= 2'b00;
end
endmodule
