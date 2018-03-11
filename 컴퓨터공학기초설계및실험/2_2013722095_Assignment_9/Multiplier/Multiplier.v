module Multiplier(reset_n, clk, multiplier, multiplicand, op_start, op_clear, op_done, result); // top module
input [63:0] multiplier, multiplicand;
input	op_start, op_clear, reset_n, clk;
output reg op_done;
output reg [127:0] result;
reg next_x1,x1;
reg [63:0] u, next_u, next_v, v, next_x, x;
reg [63:0] add, sub;
reg [6:0] next_count, count;
reg [1:0] state, next_state;


/* state parameter */
parameter IDLE = 2'b00;
parameter EXEC = 2'b01;
parameter DONE = 2'b10;

// reset_n에 따라서 IDLE상태를 유지할 것인지 다음 state로 넘어갈 것인지를 결정 
always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 1'b0)
		state <= IDLE;
	else
		state <= next_state;
end

always@(posedge clk or negedge reset_n)
begin
	if(reset_n == 1'b0) // reset_n =0일 때 아래의 reg들을 초기화함.
		begin
		u 			=64'b0;
		v			=64'b0;
		x			=64'b0;
		x1			=1'b0;
		add		=64'b0;
		sub		=64'b0;
		count		=7'b0;
		end
	else			// reset_n =1이면 다음과 같이 next 변수들을 입력해줌
		begin
		u 			= next_u;
		v			= next_v;
		x			= next_x;
		x1			= next_x1;
		add		= 64'b0;
		sub		= 64'b0;
		count		=	next_count;
		end
end

// 현재 state와 op_start signal, op_clear signal, count를 보고 
//	다음 state를 무엇으로 할지를 결정
always@(op_start , op_clear , state , count)
	begin
	next_state = 2'b00;
	case(state)
	{IDLE}:// 아무것도 진행하는것이 없는 상태
		begin
			if	(op_clear == 1'b1)
				begin next_state <= IDLE; end
			else if(op_start ==1'b1 && op_clear == 1'b0) //op_start signal이 올라가면 EXEC state로 넘어간다.
				begin next_state <= EXEC; end
			else				// 그 외의 경우는 IDLE state로 넘어감.
				begin next_state <= IDLE; end
		end
	{EXEC}:// 현재 연산중인 상태
		begin
			if(op_clear == 1'b1) // op_clear signal이 올라가면 
				begin next_state <= IDLE; end	// IDLE state로 넘어감.
			else if(op_clear == 1'b0 && count < 7'b1000000) //아직 곱셈연산이 끝나지 않은경우
				begin next_state <= EXEC; end	// EXEC state를 유지
			else if(op_clear == 1'b0 && count == 7'b1000000) // 곱셈연산을 끝냈을 경우 
				begin next_state <= DONE; end	// DONE state로 넘어감
			else begin next_state <= 2'bx; end // default case
		end
	{DONE}:// 연산이 끝난경우
		begin
			if(op_clear ==1'b1)	//op_clear signal이 올라가면
				begin next_state <= IDLE; end	//IDLE state로 넘어감
			else
				begin next_state <= DONE; end	// 그 외의 경우는 DONE상태를 유지
		end
	default: begin next_state <= 2'bx; end 
	endcase
	end

	// 실질적으로 bit 연산하는 부분 
	// x, x1(x-1), u, v를 통해서 booth multiplication을 진행함.
always@(multiplicand , multiplier ,state, x , x1 , u , v , count)
begin
	// case문 들어가기 전에 사용되는 port 초기화 
	next_x = 64'h0;	
	next_x1= 1'b0;	
	next_u = 64'h0;
	next_v = 64'h0;
	next_count = 7'b0;
	add = 64'b0;		
	sub = 64'b0;
	if(count == 7'b1000000) // 연산완료시 done으로 넘어갈때 값을 같이 넘기기 위해 다음과 같이 초기화함.
	begin
		next_x = x;	
		next_x1= x1;	
		next_u = u;	
		next_v = v;
		next_count = count;
	end
	case(state)
		{IDLE}:// 현 상태가 IDLE이면 아래와 같이 초기화함.
		begin
			next_x = multiplier;	
			next_x1= 1'b0;	
			next_u = 64'h0;	
			next_v = 64'h0;	
			add = 64'b0;		
			sub = 64'b0;			
			next_count = 7'b0;	
		end
		
		{EXEC}:// 현재 state가 EXEC이면 
		begin
			if(count <7'b1000000) // 연산 횟수가 64번보다 작을 경우 
			begin
				if({x[0],x1} == 2'b01)	// add and shift
				begin
					sub = 64'b0;
					add = u + multiplicand;
					next_v = {add[0],v[63:1]};			// u의 lsb를 v의 msb로 받아옴
					next_u = {add[63],add[63:1]};		// u를 ASR 1bit 함.
					next_x1 = x[0];						// x1은 x의 lsb
					next_x = {x[0], x[63:1]};			// x를 circular shift 1bit함 
					next_count = count +7'b0000001;				// count +1
				end
				
				else if({x[0],x1} == 2'b10) // sub and shift
				begin
					add = 64'b0;
					sub = u - multiplicand;
					next_v = {sub[0],v[63:1]};			// u의 lsb를 v의 msb로 받아옴
					next_u = {sub[63],sub[63:1]};		// u를 ASR 1bit 함.
					next_x1 = x[0];						// x1은 x의 lsb
					next_x = {x[0], x[63:1]};			// x를 circular shift 1bit함 
					next_count = count +7'b0000001;				// count +1
				end
				
				else // 00 / 11 case
				begin
					add = 64'h0;
					sub = 64'h0;
					next_v = {u[0],v[63:1]};			// u의 lsb를 v의 msb로 받아옴
					next_u = {u[63],u[63:1]};			// u를 ASR 1bit 함.
					next_x1 = x[0];						// x1은 x의 lsb
					next_x = {x[0], x[63:1]};			// x를 circular shift 1bit함 
					next_count = count +7'b0000001;				// count +1
				end
			end			
		end
	
		{DONE}:// 현재 state가 DONE이면 
		begin				
			next_u = u;			// 현재 입출력인자 상태를 유지함.
			next_v = v;
			next_x = x;
			next_x1 = x1;
			next_count = count;
			add = 64'b0;
			sub = 64'b0;
		end
		default: // defualt : 모든 포트에 x 대입
		begin
			next_x = 64'bx;	
			next_x1= 1'bx;	
			next_u = 64'hx;	
			next_v = 64'hx;	
			add = 64'bx;		
			sub = 64'bx;			
			next_count = 7'bx;	
		end
	endcase
end
 // result 출력을 위한 always문 
always@(state, u, v)
begin
	if(state == DONE) // 현재 state가 DONE이라면 아래와 같이 output을 출력함.
		begin 	 result = {u, v};   op_done = 1'b1; end
	else if(state == IDLE)// 현재 state가 IDLE이라면 아래와 같이 output을 출력함.
		begin		result = 0;   op_done = 1'b0; end
	else						// 그 외의 경우에는 아래와 같이 output을 출력함.
		begin   result = {u, v};   op_done = 1'b0; end
end

endmodule
