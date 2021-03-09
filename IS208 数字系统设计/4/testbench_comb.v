`timescale 1ns/1ns
`include "com_str.v"
`include "comb_dataflow.v"
`include "comb_behavior.v"
`include "comb_prim.v"

module testbench_comb();
reg pa,pb,pc,pd;
wire str, dataflow, behavior, prim;
parameter LOOP =15;

initial begin
    {pd,pc,pb,pa} = 4'b0;
    repeat (LOOP)begin
        #1 {pd,pc,pb,pa} = {pd,pc,pb,pa}+1;
    end
end

comb_str a1 (str,pa,pb,pc,pd);
comb_dataflow a2 (dataflow,pa,pb,pc,pd);
comb_behavior a3 (behavior,pa,pb,pc,pd);
comb_prim a4 (prim,pa,pb,pc,pd);

initial
    $monitor("At time %4t, DCBA= %4b,str = %b, dataflow = %b, behavior = %b, prim = %b", $time,  {pd,pc,pb,pa},str, dataflow, behavior, prim);

endmodule


