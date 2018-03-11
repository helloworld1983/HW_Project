`timescale 1ns/100ps
module tb_tl_cntl_left;
reg tb_Ta, tb_Tb, tb_Tal, tb_Tbl;
reg clk, reset;

wire [1:0] tb_La, tb_Lb;
parameter STEP = 10;

tl_cntl_left U0(.clk(clk),.reset(reset),.Ta(tb_Ta), .Tal(tb_Tal),.Tb(tb_Tb), .Tbl(tb_Tbl),.La(tb_La),.Lb(tb_Lb)); // 선언된 TB인자들을 TOP MODULE에 적용 

always #(STEP/2) clk = ~clk; // 클럭이 반주기인 5NS마다 반전됨 
	initial
	begin
		clk = 0 ; reset = 0 ; tb_Ta= 0 ; tb_Tb = 0 ; tb_Tal = 0; tb_Tbl =0; // 모든 초기상태 설정 
	#5 reset = 1;tb_Ta = 1;// A길에 차량이 존재// RESET값을 NEGATIVE로 함. 
	#20  tb_Tal =1; // A길에 좌회전 차량 있음  
	#10 tb_Ta = 0;	// A길에 직진 차량 없음 
	#25 tb_Tal = 0;  // A길에 차량 없음 
	#15 tb_Tb = 1;		// B길에 직진차량 있음 
	#10 tb_Tbl = 1;	// B길에 좌회전 차량 있음
	#5	 tb_Tb =0; 		//B길에 직진차량 없음
	#30 tb_Tbl =0;    // B길에 좌회전 차량 없음 
	#40 $stop;
	end
endmodule
