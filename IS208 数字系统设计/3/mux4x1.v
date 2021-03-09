`timescale 1ns/1ns
`include "mux2x1.v"

module mux4x1(output dout, input [1:0]sel, [3:0]din);
wire out1,out2;
mux2x1 mux1(out1, sel[0], din[1:0]);
mux2x1 mux2(out2, sel[0], din[3:2]);
mux2x1 mux3(dout, sel[1],{out2,out1} );
endmodule
