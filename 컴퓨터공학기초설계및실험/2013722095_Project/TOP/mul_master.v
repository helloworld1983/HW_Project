// multiply module
module mul_master(reset_n, clk, multiplier, multiplicand, op_start, op_clear, op_done, cand_empty, lier_empty, result, state, wr_en, waddr, rd_en); // top module
//input values
input [31:0] 	multiplier, multiplicand;
input				op_start, op_clear, reset_n, clk;
input 			cand_empty, lier_empty;

// output values
output reg 				op_done;
output reg 	[2:0] 	state;
output reg 	[31:0] 	result;
output reg				wr_en, rd_en;
output 		[3:0]		waddr;
//inner register
reg 			next_x1,x1;
reg [31:0] 	u, next_u, next_v, v, next_x, x;
reg [31:0] 	after;
reg [5:0] 	next_count, count;
reg [2:0] 	next_state;

reg [31:0]	MULTIPLICAND, MULTIPLIER;
reg [31:0]	NEXT_MULTIPLICAND, NEXT_MULTIPLIER;
reg [3:0]	WADDR, NEXT_WADDR;


/* state parameter */
parameter IDLE		= 3'b000;
parameter POP 		= 3'b001;
parameter EXEC 	= 3'b010;
parameter WRITE_B	= 3'b011;
parameter WRITE_F = 3'b100;
parameter DONE		= 3'b101;

// assign output signal
assign waddr = WADDR;

// choose keep current state or move to next state accoding to reset_ n 
always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 1'b0) // if reset_n is 0, initialize under values to 0
	begin
		MULTIPLICAND 	<= 0;
		MULTIPLIER		<= 0;
		state				<= IDLE;
		u 					<= 32'b0;
		v					<= 32'b0;
		x					<= 32'b0;
		x1					<= 1'b0;
		after				<= 32'b0;
		count				<= 6'b0;
		WADDR				<= 0;
	end
	
	else				// if reset_n =  1, pass next values to each parent values
	begin
		state 			<= next_state;
		MULTIPLICAND 	<= NEXT_MULTIPLICAND;
		MULTIPLIER		<= NEXT_MULTIPLIER;
		u					<= next_u;
		v 					<= next_v;
		x					<= next_x;
		x1					<= next_x1;
		after				<= 32'b0;
		count				<=	next_count;
		WADDR				<= NEXT_WADDR;
	end
		
end

// decide next state accding to current state, opstart signal, opclear signal, count value
always@(op_start , op_clear , state , count, cand_empty, lier_empty, multiplicand, multiplier)
	begin
	next_state = 2'b00;
	case(state)
	{IDLE}://if current state is IDLE
		begin
			if	(op_clear == 1'b1) // if opclear = 1, move to IDLE state
				begin next_state <= IDLE; end
			else if(op_start ==1'b1 && op_clear == 1'b0) //op_start signal = 1, move to POP state
				begin next_state <= POP; end
			else				// in else case, move to IDLE state
				begin next_state <= IDLE; end
		end
		
		
	{POP}: //if current state is POP
		begin
			if(op_clear == 1) // if opclear = 1, move to IDLE state
				next_state <= IDLE;
			else
			begin // if opclear = 0
				// if cand fifo, lier fifo is empty, and multiplicand or multiplier value is 0
				if(cand_empty == 1 && lier_empty == 1 && (multiplier == 0 || multiplicand == 0))
					next_state = DONE; // move to DONE state
				else
					next_state = EXEC; // in else case move to EXEC state
			end
		end
		
		
		
	{EXEC}:// if current state is EXEC
		begin
			if(op_clear == 1'b1) // if opclear is 1
				begin next_state <= IDLE; end	// move to IDLE state
			else 
			begin
				if(op_clear == 1'b0 && count < 6'b100000) //if calculation isn't over 
				begin next_state <= EXEC; end	// keep EXEC state
				
				else// if calculation is over 
				begin next_state <= WRITE_B; end	// move to WRITE_B state
			
			end	
		end
	
		
	{DONE}:// if current state is DONE, keep DONE state until opclear rise up
		begin
			if(op_clear ==1'b1)
				begin next_state <= IDLE; end
			else 
				begin next_state <= DONE; end 
		end
		
	{WRITE_B}:// if current state is WRITE_B, move to WRITE_F state
		begin
			next_state <= WRITE_F;
		end
	{WRITE_F}: // if current state is WRITE_F, move to POP state
		begin
			next_state <= POP;
		end
		
		
	
	default: begin next_state <= 2'bx; end // default case 
	endcase
	end


	// calculate result accoding to x, x1(x-1), u, v
always@(multiplicand , multiplier ,state, x , x1 , u , v , count, MULTIPLICAND, MULTIPLIER)
begin
	// initialize next values to 0
		next_x = 0;	
		next_x1= 0;	
		next_u = 0;	
		next_v = 0;
		after = 0;
		next_count = 0;
		NEXT_MULTIPLICAND = 0;
		NEXT_MULTIPLIER = 0;
	if(count == 6'b100000) //if calculation is over, keep its values
	begin
		NEXT_MULTIPLICAND = MULTIPLICAND;
		NEXT_MULTIPLIER   = MULTIPLIER;
		next_x = x;	
		next_x1= x1;	
		next_u = u;	
		next_v = v;
		next_count = count;
	end
	
	case(state)
		{IDLE}:// if current state is IDLE, initialize under values to 0
		begin
			NEXT_MULTIPLICAND = 0;
			NEXT_MULTIPLIER   = 0;
			next_x = 32'h0;	
			next_x1= 1'b0;	
			next_u = 32'h0;
			next_v = 32'h0;
			next_count = 6'b0;
			after = 0;
		end
		
		
		{POP}: // if current state is POP, set next values like under that
		begin
			NEXT_MULTIPLICAND = multiplicand;
			NEXT_MULTIPLIER	= multiplier;
			next_x 				= multiplier;	
			next_x1				= 1'b0;	
			next_u 				= 32'h0;
			next_v 				= 32'h0;
			next_count 			= 6'b0;
			after					= 0;
		end
		
		
		{EXEC}:// if current state is EXEC
		begin
			if(count < 6'b100000) // if calculation is not over 
			begin
				NEXT_MULTIPLICAND = MULTIPLICAND; // keep MULTIPLICAND, MULTIPLIER values
				NEXT_MULTIPLIER	= MULTIPLIER;
				
				if({x[0],x1} == 2'b01)	// add and shift
					after = u + MULTIPLICAND;				
				else if({x[0],x1} == 2'b10) // sub and shift
					after = u - MULTIPLICAND;
				else// 00 / 11 case
					after = u;

				next_v = {after[0],v[31:1]};			// pass u's lsb to v's msb
				next_u = {after[31], after[31:1]};		// arithmatic shift u 1bit
				next_x1 = x[0];						// pass x's lsb to x1
				next_x = {x[0], x[31:1]};			//  circular shift x 1bit
				next_count = count +6'b000001;				// count +1
			end			
		end
	
		{DONE}://if current state is DONE
		begin		
			NEXT_MULTIPLICAND = MULTIPLICAND; // keep MUTIPLIER, MULTIPLICAND
			NEXT_MULTIPLIER 	= MULTIPLIER;
			next_u = u;			// keep values
			next_v = v;
			next_x = x;
			next_x1 = x1;
			next_count = count;
		end
			{WRITE_B}: // if current state is WRITE_B , keep values like under that
		begin
			NEXT_MULTIPLICAND = MULTIPLICAND;
			NEXT_MULTIPLIER = MULTIPLIER;
			after = 0;
			next_u = u;
			next_v = v;
			next_x = x;
			next_x1 = x1;
			next_count = 0;
		end
		
		
		{WRITE_F}: // if current state is WRITE_F , keep values like under that
		begin
			NEXT_MULTIPLICAND = MULTIPLICAND;
			NEXT_MULTIPLIER = MULTIPLIER;
			after = 0;
			next_u = u;
			next_v = v;
			next_x = x;
			next_x1 = x1;
			next_count = 0;
		end
		
	
		
		default: // defualt case 
		begin
			next_x = 32'bx;	
			next_x1= 1'bx;	
			next_u = 32'hx;	
			next_v = 32'hx;			
			next_count = 6'bx;
		end
	endcase
end

always@(next_state) // when next state is POP, set read enable to 1
begin
	rd_en = 0;
	if(next_state == POP)
		rd_en = 1;
	else 
		rd_en = 0;
end

always@(state) // when current state is DONE, set opdone signal to 1
begin
	if(state == DONE)
		op_done = 1;
	else
		op_done = 0;
end


always@(state, next_state, u, v, WADDR)
begin
	result = 0;
	wr_en = 0;
	NEXT_WADDR = WADDR; // keep WADDR vaule
	if(state == WRITE_B) // if current state is WRITE_B
	begin
		wr_en = 1;			// set write enable to 1
		result = v;			// result output is low-lanking 32bits of result of calculating
		NEXT_WADDR = WADDR + 4'b1; // plus next waddr
	end
	
	
	if(state == WRITE_F) // if current state is WRITE_F
	begin
		wr_en = 1;				// set write enable to 1
		result = u;				// result output is high-lanking 32bits of result of calculating
		NEXT_WADDR = WADDR + 4'b1; // plus next waddr
	end
	
	if(state == POP) // if current state is POP
	begin
		if(next_state == DONE) // if next state is DONE
		begin
			wr_en = 0;				// set write enable, result to 0
			result = 0;
			NEXT_WADDR = WADDR;	// keep write address  
		end
		
		else // if current state is not POP
		begin
			wr_en = 1;				// set write enable to 1, result to 0
			result = 0;
			NEXT_WADDR = WADDR;	/// keep write address  
		end
	end
	
end


endmodule 