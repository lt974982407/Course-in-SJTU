`timescale 1ns/1ns

`include "mux2x1.v" 

module tb_mux2x1();

parameter LOOP =7;

wire out;
reg [1:0] din;
reg sel;

initial begin
    din = 2'b0;
    sel = 0;
    repeat (LOOP) begin
        #1 {din, sel} = {din,sel}+1;
    end
end

mux2x1 a(out,sel, din);

initial 
$monitor ("At time %3t, sel=%b, din= %2b, out=%b",$time, sel, din,out); 
endmodule
