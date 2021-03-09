`timescale 1 ns/1 ns
`include "mealy.v"
`include "moore.v"

module top;

    parameter STEP = 32;
    integer k;

    wire mealy_flag, moore_flag;
    reg [31:0] data;
    reg din, clk, rst;

    mealy a(mealy_flag, din, clk, rst);
    moore b(moore_flag, din, clk, rst);

    initial begin
        clk = 1'b0;
        forever #10 clk = ~clk;
    end

    initial begin
        rst = 1'b1;
        #15 rst = 1'b0;
    end

    initial begin
        data = 32'b0110_1010_1010_0011_0110_0001_0101_0101;
        for ( k=0; k<STEP; k=k+1) begin
            #20;
            din = data[0];
            data = data >> 1;
        end
    end

endmodule
