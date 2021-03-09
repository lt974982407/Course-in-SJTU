  
`timescale 10 ns / 1 ns
`include "Encoder8x3.v"

module tb_Encoder8x3();

reg [7:0] data;
wire [2:0] code;

initial begin
	data = 8'b0000_0001;
	repeat(7) begin
		#1 data = data << 1;
	end
end

Encoder8x3 encoder8x3(code, data);

initial
	$monitor( "At time %3t,data = %8b, code = %3b",$time, data, code);

endmodule
