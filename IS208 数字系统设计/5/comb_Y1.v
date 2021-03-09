`timescale 1ns / 1 ns

module comb_Y1(output Y, input A, B, C);

assign Y = A ? ~B : B ^ C;

endmodule
