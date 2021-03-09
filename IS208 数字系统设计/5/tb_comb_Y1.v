`timescale 1ns / 1 ns
`include "comb_Y1.v"

module tb_comb_Y1();

reg A, B, C;
wire Y;

initial begin
	{A, B, C} = 3'b0;
	repeat(7) begin
		#1 {A, B, C} = {A, B, C} + 1;
    end
end

comb_Y1 Y1(Y, A, B, C);

initial
	$monitor( "At time %3t,ABC = %3b, Y1 = %b",$time, {A, B, C}, Y);

endmodule
