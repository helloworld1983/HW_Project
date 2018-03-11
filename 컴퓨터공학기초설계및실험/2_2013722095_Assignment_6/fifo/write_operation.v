module write_operation(Addr, we, to_reg); // we가 1일 때만 decoding 된 Addr값을 register로 넘겨주는 module
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

module _3_to_8_decoder(d,q); // decoding module
input [2:0] d;
output reg [7:0]q;
// 3bit입력값에 따라 각각의 8bit output값을 출력한다.
always @(d) 
begin
	case(d)
		3'b000 : q = 8'b00000001;
		3'b001 : q = 8'b00000010;
		3'b010 : q = 8'b00000100;
		3'b011 : q = 8'b00001000;
		3'b100 : q = 8'b00010000;
		3'b101 : q = 8'b00100000;
		3'b110 : q = 8'b01000000;
		3'b111 : q = 8'b10000000;
		default : q = 8'hx;
	endcase
end
endmodule 