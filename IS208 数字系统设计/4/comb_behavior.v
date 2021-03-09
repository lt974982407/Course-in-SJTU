`timescale 1ns/1ns

module comb_behavior(output reg Y, input A, B, C, D);
 
always @(*)
    Y = ~(A | D) & (B & C & (~D));
endmodule
