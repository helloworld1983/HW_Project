// 두 개의 master와 slave를 갖는 bus module
module bus(clk, reset_n, M0_req,M0_wr, M0_address, M0_dout, M1_req, M1_wr, M1_address, M1_dout, S0_dout, S1_dout, M0_grant, M1_grant, M_din, S0_sel, S1_sel, S_address, S_wr, S_din);

input clk, reset_n, M0_req, M0_wr, M1_req, M1_wr;
input [7:0]M0_address, M1_address;
input [31:0]M0_dout, M1_dout, S0_dout, S1_dout;
output M0_grant, M1_grant, S0_sel, S1_sel, S_wr;
output [7:0] S_address;
output [31:0] M_din,S_din;

wire [1:0]arbit_out, dec_out, to_mux3;
wire [7:0] address_wire;
// instance arbiter module
bus_arbit U0_arbiter(.M0_req(M0_req), .M1_req(M1_req), .reset_n(reset_n), .clk(clk), .out(arbit_out));
assign M0_grant = arbit_out[0];
assign M1_grant = arbit_out[1];
// instance multiplexer module
mux2 			U1_mux2_1bit(.d0(M0_wr), .d1(M1_wr), .s(arbit_out[1]), .y(S_wr));
mux2_8bit 	U2_mux2_8bit(.d0(M0_address), .d1(M1_address), .s(arbit_out[1]), .y(address_wire));
assign S_address = address_wire;
mux2_32bit	U3_mux2_32bit(.d0(M0_dout), .d1(M1_dout), .s(arbit_out[1]), .y(S_din));
//instance address decoder module
bus_addr 	U4_address_dec(.s_address(address_wire), .s0_sel(S0_sel), .s1_sel(S1_sel));
assign dec_out = {S0_sel, S1_sel};
//instance d-flip flop
dff_r			U5_ff(.clk(clk), .reset_n(reset_n), .d(dec_out), .q(to_mux3));

mux3_32bit	U6_mux3_32bit(.d0(S0_dout), .d1(S1_dout), .s(to_mux3), .y(M_din));


endmodule
