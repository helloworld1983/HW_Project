module print_RCA_result_to_7segment(a,b,cin,dec_out);

input [3:0] a,b;
input cin;
output [13:0] dec_out;
wire [3:0] s;
wire c;

_4RCA U0(.a(a), .b(b), .co(c), .ci(cin), .s(s)); // 4bit RCA module에 각각 입력값과 출력값을 적용
seg_dec U1_seg_dec(.iHex(s), .oSEG7(dec_out[6:0]));				 // 7-segment에 출력해주는 module seg_dec에 합을 넣어줌
seg_dec U2_seg_dec(.iHex({3'b000,c}), .oSEG7(dec_out[13:7]));	 // 7-segment에 a,b 연산에서 발생한 carry out값을 넣어줌

endmodule
