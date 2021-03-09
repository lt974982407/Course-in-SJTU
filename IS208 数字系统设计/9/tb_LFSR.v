`timescale 10 ns / 1 ns
`include "LFSR.v"

module tb_LFSR();

wire [1:26] q;	// 26 bit data output.
reg clk;		// Clock input.
reg rst_n;		// Synchronous reset input.
reg load;		// Synchronous load input.
reg [1:26] din;	// 26 bit parallel data input.

initial begin
	clk = 1'b0;
	forever
		#5 clk = ~clk;
end

initial begin
	rst_n = 1'b0;
	#6 rst_n = 1'b1;
end

initial begin
	din = 26'b1101_1001_0101_1010_1101_0110_01;
	load = 1'b1;
	#22 load = 1'b0;
end

LFSR lsfr(q, clk, rst_n, load, din);

initial
	$monitor($time, "\tq = %26b", q);

endmodule
