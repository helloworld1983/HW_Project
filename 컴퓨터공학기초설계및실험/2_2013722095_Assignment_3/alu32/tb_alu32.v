`timescale 1ns/100ps

module tb_alu32;
   reg            clk, reset;   // Clock, Reset input
   reg   [31:0]   tb_a, tb_b, result_expected;   // expected result
   reg   [2:0]      tb_op;
   reg               null;
   wire   [31:0]   tb_result;
   wire            tb_c, tb_n, tb_z, tb_v;   // Flags ( Carry, Negative, Zero, oVerflow )
   
   reg   [100:0]   vectornum; // 몇번째 줄인지 확인하기 위한 vectornum
   reg    [98:0]   testvectors[200:0];    // 한번에 200bit를 저장할 수 있는 배열형식의 testvector(testvector)
      
      
   alu32 U0_alu32(.a(tb_a), .b(tb_b), .op(tb_op), .result(tb_result), .c(tb_c), .n(tb_n), .z(tb_z), .v(tb_v)); // alu32 module에 tb인자 적용 

   always
      begin
         #5; clk = ~clk;   //  10ns가 1주기고 그 반주기마다 clk이 반전됨 
      end
   
   initial
   begin
      $readmemh("example.tv", testvectors);   // example.tv 파일을 읽어옴 
      vectornum = 0;    clk = 1; 
      reset = 1;   #5;   reset = 0;
   end
   
   always   @   (posedge   clk)   // clk이 rising할 때 
      begin      
         #1; {tb_a, tb_b, null, tb_op, result_expected} = testvectors[vectornum];// testvectors에서 한줄 읽어옴 
      end
      
   always   @   (negedge   clk)   // each negative edge
      if   (~reset)   begin   // if reset == 0
         if   (tb_result !== result_expected ) begin   
            $display ("Error : inputs = %b", {tb_a, tb_b, null, tb_op});
            $display ("outputs = %b ( %b expected )", tb_result, result_expected );

         end
         
         vectornum = vectornum + 1;   // 다음줄로 이동
         if (testvectors[vectornum] === 99'hx) begin   // 다음줄이 (읽어올 값이) 없으면 
            $display("All tests completed");
            $finish;      // finish
         end
      end

endmodule 