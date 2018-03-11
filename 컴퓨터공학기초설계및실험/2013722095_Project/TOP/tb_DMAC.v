`timescale 1ns/100ps

module tb_DMAC;

   reg         clk, reset_n;      // clock, reset
   reg         M_grant;            // Master grant
   reg [31:0]   M_din;            // Master data reg
   reg          S_sel, S_wr;      // slave select, write/read
   reg [7:0]   S_address;         // Slave address
   reg [31:0]   S_din;            // Slave data reg
   
   wire         M_req, M_wr;      // Master request, write/read
   wire [7:0]   M_address;         // Master address
   wire [31:0]   M_dout, S_dout;   // Master/Slave data wire
   wire         interrupt;         // dmac interrupt
   
   // Instance of dmac
   DMAC U0_DMAC_Top(.clk(clk), .reset_n(reset_n), .M_grant(M_grant), .M_din(M_din), .S_sel(S_sel), .S_wr(S_wr), .S_address(S_address),
                .S_din(S_din), .M_req(M_req), .M_wr(M_wr), .M_address(M_address), .M_dout(M_dout), .S_dout(S_dout), .interrupt(interrupt));
            
   // Generate clock with period 20ns
   parameter T = 20;
   always begin clk = ~clk; #10; end
   
   initial begin
                  clk = 1; reset_n = 0; M_grant = 1'b0; M_din = 32'b0;
                  S_sel = 1'b0; S_wr = 1'b0; S_address = 8'b0; S_din = 32'b0;
      #(3*T/2);   reset_n = 1'b1;
      #T;         S_sel = 1'b1; S_address = 8'h01;
      #(2*T);      S_wr = 1'b1; S_address = 8'h02; S_din = 32'h0000_0001;
      #T;         S_address = 8'h04; S_din = 32'h0000_0010;
      #T;         S_address = 8'h05; S_din = 32'h0000_0020;
      #T;         S_address = 8'h06; S_din = 32'h0000_0004;
      #T;         S_address = 8'h03; S_din = 32'h0000_0001;
      #T;         S_address = 8'h04; S_din = 32'h0000_0030;
      #T;         S_address = 8'h05; S_din = 32'h0000_0040;
      #T;         S_address = 8'h06; S_din = 32'h0000_0005;
      #T;         S_address = 8'h03; S_din = 32'h0000_0001;
      #T;         S_address = 8'h00;
      #T;         S_din = 32'h0000_0001;
      #T;         S_address = 8'h08; S_din = 32'h0000_0003;
      #T;         S_address = 8'h00; S_din = 32'h0000_0000;
      #T;         S_sel = 0; S_wr = 0; S_din = 0;
      #T;         S_wr = 1'b0;
      #(4*T);      M_grant = 1'b1;
      #(22*T);      S_sel = 1'b1; S_wr = 1'b1; S_address = 8'h01; S_din = 32'h0000_0001;
      #T;         S_sel = 1'b0; S_wr = 1'b0; S_address = 8'h00; S_din = 32'h0000_0000;
      #(4*T);      $stop;
   end
   
endmodule