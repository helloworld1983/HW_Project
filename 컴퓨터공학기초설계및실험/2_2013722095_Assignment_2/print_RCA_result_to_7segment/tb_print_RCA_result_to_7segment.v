`timescale 1ns/100ps

module  tb_print_RCA_result_to_7segment;

reg [3:0] tb_a, tb_b;
reg tb_cin;
wire [13:0] tb_dec_out;

print_RCA_result_to_7segment U0(.a(tb_a), .b(tb_b), .cin(tb_cin), .dec_out(tb_dec_out)); // 7segment 출력 module에 tb의 인자들을 적용

initial
begin
		tb_a=0; tb_b=0; tb_cin =0;
#10 tb_a= 4'h 1; // 합이 1 , 결과는 10000001111001로 나와야함
#10 tb_b= 4'h 1; // 합이 2 , 결과는 10000000100100로 나와야함
#10 tb_a= 4'h 4; tb_b= 4'h 5; tb_cin =1; // 합이 10, 결과는 10000000001000로 나와야함
#10 tb_a= 4'h 7; tb_b=4'h 8; tb_cin =0; // 합이 15, 결과는  10000000001110로 나와야함
#10 tb_cin = 1; // 합이 16, 결과는 11110011000000로 나와야함
#10 tb_a= 4'h f; tb_b= 4'h f; tb_cin =0; // 합이 30, 결과는 11110010000110로 나와야함
#20 $stop;
end
endmodule

