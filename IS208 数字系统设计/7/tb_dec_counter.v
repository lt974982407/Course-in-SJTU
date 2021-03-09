  
`timescale 1 ns / 1 ns
`include "dec_counter.v"

module tb_dec_counter();

reg clk;
reg rst;
wire [3:0] count;

initial begin
	clk = 1'b0;
	repeat(36) begin
		#1 clk = ~clk;
    end
end

initial begin
	rst = 1'b0;
    #25 rst = 1'b1;
    #4 rst = 1'b0;
end

dec_counter counter(count, clk, rst);

initial
	$monitor( "At time %5t, count = %d, rst=%b", $time, count, rst);

endmodule
