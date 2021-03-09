`timescale 1 ns / 1 ns
`include "seq_detect.v"

module tb_seq_detect();

wire flag;
reg clk;
reg rst_n;

reg [2:0] buffer;
reg din;

initial begin
	clk = 1'b0;
	forever
		#1 clk = ~clk;
end

initial begin
	rst_n = 1'b1;
	#22 rst_n = 1'b0;
	#26 rst_n = 1'b1;
end

initial begin
	{buffer, din} = 4'bxxx1;
	#1 {buffer, din} = 4'bxx10;
	#2 {buffer, din} = 4'bx101;
	#2 {buffer, din} = 4'b1011;
	#2 {buffer, din} = 4'b0110;
	#2 {buffer, din} = 4'b1101;
	#2 {buffer, din} = 4'b1011;
	#2 {buffer, din} = 4'b0111;
	#2 {buffer, din} = 4'b1110;
	#2 {buffer, din} = 4'b1101;
	#2 {buffer, din} = 4'b1010;
	#8 {buffer, din} = 4'b0101;
	#2 {buffer, din} = 4'b1011;
	#2 {buffer, din} = 4'b0110;
    #2 {buffer, din} = 4'b1100;
end

seq_detect dectect(flag, din, clk, rst_n);

endmodule
