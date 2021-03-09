`timescale 1ns/1ns

module ones_count(output reg [3:0]  count, input [7:0] data_in);

always @(*)
    count = data_in[0]+data_in[1]+data_in[2]+data_in[2]+data_in[4]+data_in[5]+data_in[6]+data_in[7];

endmodule 

