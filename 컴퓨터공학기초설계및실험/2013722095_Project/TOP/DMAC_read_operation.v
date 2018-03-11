//print output accoding to Addr selectively by using 8to1 multiplexer(for DMAC)
module DMAC_read_operation(Addr, Data, from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7);
input [7:0] from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7;
input [2:0] Addr;
output [7:0] Data;

_8_to_1_MUX U0_81MUX(from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7, Addr, Data);

endmodule

/*8 to 1 MUX : print each value accoding to 3bit selector.*/
module _8_to_1_MUX(a,b,c,d,e,f,g,h, sel, d_out);
input [7:0] a,b,c,d,e,f,g,h;
input [2:0]	 sel;
output reg [7:0] d_out;

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
	default 	:	d_out = 8'hx;
	endcase
end
endmodule 