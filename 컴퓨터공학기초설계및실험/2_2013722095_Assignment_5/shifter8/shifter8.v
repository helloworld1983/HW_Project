module shifter8(clk, reset_n, op, shamt, d_in, d_out);// register에서 기억하고 있는 값을 입력된 shamt만큼 이동시키는 topmodule
input clk, reset_n;
input [2:0] op;
input [1:0] shamt;
input [7:0] d_in;
output [7:0] d_out;

wire [7:0] do_next;

_register8_r 	U0(.clk(clk), .reset_n(reset_n), .d(do_next), .q(d_out)); // 기억소자 
cc_logic 		U1(.op(op), .shamt(shamt), .d_in(d_in), .d_out(d_out), .do_next(do_next)); // 실제 bit stream을 opcode에 따라 움직이는 module

endmodule