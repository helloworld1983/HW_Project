module DMAC_Top(clk, reset_n, M_grant, M_din, S_sel, S_wr, S_address, S_din, M_req, M_wr, M_address, M_dout, S_dout, interrupt); // DMAC TOP
// input value
input clk, reset_n, M_grant, S_sel, S_wr;
input [31:0]  M_din, S_din;
input [7:0] S_address;

// output value
output  M_req, M_wr, interrupt;
output [7:0] M_address;
output [31:0] M_dout, S_dout;

// inner wire
wire wr_en_to_ff, rd_en_to_ff;
wire op_done_from_master;
wire op_start_to_master, op_done_clear_to_master;
wire [7:0] source_addr_to_ff, dest_addr_to_ff, data_size_to_ff;
wire [7:0] source_addr_to_master, dest_addr_to_master, data_size_to_master;
wire [2:0] opmode_to_master;
wire [3:0] datacount_to_master;

// instance sub modules
DMAC_slave U0_dmac_slave(.clk(clk), .reset_n(reset_n), .S_sel(S_sel), .S_wr(S_wr), .op_done(op_done_from_master),
 .S_address(S_address), .S_din(S_din), .source_addr(source_addr_to_ff), .dest_addr(dest_addr_to_ff), .datasize(data_size_to_ff),
 .interrupt(interrupt), .op_start(op_start_to_master), .op_done_clear(op_done_clear_to_master), .wr_en(wr_en_to_ff), .S_dout(S_dout), 
 .opmode(opmode_to_master));

 
DMAC_fifo U1_dmac_fifo(.clk(clk), .reset_n(reset_n), .rd_en(rd_en_to_ff), .wr_en(wr_en_to_ff), .in_sourceaddr(source_addr_to_ff), 
  .in_datasize(data_size_to_ff), .in_desaddr(dest_addr_to_ff), .data_count(datacount_to_master), .out_sourceaddr(source_addr_to_master), 
  .out_datasize(data_size_to_master), .out_desaddr(dest_addr_to_master));
  
DMAC_master U2_dmac_master(.clk(clk), .reset_n(reset_n), .opstart(op_start_to_master), .opdone(op_done_from_master), .opdone_clear(op_done_clear_to_master), 
 .source_addr(source_addr_to_master), .dest_addr(dest_addr_to_master), .data_size(data_size_to_master), .data_count(datacount_to_master), 
 .rd_en(rd_en_to_ff), .M_req(M_req), .M_grant(M_grant), .M_wr(M_wr), .M_address(M_address), .M_dout(M_dout), .M_din(M_din), .opmode(opmode_to_master));

 endmodule

