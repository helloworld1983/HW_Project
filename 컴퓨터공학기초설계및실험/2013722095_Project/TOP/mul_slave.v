module mul_slave(clk, reset_n, S_sel, S_wr, S_address, S_din, S_dout, m_interrupt, opstart, opclear, opdone, result, 
								multiplicand, multiplier, cand_we, lier_we, master_state, rAddr); // multiplier slave module
// intput  value
	input clk, reset_n, S_sel, S_wr;
	input [7:0]	S_address;
	input [31:0]	S_din;
	input [31:0]	result;
	input opdone;
	input [2:0] master_state;
// output value
	output [3:0] rAddr;
	output [31:0]	S_dout;
	output m_interrupt;
	output opstart, opclear;
	output [31:0]	multiplicand, multiplier;
	output 	cand_we, lier_we;
// inner register
	reg [3:0]	RADDR, NEXT_RADDR;
	reg			CAND_WE, NEXT_CAND_WE;
	reg			LIER_WE, NEXT_LIER_WE;
	reg [31:0]	OPERATION_START, NEXT_OPERATION_START;
	reg [31:0]	OPERATION_DONE, NEXT_OPERATION_DONE;
	reg [31:0]	OPERATION_CLEAR, NEXT_OPERATION_CLEAR;
	reg [31:0]	MULTIPLICAND, NEXT_MULTIPLICAND;
	reg [31:0]	MULTIPLIER, NEXT_MULTIPLIER;
	reg [31:0]	INTERRUPT_ENABLE, NEXT_INTERRUPT_ENABLE;
	reg [31:0]	RESULT, NEXT_RESULT;
	reg [31:0]	S_dout, next_S_dout;
// assign output values
	assign opstart = OPERATION_START[0];
	assign opclear = OPERATION_CLEAR[0];
	assign m_interrupt = (INTERRUPT_ENABLE[0] == 1'b1)?opdone:1'b0;
	assign multiplicand = MULTIPLICAND;
	assign multiplier = MULTIPLIER;
	assign cand_we = CAND_WE;
	assign lier_we = LIER_WE;
	assign rAddr = RADDR;
	
	always @ (posedge clk or negedge reset_n) // set registers when clk is rising or reset_n is falling
	begin
		if(reset_n == 0) // reset_n case, set registers to 0
			begin
				CAND_WE <= 1'b0;
				LIER_WE <= 1'b0;
				OPERATION_START <= 32'b0;
				OPERATION_DONE <= 32'b0;
				OPERATION_CLEAR <= 32'b0;
				MULTIPLICAND <= 32'b0;
				MULTIPLIER <= 32'b0;
				INTERRUPT_ENABLE <= 32'b0;
				RESULT <= 32'b0;
				S_dout <= 32'b0;
			end
			else if(reset_n == 1 && opclear == 1) // reset_n == 1, opclear == 1 case, set registers to 0
			begin
				OPERATION_START <= 32'b0;
				OPERATION_DONE <= 32'b0;
				OPERATION_CLEAR <= 32'b0;
				MULTIPLICAND <= 32'b0;
				MULTIPLIER <= 32'b0;
				INTERRUPT_ENABLE <= 32'b0;
				RESULT <= 32'b0;
			end
		else if(reset_n == 1) // reset_n = 1 opclear = 0 case , insert next_registers to its parent registers
			begin
				CAND_WE <= NEXT_CAND_WE;
				LIER_WE <= NEXT_LIER_WE;
				OPERATION_START[0] <= NEXT_OPERATION_START[0];
				OPERATION_DONE[0] <= NEXT_OPERATION_DONE[0];
				OPERATION_CLEAR[0] <= NEXT_OPERATION_CLEAR[0];
				MULTIPLICAND <= NEXT_MULTIPLICAND;
				MULTIPLIER <= NEXT_MULTIPLIER;
				INTERRUPT_ENABLE[0] <= NEXT_INTERRUPT_ENABLE[0];
				RESULT <= NEXT_RESULT;
				S_dout <= next_S_dout;
			end
		
		else begin end
	end
	// read address control
	always @ (posedge clk or negedge reset_n)
		begin
			if(reset_n == 1'b0)	// reset = 0 -> Initialize to 0
				begin
					RADDR = 4'b0;
				
				end
			else //if(reset_n == 1'b1), pass NEXT_RADDR to RADDR
				begin
					RADDR = NEXT_RADDR;
				end
		end 
	
	// Offset = 0x0 / MULTIPLICAND / offset = 0x1 / MULTIPLIER 
	always @ (S_sel, S_wr, S_address, S_din, MULTIPLICAND, MULTIPLIER, master_state)
	begin
		// initialize next_registers to 0
		NEXT_MULTIPLICAND = 0;
		NEXT_MULTIPLIER	= 0;
		NEXT_CAND_WE 		= 0;
		NEXT_LIER_WE 		= 0;
		
		if( (S_sel == 1'b1) && (S_wr == 1'b1) && (master_state == 3'b000)) // if master_state is IDLE and sel, wr is 0
		begin
			if(S_address[3:0] == 4'h0) // if offset is 0
			begin
				NEXT_MULTIPLICAND <= S_din; // pass S_din's value to NEXT_MULTIPLICAND
				NEXT_CAND_WE <= 1'b1;		  // pass 1 to NEXT_CAND_WE
			end
			if(S_address[3:0] == 4'h1) // if offset is 1
			begin
				NEXT_MULTIPLIER <= S_din; // pass S_din's value to NEXT_MULTIPLIER
				NEXT_LIER_WE <= 1'b1;		// pass 1 to NEXT_LIER_WE
			end
		end
		
		else										// else case
			begin
				NEXT_MULTIPLICAND <= MULTIPLICAND; // keep MULTIPLCAND value
				NEXT_CAND_WE <= 1'b0;					// set CAND_WE and LIER_WE to 0
				NEXT_LIER_WE <= 1'b0;
			end
	end
	
	//  Offset = 0x2 / RESULT 
	always @ (S_sel, S_wr, S_address, RADDR, RESULT, result)
	begin
		// initialize NEXT_registers to 0
		NEXT_RESULT = 0;
		NEXT_RADDR	= 0;
		if( (S_sel == 1'b1) && (S_wr == 1'b0) && (S_address[3:0] == 4'h2) ) // if sel, wr is 1 and offset is 2
			begin
				NEXT_RESULT = result;							// pass result value to NEXT_RESULT
				NEXT_RADDR = RADDR + 4'b1;						// plus 1 at Raddr
			end
		else		
			begin
				NEXT_RESULT = RESULT;							// in else case, keep RESULT value and RADDR value
				NEXT_RADDR = RADDR;
			end
	end
	
	//  Offset = 0x3 : INTERRUPT_ENABLE
	always @ (S_sel, S_wr, S_address, S_din, INTERRUPT_ENABLE)
	begin
		if( (S_sel == 1'b1) && (S_wr == 1'b1) && (S_address[3:0] == 4'h3) )	
			NEXT_INTERRUPT_ENABLE[0] <= S_din[0];
		else		
			NEXT_INTERRUPT_ENABLE[0] <= INTERRUPT_ENABLE[0];
	end
	
	
	//Offset = 0x4 / OPERATION_START
	always@(S_sel, S_wr, S_address, S_din, opdone)
	begin
		// sel = 1, wr = 1, S_din[0] = 1, opdone = 0 and offset is 4
		if( (S_sel == 1'b1) && (S_wr == 1'b1) && (S_address[3:0] == 4'h4) && (S_din[0] == 1'b1) && (opdone == 1'b0)) 
			NEXT_OPERATION_START[0] <= 1'b1; // set NEXT_OPERATION_START[0] to 1
		else									// in else case 
			NEXT_OPERATION_START[0] <= 1'b0;  // set NEXT_OPERATION_START[0] to 0
	end
	
	
	//Offset = 0x5 / OPERATION_CLEAR
	always@(S_sel, S_wr, S_address, S_din, opdone)
	begin
		// if sel, wr = 1, S_din[0] = 1, opdone = 1 and offset is 5
		if( (S_sel == 1'b1) && (S_wr == 1'b1) && (S_address[3:0] == 4'h5) && (S_din[0] == 1'b1) && (opdone == 1'b1) ) 
			NEXT_OPERATION_CLEAR[0] <= 1'b1; // set NEXT_OPERATION_CLEAR[0] to 1
		else 								// in else case 
			NEXT_OPERATION_CLEAR[0] <= 1'b0;// set NEXT_OPERATION_CLEAR[0] to 0
		
	end

	
	// Offset = 0x6 / OPERATION_DONE
	always @ (S_sel, S_wr, S_address, OPERATION_DONE)
	begin
		if( (S_sel == 1'b1) && (S_wr == 1'b0) && (S_address[3:0] == 4'h6))// if sel=1.  wr = 0 and offset is 6
			NEXT_OPERATION_DONE[0] <= 1'b1;							// set NEXT_OPERATION_DONE[0] to 1
		else																 	// in else case 
			NEXT_OPERATION_DONE[0] <= OPERATION_DONE[0];			// keep OPERATION_DONE's value
	end
	
	// print S_dout by offset
	always @ (S_sel, S_wr, S_address, opstart, opclear, multiplicand, multiplier, NEXT_RESULT, INTERRUPT_ENABLE, OPERATION_DONE)
	begin
		next_S_dout = 0;
		if((S_sel == 1'b1) && (S_wr == 1'b0))	// S_sel = 1 , S_wr = 0
		begin 
			if		 (S_address[3:0]==4'h0)  next_S_dout = {multiplicand};
			else if(S_address[3:0]==4'h1)  next_S_dout = {multiplier};
			else if(S_address[3:0]==4'h2)  next_S_dout = {NEXT_RESULT};
			else if(S_address[3:0]==4'h3)  next_S_dout = {31'b0, INTERRUPT_ENABLE[0]};
			else if(S_address[3:0]==4'h4)  next_S_dout = {31'b0, opstart};
			else if(S_address[3:0]==4'h5)  next_S_dout = {31'b0, opclear};
			else if(S_address[3:0]==4'h6)  next_S_dout = {31'b0, OPERATION_DONE[0]};
			else next_S_dout = 32'b0;   
		end 
		else	  next_S_dout = 32'b0;  // default case 
	end	
endmodule

	