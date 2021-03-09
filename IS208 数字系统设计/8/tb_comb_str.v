`timescale 1 ns / 1 ns
`include "comb_str.v"

module tb_comb_str();

reg sel, A, B, C, D;
wire Y;

initial begin
	{sel, A, B, C, D} = 5'b0;
	repeat (31) begin
		#10 {sel, A, B, C, D} = {sel, A, B, C, D} + 1;
    end
end

comb_str comb(Y, sel, A, B, C, D);

initial
	$monitor(
		 "At time %3t\tsel = %b\tAB = %2b\tCD = %2b\tY = %b",
		$time, sel, {A, B}, {C, D}, Y
	);

endmodule
