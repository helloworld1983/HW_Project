module bus_arbit(M0_req,M1_req,reset_n,clk, out); // decide priority of master
input clk, reset_n, M0_req, M1_req;
output reg [1:0] out;
reg [1:0]next;
// parameter setting 
parameter M0_GRANT = 2'b10;
parameter M1_GRANT = 2'b01;

// set start state to M0_GRANT
initial
begin
	out <= M0_GRANT;
end

always@(posedge clk or negedge reset_n) 
begin
	if(reset_n == 1'b0)  // if reset_n = 0, M0 has priority
	begin
		if(out != M0_GRANT)  out<= 2'b00;
	end
	else	out <= next;						//in else case next master has priority 
end


always@(M0_req, M1_req, out) 
begin 
	next <= 2'b0;
	if(out == 2'b10) // if M0 has priority
	begin
		if((M0_req==0 && M1_req ==0)||M0_req ==1) next <= M0_GRANT; // (M0_req = 0 && M1 req= 0) or M0_req = 1, M0 keep priority
		if((M0_req ==0)&&(M1_req==1)) next <= M1_GRANT;			//	if M0_req =0, M1_req = 1, M1 get priority 
	end
	
	if(out == 2'b01) // if M1 has priority
	begin
		if(M1_req == 1) next <= M1_GRANT;								// M1_req= 1, M1 keep priority
		if(M1_req == 0) next <= M0_GRANT;								// M1_req= 0, M0 getpriority
	end
end

endmodule 