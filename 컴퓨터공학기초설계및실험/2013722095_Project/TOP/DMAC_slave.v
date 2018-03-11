module DMAC_slave(clk, reset_n, S_sel, S_wr, op_done, S_address, S_din, source_addr, dest_addr, datasize,interrupt, op_start, op_done_clear, wr_en, S_dout, opmode);
 // input value
 input clk, reset_n, S_sel, S_wr, op_done;
 input  [7:0] S_address;
 input [31:0] S_din;

 // output value 
 output [7:0] source_addr, dest_addr, datasize ;
 output  	  interrupt, op_start, op_done_clear, wr_en;
 output [31:0] S_dout;
 output [2:0] opmode;
 // inner register
  reg [31:0] OPERATION_START, next_OPERATION_START;
  reg [31:0] OPERATION_CLEAR, next_OPERATION_CLEAR;
  reg [31:0] INTERRUPT_ENABLE, next_INTERRUPT_ENABLE;
  reg [31:0] PUSH_DESCRIPTOR, next_PUSH_DESCRIPTOR;
  reg [7:0] SOURCE_ADDRESS, next_SOURCE_ADDRESS; 
  reg [7:0] DESTINATION_ADDRESS, next_DESTINATION_ADDRESS;
  reg [7:0] DATA_SIZE, next_DATA_SIZE; 
  reg [31:0] S_dout, next_S_dout;
  reg	[31:0]  DESCRIPTOR_SIZE, next_DESCRIPTOR_SIZE;
  reg [31:0]  OPMODE, next_OPMODE; //////
  
 assign op_start 			= OPERATION_START[0];
 assign op_done_clear	= OPERATION_CLEAR[0];
 assign interrupt 		= (INTERRUPT_ENABLE[0] == 1) ? op_done : 1'b0;
 assign wr_en 				= PUSH_DESCRIPTOR[0];
 assign source_addr 		= SOURCE_ADDRESS;
 assign dest_addr 		= DESTINATION_ADDRESS;
 assign datasize 			= DATA_SIZE;
 assign opmode 			= OPMODE[2:0];
 
 
 always@(posedge clk or negedge reset_n)
 begin
	if(reset_n == 0) // when reset_n = 0, set under values to 0
	begin
		OPERATION_START = 0;
		OPERATION_CLEAR = 0;
		INTERRUPT_ENABLE = 0;
		PUSH_DESCRIPTOR = 0;
		SOURCE_ADDRESS =0;
		DESTINATION_ADDRESS = 0;
		DATA_SIZE =0;
		S_dout = 0;
		DESCRIPTOR_SIZE =0;
		OPMODE =0;
	end
	else if(reset_n == 1) // when reset_n = 1, pass next values to its parent register 
	begin
		OPERATION_START[0] 			= next_OPERATION_START[0];
		OPERATION_CLEAR[0] 			= next_OPERATION_CLEAR[0];
		INTERRUPT_ENABLE[0] 			= next_INTERRUPT_ENABLE[0];
		PUSH_DESCRIPTOR[0] 			= next_PUSH_DESCRIPTOR[0];
		SOURCE_ADDRESS[7:0] 			= next_SOURCE_ADDRESS[7:0];
		DESTINATION_ADDRESS[7:0]	= next_DESTINATION_ADDRESS[7:0];
		DATA_SIZE[7:0] 				= next_DATA_SIZE[7:0];
		S_dout 							= next_S_dout;
		DESCRIPTOR_SIZE[3:0] 		= next_DESCRIPTOR_SIZE[3:0];
		OPMODE[2:0] 					= next_OPMODE[2:0];
	end	
	else // default case, set under registers to 0
	begin
		OPERATION_CLEAR 	= 0;
		OPERATION_START	= 0;
		INTERRUPT_ENABLE 	= 0;
		SOURCE_ADDRESS		= 0;
		PUSH_DESCRIPTOR	= 0;
		DATA_SIZE			= 0;
		DESTINATION_ADDRESS=0;
		DESCRIPTOR_SIZE	= 0;
		OPMODE				= 0;
		S_dout				= 0;
	end
 end
 // offset = 0x0 /  operation start
 always@(S_sel, S_address, S_wr, op_done, S_din)
 begin
	if((S_sel ==1) && (S_wr == 1) && (op_done == 0) && (S_address[3:0] == 4'b0) && (S_din[0] == 1'b1))
		next_OPERATION_START[0] = 1'b1;
	else
		next_OPERATION_START[0] = 1'b0;
 end
 // offset = 0x1 /  operation clear
 always@(S_sel, S_address, S_wr, op_done, S_din)
 begin
	if((S_sel ==1) && (S_wr == 1) && (op_done == 1) && (S_address[3:0] == 4'h1) && (S_din[0] == 1'b1))
		next_OPERATION_CLEAR[0] = 1'b1;
	else
		next_OPERATION_CLEAR[0] = 1'b0;
 end
 // offset = 0x2 / interrupt enable
 always@(S_sel, S_address, S_wr, INTERRUPT_ENABLE, S_din)
 begin
	if((S_sel == 1) && (S_address[3:0] == 4'h2) && (S_wr == 1))
		next_INTERRUPT_ENABLE[0] = S_din[0];
	else
		next_INTERRUPT_ENABLE[0] = INTERRUPT_ENABLE[0];
 end	
 // offset = 0x3 /  push descpiter
 always@(S_sel, S_address, S_wr, S_din)
 begin
	if((S_sel == 1) && (S_address[3:0] == 4'h3) && (S_wr == 1) && (S_din[0] == 1))
		next_PUSH_DESCRIPTOR[0] = 1'b1;
	else
		next_PUSH_DESCRIPTOR[0] = 1'b0;
 end
 // offset = 0x4 /  source address ( save source_addr)
 always@(S_sel, S_address, S_wr, S_din, SOURCE_ADDRESS)
 begin
	if((S_sel == 1) && (S_address[3:0] == 4'h4) && (S_wr == 1))
		next_SOURCE_ADDRESS = S_din[7:0];
	else
		next_SOURCE_ADDRESS = SOURCE_ADDRESS;	// in else case keep source address 
 end
 // offset = 0x5/  destination address
 always@(S_sel, S_address, S_wr, S_din, DESTINATION_ADDRESS)
 begin
	if((S_sel == 1)&&(S_address[3:0] == 4'h5)&&(S_wr == 1))
		next_DESTINATION_ADDRESS <= S_din[7:0];
	else
		next_DESTINATION_ADDRESS <= DESTINATION_ADDRESS; // in else case keep destination address 
 end
 // offset = 0x6 /  data size
 always@(S_sel, S_address, S_wr, S_din, DATA_SIZE)
 begin
	if((S_sel == 1)&&(S_address[3:0] == 4'h6)&&(S_wr == 1))
		next_DATA_SIZE <= S_din[7:0];
	else
		next_DATA_SIZE <= DATA_SIZE; //in else case keep data size
 end
 // offset = 0x7 /  desciptor size
 always@(S_sel, S_address, S_wr, S_din, DESCRIPTOR_SIZE)
 begin
	if((S_sel == 1)&&(S_address[3:0] == 4'h7)&&(S_wr == 1))
		next_DESCRIPTOR_SIZE[3:0] <= S_din[3:0];
	else
		next_DESCRIPTOR_SIZE[3:0] <= DESCRIPTOR_SIZE[3:0]; // in else case keep descriptor size 
 end
  // offset = 0x8 /  operation mode
 always@(S_sel, S_address, S_wr, S_din, DESCRIPTOR_SIZE, OPMODE)
 begin
	if((S_sel == 1)&&(S_address[3:0] == 4'h8)&&(S_wr == 1))
		next_OPMODE[2:0] <= S_din[2:0];
	else
		next_OPMODE[2:0] <= OPMODE[2:0]; // in else keep opmode 
 end
 
	// print output accoding to offset 
 always@(S_sel, S_wr, S_address, op_start, op_done_clear, OPERATION_START, OPERATION_CLEAR, INTERRUPT_ENABLE, PUSH_DESCRIPTOR, SOURCE_ADDRESS, DESTINATION_ADDRESS, DATA_SIZE, DESCRIPTOR_SIZE,OPMODE)
 begin
	if((S_sel == 1)&& (S_wr == 0))  // when S_sel = 1, S_wr = 0
	begin
		if(S_address[3:0] == 4'h0) next_S_dout = {31'b0, op_start};
		else if(S_address[3:0] == 4'h1) next_S_dout = {31'b0, op_done_clear};
		else if(S_address[3:0] == 4'h2) next_S_dout = {31'b0, INTERRUPT_ENABLE[0]};
		else if(S_address[3:0] == 4'h3) next_S_dout = {31'b0, PUSH_DESCRIPTOR[0]};
		else if(S_address[3:0] == 4'h4) next_S_dout = {24'b0, SOURCE_ADDRESS};
		else if(S_address[3:0] == 4'h5) next_S_dout = {24'b0, DESTINATION_ADDRESS};
		else if(S_address[3:0] == 4'h6) next_S_dout = {24'b0, DATA_SIZE};
		else if(S_address[3:0] == 4'h7) next_S_dout = {28'b0, DESCRIPTOR_SIZE[3:0]};
		else if(S_address[3:0] == 4'h8) next_S_dout = {29'b0, OPMODE[2:0]};
		else next_S_dout = 32'b0;
	end
	else	next_S_dout = 32'b0; // default case 
 end
endmodule
 
 
 
 
 