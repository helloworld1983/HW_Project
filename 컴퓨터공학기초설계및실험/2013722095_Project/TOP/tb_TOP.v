`timescale 1ns/100ps

module tb_TOP;

   reg               clk, reset_n;                     // clock, reset
   reg               M0_req, M0_wr;                     // Master0 request, write/read
   reg      [7:0]      M0_address;                        // Master0 address
   reg      [31:0]   M0_dout;                           // Master0 data output
   wire               M0_grant;                        // Master0 grant
   wire               m_interrupt, d_interrupt;         // multiplier / dmac interrupt
   wire      [31:0]   M_din;                           // Master data input
   
   // Instantiation of TOP
   TOP U0(clk, reset_n, M0_req, M0_wr, M0_address, M0_dout, M0_grant, M_din, d_interrupt, m_interrupt);
	
   // Generate clock with period 20 ns
   parameter T = 2;
   always begin clk = ~clk; #(T/2); end
   
   integer i = 0;
   initial begin
   
            clk = 1; reset_n = 0; M0_req = 0; M0_wr = 0; M0_address = 0; M0_dout = 0;
   #(T/2);   reset_n = 1;
   #T;      M0_req = 0; M0_wr = 1;
   #T;      M0_address = 8'h20; M0_dout = 9999;   // multiplier
   #T;      M0_address = 8'h40; M0_dout = 9999;   // multiplicand
   #T;      M0_address = 8'h21; M0_dout = 18;   // multiplier
   #T;      M0_address = 8'h41; M0_dout = 17;   // multiplicand
   #T;      M0_address = 8'h22; M0_dout = 16;   // multiplier
   #T;      M0_address = 8'h42; M0_dout = 15;   // multiplicand
   #T;      M0_address = 8'h23; M0_dout = 14;   // multiplier
   #T;      M0_address = 8'h43; M0_dout = 13;   // multiplicand
   #T;      M0_address = 8'h24; M0_dout = 12;   // multiplier
   #T;      M0_address = 8'h44; M0_dout = 11;   // multiplicand
   #T;      M0_address = 8'h25; M0_dout = 10;   // multiplier
   #T;      M0_address = 8'h45; M0_dout = 09;   // multiplicand
   #T;      M0_address = 8'h26; M0_dout = 08;   // multiplier
   #T;      M0_address = 8'h46; M0_dout = 07;   // multiplicand
   #T;      M0_address = 8'h27; M0_dout = 06;   // multiplier
   #T;      M0_address = 8'h47; M0_dout = 05;   // multiplicand
   #T;      M0_address = 8'h28; M0_dout = 04;   // multiplier      (must be disregarded)
   #T;      M0_address = 8'h48; M0_dout = 03;   // multiplicand   (must be disregarded)

   #T;      M0_address = 8'h04; M0_dout = 32'h20;   // source address = 0x20
   #T;      M0_address = 8'h05; M0_dout = 32'h11;   // destination address = 0x11
   #T;      M0_address = 8'h06; M0_dout = 32'h09;   // data size = 4
   #T;      M0_address = 8'h03; M0_dout = 32'h01;   // push descriptor

   #T;      M0_address = 8'h04; M0_dout = 32'h40;   // source address = 0x20
   #T;      M0_address = 8'h05; M0_dout = 32'h10;   // destination address = 0x11
   #T;      M0_address = 8'h06; M0_dout = 32'h09;   // data size = 4
   #T;      M0_address = 8'h03; M0_dout = 32'h01;   // push descriptor

   #T;      M0_address = 8'h02; M0_dout = 32'h01;   // DMAC interrupt is enabled
   #T;      M0_address = 8'h13; M0_dout = 32'h01;   // Multiplier interrupt is enabled

   #T;      M0_address = 8'h08; M0_dout = 32'h01;   // opmode: source increment
   
   #T;      M0_address = 8'h00; M0_dout = 32'h01;   // DMAC op_start
   #T;      for(i = 0; d_interrupt != 1'b1; i = i + 1) #T;   // multiple data transfer
   #T;      M0_address = 8'h01; M0_dout = 32'h01;   // DMAC operation clear
   #T;      M0_address = 8'h01; M0_dout = 32'h00;   // DMAC operation clear

   #T;      M0_address = 8'h14; M0_dout = 32'h01;   // Multiplier op_start
   #T;      for(i = 0; m_interrupt != 1'b1; i = i + 1) #T;   // multiplication
   #T;      M0_wr = 0;   M0_address = 8'h16;         // Multiplier operation done register
   #T;      M0_wr = 1;
//   #T;      M0_address = 8'h15; M0_dout = 32'b01;   // Multiplier operation clear
//   #T;      M0_address = 8'h15; M0_dout = 32'b00;   // Multiplier operation clear

   #T;      M0_address = 8'h04; M0_dout = 32'h12;   // source address = 0x12
   #T;      M0_address = 8'h05; M0_dout = 32'h60;   // destination address = 0x60
   #T;      M0_address = 8'h06; M0_dout = 32'h12;   // data size = 8
   #T;      M0_address = 8'h03; M0_dout = 32'h01;   // push descriptor
   #T;      M0_address = 8'h08; M0_dout = 32'h02;   // opmode: destination increment mode
   
   #T;      M0_address = 8'h02; M0_dout = 32'h01;   // DMAC interrupt is enabled
   #T;      M0_address = 8'h00; M0_dout = 32'h01;   // DMAC op_start
   #T;      for(i = 0; d_interrupt != 1'b1; i = i + 1) #T;
   #T;      M0_address = 8'h01; M0_dout = 32'h01;   // DMAC operation clear
   #T;      M0_address = 8'h01; M0_dout = 32'h00;   // DMAC operation clear
   
   #T;      M0_wr = 0;
   #T;      M0_address = 8'h20;
   #T;      M0_address = 8'h40;
   #T;      M0_address = 8'h60;
   #T;      M0_address = 8'h61;
   
   #T;      M0_address = 8'h21;
   #T;      M0_address = 8'h41;
   #T;      M0_address = 8'h62;
   #T;      M0_address = 8'h63;
   
   #T;      M0_address = 8'h22;
   #T;      M0_address = 8'h42;
   #T;      M0_address = 8'h64;
   #T;      M0_address = 8'h65;
   
   #T;      M0_address = 8'h23;
   #T;      M0_address = 8'h43;
   #T;      M0_address = 8'h66;
   #T;      M0_address = 8'h67;
   
   #T;      M0_address = 8'h24;
   #T;      M0_address = 8'h44;
   #T;      M0_address = 8'h68;
   #T;      M0_address = 8'h69;
   
   #T;      M0_address = 8'h25;
   #T;      M0_address = 8'h45;
   #T;      M0_address = 8'h6A;
   #T;      M0_address = 8'h6B;
   
   #T;      M0_address = 8'h26;
   #T;      M0_address = 8'h46;
   #T;      M0_address = 8'h6C;
   #T;      M0_address = 8'h6D;
   
   #T;      M0_address = 8'h27;
   #T;      M0_address = 8'h47;
   #T;      M0_address = 8'h6E;
   #T;      M0_address = 8'h6F;
   
   #T;      M0_address = 8'h28;
   #T;      M0_address = 8'h48;
   #T;      M0_address = 8'h70;
   #T;      M0_address = 8'h71;
   
   #T;      $stop;
   
   end
   
endmodule