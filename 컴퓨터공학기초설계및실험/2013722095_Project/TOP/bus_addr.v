module bus_addr(s_address,s0_sel, s1_sel, s2_sel, s3_sel, s4_sel); // address decoder module
input [7:0]s_address;
output reg s0_sel, s1_sel, s2_sel, s3_sel, s4_sel;

always@(s_address)
	begin
	/* DMAC address region */
	if((s_address >= 8'h00)&&(s_address <8'h10)) // 0x00=<s_address <0x10
		{s0_sel, s1_sel, s2_sel, s3_sel, s4_sel} <= 5'b10000;	 // s0_sel  = 1
	/* MULTIPLIER address region */		//
	else if((s_address >= 8'h10)&&(s_address <8'h20)) // 0x10=<s_address <0x20
		{s0_sel, s1_sel, s2_sel, s3_sel, s4_sel} <= 5'b01000;	 // s1_sel = 1		
	/* RAM(multiplier) */
	else if((s_address >= 8'h20)&&(s_address <8'h40)) // 0x20=<s_address <0x40
		{s0_sel, s1_sel, s2_sel, s3_sel, s4_sel} <= 5'b00100;	 // s2_sel = 1		
	/* RAM(multiplicand) */
	else if((s_address >= 8'h40)&&(s_address <8'h60)) // 0x40=<s_address <0x60
		{s0_sel, s1_sel, s2_sel, s3_sel, s4_sel} <= 5'b00010;	 // s3_sel = 1							
	/* RAM(result) */
	else if((s_address >= 8'h60)&&(s_address <8'h80)) // 0x60=<s_address <0x80
		{s0_sel, s1_sel, s2_sel, s3_sel, s4_sel} <= 5'b00001;	 // s4_sel = 1							
	/* Default */
	else
		{s0_sel, s1_sel, s2_sel, s3_sel, s4_sel} <= 5'bx;	
		
		
	end
endmodule
