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
		if(data_count == 4'h0) // if fifo is empty
		begin
			full <= 1'b0;			// full =0. empty =1
			empty <= 1'b 1;
		end
		
		else if(data_count == 4'h8)// if fifo is full
		begin
			full <= 1'b1;				// full = 1, empty =0
			empty <= 1'b0;
		end
		else								//in else case 
		begin
			full <= 1'b0;				// full, empty = 0
			empty <= 1'b0;
		end
	end

// handshake setting	
always@(state)
	begin
		if(state == IDLE)	// if current state is IDLE
		begin
			wr_ack <= 0;
			wr_err <= 0;
			rd_ack <= 0;
			rd_err <= 0;
		end
		else if(state == WRITE)	// if current state is WRITE
		begin
			wr_ack <= 1;			// set wr_ack to 1
			wr_err <= 0;
			rd_ack <= 0;
			rd_err <= 0;
		end
		else if(state == READ)	// if current state is READ
		begin
			wr_ack <= 0;
			wr_err <= 0;
			rd_ack <= 1;			// set rd_ack to 1
			rd_err <= 0;
		end
		else if(state == WR_ERROR)	// if current state is WR_ERROR
		begin
			wr_ack <= 0;
			wr_err <= 1;				// set wr_err to 1
			rd_ack <= 0;
			rd_err <= 0;
		end
		else								// if current state is RD_ERROR
		begin
			wr_ack <= 0;
			wr_err <= 0;
			rd_ack <= 0;
			rd_err <= 1;				// set rd_err to 1
		end
	end
endmodule

