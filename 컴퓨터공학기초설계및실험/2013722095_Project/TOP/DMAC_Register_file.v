module DMAC_Register_file(clk, reset_n, wAddr, wData, we, rAddr, rData); // Register file for DMAC
// input value 
input clk, reset_n, we;
input [2:0] wAddr, rAddr;
input [7:0] wData;
// output value 
output [7:0] rData;
// inner wire 
wire [7:0] to_reg;
wire [7:0] from_reg[7:0];

/*pass input data accoding to reset_n and we.*/
DMAC_write_operation U0_write_operation(wAddr, we, to_reg);
/*save wData to register file from write operation accoding to write address*/
register8_8 U1_register8_8(clk, reset_n, to_reg, wData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7]);
/*print data in register file accoding to read address */
DMAC_read_operation U2_read_operation(rAddr, rData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7]);

endmodule
