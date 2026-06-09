`timescale 1ns / 1ps

// ==============================================================
// Full Adder Implementations using Dataflow and Behavioral Modeling
// ==============================================================
// This file demonstrates the modern ways to design digital logic 
// using Verilog syntax: 'assign', 'if-else', and 'case' statements.

// -------------------------------------------------------------
// Method 1: Dataflow modeling using 'assign'
// -------------------------------------------------------------
// This is the most common modern way to write combinational logic.
module full_adder_assign (
    input  wire a, b, cin,
    output wire sum, cout
);
    // The concatenation operator {} joins cout and sum into a 2-bit vector.
    // The synthesis tool automatically infers the optimal adder logic.
    assign {cout, sum} = a + b + cin;
endmodule


// -------------------------------------------------------------
// Method 2: Behavioral modeling using 'if-else'
// -------------------------------------------------------------
// This uses sequential-style blocks (always @) to define logic.
module full_adder_ifelse (
    input  wire a, b, cin,
    output reg  sum, cout   // 'reg' type is required inside always blocks
);
    // always @(*) means trigger whenever any input changes
    always @(*) begin
        if      (a == 0 && b == 0 && cin == 0) begin sum = 0; cout = 0; end
        else if (a == 0 && b == 0 && cin == 1) begin sum = 1; cout = 0; end
        else if (a == 0 && b == 1 && cin == 0) begin sum = 1; cout = 0; end
        else if (a == 0 && b == 1 && cin == 1) begin sum = 0; cout = 1; end
        else if (a == 1 && b == 0 && cin == 0) begin sum = 1; cout = 0; end
        else if (a == 1 && b == 0 && cin == 1) begin sum = 0; cout = 1; end
        else if (a == 1 && b == 1 && cin == 0) begin sum = 0; cout = 1; end
        else if (a == 1 && b == 1 && cin == 1) begin sum = 1; cout = 1; end
        else                                   begin sum = 0; cout = 0; end // Fallback
    end
endmodule


// -------------------------------------------------------------
// Method 3: Behavioral modeling using 'case' statements
// -------------------------------------------------------------
// Case statements are excellent for defining Truth Tables directly in hardware.
module full_adder_case (
    input  wire a, b, cin,
    output reg  sum, cout
);
    // Combine the 3 individual 1-bit inputs into a single 3-bit bus
    wire [2:0] inputs_bus;
    assign inputs_bus = {a, b, cin}; 

    always @(*) begin
        // Evaluate the 3-bit bus directly like a truth table
        case (inputs_bus)
            3'b000: begin sum = 0; cout = 0; end
            3'b001: begin sum = 1; cout = 0; end
            3'b010: begin sum = 1; cout = 0; end
            3'b011: begin sum = 0; cout = 1; end
            3'b100: begin sum = 1; cout = 0; end
            3'b101: begin sum = 0; cout = 1; end
            3'b110: begin sum = 0; cout = 1; end
            3'b111: begin sum = 1; cout = 1; end
            default: begin sum = 0; cout = 0; end // Good practice to prevent latches
        endcase
    end
endmodule
