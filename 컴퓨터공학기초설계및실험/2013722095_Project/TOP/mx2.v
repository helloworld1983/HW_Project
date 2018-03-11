module mx2(d1, s, y); // Selectively choose two values by 1bit sel

input [31:0] d1;
input s;
output reg [31:0] y;

always@(s, d1)
begin
	if (s == 0)
		y <= 32'h00000000;
	else
		y<= d1;
end
endmodule
// Selectively choose 8 values by 4bit sel
module mx8_32bit(a,b,c,d,e,f,g,s,y);
input [31:0] a,b,c,d,e,f,g;
input [3:0] s;
output reg [31:0] y;
always@(s,a,b,c,d,e,f,g)
begin
	if(s == 4'b0000)
		y<= a;
	else if(s == 4'b0001)
		y<= b;
	else if(s == 4'b0010)
		y <=c;
	else if(s == 4'b0011)
		y<=d;
	else if(s == 4'b0100)
		y<=e;
	else if(s == 4'b0101)
		y<=f;
	else if(s == 4'b0110)
		y<=g;
	else 
		y<=32'b0;
end
endmodule 