module fifo_out(state, data_count, full, empty, wr_ack, wr_err, rd_ack, rd_err); // full, empty, handshake signal을 출력하는 module
input [2:0] state;
input [3:0] data_count;
output reg full, empty, wr_ack, wr_err, rd_ack, rd_err;
// parameter setting
parameter IDLE = 3'b000;
parameter WRITE = 3'b001;
parameter READ = 3'b010;
parameter WR_ERROR = 3'b011;
parameter RD_ERROR = 3'b100;
// full, empty setting
always @(data_count)
	begin
		if(data_count == 4'h0) // data가 없는경우 
		begin
			full <= 1'b0;			// full =0. empty =1로 setting
			empty <= 1'b 1;
		end
		
		else if(data_count == 4'h8)// register에 data가 가득찼을 경우 
		begin
			full <= 1'b1;				// full = 1, empty =0로 setting
			empty <= 1'b0;
		end
		else								// 그 외의 경우 
		begin
			full <= 1'b0;				// full, empty = 0
			empty <= 1'b0;
		end
	end

// handshake setting	
always@(state)
	begin
		if(state == IDLE)	// state가 IDLE일 때 
		begin
			wr_ack <= 0;
			wr_err <= 0;
			rd_ack <= 0;
			rd_err <= 0;
		end
		else if(state == WRITE)	// state가 WRITE일 때 
		begin
			wr_ack <= 1;
			wr_err <= 0;
			rd_ack <= 0;
			rd_err <= 0;
		end
		else if(state == READ)	// state가 READ일 때
		begin
			wr_ack <= 0;
			wr_err <= 0;
			rd_ack <= 1;
			rd_err <= 0;
		end
		else if(state == WR_ERROR)	// state가 WR_ERROR일 때
		begin
			wr_ack <= 0;
			wr_err <= 1;
			rd_ack <= 0;
			rd_err <= 0;
		end
		else								// state가 RD_ERROR일 때
		begin
			wr_ack <= 0;
			wr_err <= 0;
			rd_ack <= 0;
			rd_err <= 1;
		end
	end
endmodule

