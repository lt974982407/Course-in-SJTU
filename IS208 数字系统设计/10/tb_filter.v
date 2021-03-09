`timescale 1 ns / 1 ns
`include "filter.v"

module tb_filter();

wire sig_out;
reg clock, reset, sig_in;

initial begin
	clock = 1'b0;
	repeat (200) begin
		#1 clock = ~clock;
    end
end

initial begin
    reset = 1'b1;
    #50 reset = 1'b0;
    #50 reset = 1'b1;
end

initial begin
    sig_in = 1'b1;
    forever #10 sig_in = $random % 2;
end

filter ftr(sig_out, clock, reset, sig_in);

endmodule