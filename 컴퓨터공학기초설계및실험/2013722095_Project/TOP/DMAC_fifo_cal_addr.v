// recieve current register's status and then calculate next status
module DMAC_fifo_cal_addr(state, head, tail, data_count, we,re,next_head, next_tail, next_data_count); 
// input value 
input [2:0] state, head, tail;
input [3:0] data_count;
// output  value 
output reg we, re;
output reg [2:0] next_head, next_tail;
output reg [3:0] next_data_count;
// parameter setting
parameter IDLE = 3'b000;
parameter WRITE = 3'b001;
parameter READ = 3'b010;
parameter WR_ERROR = 3'b011;
parameter RD_ERROR = 3'b100;

// In each case of current state, set registers like under that
always @(state, data_count, head, tail)
begin
	next_data_count = 0;
	next_head =0;
	next_tail =0;
	we =0;
	re =0;
	case(state)
		IDLE :  // if current state is IDLE
		begin 
			next_data_count <= data_count;// keep data count, head, tail
			next_head <= head;
			next_tail <= tail;
			we <= 0;						// set write enable, read enable to 0
			re <= 0;
		end
		WRITE:  // if current state is WRITE
		begin
			next_data_count <= data_count + 1'b1; //plus 1 at current data count
			next_head <= head;			// keep head
			next_tail <= tail + 3'h1;	// plus 1 at tail
			we <= 1;							// set write enable to 1
			re <= 0;
		end
			READ:	// if current state is READ
		begin
			next_data_count <= data_count - 1'b1;	// minus 1 at data count
			next_head <= head + 3'h1;					// plus 1 at head, keep tail
			we <= 0;
			re <= 1;											// set read enable to 1
		end
		// if current state is WR_ERROR or RD_ERROR, keep current values
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
		default: begin end // default case 
	endcase
end
endmodule
