`timescale 1ns / 1ps

// Top-level Cryptographic Accelerator with Modern SoC Integration
// Demonstrates AXI4-Lite Memory Mapped interface for processor control
module crypto_top_axi (
    input wire clk,
    input wire rst_n,
    
    // Modern AXI Lite Control Interface (Used by ARM Cores to configure the Crypto IP)
    input  wire [31:0] axi_awaddr,
    input  wire        axi_awvalid,
    output wire        axi_awready,
    
    input  wire [31:0] axi_wdata,
    input  wire        axi_wvalid,
    output wire        axi_wready
);

    // Basic logic gates for handshaking synchronization (AND gate)
    assign axi_awready = axi_awvalid & axi_wvalid;
    assign axi_wready  = axi_awvalid & axi_wvalid;
    
    // Internal Control Registers
    reg [31:0] ctrl_reg;
    reg [31:0] key_reg_0;
    
    // Synchronous state machine logic for SoC interface writes
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ctrl_reg  <= 32'h0;
            key_reg_0 <= 32'h0;
        end else if (axi_awready && axi_wready) begin
            // Address decoding using multiplexer logic
            case (axi_awaddr[7:0])
                8'h00: ctrl_reg  <= axi_wdata;
                8'h04: key_reg_0 <= axi_wdata;
            endcase
        end
    end

endmodule
