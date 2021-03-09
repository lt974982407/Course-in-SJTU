`timescale 1 ns / 1 ns
`include "shift_counter.v"

module tb_shift_counter();

reg clk,reset;
wire [7:0] cout;

initial begin
    clk = 1'b0;
    repeat (100) begin
        #1 clk = ~clk;
    end
end

initial begin
    reset = 1'b0;
    #40 reset = 1'b1;
    #4 reset = 1'b0;
end

shift_counter counter(cout, clk, reset);

initial
    $monitor("At time %7t, count = %8b, reset = %b",$time,cout,reset);

endmodule