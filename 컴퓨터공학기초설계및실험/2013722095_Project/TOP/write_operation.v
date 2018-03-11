module write_operation(Addr, we, to_reg); // when we = 1, pass decoded address to register for FIFO
input we;
input [2:0] Addr;
output [7:0] to_reg;

wire [7:0] write_wire;
_3_to_8_decoder U0(Addr, write_wire); // decoding

_and2 U1_and2(write_wire[0], we, to_reg[0]);
_and2 U2_and2(write_wire[1], we, to_reg[1]);
_and2 U3_and2(write_wire[2], we, to_reg[2]);
_and2 U4_and2(write_wire[3], we, to_reg[3]);
_and2 U5_and2(write_wire[4], we, to_reg[4]);
_and2 U6_and2(write_wire[5], we, to_reg[5]);
_and2 U7_and2(write_wire[6], we, to_reg[6]);
_and2 U8_and2(write_wire[7], we, to_reg[7]);
endmodule



module write_operation_result(Addr, we, to_reg);  // when we = 1, pass decoded address to register for REGISTER FILE
input we;
input [3:0] Addr;
output [15:0] to_reg;

wire [15:0] write_wire;
_4_to_16_decoder U0(Addr, write_wire); // decoding

_and2 U1_and2(write_wire[0], we, to_reg[0]);
_and2 U2_and2(write_wire[1], we, to_reg[1]);
_and2 U3_and2(write_wire[2], we, to_reg[2]);
_and2 U4_and2(write_wire[3], we, to_reg[3]);
_and2 U5_and2(write_wire[4], we, to_reg[4]);
_and2 U6_and2(write_wire[5], we, to_reg[5]);
_and2 U7_and2(write_wire[6], we, to_reg[6]);
_and2 U8_and2(write_wire[7], we, to_reg[7]);
_and2 U9_and2(write_wire[8], we, to_reg[8]);
_and2 U10_and2(write_wire[9], we, to_reg[9]);
_and2 U11_and2(write_wire[10], we, to_reg[10]);
_and2 U12_and2(write_wire[11], we, to_reg[11]);
_and2 U13_and2(write_wire[12], we, to_reg[12]);
_and2 U14_and2(write_wire[13], we, to_reg[13]);
_and2 U15_and2(write_wire[14], we, to_reg[14]);
_and2 U16_and2(write_wire[15], we, to_reg[15]);

endmodule


module _4_to_16_decoder(d,q); // decoding module
input [3:0] d;
output reg [15:0]q;
// print 8bit value accoding to input value
always @(d) 
begin
	case(d)
		4'b0000 : q = 16'b0000000000000001;
		4'b0001 : q = 16'b0000000000000010;
		4'b0010 : q = 16'b0000000000000100;
		4'b0011 : q = 16'b0000000000001000;
		4'b0100 : q = 16'b0000000000010000;
		4'b0101 : q = 16'b0000000000100000;
		4'b0110 : q = 16'b0000000001000000;
		4'b0111 : q = 16'b0000000010000000;
		4'b1000 : q = 16'b0000000100000000;
		4'b1001 : q = 16'b0000001000000000;
		4'b1010 : q = 16'b0000010000000000;
		4'b1011 : q = 16'b0000100000000000;
		4'b1100 : q = 16'b0001000000000000;
		4'b1101 : q = 16'b0010000000000000;
		4'b1110 : q = 16'b0100000000000000;
		4'b1111 : q = 16'b1000000000000000;
		default : q = 16'hx;
	endcase
end
endmodule 