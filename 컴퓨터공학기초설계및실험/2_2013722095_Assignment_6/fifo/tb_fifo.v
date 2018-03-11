`timescale 1ns/100ps

module tb_fifo;

      reg clk, reset_n, tb_rd_en, tb_wr_en;
      reg [31:0] tb_din;
      wire [31:0] tb_dout;
      wire tb_full, tb_empty;
      wire tb_wr_ack, tb_wr_err, tb_rd_ack, tb_rd_err;
      wire [3:0] tb_data_count;
		parameter temp = 10;
   
   fifo  U0_fifo (clk, reset_n, tb_rd_en, tb_wr_en, tb_din, tb_dout, tb_data_count, tb_full, tb_empty, tb_wr_ack, tb_wr_err, tb_rd_ack, tb_rd_err);
            
      always#(temp/2) clk = ~clk; // 5ns마다 clk값을 반전시킴. 
      
      initial
      begin
          clk = 0;   reset_n = 0; tb_rd_en = 0; tb_wr_en = 0; tb_din = 32'h00000000; // 초기값 setting
      #12;  reset_n = 1; // 값을 받아들일 준비
      #10;  tb_rd_en = 1; // rd enable을 1로 
      #10;  tb_rd_en = 0;   tb_wr_en = 1; tb_din = 32'h00000011; // rd enable =0, wr enable =1로 하여 아래와 같은 값들을 저장함.
      #10;  tb_din = 32'h00000022;
      #10;  tb_din = 32'h00000033;
      #10;  tb_din = 32'h00000044;
      #10;  tb_din = 32'h00000055;
      #10;  tb_din = 32'h00000066;
      #10;  tb_din = 32'h00000077;
      #10;  tb_din = 32'h00000088;
      #10;  tb_din = 32'h00000099;
      #10;  tb_din = 32'h000000aa;
      #10;  tb_din = 32'h000000bb;
      #10;  tb_rd_en = 1; tb_wr_en = 0;// register에 있는 값들을 출력.
      #100;
      #20; $stop;
      end

endmodule