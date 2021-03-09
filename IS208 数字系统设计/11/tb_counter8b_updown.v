`timescale 1 ns / 1 ns
`include "counter8b_updown.v"

module tb_counter8b_updown();

reg clk, reset, dir;
wire [7:0] count;

initial begin
    clk = 1'b0;
    repeat (100)begin
        #1 clk = ~clk;
    end
end

initial begin
    reset = 1'b0;
    dir = 1'b1;
    #20 dir = 1'b0;
    #10 reset = 1'b1;
    #6 reset = 1'b0;
    #2 dir = 1'b1;
end

counter8b_updown counter(count, clk, reset, dir);

initial
    $monitor("At time %7t,count = %d, reset = %b, dir = %b",$time, count, reset, dir);

endmodule