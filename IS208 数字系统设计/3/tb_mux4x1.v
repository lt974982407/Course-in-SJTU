`timescale 1ns/1ns

`include "mux4x1.v" 

module tb_mux4x1();

parameter LOOP =63;

wire out;
reg [3:0] din;
reg [1:0] sel;

initial begin
    din = 4'b0;
    sel = 2'b0;
    repeat (LOOP) begin
        #1 {din, sel} = {din,sel}+1;
    end
end

mux4x1 a(out,sel, din);

initial 
$monitor ("At time %3t, sel=%2b, din= %4b, out=%b",$time, sel, din,out); 
endmodule
