//mux를 통해 연결되어있는  register 8개의 각 대이터를 입력된 Addr에 따라서 선택적으로 출력해주는 read operation module
module read_operation(Addr, Data, from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7);
input [31:0] from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7;
input [2:0] Addr;
output [31:0] Data;

_8_to_1_MUX U0_8_to_1_MUX(from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7, Addr, Data);

endmodule
/*8 to 1 MUX로써 입력된 opcode(sel)에 따라서 다른 값을 출력한다.*/
module _8_to_1_MUX(a,b,c,d,e,f,g,h, sel, d_out);
input [31:0] a,b,c,d,e,f,g,h;
input [2:0]	 sel;
output reg [31:0] d_out;

always@(sel,a,b,c,d,e,f,g,h)
begin
	case(sel)
	3'b000	:	d_out = a;
	3'b001	:	d_out = b;
	3'b010 	:	d_out = c;
	3'b011	:	d_out = d;
	3'b100	:	d_out = e;
	3'b101 	:	d_out = f;
	3'b110	:	d_out = g;
	3'b111	:	d_out = h;
	default 	:	d_out = 32'hx;
	endcase
end
endmodule