module cal_flags4(op, result, co_add, c3_add, co_sub, c3_sub, c, n, z, v);
	input [2:0] 	op;
	input [3:0]		result;
	input 			co_add, c3_add, co_sub, c3_sub;
	output 			c,n,z,v;
	
	assign c = (op[2:1] != 2'b11) ? 1'b0 : ((op[0] == 1'b0) ? co_add : co_sub); // op[2:1]가 11이 아니면 0을 반환한다. 맞는 경우에 op[0]가 0이면 co_add를 반환 0이 아니면 co_sub 반환
	assign n = result[3];
	assign z = (result == 4'b0) ? 1'b1 : 1'b0;
	assign v = (op[2:1] != 2'b11) ? 1'b0 : ((op[0] == 1'b0) ? (co_add^c3_add) : (co_sub^c3_sub));
endmodule
