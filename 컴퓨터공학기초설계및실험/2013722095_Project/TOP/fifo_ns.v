module fifo_ns(wr_en, rd_en, state, data_count, next_state); // next state module 
// input value
input wr_en, rd_en;
input [2:0] state;
input [3:0] data_count;

// output value 
output [2:0] next_state;

// inner register 
reg [2:0] next_state;

// parameter setting
parameter IDLE = 3'b000;
parameter WRITE = 3'b001;
parameter READ = 3'b010;
parameter WR_ERROR = 3'b011;
parameter RD_ERROR = 3'b100;

// set next state accding to current state, data count, wr_en, rd_en .
always @(wr_en,rd_en, state, data_count)
begin
	next_state = 0;
	case(state)
		// when current state is IDLE
		(IDLE):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count < 4'h8))	next_state <= WRITE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count == 4'h8))	next_state <= WR_ERROR;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count > 4'h0))	next_state <= READ;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count == 4'h0))	next_state <= RD_ERROR;					
					else;
					end
		// when current state is READ
		(READ):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count < 4'h8))	next_state <= WRITE;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count > 4'h0))	next_state <= READ;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count == 4'h0))	next_state <= RD_ERROR;
					else;
					end
		// when current state is RD_ERROR
		(RD_ERROR):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
						else if ((wr_en == 1)&&(rd_en == 0)&&(data_count < 4'h8))	next_state <= WRITE;
						else if ((wr_en == 0)&&(rd_en == 1)&&(data_count == 4'h0))	next_state <= RD_ERROR;					
						else;
						end
		// when current state is WR_ERROR
		(WR_ERROR):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
						else if ((wr_en == 0)&&(rd_en == 1)&&(data_count > 4'h0))	next_state <= READ;
						else if ((wr_en == 0)&&(rd_en == 1)&&(data_count == 4'h0))	next_state <= RD_ERROR;					
						else;
						end
		(WRITE):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count < 4'h8))	next_state <= WRITE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count == 4'h8))	next_state <= WR_ERROR;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count > 4'h0))	next_state <= READ;
					else;
					end
		default: begin end // default case 
	endcase
end
endmodule
