module _srlatch(r, s, q, q_bar); // reset과 set신호를 받아서 q, ~q를 출력해주는 srlatch module
input r, s;
output q, q_bar;
_nor2 U0_nor2(.a(r), .b(q_bar), .y(q)); // nor gate에 reset과 q_bar를 input으로 하면 output으로 q가 나옴 
_nor2 U1_nor2(.a(s), .b(q), .y(q_bar)); // nor gate에 set과 q를 input으로 주면 output으로 q_bar가 나옴 

endmodule
