`timescale 1ns/100ps

module tb_dff_rs;
reg clk, tb_d, set, reset;
wire tb_q;

parameter temp = 10;

 _dff_rs U0 (clk, tb_d, tb_q, set, reset); // tb인자들을 module에 적용 
 
 always #(temp/2) clk = ~clk; // 클럭을 반주기마다 반전 시킴 
 initial
 begin
 clk =0; set =0; reset= 1; tb_d =0; // 입력값 초기화 
 
 #10 reset =0; tb_d =1; // reset =0
 #10 set =1; tb_d =0; // set=1인 동안에는 이전의 q를 유지함. 
 #10 tb_d =1;
 #10 set =0; tb_d =0; // set =0이므로 입력에 따라 출력이 변동됨
 #10 tb_d =1;
 #10 $stop;
 end
 endmodule
 