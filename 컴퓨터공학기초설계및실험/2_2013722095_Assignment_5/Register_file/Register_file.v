module Register_file(clk, reset_n, wAddr, wData, we, rAddr, rData);
input clk, reset_n, we;
input [2:0] wAddr, rAddr;
input [31:0] wData;
output [31:0] rData;

wire [7:0] to_reg;
wire [31:0] from_reg[7:0];
/*reset_n, we에 의해서 입력데이터를 register로 넘길 수 있다.*/
write_operation U0_write_operation(wAddr, we, to_reg);
/*write operation에서 넘어온 register 주소에 wData값을 저장*/
register32_8 U1_register32_8(clk, reset_n, to_reg, wData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7]);
/*register에 저장되어있는 값을 rAddr 값에 따라서 출력*/
read_operation U2_read_operation(rAddr, rData, from_reg[0], from_reg[1], from_reg[2], from_reg[3], from_reg[4], from_reg[5], from_reg[6], from_reg[7]);

endmodule
