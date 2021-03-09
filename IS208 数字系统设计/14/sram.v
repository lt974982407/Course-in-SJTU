`timescale 1 ns / 1 ns

module sram(
	output reg [7:0] dout,
	input [7:0] din,
	input [7:0] addr,
	input wr,
	input rd,
	input cs
);

reg [7:0] mem[0:255];

always @ (negedge wr) begin
    if(cs)
        mem[addr] <= din;
end

always @(*) begin
    if(cs && !rd)
        dout <= mem[addr];
end

endmodule