//  register file for fifo
module Register_file(clk, reset_n, wAddr, wData, we, rAddr, rData);
input clk, reset_n, we;
input [2:0] wAddr, rAddr;
input [31:0] wData;
output [31:0] rData;

wire [7:0] to_reg;
wire [31:0] from_reg[7:0];
/*pass input data accoding to reset_n and we.*/
write_operation U0_write_operation(wAddr, we, to_reg);
/*save wData to register file from write operation accoding to write address*/
register32_8 U1_register32_8(clk, reset_n, to_reg, wData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7]);
/*print data in register file accoding to read address */
read_operation U2_read_operation(rAddr, rData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7]);

endmodule


// register file for result
module Register_file_result(clk, reset_n, wAddr, wData, we, rAddr, rData);
input clk, reset_n, we;
input [3:0] wAddr, rAddr;
input [31:0] wData;
output [31:0] rData;

wire [15:0] to_reg;
wire [31:0] from_reg[15:0];
/*pass input data accoding to reset_n and we.*/
write_operation_result U0_write_operation(wAddr, we, to_reg);
/*save wData to register file from write operation accoding to write address*/
register32_15 U1_register32_15(clk, reset_n, to_reg, wData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7],
											from_reg[8], from_reg[9], from_reg[10], from_reg[11], from_reg[12], from_reg[13], from_reg[14], from_reg[15]);
/*print data in register file accoding to read address */
read_operation_result U2_read_operation(rAddr, rData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7],
											from_reg[8], from_reg[9], from_reg[10], from_reg[11], from_reg[12], from_reg[13], from_reg[14], from_reg[15]);

endmodule 