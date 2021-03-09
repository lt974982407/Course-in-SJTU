`timescale 10 ns / 1 ns

module wagven ();

reg out;

initial fork
	out = 1'b0;
	#2 out = 1'b1;
	#3 out = 1'b0;
	#12 out = 1'b1;
	#22 out = 1'b0;
	#24 out = 1'b1;
	#27 out = 1'b0;
	#32 out = 1'b1;
join

initial
	$monitor($time, "out = %b", out);

endmodule
