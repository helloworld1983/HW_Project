module Multiplier_Top(clk, reset_n, S_sel, S_wr, S_address, S_din, S_dout, m_interrupt); // Multiplier Top
// input value
input 			clk, reset_n;
input 			S_sel, S_wr;
input [7:0] 	S_address;
input [31:0] 	S_din;
// output value
output [31:0]	S_dout;
output			m_interrupt;

// inner wire

wire 				opstart, opclear, opdone;
wire [31:0]		result;
wire [31:0]		MULTIPLICAND, MULTIPLIER; 
wire 				cand_empty, lier_empty;
wire				result_we, cand_we, lier_we, rd_en;
wire [31:0]		write_data, to_RESULT,    out_cand, out_lier;
wire [2:0]		master_state;
wire [3:0]		RF_waddr, RF_raddr;

// instance sub modules
mul_slave multiplier_slave(clk, reset_n, S_sel, S_wr, S_address, S_din, S_dout, m_interrupt, opstart, opclear, opdone, to_RESULT, 
								MULTIPLICAND, MULTIPLIER, cand_we, lier_we, master_state, RF_raddr);

fifo U0_multiplicand(.clk(clk), .reset_n(reset_n), .rd_en(rd_en), .wr_en(cand_we), .din(MULTIPLICAND) , .dout(out_cand), .empty(cand_empty));
fifo U1_multiplier(.clk(clk), .reset_n(reset_n), .rd_en(rd_en), .wr_en(lier_we), .din(MULTIPLIER) , .dout(out_lier), .empty(lier_empty));

Register_file_result U2_result_RF(.clk(clk), .reset_n(reset_n), .wAddr(RF_waddr), .wData(write_data), .we(result_we),
									.rAddr(RF_raddr), .rData(to_RESULT));
									
									
mul_master multiplier_master(reset_n, clk, out_lier, out_cand, opstart, opclear, opdone, cand_empty, lier_empty, write_data, master_state, result_we, RF_waddr, rd_en); // top module


endmodule 
