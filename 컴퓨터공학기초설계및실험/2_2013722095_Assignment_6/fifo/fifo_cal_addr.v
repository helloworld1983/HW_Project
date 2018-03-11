// 현재 register의 상태정보를 받아 다음의 상태정보를 계산하는 module
module fifo_cal_addr(state, head, tail, data_count, we,re,next_head, next_tail, next_data_count); 
input [2:0] state, head, tail;
input [3:0] data_count;
output reg we, re;
output reg [2:0] next_head, next_tail;
output reg [3:0] next_data_count;
// parameter setting
parameter IDLE = 3'b000;
parameter WRITE = 3'b001;
parameter READ = 3'b010;
parameter WR_ERROR = 3'b011;
parameter RD_ERROR = 3'b100;

// 각 state에 대하여 다음과 같이 setting 함.
always @(state, data_count, head, tail)
begin
	case(state)
		IDLE : 
		begin 
			next_data_count <= data_count;
			next_head <= head;
			next_tail <= tail;
			we <= 0;
			re <= 0;
		end
		WRITE: // data가 입력되었으므로 
		begin
			next_data_count <= data_count + 1'b1; // data count를 1 증가시키고 
			next_head <= head;
			next_tail <= tail + 3'h1;	// tail pointer를 새로운 data가 입력된 register로 옮긴 뒤
			we <= 1;							// we signal을 1로 setting
			re <= 0;
		end
		READ:	// data가 출력되었으므로 
		begin
			next_data_count <= data_count - 1'b1;	// data count를 1 감소시키고 
			next_head <= head + 3'h1;					// head pointer를 다음 register로 옮긴 뒤
			we <= 0;
			re <= 1;											// re signal을 1로 setting
		end
		// 아래의 두 case에서는 상태정보가 바뀌지 않음.
		WR_ERROR:
		begin 
			next_data_count <= data_count;
			next_head <= head;
			next_tail <= tail;
			we <= 0;
			re <= 0;
		end
		RD_ERROR:
		begin 
			next_data_count <= data_count;
			next_head <= head;
			next_tail <= tail;
			we <= 0;
			re <= 0;
		end
		default: ;
	endcase
end
endmodule
