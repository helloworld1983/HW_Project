module _dff_rs_sync_async(clk, set_n, reset_n, d, q_sync, q_async);
input clk, set_n, reset_n, d;
output q_sync, q_async;

_dff_rs_sync	U0(clk, set_n, reset_n, d, q_sync); // clk동기식 dff_rs module 
_dff_rs_async	U1(clk, set_n, reset_n, d, q_async);// clk 비동기식 dff_rs module 

endmodule
