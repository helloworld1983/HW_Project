module seg_dec(iHex,oSEG7);
input [3:0] iHex;
output reg [6:0] oSEG7;
// 입력값의 case별로 7-segment의 어떤 부분에 불이 들어와야 하는지 정의
always @ (iHex)
	begin 
		case(iHex)
			4'h0 : oSEG7=7'b1000000;
			4'h1 : oSEG7=7'b1111001;
			4'h2 : oSEG7=7'b0100100;
			4'h3 : oSEG7=7'b0110000;
			4'h4 : oSEG7=7'b0011001;
			4'h5 : oSEG7=7'b0010010;
			4'h6 : oSEG7=7'b0000010;
			4'h7 : oSEG7=7'b1111000;
			4'h8 : oSEG7=7'b0000000;
			4'h9 : oSEG7=7'b0011000;
			4'ha : oSEG7=7'b0001000;
			4'hb : oSEG7=7'b0000011;
			4'hc : oSEG7=7'b1000110;
			4'hd : oSEG7=7'b0100001;
			4'he : oSEG7=7'b0000110;
			4'hf : oSEG7=7'b0001110;
			default : oSEG7 = 7'b 1111111;
		endcase
	end
endmodule
