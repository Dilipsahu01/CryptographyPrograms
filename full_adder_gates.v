`timescale 1ns / 1ps

// ==============================================================
// Full Adder Implementation using Structural Gate-Level Modeling
// ==============================================================
// This module demonstrates how to build a 1-bit full adder purely
// by instantiating fundamental hardware logic gates (AND, OR, XOR).
// This represents the lowest level of digital design before transistors.

module full_adder_gates (
    input  wire a,      // Input bit A
    input  wire b,      // Input bit B
    input  wire cin,    // Carry in from previous stage
    output wire sum,    // Sum output
    output wire cout    // Carry out to next stage
);

    // Intermediate wires to connect the logic gates
    wire w_xor_ab;
    wire w_and_ab;
    wire w_and_cin;

    // ----------------------------------------------------------
    // Sum calculation logic: sum = a XOR b XOR cin
    // ----------------------------------------------------------
    // Primitive gate instantiation: gate_type instance_name(output, input1, input2);
    xor xor_gate1 (w_xor_ab, a, b);        // w_xor_ab = a ^ b
    xor xor_gate2 (sum, w_xor_ab, cin);    // sum = w_xor_ab ^ cin

    // ----------------------------------------------------------
    // Carry out calculation logic: cout = (a AND b) OR (cin AND (a XOR b))
    // ----------------------------------------------------------
    and and_gate1 (w_and_ab, a, b);        // w_and_ab = a & b
    and and_gate2 (w_and_cin, w_xor_ab, cin); // w_and_cin = (a ^ b) & cin
    
    or  or_gate1  (cout, w_and_ab, w_and_cin); // cout = w_and_ab | w_and_cin

endmodule
