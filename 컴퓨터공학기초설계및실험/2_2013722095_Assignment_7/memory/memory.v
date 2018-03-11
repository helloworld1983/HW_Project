module memory(clk, cen, wen, addr, din, dout); // 32bit 입력값을 32개까지 저장할 수 있는 memory module
input clk, cen, wen;
input [7:0] addr;
input [31:0] din;
output reg [31:0] dout;
reg [31:0] mem [0:31];

integer i;
// memory 초기화 
initial 
begin
	for(i = 0; i<32; i = i+1)
		mem[i] <= 5'b0;
end

always @(posedge clk)
	begin
		if(cen == 1 && wen ==1) 		// chip enable과 write enable이 모두 1이면 
			begin 
			mem[addr] <= din; dout <= 32'b0; // 입력받은 값을 입력받은 주소에 저장함.
			end
		else if(cen ==1 && wen ==0) dout <= mem[addr]; // chip enable이 1이고 write enable이 0이면 입력받은 주소위치에 있는 data를 출력
		else dout <= 32'b0;						// 그 이외의 경우에는 0을 출력한다.
	end

endmodule