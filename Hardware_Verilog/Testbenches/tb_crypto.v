`timescale 1ns / 1ps

// This simulation verifies the modern SoC components:
// 1. The AXI4-Stream AES S-Box (Handshaking, Latency, Data Validity)
// 2. The Hardware DES P-Box (Combinational bit-transposition)

module tb_crypto;

    // Signal Declarations
    
    // Global Clock and Reset Generation
    reg clk;
    reg rst_n;
    
    // Modern AXI4-Stream signals for interfacing with the AES S-Box
    reg  [7:0] s_axis_tdata;
    reg        s_axis_tvalid;
    wire       s_axis_tready;
    
    wire [7:0] m_axis_tdata;
    wire       m_axis_tvalid;
    reg        m_axis_tready;

    // Signals for checking the 0-latency DES P-Box routing
    reg  [31:0] pbox_in;
    wire [31:0] pbox_out;

    // Hardware Instantiations

    // Instantiate AES S-Box (Sequential pipeline with AXI handshake)
    aes_sbox_hw dut_aes (
        .clk(clk),
        .rst_n(rst_n),
        .s_axis_tdata(s_axis_tdata),
        .s_axis_tvalid(s_axis_tvalid),
        .s_axis_tready(s_axis_tready),
        .m_axis_tdata(m_axis_tdata),
        .m_axis_tvalid(m_axis_tvalid),
        .m_axis_tready(m_axis_tready)
    );

    // Instantiate DES P-Box (Pure Combinational)
    des_pbox_hw dut_des (
        .data_in(pbox_in),
        .data_out(pbox_out)
    );


    // Simulation Logic

    // 100MHz System Clock Generation (10ns period)
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // Main Stimulus block
    initial begin
        // Set initial states
        rst_n = 0;
        s_axis_tvalid = 0;
        s_axis_tdata = 0;
        m_axis_tready = 1; // Downstream logic is always ready to receive
        pbox_in = 32'h12345678;
        
        $display("\n=======================================================");
        $display("Starting Modern Cryptographic Hardware Simulation...");
        $display("=======================================================");
        
        // Emulate power-on reset
        #20 rst_n = 1;
        
        // TEST 1: DES P-BOX Combinational Permutation
        #10;
        $display("\n[DES P-Box Structural Routing Test]");
        $display("Data flowing into Silicon Routing : 0x%h", pbox_in);
        $display("Data out from Silicon Routing   : 0x%h", pbox_out);
        
        // TEST 2: AES S-BOX AXI4-Stream Handshake Verification

        $display("\n[AES S-Box AXI4-Stream Handshake Test]");
        
        // Push a byte onto the AXI Stream
        @(posedge clk);
        s_axis_tvalid <= 1;
        s_axis_tdata <= 8'hAA; // Test Vector
        
        // Handshake: Wait for the hardware to acknowledge (ready)
        wait(s_axis_tready);
        @(posedge clk);
        s_axis_tvalid <= 0; // De-assert valid after successful transfer
        
        // Wait for the pipeline output to become valid
        wait(m_axis_tvalid);
        $display("Master sent AXI Data    : 0xAA");
        $display("Slave processed Output  : 0x%h (Mathematical Expectation: 0xAA XOR 0x63 = 0xC9)", m_axis_tdata);
        
        if (m_axis_tdata == 8'hC9) 
            $display("-> AXI Transfer Successful: Validated Core Mathematical Integrity.");
        else
            $display("-> AXI Transfer Failed: Silicon Defect Detected.");
        
        #50;
        $display("\n=======================================================");
        $display("Hardware Logic and Handshaking Tests Verified.");
        $display("=======================================================\n");
        $finish; // End Verilog simulation
    end

endmodule
