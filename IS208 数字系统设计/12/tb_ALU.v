`timescale 1 ns / 1 ns
`include "ALU.v"

module tb_ALU();

wire c_out;
wire [7:0] sum;
reg [2:0] oper;
reg [7:0] a;
reg [7:0] b;
reg c_in;

initial begin
    a = 8'b1101_0010;
	b = 8'b1011_0110;
    c_in = 1'b1;
    oper = 3'b0;
    repeat (7)begin
        #1 oper = oper+1;
    end
end

ALU alu(c_out, sum, oper, a, b, c_in);

initial
    $monitor("At time %3t, a= %8b, b= %8b, c_in= %b, oper = %b, sum= %b, c_out = %b",$time, a,b,c_in, oper,sum,c_out);

endmodule