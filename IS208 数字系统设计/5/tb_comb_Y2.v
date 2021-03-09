`timescale 1ns / 1 ns
`include "comb_Y2.v"

module tb_comb_Y2();

reg A, B, C, D;
wire Y;

initial begin
	{A, B, C, D} = 4'b0;
	repeat (15)begin
		#1 {A, B, C, D} = {A, B, C, D} + 1;
    end
end

comb_Y2 Y2(Y, A, B, C, D);

initial
	$monitor(
		 "At time %4t, ABCD = %4b, Y2 = %b",
		$time,{A, B, C, D}, Y
	);

endmodule
