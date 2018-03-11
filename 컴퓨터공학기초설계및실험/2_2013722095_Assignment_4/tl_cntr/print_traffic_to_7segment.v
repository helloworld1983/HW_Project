module print_traffic_to_7segment(CLK, Reset, Ta, Tb, dec_out);
input CLK, Reset, Ta, Tb;
output [27:0] dec_out;

wire [1:0] w_La, w_Lb;

tl_cntr U0_cntr(.CLK(CLK), .Reset(Reset), .Ta(Ta), .Tb(Tb), .La(w_La), .Lb(w_Lb));
seg_dec U1_dec(.iHex({3'b0,w_La[1]}), .oSEG7(dec_out[27:21]));
seg_dec U2_dec(.iHex({3'b0,w_La[0]}), .oSEG7(dec_out[20:14]));
seg_dec U3_dec(.iHex({3'b0,w_La[1]}), .oSEG7(dec_out[13:7]));
seg_dec U4_dec(.iHex({3'b0,w_La[0]}), .oSEG7(dec_out[6:0]));

endmodule
