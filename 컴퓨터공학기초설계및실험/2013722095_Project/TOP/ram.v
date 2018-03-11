module ram(clk, cen, wen, addr, din, dout); //  ram module can save 32 data which bandwdith is 32bit
// input value
input clk, cen, wen;
input [4:0] addr;
input [31:0] din;
// output value
output reg [31:0] dout;

// inner register
reg [31:0] mem [0:31];

integer i;
// initialize memory
initial 
begin
	for(i = 0; i<32; i = i+1)
		mem[i] <= 5'b0;
end

always @(posedge clk)
	begin
		if(cen == 1 && wen ==1) 		// if chip enable = 1 enable = 1
			begin 
			mem[addr] <= din; dout <= 32'b0; // save input data in memory 
			end
		else if(cen ==1 && wen ==0) dout <= mem[addr]; // chip enable = 1, write enable = 1 -> print data place in inserted address
		else dout <= 32'b0;						// in else case, print 0
	end

endmodule 