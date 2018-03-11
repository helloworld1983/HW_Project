module TOP(clk, reset_n, M0_req, M0_wr, M0_address, M0_dout, M0_grant, M_din, d_interrupt, m_interrupt); // TOP modujle 
// input value
input 		 clk, reset_n;
input 		 M0_req, M0_wr;
input [7:0]	 M0_address;
input [31:0] M0_dout;

//output value
output		  M0_grant, d_interrupt, m_interrupt;
output [31:0] M_din;

// inner wire
wire 			M1_req, M1_wr, M1_grant;
wire [7:0]	M1_address;
wire [31:0] M1_dout; 
wire [31:0] wM_din;
wire [31:0] S0_dout, S1_dout, S2_dout, S3_dout, S4_dout;
wire			S0_sel, S1_sel, S2_sel, S3_sel, S4_sel;
wire			S_wr;
wire [7:0]	S_address;
wire [31:0] S_din;

// assign output
assign M_din = wM_din;

// module instance
BUS U0_bus(.clk(clk), .reset_n(reset_n), .M0_req(M0_req), .M0_wr(M0_wr), .M0_address(M0_address), .M0_dout(M0_dout), .M1_req(M1_req), 
				.M1_wr(M1_wr), .M1_address(M1_address), .M1_dout(M1_dout), .S0_dout(S0_dout), .S1_dout(S1_dout), .S2_dout(S2_dout), 
				.S3_dout(S3_dout), .S4_dout(S4_dout), .M0_grant(M0_grant), .M1_grant(M1_grant), .M_din(wM_din), .S0_sel(S0_sel),
				.S1_sel(S1_sel), .S2_sel(S2_sel), .S3_sel(S3_sel), .S4_sel(S4_sel), .S_address(S_address), .S_wr(S_wr), .S_din(S_din));


DMAC_Top U1_dmac(.clk(clk), .reset_n(reset_n), .M_grant(M1_grant), .M_din(wM_din), .S_sel(S0_sel), .S_wr(S_wr), .S_address(S_address), 
				.S_din(S_din), .M_req(M1_req), .M_wr(M1_wr), .M_address(M1_address), .M_dout(M1_dout), .S_dout(S0_dout), .interrupt(d_interrupt));


Multiplier_Top U2_multiplier_top(.clk(clk), .reset_n(reset_n), .S_sel(S1_sel), .S_wr(S_wr), .S_address(S_address), .S_din(S_din), 
											.S_dout(S1_dout), .m_interrupt(m_interrupt)); 


ram U3_ram_multiplicand	(.clk(clk), .cen(S2_sel), .wen(S_wr), .addr(S_address), .din(S_din), .dout(S2_dout)); // 32bit 입력값을 32개까지 저장할 수 있는 ram module
ram U4_ram_multiplier	(.clk(clk), .cen(S3_sel), .wen(S_wr), .addr(S_address), .din(S_din), .dout(S3_dout)); // 32bit 입력값을 32개까지 저장할 수 있는 ram module
ram U5_ram_result			(.clk(clk), .cen(S4_sel), .wen(S_wr), .addr(S_address), .din(S_din), .dout(S4_dout)); // 32bit 입력값을 32개까지 저장할 수 있는 ram module

endmodule 