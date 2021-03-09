`timescale 1ns / 1 ns

module comb_Y2(output Y, input A, B, C, D);

assign Y = B ? ~(A & C) : (A & C & D);

endmodule
