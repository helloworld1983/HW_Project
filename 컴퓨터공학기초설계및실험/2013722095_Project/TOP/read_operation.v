//print output accoding to Addr selectively by using 8to1 multiplexer(for FIFO)
module read_operation(Addr, Data, from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7);
input [31:0] from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7;
input [2:0] Addr;
output [31:0] Data;

_8_to_1_MUX_32bit U0_8_to_1_MUX_32bit(from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7, Addr, Data);

endmodule


//print output accoding to Addr selectively by using 16to1 multiplexer(for Register file)
module read_operation_result(Addr, Data, from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7, from_reg8,
							  from_reg9, from_reg10, from_reg11, from_reg12, from_reg13, from_reg14, from_reg15);
							  
input [31:0]from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7, from_reg8,
							  from_reg9, from_reg10, from_reg11, from_reg12, from_reg13, from_reg14, from_reg15;
input [3:0] Addr;
output [31:0] Data;

_16_to_1_MUX U0_16_to_1_MUX(from_reg0, from_reg1, from_reg2, from_reg3, from_reg4, from_reg5, from_reg6, from_reg7, from_reg8,
							  from_reg9, from_reg10, from_reg11, from_reg12, from_reg13, from_reg14, from_reg15, Addr, Data);

endmodule



/*16to1 multiplexer : print 8 values by using 4bit selector.*/
module _16_to_1_MUX(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p, sel, d_out);
input [31:0] a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p;
input [3:0]	 sel;
output reg [31:0] d_out;

always@(sel,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
begin
	case(sel)
	4'b0000	:	d_out = a;
	4'b0001	:	d_out = b;
	4'b0010 	:	d_out = c;
	4'b0011	:	d_out = d;
	4'b0100	:	d_out = e;
	4'b0101 	:	d_out = f;
	4'b0110	:	d_out = g;
	4'b0111	:	d_out = h;
	4'b1000	:	d_out = i;
	4'b1001	:	d_out = j;
	4'b1010	:	d_out = k;
	4'b1011	:	d_out = l;
	4'b1100	:	d_out = m;
	4'b1101	:	d_out = n;
	4'b1110	:	d_out = o;
	4'b1111	:	d_out = p;
	default 	:	d_out = 32'hx;
	endcase
end
endmodule


/*8to1 multiplexer : print 8 values by using 3bit selector.*/
module _8_to_1_MUX_32bit(a,b,c,d,e,f,g,h, sel, d_out);
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
	default 	:	d_out = 8'hx;
	endcase
end
endmodule 
