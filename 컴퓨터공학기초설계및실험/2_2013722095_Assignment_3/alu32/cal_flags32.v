module cal_flags32(op, result, co_add, co_prev_add, co_sub, co_prev_sub, c,n,z,v); // 두 입력의 연산 결과를 비교하기 위한 flag module

input 	[2:0]		op;
input 	[31:0]	result;
input 				co_add, co_prev_add, co_sub, co_prev_sub;
output 				c,n,z,v;

assign c = (op[2:1] != 2'b11) ? 1'b0 : ((op[0] == 1'b0) ? co_add : co_sub);
/*
3bit인 opcode의 2,3번째가 11이 아니라면 0을반환한다. (add, sub의 경우에만 보겠다는 의미 )
add, sub의 경우에 1번째 bit가 0이라면 co_add를 반환하고 아니라면 co_sub를 반환한다.
*/
assign n = result[31];
// result의 sign bit인 32번째 비트가 0이라면 양수(n=0) 1이라면 음수(n=1) 
assign z = (result == 32'b0) ? 1'b1 : 1'b0;
// result가 모두 0인 경우에 1, 하나의 비트라도 0이 아니라면 0이 된다.
assign v = (op[2:1] != 2'b11) ? 1'b0 : ((op[0] == 1'b0) ? (co_add^co_prev_add) : (co_sub^co_prev_sub));
/*
3bit인 opcode의 2,3번째가 11이 아니라면 0을반환한다. (add, sub의 경우에만 보겠다는 의미)
add, sub의 경우에 1번째 bit가 0이라면 co_add^co_prev_add를 반환하고 아니라면 co_sub^co_prev_sub를 반환한다.
*/
endmodule
