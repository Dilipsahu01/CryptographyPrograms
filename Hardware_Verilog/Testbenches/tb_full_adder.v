`timescale 1ns / 1ps

// =====================================================================
// Comprehensive Testbench for Full Adder Implementations
// =====================================================================
// This testbench verifies all four full adder modeling methodologies:
// 1. Structural Gate-Level (Primitive Gates)
// 2. Dataflow (assign keyword)
// 3. Behavioral (if-else statements)
// 4. Behavioral (case statements)
//
// It applies exhaustive stimulus (all 8 binary combinations of a, b, cin)
// and displays the results of each module side-by-side to ensure
// they are mathematically and logically identical.
// =====================================================================

module tb_full_adder;

    // Testbench registers to drive stimulus (inputs to the DUTs)
    reg a;
    reg b;
    reg cin;

    // Wires to capture the output of each Design Under Test (DUT)
    wire sum_gates, cout_gates;
    wire sum_assign, cout_assign;
    wire sum_ifelse, cout_ifelse;
    wire sum_case, cout_case;

    // ---------------------------------------------------------
    // Instantiate all 4 Design Under Test (DUT) variations
    // ---------------------------------------------------------
    
    // Module 1: Gate-Level Full Adder
    full_adder_gates dut1_gates (
        .a(a), .b(b), .cin(cin),
        .sum(sum_gates), .cout(cout_gates)
    );

    // Module 2: Dataflow (assign) Full Adder
    full_adder_assign dut2_assign (
        .a(a), .b(b), .cin(cin),
        .sum(sum_assign), .cout(cout_assign)
    );

    // Module 3: Behavioral (if-else) Full Adder
    full_adder_ifelse dut3_ifelse (
        .a(a), .b(b), .cin(cin),
        .sum(sum_ifelse), .cout(cout_ifelse)
    );

    // Module 4: Behavioral (case) Full Adder
    full_adder_case dut4_case (
        .a(a), .b(b), .cin(cin),
        .sum(sum_case), .cout(cout_case)
    );

    // ---------------------------------------------------------
    // Stimulus Generation Block
    // ---------------------------------------------------------
    initial begin
        // Display header formatting for the terminal output
        $display("\n===================================================================================");
        $display("   TESTING FULL ADDER IMPLEMENTATIONS: ALL 8 PERMUTATIONS");
        $display("===================================================================================");
        $display("Inputs: A B Cin | Gate (S,Co) | Assign (S,Co) | If-Else (S,Co) | Case (S,Co)");
        $display("-----------------------------------------------------------------------------------");

        // Initialize inputs
        {a, b, cin} = 3'b000;
        
        // Loop through all 8 possible input states (000 to 111)
        repeat (8) begin
            #10; // Wait 10ns for combinational logic to settle and calculate
            
            // Print the outputs side-by-side for visual inspection
            $display("        %b %b  %b  |    (%b,%b)    |     (%b,%b)     |      (%b,%b)     |    (%b,%b)", 
                      a, b, cin, 
                      sum_gates, cout_gates,
                      sum_assign, cout_assign,
                      sum_ifelse, cout_ifelse,
                      sum_case, cout_case);
            
            // Safety Assertion: Automatically check if any logic module fails to match the others
            if ({cout_gates, sum_gates} !== {cout_assign, sum_assign} ||
                {cout_gates, sum_gates} !== {cout_ifelse, sum_ifelse} ||
                {cout_gates, sum_gates} !== {cout_case, sum_case}) begin
                $display("ERROR: Mathematical mismatch detected at input %b%b%b!", a, b, cin);
            end

            // Increment inputs to test the next state in the truth table
            {a, b, cin} = {a, b, cin} + 1;
        end
        
        $display("===================================================================================");
        $display("All Adder logic simulations completed successfully. All hardware modules match!");
        $display("===================================================================================\n");
        
        $finish; // End Verilog simulation
    end

endmodule
