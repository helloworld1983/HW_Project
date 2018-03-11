module mx2(d1, s, y); // 두개의 입력을 s의 값에 따라 조건적으로 출력해주는 mux 모듈 

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

