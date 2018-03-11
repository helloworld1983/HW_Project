module cc_logic(op, shamt, d_in, d_out, do_next);
input [2:0] op;
input [1:0] shamt;
input [7:0] d_in;
input [7:0] d_out;
output [7:0] do_next;
reg	[7:0]	do_next;

wire 	[7:0]	d_lsl;
wire	[7:0]	d_lsr;
wire 	[7:0]	d_asr;
/* parameter setting */
parameter NOP 	= 3'b000;
parameter LOAD = 3'b001;
parameter LSL 	= 3'b010;
parameter LSR 	= 3'b011;
parameter ASR	= 3'b100;

always @(op, shamt, d_in, d_out, d_lsl, d_lsr, d_asr) // op code에 따라서 각각 다른 값을 next로 함. 
begin
	case(op)
	{NOP}	:	do_next <= d_out; // 현재 값 유지 
	{LOAD}:	do_next <= d_in;  // d_in값을 출력 
	{LSL}	:	do_next <= d_lsl; // shamt만큼 lsl한 값을 출력 
	{LSR}	:	do_next <= d_lsr; // shamt만큼 lsr한 값을 출력 
	{ASR}	:	do_next <= d_asr; // shamt만큼 art한 값을 출력 
	default :	do_next <= 8'bx;
	endcase
end

LSL8 U0(d_out, shamt, d_lsl); // d_out을 입력으로 하여 shamt만큼 lsl한 값을 출력 
LSR8 U1(d_out, shamt, d_lsr);// d_out을 입력으로 하여 shamt만큼 lsr한 값을 출력 
ASR8 U2(d_out, shamt, d_asr);// d_out을 입력으로 하여 shamt만큼 asr한 값을 출력 

endmodule
