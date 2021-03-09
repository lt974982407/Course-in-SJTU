`timescale 1ns/1ns

module comb_str(output Y, input A, B, C, D);

wire bard, o1,o2,o3;

not a1(bard,D);
or a2(o1,A,D);
not a3(o2,o1);
and a4(o3,B,C,bard);
and a5(Y, o3,o2);

endmodule
