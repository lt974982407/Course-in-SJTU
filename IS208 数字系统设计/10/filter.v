`timescale 1ns / 1 ns

module filter(output reg sig_out, input clock, reset, sig_in);

reg [0:3] q;

wire j,k;

assign j = &q[1:3];
assign k = &(~q[1:3]);

always @(posedge clock or negedge reset) begin
    if (!reset)
        q <= 4'b0;
    else begin
        q[0] <= sig_in;
        q[1] <= q[0];
        q[2] <= q[1];
        q[3] <= q[2];
    end 
end

always @(posedge clock or negedge reset) begin
    if (!reset)
        sig_out = 1'b0;
    else begin
        case ({j,k}) 
            2'b00 : sig_out <= sig_out;
            2'b01 : sig_out <= 1'b0;
            2'b10 : sig_out <= 1'b1;
            2'b11 : sig_out <= ~sig_out;
        endcase
    end
end

endmodule
