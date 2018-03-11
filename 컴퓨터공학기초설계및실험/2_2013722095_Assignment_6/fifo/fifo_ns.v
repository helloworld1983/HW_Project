module fifo_ns(wr_en, rd_en, state, data_count, next_state); // 현재 STATE와 WRITE, READ ENABLE을 입력으로 받아 다음 STATE를 계산하는 MODULE
input wr_en, rd_en;
input [2:0] state;
input [3:0] data_count;
output [2:0] next_state;
reg [2:0] next_state;
// parameter setting
parameter IDLE = 3'b000;
parameter WRITE = 3'b001;
parameter READ = 3'b010;
parameter WR_ERROR = 3'b011;
parameter RD_ERROR = 3'b100;

// 현재 state에서 data_count, wr_en, rd_en을 기준으로하여 아래와 같이 next_state를 설정함.
always @(wr_en,rd_en, state, data_count)
begin
	case(state)
		(IDLE):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count < 4'h8))	next_state <= WRITE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count == 4'h8))	next_state <= WR_ERROR;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count > 4'h0))	next_state <= READ;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count == 4'h0))	next_state <= RD_ERROR;					
					else;
					end
					
		(READ):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
					else if ((wr_en == 1)&&(rd_en == 0)&&(data_count < 4'h8))	next_state <= WRITE;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count > 4'h0))	next_state <= READ;
					else if ((wr_en == 0)&&(rd_en == 1)&&(data_count == 4'h0))	next_state <= RD_ERROR;
					else;
					end
					
		(RD_ERROR):	begin if((wr_en == 0)&&(rd_en == 0)) 								next_state <= IDLE;
						else if ((wr_en == 1)&&(rd_en == 0)&&(data_count < 4'h8))	next_state <= WRITE;
						else if ((wr_en == 0)&&(rd_en == 1)&&(data_count == 4'h0))	next_state <= RD_ERROR;					
						else;
						end
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
	endcase
end
endmodule
