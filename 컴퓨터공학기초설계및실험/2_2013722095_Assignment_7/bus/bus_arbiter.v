module bus_arbit(M0_req,M1_req,reset_n,clk, out);
input clk, reset_n, M0_req, M1_req;
output reg [1:0] out;
reg [1:0]next;

parameter M0_GRANT = 2'b01;
parameter M1_GRANT = 2'b10;

// 초기 출력값을 M0_GRANT로 설정함.
initial
begin
	out <= M0_GRANT;
end

always@(posedge clk or negedge reset_n) // reset_n이 0이면 00값을 출력 
begin
	if(reset_n == 1'b0) 
	begin
		if(out != M0_GRANT)  out<= 2'b00;
	end
	else	out <= next;						//	그 외의 경우에는 next값을 출력
end


always@(M0_req, M1_req) 
begin 
	if((M0_req==0 && M1_req ==0)||M0_req ==1) next <= M0_GRANT; // M0_req, M1_req가 0이거나 M0_req가 0dlaus M0_GRANT를 next에 넣음
	else if((M0_req ==0)&&(M1_req==1)) next <= M1_GRANT;			//	M0_req =0이고 M1_req = 1이면 M1_GRANT를 출력
	else if(M1_req == 1) next <= M1_GRANT;								// M1_req가 1이면 M1_GRANT를 출력
	else if(M1_req == 0) next <= M0_GRANT;								// M1_req각 0이면 M0_GRANT를 출력
	else ;
end

endmodule