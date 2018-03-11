module DMAC_master(clk, reset_n, opstart, opdone, opdone_clear, source_addr, dest_addr, data_size, data_count, rd_en, M_req, M_grant, M_wr, M_address, M_dout,M_din, opmode);
// input value 
input clk, reset_n, M_grant, opdone_clear, opstart;
input [7:0] source_addr, dest_addr, data_size;
input [3:0] data_count;
input [31:0] M_din;
input [2:0] opmode;
// output value 
output reg opdone, rd_en ;
output reg M_req, M_wr;
output reg [7:0] M_address;
output reg [31:0] M_dout;

/* state parameter */
parameter IDLE 			= 3'b000;
parameter FIFO_POP 		= 3'b001;
parameter BUS_REQUEST 	= 3'b010;
parameter MEMORY_READ 	= 3'b011;
parameter MEMORY_WRITE 	= 3'b100;
parameter DONE 			= 3'b101;

reg [2:0] state, next_state;
reg [7:0] master_source, next_master_source, master_destaddr, next_master_destaddr, master_datasize, next_master_datasize;

/* move state accoding to reset_n*/
always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 0) // if reset_n =0, keep state in IDLE
		state <= IDLE;
	else if (reset_n == 1) // if reset_n = 1, pass next state value to state 
		state <= next_state;
	else				// default case 
		state = 3'bx;
end

// decide next state accoding to current state, opstart, opdone_clear, next_master_datasize, data_count, m_grant 
always@(state, opstart, opdone_clear, next_master_datasize, data_count, M_grant) 
begin
	case(state)
		{IDLE}: // if current state is IDLE 
		begin
			if(opstart == 1) next_state = FIFO_POP; // if opstart is 1, move to FIFO_POP state
			else					next_state = IDLE;	 // else keep current state 
		end
		
		
		{FIFO_POP}:	// if current state is FIFO_POP
		begin
			if(next_master_datasize != 8'h0) // if next_master's datasize isn't equal with 0
				next_state = BUS_REQUEST;		// move to BUS_REQUEST state 
			
			else 										// if next_master's datasize is equal with 0
			begin
				if(data_count == 4'h0)			// if current data_count is 0
					next_state = DONE;			// move to DONE state
				else									// in else case, move to FIFO_POP state 
					next_state = FIFO_POP;	
			end
		end
		
		
		{BUS_REQUEST}:	// if current state is BUS_REQUEST 
		begin
			if(M_grant == 1)				
				next_state = MEMORY_READ; // if M_grant ==1, move to MEMORY_READ state
			else								
				next_state = BUS_REQUEST;	// in else case, keep current state
		end
		
		
		{MEMORY_READ}: // if current state is MEMORY_READ, move to MEMORY_WRITE state
		begin	next_state = MEMORY_WRITE; end

		
		
		{MEMORY_WRITE}: // if current state is MEMORY_WRITE
		begin
			if(next_master_datasize != 8'h0) // if next master's datasize isn't equal with 0
				next_state = MEMORY_READ;		// move to MEMORY_READ state
			else 
			
			begin
				if(data_count == 4'h0)			// if current data count is 0
					next_state = DONE;			// move to DONE state 
				else 
					next_state = FIFO_POP;	// in else case, move to FIFO_POP state
			end
		end

		
		{DONE}:			// if current state is DONE
		begin
			if(opdone_clear == 1) 	// if opdone_clear = 1, move to IDLE state
				next_state = IDLE;
			else						 
				next_state = DONE;	// if opdone_clear == 0, keep current state 
		end
		
		
		default: next_state = 3'bx;
	endcase
end


/* decide source_addr, data_size, destaddr*/
always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 0)//if reset_n is 0, set under values to 0
	begin
		master_source = 8'h0;
		master_destaddr = 8'h0;
		master_datasize = 8'h0;
	end
	
	else if (reset_n == 1)//if reset_n is 0, pass next valus to its parent register 
	begin
		master_source = next_master_source;
		master_destaddr = next_master_destaddr;
		master_datasize = next_master_datasize;
	end
	
	else // default case, set under values to 0
	begin
		master_source = 8'h0;
		master_destaddr = 8'h0;
		master_datasize = 8'h0;
	end
end


/* set next vaules accoding to current state */
always@(state, source_addr, master_source, dest_addr, master_destaddr, data_size, master_datasize, opmode)
begin
	case(state)
	{FIFO_POP}:	// if current state is POP, pass current values to each next registers
	begin
		next_master_source  	= source_addr;
		next_master_destaddr = dest_addr;
		next_master_datasize = data_size;
	end
	
	
	{BUS_REQUEST}:	// if current state is BUS_REQUEST, keep under current values
	begin
		next_master_source = master_source;
		next_master_destaddr = master_destaddr;
		next_master_datasize = master_datasize;
	end
	
	
	{MEMORY_READ}: // if current state is MEMORY_READ
	begin
		next_master_source = master_source;		// keep master_source, master_desaddr
		next_master_destaddr = master_destaddr;
		next_master_datasize = master_datasize - 8'b00000001; // minus 1 at master datasize
	end
	
	
	{MEMORY_WRITE}:// if current state is MEMORY_WRITE
	begin
		if(opmode[2] == 1) // if opmode[2] == 1, zero initialize(set destination address to 0)
		begin
			if(opmode[0] == 1)
			begin
				next_master_source = master_source + 8'b00000001; // plus 1 at master_source 
				next_master_destaddr = 0;
			end
			else
			begin
				next_master_source = master_source; // in else case keep master_source
				next_master_destaddr = 0;
			end
		end
		else // opmode[2] != 1
		begin
			if((opmode[1] == 0) && (opmode[0] == 1)) // source address increment mode
			begin 
				next_master_source = master_source + 8'b00000001; // plus 1 at master_source
				next_master_destaddr = master_destaddr;				// keep master_desaddr
			end
			else if((opmode[1] == 1) && (opmode[0] == 0)) // destination address increment mode
			begin 
				next_master_source = master_source;					// keep master_source
				next_master_destaddr = master_destaddr+ 8'b00000001; // plus 1 at master_destaddr
			end
			else if((opmode[1] == 1) && (opmode[0] == 1)) // destination, source address increment mode
			begin 
				next_master_source = master_source + 8'b00000001;		// plus 1 at master_source, master_desaddr
				next_master_destaddr = master_destaddr+ 8'b00000001;
			end
			else // ((opmode[1] == 0) && (opmode[0] == 0))-> keep master_source, master_destaddr 
			begin
				next_master_source = master_source;
				next_master_destaddr = master_destaddr;
			end
		end
	next_master_datasize = master_datasize; // keep master_datasize 
	end
	
	default : // in default case, set under registers to 0
	begin
		next_master_source = 8'h0;
		next_master_destaddr = 8'h0;
		next_master_datasize = 8'h0;
	end
	endcase
end
/* rd_en setting */
always@(next_state)
begin
	case(next_state)
		{IDLE} : rd_en = 1'b0;
		{FIFO_POP} : rd_en = 1'b1; // when next state is FIFO_POP, set read enable to 1
		{BUS_REQUEST} : rd_en = 1'b0;
		{MEMORY_READ} : rd_en = 1'b0;
		{MEMORY_WRITE} : rd_en = 1'b0;
		{DONE}	: rd_en = 1'b0;
		default : rd_en = 1'b0;
	endcase
end

/*MASTER output values setting*/
always@(state, master_source, master_destaddr, master_datasize, M_din)
begin
	case(state)
		{IDLE}: // in IDLE state 
		begin
			M_req 		= 1'b0;
			M_wr			= 1'b0;
			M_dout		= 32'b0;
			M_address 	= 8'b0;
			opdone 		= 1'b0;
		end
		{FIFO_POP}: // in FIFO_POP state 
		begin
			M_req 		= 1'b0;
			M_wr			= 1'b0;
			M_dout		= 32'b0;
			M_address 	= 8'b0;
			opdone 		= 1'b0;
		end
		{BUS_REQUEST}: // in BUS_REQUEST state
		begin
			M_req 		= 1'b1;	// set M_req to 1
			M_wr			= 1'b0;
			M_dout		= 32'b0;
			M_address 	= 8'b0;
			opdone 		= 1'b0;
		end
		{MEMORY_READ}:	// in MEMORY_READ state 
		begin
			M_req 		= 1'b1;	// set M_req to 1
			M_wr			= 1'b0;
			M_dout		= 32'b0;
			M_address 	= master_source; // pass master_source to M_address 
			opdone 		= 1'b0;
		end
		{MEMORY_WRITE}:  // in MEMORY_WRITE state 
		begin
			M_req 		= 1'b1;	// set M_req to 1
			M_wr			= 1'b1;	// set M_wr to 1
			M_dout		= M_din;	// pass M_din to M_dout 
			M_address 	= master_destaddr; // pass master_destaddr to M_address 
			opdone 		= 1'b0;
		end
		{DONE}: // in DONE state
		begin
			M_req 		= 1'b0;
			M_wr			= 1'b0;
			M_dout		= 32'b0;
			M_address 	= 8'b0;
			opdone 		= 1'b1; // set opdone to 1
		end
		default:		// in default case, set under values to 0
		begin
			M_req 		= 1'b0;
			M_wr			= 1'b0;
			M_dout		= 32'b0;
			M_address 	= 8'b0;
			opdone 		= 1'b0;
		end
	endcase
end
endmodule
