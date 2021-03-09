`timescale 1 ns / 1 ns

module comb_str(output y, input sel, A, B, C, D);

wire o1,o2,selbar,o3,o4;

nand #(3) a1(o1,A,B);
nand #(4) a2(o2,C,D);
not a3(selbar,sel);
and a4(o3,selbar,o1);
and a5(o4,sel,o2);
or a6 (y,o3,o4);

endmodule
