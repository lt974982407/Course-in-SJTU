`timescale 1ns/1ns
`include "ones_count.v"

module tb_ones_count();

wire  [3:0] out;
reg [7:0] in;

initial begin
    in = 8'b11111111;
    repeat (8) begin
        #1 in = in << 1;
    end
end

ones_count a(out, in);

initial 
    $monitor("At time %3t, data_in = %8b, cout = %4b", $time, in, out);

endmodule