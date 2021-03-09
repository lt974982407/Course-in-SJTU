`timescale 1ns/1ns

module dec_counter(output reg [3:0] count, input clk, reset);

initial
count = 4'b0;

always @(posedge clk) begin
    if (reset)
        count <= 4'b0;
    else begin
		if (count == 10)
			count <= 0;
		else
			count <= count + 1;
    end
end

endmodule