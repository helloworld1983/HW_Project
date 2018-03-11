//  bus module(master : 2 / slave : 3)
module BUS(clk, reset_n, M0_req,M0_wr, M0_address, M0_dout, M1_req, M1_wr, M1_address, M1_dout, S0_dout, S1_dout, S2_dout, S3_dout, S4_dout, M0_grant, M1_grant, M_din, S0_sel, S1_sel, S2_sel, S3_sel, S4_sel, S_address, S_wr, S_din);

input clk, reset_n, M0_req, M0_wr, M1_req, M1_wr;
input [7:0]M0_address, M1_address;
input [31:0]M0_dout, M1_dout, S0_dout, S1_dout, S2_dout, S3_dout, S4_dout;
output M0_grant, M1_grant, S0_sel, S1_sel, S2_sel, S3_sel, S4_sel, S_wr;
output [7:0] S_address;
output [31:0] M_din,S_din;
wire [4:0] dec_out;
wire [1:0] arbit_out;
wire [4:0] to_mux6;

// instance arbiter module
bus_arbit U0_arbiter(.M0_req(M0_req), .M1_req(M1_req), .reset_n(reset_n), .clk(clk), .out(arbit_out));
assign M0_grant = arbit_out[1];
assign M1_grant = arbit_out[0];
// instance multiplexer module
mux2 			U1_mux2_1bit(.d0(M0_wr), .d1(M1_wr), .s(arbit_out[0]), .y(S_wr)); // s = 0 이면 M0_wr을 내보낼것임 
mux2_8bit 	U2_mux2_8bit(.d0(M0_address), .d1(M1_address), .s(arbit_out[0]), .y(S_address));
mux2_32bit	U3_mux2_32bit(.d0(M0_dout), .d1(M1_dout), .s(arbit_out[0]), .y(S_din)); 
//instance address decoder module
bus_addr 	U4_address_dec(.s_address(S_address), .s0_sel(S0_sel), .s1_sel(S1_sel), .s2_sel(S2_sel), .s3_sel(S3_sel), .s4_sel(S4_sel));
assign dec_out = {S0_sel, S1_sel, S2_sel, S3_sel, S4_sel};
//instance d-flip flop
dff_r			U5_ff(.clk(clk), .reset_n(reset_n), .d(dec_out), .q(to_mux6));

mux6_32bit	U6_mux6_32bit(.d0(S0_dout), .d1(S1_dout), .d2(S2_dout), .d3(S3_dout), .d4(S4_dout), .s(to_mux6), .y(M_din));


endmodule
