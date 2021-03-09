`timescale 1ns/1ns

module comb_dataflow(output Y, input A, B, C, D);
 wire o1, o2;
 assign o1 = A|D;
 assign o2 = (B&C&~D);
 assign Y = (~o1)&o2;
 
endmodule