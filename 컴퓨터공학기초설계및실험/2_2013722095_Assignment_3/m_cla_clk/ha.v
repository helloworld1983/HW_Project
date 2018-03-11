module ha(input a,b,output co,s);
assign co = a&b;
assign s = a^b;
endmodule
