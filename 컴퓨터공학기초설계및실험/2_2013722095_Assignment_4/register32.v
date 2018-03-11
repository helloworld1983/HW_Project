module register32(clk,d,q); // reset 신호를 통해 기억하고 있던 값들을 초기화 할수 있는 register module

   input clk;
   input [31:0] d;
   output [31:0] q;
   /* 1bit register 32개를 instance하여 구현하였습니다*/
   _dff U0(clk, d[0], q[0]);
	_dff U1(clk, d[1], q[1]);
	_dff U2(clk, d[2], q[2]);
	_dff U3(clk, d[3], q[3]);
	_dff U4(clk, d[4], q[4]);
	_dff U5(clk, d[5], q[5]);
	_dff U6(clk, d[6], q[6]);
	_dff U7(clk, d[7], q[7]);
	_dff U8(clk, d[8], q[8]);
	_dff U9(clk, d[9], q[9]);
	_dff U10(clk, d[10], q[10]);
	_dff U11(clk, d[11], q[11]);
	_dff U12(clk, d[12], q[12]);
	_dff U13(clk, d[13], q[13]);
	_dff U14(clk, d[14], q[14]);
	_dff U15(clk, d[15], q[15]);
	_dff U16(clk, d[16], q[16]);
	_dff U17(clk, d[17], q[17]);
	_dff U18(clk, d[18], q[18]);
	_dff U19(clk, d[19], q[19]);
	_dff U20(clk, d[20], q[20]);
	_dff U21(clk, d[21], q[21]);
	_dff U22(clk, d[22], q[22]);
	_dff U23(clk, d[23], q[23]);
	_dff U24(clk, d[24], q[24]);
	_dff U25(clk, d[25], q[25]);
	_dff U26(clk, d[26], q[26]);
	_dff U27(clk, d[27], q[27]);
	_dff U28(clk, d[28], q[28]);
	_dff U29(clk, d[29], q[29]);
	_dff U30(clk, d[30], q[30]);
	_dff U31(clk, d[31], q[31]);
  endmodule 