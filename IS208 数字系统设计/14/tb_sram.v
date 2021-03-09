`timescale 1 ns / 1 ns
`include "sram.v"

module tb_sram();

wire [7:0] dout;
reg [7:0] din;
reg [7:0] addr;
reg wr;
reg rd;
reg cs;

initial begin
	addr = 8'b1001_1010;
    cs = 1'b1;
	din = 8'b1011_0101;
    wr = 1'b0;
    rd = 1'b1;
    #10 wr = 1'b1;
    #1 wr = 1'b0;
    #3 rd = 1'b0;
    din = 8'b1011_0000;
    #10 wr = 1'b1;
    #1 wr = 1'b0;
    #3 rd = 1'b0; 
end

sram sr(dout, din, addr, wr, rd, cs);

initial
    $monitor ("At time %6t, addr = %d, wr = %b, rd = %b, din = %8b, dout = %8b",$time,addr,wr,rd,din,dout);

endmodule