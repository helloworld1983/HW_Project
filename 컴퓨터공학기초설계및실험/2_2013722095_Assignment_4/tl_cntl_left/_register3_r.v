module _register3_r(clk,d,q,reset); // reset 신호를 통해 기억하고 있던 값들을 초기화 할수 있는 register module

   input clk,reset;
   input [2:0] d;
   output [2:0] q;
   
   _dff_r_async   U0(.clk(clk),.reset_n(reset),.d(d[0]),.q(q[0])); // 1번째 비트 기억
   _dff_r_async   U1(.clk(clk),.reset_n(reset),.d(d[1]),.q(q[1])); // 2번째 비트 기억 
   _dff_r_async   U2(.clk(clk),.reset_n(reset),.d(d[2]),.q(q[2])); // 3번째 비트 기억 
  endmodule 