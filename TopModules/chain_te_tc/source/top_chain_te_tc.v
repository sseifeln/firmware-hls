`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 08/08/2019 01:14:35 PM
// Design Name:
// Module Name: top_chain_te_tc
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////


module top_chain_te_tc(
    input clk_p,
    input clk_n,
    input sw_N,
    input sw_E,
    input sw_S,
    input sw_W,
    input sw_C,
    output LED0,
    output LED1,
    output LED2,
    output LED3,
    output LED4,
    output LED5,
    output LED6,
    output LED7
    );

    parameter RESET = 3'b0;
    parameter START = 3'b1;
    parameter CHECK = 3'b10;
    parameter HOLD = 3'b11;
    parameter READ = 3'b100;

////////////////////////////////////////////////////////////////////////////////
// Clock output wires
////////////////////////////////////////////////////////////////////////////////
    wire clk_200;
    wire fast_clk;
    wire slow_clk;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// TC_L1L2G input wires
////////////////////////////////////////////////////////////////////////////////
    wire  [35:0]  tc_innerStubs_data;
    wire   [35:0]  tc_outerStubs_data;
    wire   [13:0]  tc_stubPairs_data;
    //wire  [7:0]   tc_stubPairs_nentries[12:0][1:0];
    reg   [7:0]   trackletParameters_nentries_in[3:0];
    reg   [7:0]   projout_L3PHIB_nentries_in[1:0];
    reg   [7:0]   projout_L3PHIC_nentries_in[1:0];
    reg   [7:0]   projout_L4PHIB_nentries_in[1:0];
    reg   [7:0]   projout_L4PHIC_nentries_in[1:0];
    reg   [7:0]   projout_L4PHID_nentries_in[1:0];
    reg   [7:0]   projout_L5PHIB_nentries_in[1:0];
    reg   [7:0]   projout_L5PHIC_nentries_in[1:0];
    reg   [7:0]   projout_L5PHID_nentries_in[1:0];
    reg   [7:0]   projout_L6PHIB_nentries_in[1:0];
    reg   [7:0]   projout_L6PHIC_nentries_in[1:0];
    reg   [7:0]   projout_L6PHID_nentries_in[1:0];
    reg   [7:0]   projout_D1PHIB_nentries_in[1:0];
    reg   [7:0]   projout_D1PHIC_nentries_in[1:0];
    reg   [7:0]   projout_D1PHID_nentries_in[1:0];
    reg   [7:0]   projout_D2PHIB_nentries_in[1:0];
    reg   [7:0]   projout_D2PHIC_nentries_in[1:0];
    reg   [7:0]   projout_D2PHID_nentries_in[1:0];
    reg   [7:0]   projout_D3PHIB_nentries_in[1:0];
    reg   [7:0]   projout_D3PHIC_nentries_in[1:0];
    reg   [7:0]   projout_D3PHID_nentries_in[1:0];
    reg   [7:0]   projout_D4PHIB_nentries_in[1:0];
    reg   [7:0]   projout_D4PHIC_nentries_in[1:0];
    reg   [7:0]   projout_D4PHID_nentries_in[1:0];
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// TC_L1L2G output wires
////////////////////////////////////////////////////////////////////////////////
    wire          tc_done;
    wire          tc_idle;
    wire          tc_ready;
    wire  [9:0]   tc_innerStubs_address;
    wire          tc_innerStubs_ce;
    wire  [10:0]  tc_outerStubs_address;
    wire          tc_outerStubs_memory;
    wire          tc_outerStubs_ce;
    wire  [11:0]  tc_stubPairs_address;
    wire  [3:0]   tc_stubPairs_memory;
    wire          tc_stubPairs_ce;
    wire  [8:0]   trackletParameters_address;
    wire          trackletParameters_ce;
    wire          trackletParameters_we;
    wire  [69:0]  trackletParameters_data;
    wire  [7:0]   trackletParameters_nentries_out[3:0];
    wire          trackletParameters_nentries_valid[3:0];
    wire  [7:0]   projout_L3PHIB_address;
    wire          projout_L3PHIB_ce;
    wire          projout_L3PHIB_we;
    wire  [59:0]  projout_L3PHIB_data;
    wire  [7:0]   projout_L3PHIB_nentries_out[1:0];
    wire          projout_L3PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_L3PHIC_address;
    wire          projout_L3PHIC_ce;
    wire          projout_L3PHIC_we;
    wire  [59:0]  projout_L3PHIC_data;
    wire  [7:0]   projout_L3PHIC_nentries_out[1:0];
    wire          projout_L3PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_L4PHIB_address;
    wire          projout_L4PHIB_ce;
    wire          projout_L4PHIB_we;
    wire  [57:0]  projout_L4PHIB_data;
    wire  [7:0]   projout_L4PHIB_nentries_out[1:0];
    wire          projout_L4PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_L4PHIC_address;
    wire          projout_L4PHIC_ce;
    wire          projout_L4PHIC_we;
    wire  [57:0]  projout_L4PHIC_data;
    wire  [7:0]   projout_L4PHIC_nentries_out[1:0];
    wire          projout_L4PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_L4PHID_address;
    wire          projout_L4PHID_ce;
    wire          projout_L4PHID_we;
    wire  [57:0]  projout_L4PHID_data;
    wire  [7:0]   projout_L4PHID_nentries_out[1:0];
    wire          projout_L4PHID_nentries_valid[1:0];
    wire  [7:0]   projout_L5PHIB_address;
    wire          projout_L5PHIB_ce;
    wire          projout_L5PHIB_we;
    wire  [57:0]  projout_L5PHIB_data;
    wire  [7:0]   projout_L5PHIB_nentries_out[1:0];
    wire          projout_L5PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_L5PHIC_address;
    wire          projout_L5PHIC_ce;
    wire          projout_L5PHIC_we;
    wire  [57:0]  projout_L5PHIC_data;
    wire  [7:0]   projout_L5PHIC_nentries_out[1:0];
    wire          projout_L5PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_L5PHID_address;
    wire          projout_L5PHID_ce;
    wire          projout_L5PHID_we;
    wire  [57:0]  projout_L5PHID_data;
    wire  [7:0]   projout_L5PHID_nentries_out[1:0];
    wire          projout_L5PHID_nentries_valid[1:0];
    wire  [7:0]   projout_L6PHIB_address;
    wire          projout_L6PHIB_ce;
    wire          projout_L6PHIB_we;
    wire  [57:0]  projout_L6PHIB_data;
    wire  [7:0]   projout_L6PHIB_nentries_out[1:0];
    wire          projout_L6PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_L6PHIC_address;
    wire          projout_L6PHIC_ce;
    wire          projout_L6PHIC_we;
    wire  [57:0]  projout_L6PHIC_data;
    wire  [7:0]   projout_L6PHIC_nentries_out[1:0];
    wire          projout_L6PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_L6PHID_address;
    wire          projout_L6PHID_ce;
    wire          projout_L6PHID_we;
    wire  [57:0]  projout_L6PHID_data;
    wire  [7:0]   projout_L6PHID_nentries_out[1:0];
    wire          projout_L6PHID_nentries_valid[1:0];
    wire  [7:0]   projout_D1PHIB_address;
    wire          projout_D1PHIB_ce;
    wire          projout_D1PHIB_we;
    wire  [58:0]  projout_D1PHIB_data;
    wire  [7:0]   projout_D1PHIB_nentries_out[1:0];
    wire          projout_D1PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_D1PHIC_address;
    wire          projout_D1PHIC_ce;
    wire          projout_D1PHIC_we;
    wire  [58:0]  projout_D1PHIC_data;
    wire  [7:0]   projout_D1PHIC_nentries_out[1:0];
    wire          projout_D1PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_D1PHID_address;
    wire          projout_D1PHID_ce;
    wire          projout_D1PHID_we;
    wire  [58:0]  projout_D1PHID_data;
    wire  [7:0]   projout_D1PHID_nentries_out[1:0];
    wire          projout_D1PHID_nentries_valid[1:0];
    wire  [7:0]   projout_D2PHIB_address;
    wire          projout_D2PHIB_ce;
    wire          projout_D2PHIB_we;
    wire  [58:0]  projout_D2PHIB_data;
    wire  [7:0]   projout_D2PHIB_nentries_out[1:0];
    wire          projout_D2PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_D2PHIC_address;
    wire          projout_D2PHIC_ce;
    wire          projout_D2PHIC_we;
    wire  [58:0]  projout_D2PHIC_data;
    wire  [7:0]   projout_D2PHIC_nentries_out[1:0];
    wire          projout_D2PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_D2PHID_address;
    wire          projout_D2PHID_ce;
    wire          projout_D2PHID_we;
    wire  [58:0]  projout_D2PHID_data;
    wire  [7:0]   projout_D2PHID_nentries_out[1:0];
    wire          projout_D2PHID_nentries_valid[1:0];
    wire  [7:0]   projout_D3PHIB_address;
    wire          projout_D3PHIB_ce;
    wire          projout_D3PHIB_we;
    wire  [58:0]  projout_D3PHIB_data;
    wire  [7:0]   projout_D3PHIB_nentries_out[1:0];
    wire          projout_D3PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_D3PHIC_address;
    wire          projout_D3PHIC_ce;
    wire          projout_D3PHIC_we;
    wire  [58:0]  projout_D3PHIC_data;
    wire  [7:0]   projout_D3PHIC_nentries_out[1:0];
    wire          projout_D3PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_D3PHID_address;
    wire          projout_D3PHID_ce;
    wire          projout_D3PHID_we;
    wire  [58:0]  projout_D3PHID_data;
    wire  [7:0]   projout_D3PHID_nentries_out[1:0];
    wire          projout_D3PHID_nentries_valid[1:0];
    wire  [7:0]   projout_D4PHIB_address;
    wire          projout_D4PHIB_ce;
    wire          projout_D4PHIB_we;
    wire  [58:0]  projout_D4PHIB_data;
    wire  [7:0]   projout_D4PHIB_nentries_out[1:0];
    wire          projout_D4PHIB_nentries_valid[1:0];
    wire  [7:0]   projout_D4PHIC_address;
    wire          projout_D4PHIC_ce;
    wire          projout_D4PHIC_we;
    wire  [58:0]  projout_D4PHIC_data;
    wire  [7:0]   projout_D4PHIC_nentries_out[1:0];
    wire          projout_D4PHIC_nentries_valid[1:0];
    wire  [7:0]   projout_D4PHID_address;
    wire          projout_D4PHID_ce;
    wire          projout_D4PHID_we;
    wire  [58:0]  projout_D4PHID_data;
    wire  [7:0]   projout_D4PHID_nentries_out[1:0];
    wire          projout_D4PHID_nentries_valid[1:0];
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// TE_L1PHIE18_L2PHIC17 input wires
////////////////////////////////////////////////////////////////////////////////
    wire  [21:0]  te_innerStubs_data;
    wire  [7:0]   te_innerStubs_nentries[1:0];
    wire  [15:0]  te_outerStubs_data;
    wire  [4:0]   te_outerStubs_nentries[3:0][7:0];
    reg   [7:0]   te_stubPairs_nentries_in[1:0];
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// TE_L1PHIE18_L2PHIC17 output wires
////////////////////////////////////////////////////////////////////////////////
    reg           te_start = 1'b0;
    wire          tc_start;
    wire          te_done;
    wire          te_idle;
    wire          te_ready;
    wire  [7:0]   te_innerStubs_address;
    wire          te_innerStubs_ce;
    wire  [8:0]   te_outerStubs_address;
    wire  [8:0]   te_outerStubs_index;
    wire          te_outerStubs_ce;
    wire  [7:0]   te_stubPairs_address;
    wire          te_stubPairs_ce;
    wire          te_stubPairs_we;
    wire  [13:0]  te_stubPairs_data;
    wire  [7:0]   te_stubPairs_nentries_out[1:0];
    wire          te_stubPairs_nentries_valid[1:0];
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Miscellaneus wires
////////////////////////////////////////////////////////////////////////////////
    reg [3:0] evt = 4'b0;
    wire [35:0] tc_outerStubs_data_0;
    wire [35:0] tc_outerStubs_data_1;
    wire [13:0] tc_stubPairs_data_0;
    wire [13:0] tc_stubPairs_data_1;
    wire [13:0] tc_stubPairs_data_2;
    wire [13:0] tc_stubPairs_data_3;
    wire [13:0] tc_stubPairs_data_4;
    wire [13:0] tc_stubPairs_data_5;
    wire [13:0] tc_stubPairs_data_6;
    wire [13:0] tc_stubPairs_data_7;
    wire [13:0] tc_stubPairs_data_8;
    wire [13:0] tc_stubPairs_data_9;
    wire [13:0] tc_stubPairs_data_10;
    wire [13:0] tc_stubPairs_data_11;
    wire [13:0] tc_stubPairs_data_12;
    reg [9:0] trackletParameters_expected_address = 10'b0;
    wire [69:0] trackletParameters_expected_data;
    wire [69:0] trackletParameters_actual_data;

// State register.
    reg [2:0] state = RESET;
// Registers for keeping track of button pressing.
    reg pressed_reset = 1'b0;
    reg pressed_start = 1'b0;
// Error counter.
    reg [7:0] trackletParameters_errors = 8'b0;
    reg [7:0] projout_L3PHIB_errors = 8'b0;
    reg [7:0] projout_L3PHIC_errors = 8'b0;
    reg [7:0] projout_L4PHIB_errors = 8'b0;
    reg [7:0] projout_L4PHIC_errors = 8'b0;
    reg [7:0] projout_L4PHID_errors = 8'b0;
    reg [7:0] projout_L5PHIB_errors = 8'b0;
    reg [7:0] projout_L5PHIC_errors = 8'b0;
    reg [7:0] projout_L5PHID_errors = 8'b0;
    reg [7:0] projout_L6PHIB_errors = 8'b0;
    reg [7:0] projout_L6PHIC_errors = 8'b0;
    reg [7:0] projout_L6PHID_errors = 8'b0;
    reg [7:0] projout_D1PHIB_errors = 8'b0;
    reg [7:0] projout_D1PHIC_errors = 8'b0;
    reg [7:0] projout_D1PHID_errors = 8'b0;
    reg [7:0] projout_D2PHIB_errors = 8'b0;
    reg [7:0] projout_D2PHIC_errors = 8'b0;
    reg [7:0] projout_D2PHID_errors = 8'b0;
    reg [7:0] projout_D3PHIB_errors = 8'b0;
    reg [7:0] projout_D3PHIC_errors = 8'b0;
    reg [7:0] projout_D3PHID_errors = 8'b0;
    reg [7:0] projout_D4PHIB_errors = 8'b0;
    reg [7:0] projout_D4PHIC_errors = 8'b0;
    reg [7:0] projout_D4PHID_errors = 8'b0;
    reg [30:0] n_errors = 31'b0;
    reg run = 1'b0;
    reg run_done = 1'b0;
    reg reset_run = 1'b0;
// LED registers.
    reg [7:0] LED = 8'b0;

    reg   [8:0]  projout_L3PHIB_expected_address = 9'b0;
    wire  [59:0]  projout_L3PHIB_expected_data;
    wire  [59:0]  projout_L3PHIB_actual_data;
    reg   [8:0]  projout_L3PHIC_expected_address = 9'b0;
    wire  [59:0]  projout_L3PHIC_expected_data;
    wire  [59:0]  projout_L3PHIC_actual_data;
    reg   [8:0]  projout_L4PHIB_expected_address = 9'b0;
    wire  [57:0]  projout_L4PHIB_expected_data;
    wire  [57:0]  projout_L4PHIB_actual_data;
    reg   [8:0]  projout_L4PHIC_expected_address = 9'b0;
    wire  [57:0]  projout_L4PHIC_expected_data;
    wire  [57:0]  projout_L4PHIC_actual_data;
    reg   [8:0]  projout_L4PHID_expected_address = 9'b0;
    wire  [57:0]  projout_L4PHID_expected_data;
    wire  [57:0]  projout_L4PHID_actual_data;
    reg   [8:0]  projout_L5PHIB_expected_address = 9'b0;
    wire  [57:0]  projout_L5PHIB_expected_data;
    wire  [57:0]  projout_L5PHIB_actual_data;
    reg   [8:0]  projout_L5PHIC_expected_address = 9'b0;
    wire  [57:0]  projout_L5PHIC_expected_data;
    wire  [57:0]  projout_L5PHIC_actual_data;
    reg   [8:0]  projout_L5PHID_expected_address = 9'b0;
    wire  [57:0]  projout_L5PHID_expected_data;
    wire  [57:0]  projout_L5PHID_actual_data;
    reg   [8:0]  projout_L6PHIB_expected_address = 9'b0;
    wire  [57:0]  projout_L6PHIB_expected_data;
    wire  [57:0]  projout_L6PHIB_actual_data;
    reg   [8:0]  projout_L6PHIC_expected_address = 9'b0;
    wire  [57:0]  projout_L6PHIC_expected_data;
    wire  [57:0]  projout_L6PHIC_actual_data;
    reg   [8:0]  projout_L6PHID_expected_address = 9'b0;
    wire  [57:0]  projout_L6PHID_expected_data;
    wire  [57:0]  projout_L6PHID_actual_data;
    reg   [8:0]  projout_D1PHIB_expected_address = 9'b0;
    wire  [58:0]  projout_D1PHIB_expected_data;
    wire  [58:0]  projout_D1PHIB_actual_data;
    reg   [8:0]  projout_D1PHIC_expected_address = 9'b0;
    wire  [58:0]  projout_D1PHIC_expected_data;
    wire  [58:0]  projout_D1PHIC_actual_data;
    reg   [8:0]  projout_D1PHID_expected_address = 9'b0;
    wire  [58:0]  projout_D1PHID_expected_data;
    wire  [58:0]  projout_D1PHID_actual_data;
    reg   [8:0]  projout_D2PHIB_expected_address = 9'b0;
    wire  [58:0]  projout_D2PHIB_expected_data;
    wire  [58:0]  projout_D2PHIB_actual_data;
    reg   [8:0]  projout_D2PHIC_expected_address = 9'b0;
    wire  [58:0]  projout_D2PHIC_expected_data;
    wire  [58:0]  projout_D2PHIC_actual_data;
    reg   [8:0]  projout_D2PHID_expected_address = 9'b0;
    wire  [58:0]  projout_D2PHID_expected_data;
    wire  [58:0]  projout_D2PHID_actual_data;
    reg   [8:0]  projout_D3PHIB_expected_address = 9'b0;
    wire  [58:0]  projout_D3PHIB_expected_data;
    wire  [58:0]  projout_D3PHIB_actual_data;
    reg   [8:0]  projout_D3PHIC_expected_address = 9'b0;
    wire  [58:0]  projout_D3PHIC_expected_data;
    wire  [58:0]  projout_D3PHIC_actual_data;
    reg   [8:0]  projout_D3PHID_expected_address = 9'b0;
    wire  [58:0]  projout_D3PHID_expected_data;
    wire  [58:0]  projout_D3PHID_actual_data;
    reg   [8:0]  projout_D4PHIB_expected_address = 9'b0;
    wire  [58:0]  projout_D4PHIB_expected_data;
    wire  [58:0]  projout_D4PHIB_actual_data;
    reg   [8:0]  projout_D4PHIC_expected_address = 9'b0;
    wire  [58:0]  projout_D4PHIC_expected_data;
    wire  [58:0]  projout_D4PHIC_actual_data;
    reg   [8:0]  projout_D4PHID_expected_address = 9'b0;
    wire  [58:0]  projout_D4PHID_expected_data;
    wire  [58:0]  projout_D4PHID_actual_data;

////////////////////////////////////////////////////////////////////////////////

    always @(posedge slow_clk) begin
      case (state)
        RESET: begin
          if ((sw_N || sw_E || sw_S || sw_W) && !sw_C) begin
            if (!pressed_start) begin
              LED[0] <= 1'b1;
              run <= 1'b1;
              reset_run <= 1'b0;
              state <= START;
              pressed_start <= 1'b1;
            end
          end
          else begin
            pressed_start <= 1'b0;
          end
        end

        START: begin
          if (run_done) begin
            run <= 1'b0;
            state <= CHECK;
          end
        end

        HOLD: begin
          if (sw_C && !(sw_N || sw_E || sw_S || sw_W)) begin
            if (!pressed_reset) begin
              trackletParameters_expected_address <= 10'b0;
              projout_L3PHIB_expected_address <= 9'b0;
              projout_L3PHIC_expected_address <= 9'b0;
              projout_L4PHIB_expected_address <= 9'b0;
              projout_L4PHIC_expected_address <= 9'b0;
              projout_L4PHID_expected_address <= 9'b0;
              projout_L5PHIB_expected_address <= 9'b0;
              projout_L5PHIC_expected_address <= 9'b0;
              projout_L5PHID_expected_address <= 9'b0;
              projout_L6PHIB_expected_address <= 9'b0;
              projout_L6PHIC_expected_address <= 9'b0;
              projout_L6PHID_expected_address <= 9'b0;
              projout_D1PHIB_expected_address <= 9'b0;
              projout_D1PHIC_expected_address <= 9'b0;
              projout_D1PHID_expected_address <= 9'b0;
              projout_D2PHIB_expected_address <= 9'b0;
              projout_D2PHIC_expected_address <= 9'b0;
              projout_D2PHID_expected_address <= 9'b0;
              projout_D3PHIB_expected_address <= 9'b0;
              projout_D3PHIC_expected_address <= 9'b0;
              projout_D3PHID_expected_address <= 9'b0;
              projout_D4PHIB_expected_address <= 9'b0;
              projout_D4PHIC_expected_address <= 9'b0;
              projout_D4PHID_expected_address <= 9'b0;
              trackletParameters_errors <= 8'b0;
              projout_L3PHIB_errors <= 8'b0;
              projout_L3PHIC_errors <= 8'b0;
              projout_L4PHIB_errors <= 8'b0;
              projout_L4PHIC_errors <= 8'b0;
              projout_L4PHID_errors <= 8'b0;
              projout_L5PHIB_errors <= 8'b0;
              projout_L5PHIC_errors <= 8'b0;
              projout_L5PHID_errors <= 8'b0;
              projout_L6PHIB_errors <= 8'b0;
              projout_L6PHIC_errors <= 8'b0;
              projout_L6PHID_errors <= 8'b0;
              projout_D1PHIB_errors <= 8'b0;
              projout_D1PHIC_errors <= 8'b0;
              projout_D1PHID_errors <= 8'b0;
              projout_D2PHIB_errors <= 8'b0;
              projout_D2PHIC_errors <= 8'b0;
              projout_D2PHID_errors <= 8'b0;
              projout_D3PHIB_errors <= 8'b0;
              projout_D3PHIC_errors <= 8'b0;
              projout_D3PHID_errors <= 8'b0;
              projout_D4PHIB_errors <= 8'b0;
              projout_D4PHIC_errors <= 8'b0;
              projout_D4PHID_errors <= 8'b0;
              n_errors <= 31'b0;
              LED <= 8'b0;
              state <= RESET;
              pressed_reset <= 1'b1;
            end
          end
          else begin
            pressed_reset <= 1'b0;
          end
        end

        CHECK: begin
          trackletParameters_expected_address <= trackletParameters_expected_address + 1;
          if (projout_L3PHIB_expected_address <= projout_L3PHIB_nentries_out[0] + 1) begin
              projout_L3PHIB_expected_address <= projout_L3PHIB_expected_address + 1;
          end
          if (projout_L3PHIC_expected_address <= projout_L3PHIC_nentries_out[0] + 1) begin
              projout_L3PHIC_expected_address <= projout_L3PHIC_expected_address + 1;
          end
          if (projout_L4PHIB_expected_address <= projout_L4PHIB_nentries_out[0] + 1) begin
              projout_L4PHIB_expected_address <= projout_L4PHIB_expected_address + 1;
          end
          if (projout_L4PHIC_expected_address <= projout_L4PHIC_nentries_out[0] + 1) begin
              projout_L4PHIC_expected_address <= projout_L4PHIC_expected_address + 1;
          end
          if (projout_L4PHID_expected_address <= projout_L4PHID_nentries_out[0] + 1) begin
              projout_L4PHID_expected_address <= projout_L4PHID_expected_address + 1;
          end
          if (projout_L5PHIB_expected_address <= projout_L5PHIB_nentries_out[0] + 1) begin
              projout_L5PHIB_expected_address <= projout_L5PHIB_expected_address + 1;
          end
          if (projout_L5PHIC_expected_address <= projout_L5PHIC_nentries_out[0] + 1) begin
              projout_L5PHIC_expected_address <= projout_L5PHIC_expected_address + 1;
          end
          if (projout_L5PHID_expected_address <= projout_L5PHID_nentries_out[0] + 1) begin
              projout_L5PHID_expected_address <= projout_L5PHID_expected_address + 1;
          end
          if (projout_L6PHIB_expected_address <= projout_L6PHIB_nentries_out[0] + 1) begin
              projout_L6PHIB_expected_address <= projout_L6PHIB_expected_address + 1;
          end
          if (projout_L6PHIC_expected_address <= projout_L6PHIC_nentries_out[0] + 1) begin
              projout_L6PHIC_expected_address <= projout_L6PHIC_expected_address + 1;
          end
          if (projout_L6PHID_expected_address <= projout_L6PHID_nentries_out[0] + 1) begin
              projout_L6PHID_expected_address <= projout_L6PHID_expected_address + 1;
          end
          if (projout_D1PHIB_expected_address <= projout_D1PHIB_nentries_out[0] + 1) begin
              projout_D1PHIB_expected_address <= projout_D1PHIB_expected_address + 1;
          end
          if (projout_D1PHIC_expected_address <= projout_D1PHIC_nentries_out[0] + 1) begin
              projout_D1PHIC_expected_address <= projout_D1PHIC_expected_address + 1;
          end
          if (projout_D1PHID_expected_address <= projout_D1PHID_nentries_out[0] + 1) begin
              projout_D1PHID_expected_address <= projout_D1PHID_expected_address + 1;
          end
          if (projout_D2PHIB_expected_address <= projout_D2PHIB_nentries_out[0] + 1) begin
              projout_D2PHIB_expected_address <= projout_D2PHIB_expected_address + 1;
          end
          if (projout_D2PHIC_expected_address <= projout_D2PHIC_nentries_out[0] + 1) begin
              projout_D2PHIC_expected_address <= projout_D2PHIC_expected_address + 1;
          end
          if (projout_D2PHID_expected_address <= projout_D2PHID_nentries_out[0] + 1) begin
              projout_D2PHID_expected_address <= projout_D2PHID_expected_address + 1;
          end
          if (projout_D3PHIB_expected_address <= projout_D3PHIB_nentries_out[0] + 1) begin
              projout_D3PHIB_expected_address <= projout_D3PHIB_expected_address + 1;
          end
          if (projout_D3PHIC_expected_address <= projout_D3PHIC_nentries_out[0] + 1) begin
              projout_D3PHIC_expected_address <= projout_D3PHIC_expected_address + 1;
          end
          if (projout_D3PHID_expected_address <= projout_D3PHID_nentries_out[0] + 1) begin
              projout_D3PHID_expected_address <= projout_D3PHID_expected_address + 1;
          end
          if (projout_D4PHIB_expected_address <= projout_D4PHIB_nentries_out[0] + 1) begin
              projout_D4PHIB_expected_address <= projout_D4PHIB_expected_address + 1;
          end
          if (projout_D4PHIC_expected_address <= projout_D4PHIC_nentries_out[0] + 1) begin
              projout_D4PHIC_expected_address <= projout_D4PHIC_expected_address + 1;
          end
          if (projout_D4PHID_expected_address <= projout_D4PHID_nentries_out[0] + 1) begin
              projout_D4PHID_expected_address <= projout_D4PHID_expected_address + 1;
          end
          if (trackletParameters_expected_address > 0 && trackletParameters_expected_address <= trackletParameters_nentries_out[0] && trackletParameters_actual_data != trackletParameters_expected_data) begin
            trackletParameters_errors <= trackletParameters_errors + 1;
          end
          if (projout_L3PHIB_expected_address > 0 && projout_L3PHIB_expected_address <= projout_L3PHIB_nentries_out[0] && projout_L3PHIB_actual_data != projout_L3PHIB_expected_data) begin
            projout_L3PHIB_errors <= projout_L3PHIB_errors + 1;
          end
          if (projout_L3PHIC_expected_address > 0 && projout_L3PHIC_expected_address <= projout_L3PHIC_nentries_out[0] && projout_L3PHIC_actual_data != projout_L3PHIC_expected_data) begin
            projout_L3PHIC_errors <= projout_L3PHIC_errors + 1;
          end
          if (projout_L4PHIB_expected_address > 0 && projout_L4PHIB_expected_address <= projout_L4PHIB_nentries_out[0] && projout_L4PHIB_actual_data != projout_L4PHIB_expected_data) begin
            projout_L4PHIB_errors <= projout_L4PHIB_errors + 1;
          end
          if (projout_L4PHIC_expected_address > 0 && projout_L4PHIC_expected_address <= projout_L4PHIC_nentries_out[0] && projout_L4PHIC_actual_data != projout_L4PHIC_expected_data) begin
            projout_L4PHIC_errors <= projout_L4PHIC_errors + 1;
          end
          if (projout_L4PHID_expected_address > 0 && projout_L4PHID_expected_address <= projout_L4PHID_nentries_out[0] && projout_L4PHID_actual_data != projout_L4PHID_expected_data) begin
            projout_L4PHID_errors <= projout_L4PHID_errors + 1;
          end
          if (projout_L5PHIB_expected_address > 0 && projout_L5PHIB_expected_address <= projout_L5PHIB_nentries_out[0] && projout_L5PHIB_actual_data != projout_L5PHIB_expected_data) begin
            projout_L5PHIB_errors <= projout_L5PHIB_errors + 1;
          end
          if (projout_L5PHIC_expected_address > 0 && projout_L5PHIC_expected_address <= projout_L5PHIC_nentries_out[0] && projout_L5PHIC_actual_data != projout_L5PHIC_expected_data) begin
            projout_L5PHIC_errors <= projout_L5PHIC_errors + 1;
          end
          if (projout_L5PHID_expected_address > 0 && projout_L5PHID_expected_address <= projout_L5PHID_nentries_out[0] && projout_L5PHID_actual_data != projout_L5PHID_expected_data) begin
            projout_L5PHID_errors <= projout_L5PHID_errors + 1;
          end
          if (projout_L6PHIB_expected_address > 0 && projout_L6PHIB_expected_address <= projout_L6PHIB_nentries_out[0] && projout_L6PHIB_actual_data != projout_L6PHIB_expected_data) begin
            projout_L6PHIB_errors <= projout_L6PHIB_errors + 1;
          end
          if (projout_L6PHIC_expected_address > 0 && projout_L6PHIC_expected_address <= projout_L6PHIC_nentries_out[0] && projout_L6PHIC_actual_data != projout_L6PHIC_expected_data) begin
            projout_L6PHIC_errors <= projout_L6PHIC_errors + 1;
          end
          if (projout_L6PHID_expected_address > 0 && projout_L6PHID_expected_address <= projout_L6PHID_nentries_out[0] && projout_L6PHID_actual_data != projout_L6PHID_expected_data) begin
            projout_L6PHID_errors <= projout_L6PHID_errors + 1;
          end
          if (projout_D1PHIB_expected_address > 0 && projout_D1PHIB_expected_address <= projout_D1PHIB_nentries_out[0] && projout_D1PHIB_actual_data != projout_D1PHIB_expected_data) begin
            projout_D1PHIB_errors <= projout_D1PHIB_errors + 1;
          end
          if (projout_D1PHIC_expected_address > 0 && projout_D1PHIC_expected_address <= projout_D1PHIC_nentries_out[0] && projout_D1PHIC_actual_data != projout_D1PHIC_expected_data) begin
            projout_D1PHIC_errors <= projout_D1PHIC_errors + 1;
          end
          if (projout_D1PHID_expected_address > 0 && projout_D1PHID_expected_address <= projout_D1PHID_nentries_out[0] && projout_D1PHID_actual_data != projout_D1PHID_expected_data) begin
            projout_D1PHID_errors <= projout_D1PHID_errors + 1;
          end
          if (projout_D2PHIB_expected_address > 0 && projout_D2PHIB_expected_address <= projout_D2PHIB_nentries_out[0] && projout_D2PHIB_actual_data != projout_D2PHIB_expected_data) begin
            projout_D2PHIB_errors <= projout_D2PHIB_errors + 1;
          end
          if (projout_D2PHIC_expected_address > 0 && projout_D2PHIC_expected_address <= projout_D2PHIC_nentries_out[0] && projout_D2PHIC_actual_data != projout_D2PHIC_expected_data) begin
            projout_D2PHIC_errors <= projout_D2PHIC_errors + 1;
          end
          if (projout_D2PHID_expected_address > 0 && projout_D2PHID_expected_address <= projout_D2PHID_nentries_out[0] && projout_D2PHID_actual_data != projout_D2PHID_expected_data) begin
            projout_D2PHID_errors <= projout_D2PHID_errors + 1;
          end
          if (projout_D3PHIB_expected_address > 0 && projout_D3PHIB_expected_address <= projout_D3PHIB_nentries_out[0] && projout_D3PHIB_actual_data != projout_D3PHIB_expected_data) begin
            projout_D3PHIB_errors <= projout_D3PHIB_errors + 1;
          end
          if (projout_D3PHIC_expected_address > 0 && projout_D3PHIC_expected_address <= projout_D3PHIC_nentries_out[0] && projout_D3PHIC_actual_data != projout_D3PHIC_expected_data) begin
            projout_D3PHIC_errors <= projout_D3PHIC_errors + 1;
          end
          if (projout_D3PHID_expected_address > 0 && projout_D3PHID_expected_address <= projout_D3PHID_nentries_out[0] && projout_D3PHID_actual_data != projout_D3PHID_expected_data) begin
            projout_D3PHID_errors <= projout_D3PHID_errors + 1;
          end
          if (projout_D4PHIB_expected_address > 0 && projout_D4PHIB_expected_address <= projout_D4PHIB_nentries_out[0] && projout_D4PHIB_actual_data != projout_D4PHIB_expected_data) begin
            projout_D4PHIB_errors <= projout_D4PHIB_errors + 1;
          end
          if (projout_D4PHIC_expected_address > 0 && projout_D4PHIC_expected_address <= projout_D4PHIC_nentries_out[0] && projout_D4PHIC_actual_data != projout_D4PHIC_expected_data) begin
            projout_D4PHIC_errors <= projout_D4PHIC_errors + 1;
          end
          if (projout_D4PHID_expected_address > 0 && projout_D4PHID_expected_address <= projout_D4PHID_nentries_out[0] && projout_D4PHID_actual_data != projout_D4PHID_expected_data) begin
            projout_D4PHID_errors <= projout_D4PHID_errors + 1;
          end
          if (LED[0] == 1'b1 && trackletParameters_expected_address >= 512 + 1) begin
            LED[0] <= 1'b0;
            n_errors <= trackletParameters_errors
                      + projout_L3PHIB_errors
                      + projout_L3PHIC_errors
                      + projout_L4PHIB_errors
                      + projout_L4PHIC_errors
                      + projout_L4PHID_errors
                      + projout_L5PHIB_errors
                      + projout_L5PHIC_errors
                      + projout_L5PHID_errors
                      + projout_L6PHIB_errors
                      + projout_L6PHIC_errors
                      + projout_L6PHID_errors
                      + projout_D1PHIB_errors
                      + projout_D1PHIC_errors
                      + projout_D1PHID_errors
                      + projout_D2PHIB_errors
                      + projout_D2PHIC_errors
                      + projout_D2PHID_errors
                      + projout_D3PHIB_errors
                      + projout_D3PHIC_errors
                      + projout_D3PHID_errors
                      + projout_D4PHIB_errors
                      + projout_D4PHIC_errors
                      + projout_D4PHID_errors;
          end
          if (LED[0] == 1'b0) begin
            if (n_errors <= 7'b1111111) begin
              LED[1] <= n_errors[0];
              LED[2] <= n_errors[1];
              LED[3] <= n_errors[2];
              LED[4] <= n_errors[3];
              LED[5] <= n_errors[4];
              LED[6] <= n_errors[5];
              LED[7] <= n_errors[6];
            end
            else begin
              LED[1] <= 1'b1;
              LED[2] <= 1'b1;
              LED[3] <= 1'b1;
              LED[4] <= 1'b1;
              LED[5] <= 1'b1;
              LED[6] <= 1'b1;
              LED[7] <= 1'b1;
            end
            reset_run <= 1'b1;
            state <= HOLD;
          end
        end

        default: begin
          state <= RESET;
        end
      endcase
    end

    always @(posedge fast_clk) begin
      if (run) begin
        if (evt < 1) begin
          te_start <= 1'b1;
          evt <= evt + 1;
        end
        else begin
          te_start <= 1'b0;
          if (tc_done) begin
            run_done <= 1'b1;
          end
        end
        trackletParameters_nentries_in[0] <= trackletParameters_nentries_out[0];
        trackletParameters_nentries_in[1] <= trackletParameters_nentries_out[1];
        trackletParameters_nentries_in[2] <= trackletParameters_nentries_out[2];
        trackletParameters_nentries_in[3] <= trackletParameters_nentries_out[3];
        te_stubPairs_nentries_in[0] <= te_stubPairs_nentries_out[0];
        te_stubPairs_nentries_in[1] <= te_stubPairs_nentries_out[1];
        projout_L3PHIB_nentries_in[0] <= projout_L3PHIB_nentries_out[0];
        projout_L3PHIB_nentries_in[1] <= projout_L3PHIB_nentries_out[1];
        projout_L3PHIC_nentries_in[0] <= projout_L3PHIC_nentries_out[0];
        projout_L3PHIC_nentries_in[1] <= projout_L3PHIC_nentries_out[1];
        projout_L4PHIB_nentries_in[0] <= projout_L4PHIB_nentries_out[0];
        projout_L4PHIB_nentries_in[1] <= projout_L4PHIB_nentries_out[1];
        projout_L4PHIC_nentries_in[0] <= projout_L4PHIC_nentries_out[0];
        projout_L4PHIC_nentries_in[1] <= projout_L4PHIC_nentries_out[1];
        projout_L4PHID_nentries_in[0] <= projout_L4PHID_nentries_out[0];
        projout_L4PHID_nentries_in[1] <= projout_L4PHID_nentries_out[1];
        projout_L5PHIB_nentries_in[0] <= projout_L5PHIB_nentries_out[0];
        projout_L5PHIB_nentries_in[1] <= projout_L5PHIB_nentries_out[1];
        projout_L5PHIC_nentries_in[0] <= projout_L5PHIC_nentries_out[0];
        projout_L5PHIC_nentries_in[1] <= projout_L5PHIC_nentries_out[1];
        projout_L5PHID_nentries_in[0] <= projout_L5PHID_nentries_out[0];
        projout_L5PHID_nentries_in[1] <= projout_L5PHID_nentries_out[1];
        projout_L6PHIB_nentries_in[0] <= projout_L6PHIB_nentries_out[0];
        projout_L6PHIB_nentries_in[1] <= projout_L6PHIB_nentries_out[1];
        projout_L6PHIC_nentries_in[0] <= projout_L6PHIC_nentries_out[0];
        projout_L6PHIC_nentries_in[1] <= projout_L6PHIC_nentries_out[1];
        projout_L6PHID_nentries_in[0] <= projout_L6PHID_nentries_out[0];
        projout_L6PHID_nentries_in[1] <= projout_L6PHID_nentries_out[1];
        projout_D1PHIB_nentries_in[0] <= projout_D1PHIB_nentries_out[0];
        projout_D1PHIB_nentries_in[1] <= projout_D1PHIB_nentries_out[1];
        projout_D1PHIC_nentries_in[0] <= projout_D1PHIC_nentries_out[0];
        projout_D1PHIC_nentries_in[1] <= projout_D1PHIC_nentries_out[1];
        projout_D1PHID_nentries_in[0] <= projout_D1PHID_nentries_out[0];
        projout_D1PHID_nentries_in[1] <= projout_D1PHID_nentries_out[1];
        projout_D2PHIB_nentries_in[0] <= projout_D2PHIB_nentries_out[0];
        projout_D2PHIB_nentries_in[1] <= projout_D2PHIB_nentries_out[1];
        projout_D2PHIC_nentries_in[0] <= projout_D2PHIC_nentries_out[0];
        projout_D2PHIC_nentries_in[1] <= projout_D2PHIC_nentries_out[1];
        projout_D2PHID_nentries_in[0] <= projout_D2PHID_nentries_out[0];
        projout_D2PHID_nentries_in[1] <= projout_D2PHID_nentries_out[1];
        projout_D3PHIB_nentries_in[0] <= projout_D3PHIB_nentries_out[0];
        projout_D3PHIB_nentries_in[1] <= projout_D3PHIB_nentries_out[1];
        projout_D3PHIC_nentries_in[0] <= projout_D3PHIC_nentries_out[0];
        projout_D3PHIC_nentries_in[1] <= projout_D3PHIC_nentries_out[1];
        projout_D3PHID_nentries_in[0] <= projout_D3PHID_nentries_out[0];
        projout_D3PHID_nentries_in[1] <= projout_D3PHID_nentries_out[1];
        projout_D4PHIB_nentries_in[0] <= projout_D4PHIB_nentries_out[0];
        projout_D4PHIB_nentries_in[1] <= projout_D4PHIB_nentries_out[1];
        projout_D4PHIC_nentries_in[0] <= projout_D4PHIC_nentries_out[0];
        projout_D4PHIC_nentries_in[1] <= projout_D4PHIC_nentries_out[1];
        projout_D4PHID_nentries_in[0] <= projout_D4PHID_nentries_out[0];
        projout_D4PHID_nentries_in[1] <= projout_D4PHID_nentries_out[1];
      end

      if (reset_run) begin
        evt <= 4'b0;
        run_done <= 1'b0;
        trackletParameters_nentries_in[0] <= 8'b0;
        trackletParameters_nentries_in[1] <= 8'b0;
        trackletParameters_nentries_in[2] <= 8'b0;
        trackletParameters_nentries_in[3] <= 8'b0;
        te_stubPairs_nentries_in[0] <= 8'b0;
        te_stubPairs_nentries_in[1] <= 8'b0;
        projout_L3PHIB_nentries_in[0] <= 8'b0;
        projout_L3PHIB_nentries_in[1] <= 8'b0;
        projout_L3PHIC_nentries_in[0] <= 8'b0;
        projout_L3PHIC_nentries_in[1] <= 8'b0;
        projout_L4PHIB_nentries_in[0] <= 8'b0;
        projout_L4PHIB_nentries_in[1] <= 8'b0;
        projout_L4PHIC_nentries_in[0] <= 8'b0;
        projout_L4PHIC_nentries_in[1] <= 8'b0;
        projout_L4PHID_nentries_in[0] <= 8'b0;
        projout_L4PHID_nentries_in[1] <= 8'b0;
        projout_L5PHIB_nentries_in[0] <= 8'b0;
        projout_L5PHIB_nentries_in[1] <= 8'b0;
        projout_L5PHIC_nentries_in[0] <= 8'b0;
        projout_L5PHIC_nentries_in[1] <= 8'b0;
        projout_L5PHID_nentries_in[0] <= 8'b0;
        projout_L5PHID_nentries_in[1] <= 8'b0;
        projout_L6PHIB_nentries_in[0] <= 8'b0;
        projout_L6PHIB_nentries_in[1] <= 8'b0;
        projout_L6PHIC_nentries_in[0] <= 8'b0;
        projout_L6PHIC_nentries_in[1] <= 8'b0;
        projout_L6PHID_nentries_in[0] <= 8'b0;
        projout_L6PHID_nentries_in[1] <= 8'b0;
        projout_D1PHIB_nentries_in[0] <= 8'b0;
        projout_D1PHIB_nentries_in[1] <= 8'b0;
        projout_D1PHIC_nentries_in[0] <= 8'b0;
        projout_D1PHIC_nentries_in[1] <= 8'b0;
        projout_D1PHID_nentries_in[0] <= 8'b0;
        projout_D1PHID_nentries_in[1] <= 8'b0;
        projout_D2PHIB_nentries_in[0] <= 8'b0;
        projout_D2PHIB_nentries_in[1] <= 8'b0;
        projout_D2PHIC_nentries_in[0] <= 8'b0;
        projout_D2PHIC_nentries_in[1] <= 8'b0;
        projout_D2PHID_nentries_in[0] <= 8'b0;
        projout_D2PHID_nentries_in[1] <= 8'b0;
        projout_D3PHIB_nentries_in[0] <= 8'b0;
        projout_D3PHIB_nentries_in[1] <= 8'b0;
        projout_D3PHIC_nentries_in[0] <= 8'b0;
        projout_D3PHIC_nentries_in[1] <= 8'b0;
        projout_D3PHID_nentries_in[0] <= 8'b0;
        projout_D3PHID_nentries_in[1] <= 8'b0;
        projout_D4PHIB_nentries_in[0] <= 8'b0;
        projout_D4PHIB_nentries_in[1] <= 8'b0;
        projout_D4PHIC_nentries_in[0] <= 8'b0;
        projout_D4PHIC_nentries_in[1] <= 8'b0;
        projout_D4PHID_nentries_in[0] <= 8'b0;
        projout_D4PHID_nentries_in[1] <= 8'b0;
      end
    end

// Assign the LED registers to the LED outputs.
    assign LED0 = LED[0];
    assign LED1 = LED[1];
    assign LED2 = LED[2];
    assign LED3 = LED[3];
    assign LED4 = LED[4];
    assign LED5 = LED[5];
    assign LED6 = LED[6];
    assign LED7 = LED[7];

    assign te_outerStubs_index = {7'b0, te_outerStubs_address[6], te_outerStubs_address[4]};
    assign tc_outerStubs_data = (tc_outerStubs_memory == 1'b0) ? tc_outerStubs_data_0
                                                               : tc_outerStubs_data_1;
    assign tc_stubPairs_data = (tc_stubPairs_memory == 4'b0000) ? tc_stubPairs_data_0
                             : (tc_stubPairs_memory == 4'b0001) ? tc_stubPairs_data_1
                             : (tc_stubPairs_memory == 4'b0010) ? tc_stubPairs_data_2
                             : (tc_stubPairs_memory == 4'b0011) ? tc_stubPairs_data_3
                             : (tc_stubPairs_memory == 4'b0100) ? tc_stubPairs_data_4
                             : (tc_stubPairs_memory == 4'b0101) ? tc_stubPairs_data_5
                             : (tc_stubPairs_memory == 4'b0110) ? tc_stubPairs_data_6
                             : (tc_stubPairs_memory == 4'b0111) ? tc_stubPairs_data_7
                             : (tc_stubPairs_memory == 4'b1000) ? tc_stubPairs_data_8
                             : (tc_stubPairs_memory == 4'b1001) ? tc_stubPairs_data_9
                             : (tc_stubPairs_memory == 4'b1010) ? tc_stubPairs_data_10
                             : (tc_stubPairs_memory == 4'b1011) ? tc_stubPairs_data_11
                                                                : tc_stubPairs_data_12;

    initial begin
        trackletParameters_nentries_in[0] <= 8'b0;
        trackletParameters_nentries_in[1] <= 8'b0;
        trackletParameters_nentries_in[2] <= 8'b0;
        trackletParameters_nentries_in[3] <= 8'b0;
        te_stubPairs_nentries_in[0] <= 8'b0;
        te_stubPairs_nentries_in[1] <= 8'b0;
        projout_L3PHIB_nentries_in[0] <= 8'b0;
        projout_L3PHIB_nentries_in[1] <= 8'b0;
        projout_L3PHIC_nentries_in[0] <= 8'b0;
        projout_L3PHIC_nentries_in[1] <= 8'b0;
        projout_L4PHIB_nentries_in[0] <= 8'b0;
        projout_L4PHIB_nentries_in[1] <= 8'b0;
        projout_L4PHIC_nentries_in[0] <= 8'b0;
        projout_L4PHIC_nentries_in[1] <= 8'b0;
        projout_L4PHID_nentries_in[0] <= 8'b0;
        projout_L4PHID_nentries_in[1] <= 8'b0;
        projout_L5PHIB_nentries_in[0] <= 8'b0;
        projout_L5PHIB_nentries_in[1] <= 8'b0;
        projout_L5PHIC_nentries_in[0] <= 8'b0;
        projout_L5PHIC_nentries_in[1] <= 8'b0;
        projout_L5PHID_nentries_in[0] <= 8'b0;
        projout_L5PHID_nentries_in[1] <= 8'b0;
        projout_L6PHIB_nentries_in[0] <= 8'b0;
        projout_L6PHIB_nentries_in[1] <= 8'b0;
        projout_L6PHIC_nentries_in[0] <= 8'b0;
        projout_L6PHIC_nentries_in[1] <= 8'b0;
        projout_L6PHID_nentries_in[0] <= 8'b0;
        projout_L6PHID_nentries_in[1] <= 8'b0;
        projout_D1PHIB_nentries_in[0] <= 8'b0;
        projout_D1PHIB_nentries_in[1] <= 8'b0;
        projout_D1PHIC_nentries_in[0] <= 8'b0;
        projout_D1PHIC_nentries_in[1] <= 8'b0;
        projout_D1PHID_nentries_in[0] <= 8'b0;
        projout_D1PHID_nentries_in[1] <= 8'b0;
        projout_D2PHIB_nentries_in[0] <= 8'b0;
        projout_D2PHIB_nentries_in[1] <= 8'b0;
        projout_D2PHIC_nentries_in[0] <= 8'b0;
        projout_D2PHIC_nentries_in[1] <= 8'b0;
        projout_D2PHID_nentries_in[0] <= 8'b0;
        projout_D2PHID_nentries_in[1] <= 8'b0;
        projout_D3PHIB_nentries_in[0] <= 8'b0;
        projout_D3PHIB_nentries_in[1] <= 8'b0;
        projout_D3PHIC_nentries_in[0] <= 8'b0;
        projout_D3PHIC_nentries_in[1] <= 8'b0;
        projout_D3PHID_nentries_in[0] <= 8'b0;
        projout_D3PHID_nentries_in[1] <= 8'b0;
        projout_D4PHIB_nentries_in[0] <= 8'b0;
        projout_D4PHIB_nentries_in[1] <= 8'b0;
        projout_D4PHIC_nentries_in[0] <= 8'b0;
        projout_D4PHIC_nentries_in[1] <= 8'b0;
        projout_D4PHID_nentries_in[0] <= 8'b0;
        projout_D4PHID_nentries_in[1] <= 8'b0;
    end

    IBUFGDS #(
      .DIFF_TERM   ("TRUE"),
      .IBUF_LOW_PWR ("FALSE")
    ) diff_clk_200 (
      .I  (clk_p),
      .IB (clk_n),
      .O  (clk_200)
    );
    clk_wiz_0 clk_wiz (
       .clk_in1 (clk_200),

       .fast_clk (fast_clk),
       .slow_clk (slow_clk)
    );

    TrackletCalculator_L1L2G_0 TC_L1L2G (
        .ap_clk    (fast_clk),
        .ap_rst    (1'b0),
        .ap_start  (tc_start),
        .ap_done   (tc_done),
        .ap_idle   (tc_idle),
        .ap_ready  (tc_ready),
        .bx_V      (3'b0),

        .innerStubs_dataarray_data_V_address0  (tc_innerStubs_address),      //  output  [9:0]
        .innerStubs_dataarray_data_V_ce0       (tc_innerStubs_ce),           //  output
        .innerStubs_dataarray_data_V_q0        (tc_innerStubs_data),         //  input   [35:0]
        .innerStubs_0_nentries_0_V             (8'b00000000),  //  input   [7:0]
        .innerStubs_0_nentries_1_V             (8'b00000000),  //  input   [7:0]
        .innerStubs_0_nentries_2_V             (8'b00000000),  //  input   [7:0]
        .innerStubs_0_nentries_3_V             (8'b00000000),  //  input   [7:0]
        .innerStubs_0_nentries_4_V             (8'b00000000),  //  input   [7:0]
        .innerStubs_0_nentries_5_V             (8'b00000000),  //  input   [7:0]
        .innerStubs_0_nentries_6_V             (8'b00000000),  //  input   [7:0]
        .innerStubs_0_nentries_7_V             (8'b00000000),  //  input   [7:0]

        .outerStubs_dataarray_data_V_address0  (tc_outerStubs_address),         //  output  [10:0]
        .outerStubs_dataarray_data_V_ce0       (tc_outerStubs_ce),              //  output
        .outerStubs_dataarray_data_V_q0        (tc_outerStubs_data),            //  input   [35:0]
        .outerStubs_0_nentries_0_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_0_nentries_1_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_0_nentries_2_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_0_nentries_3_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_0_nentries_4_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_0_nentries_5_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_0_nentries_6_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_0_nentries_7_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_0_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_1_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_2_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_3_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_4_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_5_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_6_V             (8'b00000000),  //  input   [7:0]
        .outerStubs_1_nentries_7_V             (8'b00000000),  //  input   [7:0]

        .stubPairs_dataarray_data_V_address0  (tc_stubPairs_address),          //  output  [11:0]
        .stubPairs_dataarray_data_V_ce0       (tc_stubPairs_ce),               //  output
        .stubPairs_dataarray_data_V_q0        (tc_stubPairs_data),       //  input   [13:0]
        //.stubPairs_0_nentries_0_V             (tc_stubPairs_nentries[0][0]),   //  input   [7:0]
        //.stubPairs_0_nentries_1_V             (tc_stubPairs_nentries[0][1]),   //  input   [7:0]
        //.stubPairs_1_nentries_0_V             (tc_stubPairs_nentries[1][0]),   //  input   [7:0]
        //.stubPairs_1_nentries_1_V             (tc_stubPairs_nentries[1][1]),   //  input   [7:0]
        //.stubPairs_2_nentries_0_V             (tc_stubPairs_nentries[2][0]),   //  input   [7:0]
        //.stubPairs_2_nentries_1_V             (tc_stubPairs_nentries[2][1]),   //  input   [7:0]
        //.stubPairs_3_nentries_0_V             (tc_stubPairs_nentries[3][0]),   //  input   [7:0]
        //.stubPairs_3_nentries_1_V             (tc_stubPairs_nentries[3][1]),   //  input   [7:0]
        //.stubPairs_4_nentries_0_V             (tc_stubPairs_nentries[4][0]),   //  input   [7:0]
        //.stubPairs_4_nentries_1_V             (tc_stubPairs_nentries[4][1]),   //  input   [7:0]
        //.stubPairs_5_nentries_0_V             (tc_stubPairs_nentries[5][0]),   //  input   [7:0]
        //.stubPairs_5_nentries_1_V             (tc_stubPairs_nentries[5][1]),   //  input   [7:0]
        //.stubPairs_6_nentries_0_V             (tc_stubPairs_nentries[6][0]),   //  input   [7:0]
        //.stubPairs_6_nentries_1_V             (tc_stubPairs_nentries[6][1]),   //  input   [7:0]
        //.stubPairs_7_nentries_0_V             (tc_stubPairs_nentries[7][0]),   //  input   [7:0]
        //.stubPairs_7_nentries_1_V             (tc_stubPairs_nentries[7][1]),   //  input   [7:0]
        //.stubPairs_8_nentries_0_V             (tc_stubPairs_nentries[8][0]),   //  input   [7:0]
        //.stubPairs_8_nentries_1_V             (tc_stubPairs_nentries[8][1]),   //  input   [7:0]
        //.stubPairs_9_nentries_0_V             (tc_stubPairs_nentries[9][0]),   //  input   [7:0]
        //.stubPairs_9_nentries_1_V             (tc_stubPairs_nentries[9][1]),   //  input   [7:0]
        //.stubPairs_10_nentries_0_V            (tc_stubPairs_nentries[10][0]),  //  input   [7:0]
        //.stubPairs_10_nentries_1_V            (tc_stubPairs_nentries[10][1]),  //  input   [7:0]
        //.stubPairs_11_nentries_0_V            (tc_stubPairs_nentries[11][0]),  //  input   [7:0]
        //.stubPairs_11_nentries_1_V            (tc_stubPairs_nentries[11][1]),  //  input   [7:0]
        //.stubPairs_12_nentries_0_V            (tc_stubPairs_nentries[12][0]),  //  input   [7:0]
        //.stubPairs_12_nentries_1_V            (tc_stubPairs_nentries[12][1]),  //  input   [7:0]
        .stubPairs_0_nentries_0_V             (8'd14),   //  input   [7:0]
        .stubPairs_0_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_1_nentries_0_V             (8'd3),   //  input   [7:0]
        .stubPairs_1_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_2_nentries_0_V             (8'd18),   //  input   [7:0]
        .stubPairs_2_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_3_nentries_0_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_3_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_4_nentries_0_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_4_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_5_nentries_0_V             (8'd6),   //  input   [7:0]
        .stubPairs_5_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_6_nentries_0_V             (8'd17),   //  input   [7:0]
        .stubPairs_6_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_7_nentries_0_V             (8'd3),   //  input   [7:0]
        .stubPairs_7_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_8_nentries_0_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_8_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_9_nentries_0_V             (8'd1),   //  input   [7:0]
        .stubPairs_9_nentries_1_V             (8'b00000000),   //  input   [7:0]
        .stubPairs_10_nentries_0_V            (8'd3),  //  input   [7:0]
        .stubPairs_10_nentries_1_V            (8'b00000000),  //  input   [7:0]
        .stubPairs_11_nentries_0_V            (8'd4),  //  input   [7:0]
        .stubPairs_11_nentries_1_V            (8'b00000000),  //  input   [7:0]
        .stubPairs_12_nentries_0_V            (8'd2),  //  input   [7:0]
        .stubPairs_12_nentries_1_V            (8'b00000000),  //  input   [7:0]

        .trackletParameters_dataarray_data_V_address0  (trackletParameters_address),            //  output  [8:0]
        .trackletParameters_dataarray_data_V_ce0       (trackletParameters_ce),                 //  output
        .trackletParameters_dataarray_data_V_we0       (trackletParameters_we),                 //  output
        .trackletParameters_dataarray_data_V_d0        (trackletParameters_data),               //  output  [69:0]
        .trackletParameters_nentries_0_V_i             (trackletParameters_nentries_in[0]),     //  input   [7:0]
        .trackletParameters_nentries_0_V_o             (trackletParameters_nentries_out[0]),    //  output  [7:0]
        .trackletParameters_nentries_0_V_o_ap_vld      (trackletParameters_nentries_valid[0]),  //  output
        .trackletParameters_nentries_1_V_i             (trackletParameters_nentries_in[1]),     //  input   [7:0]
        .trackletParameters_nentries_1_V_o             (trackletParameters_nentries_out[1]),    //  output  [7:0]
        .trackletParameters_nentries_1_V_o_ap_vld      (trackletParameters_nentries_valid[1]),  //  output
        .trackletParameters_nentries_2_V_i             (trackletParameters_nentries_in[2]),     //  input   [7:0]
        .trackletParameters_nentries_2_V_o             (trackletParameters_nentries_out[2]),    //  output  [7:0]
        .trackletParameters_nentries_2_V_o_ap_vld      (trackletParameters_nentries_valid[2]),  //  output
        .trackletParameters_nentries_3_V_i             (trackletParameters_nentries_in[3]),     //  input   [7:0]
        .trackletParameters_nentries_3_V_o             (trackletParameters_nentries_out[3]),    //  output  [7:0]
        .trackletParameters_nentries_3_V_o_ap_vld      (trackletParameters_nentries_valid[3]),  //  output

        .projout_L3PHIB_dataarray_data_V_address0  (projout_L3PHIB_address),            //  output  [7:0]
        .projout_L3PHIB_dataarray_data_V_ce0       (projout_L3PHIB_ce),                 //  output
        .projout_L3PHIB_dataarray_data_V_we0       (projout_L3PHIB_we),                 //  output
        .projout_L3PHIB_dataarray_data_V_d0        (projout_L3PHIB_data),               //  output  [59:0]
        .projout_L3PHIB_nentries_0_V_i             (projout_L3PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_L3PHIB_nentries_0_V_o             (projout_L3PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_L3PHIB_nentries_0_V_o_ap_vld      (projout_L3PHIB_nentries_valid[0]),  //  output
        .projout_L3PHIB_nentries_1_V_i             (projout_L3PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_L3PHIB_nentries_1_V_o             (projout_L3PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_L3PHIB_nentries_1_V_o_ap_vld      (projout_L3PHIB_nentries_valid[1]),  //  output

        .projout_L3PHIC_dataarray_data_V_address0  (projout_L3PHIC_address),            //  output  [7:0]
        .projout_L3PHIC_dataarray_data_V_ce0       (projout_L3PHIC_ce),                 //  output
        .projout_L3PHIC_dataarray_data_V_we0       (projout_L3PHIC_we),                 //  output
        .projout_L3PHIC_dataarray_data_V_d0        (projout_L3PHIC_data),               //  output  [59:0]
        .projout_L3PHIC_nentries_0_V_i             (projout_L3PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_L3PHIC_nentries_0_V_o             (projout_L3PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_L3PHIC_nentries_0_V_o_ap_vld      (projout_L3PHIC_nentries_valid[0]),  //  output
        .projout_L3PHIC_nentries_1_V_i             (projout_L3PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_L3PHIC_nentries_1_V_o             (projout_L3PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_L3PHIC_nentries_1_V_o_ap_vld      (projout_L3PHIC_nentries_valid[1]),  //  output

        .projout_L4PHIB_dataarray_data_V_address0  (projout_L4PHIB_address),            //  output  [7:0]
        .projout_L4PHIB_dataarray_data_V_ce0       (projout_L4PHIB_ce),                 //  output
        .projout_L4PHIB_dataarray_data_V_we0       (projout_L4PHIB_we),                 //  output
        .projout_L4PHIB_dataarray_data_V_d0        (projout_L4PHIB_data),               //  output  [57:0]
        .projout_L4PHIB_nentries_0_V_i             (projout_L4PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_L4PHIB_nentries_0_V_o             (projout_L4PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_L4PHIB_nentries_0_V_o_ap_vld      (projout_L4PHIB_nentries_valid[0]),  //  output
        .projout_L4PHIB_nentries_1_V_i             (projout_L4PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_L4PHIB_nentries_1_V_o             (projout_L4PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_L4PHIB_nentries_1_V_o_ap_vld      (projout_L4PHIB_nentries_valid[1]),  //  output

        .projout_L4PHIC_dataarray_data_V_address0  (projout_L4PHIC_address),            //  output  [7:0]
        .projout_L4PHIC_dataarray_data_V_ce0       (projout_L4PHIC_ce),                 //  output
        .projout_L4PHIC_dataarray_data_V_we0       (projout_L4PHIC_we),                 //  output
        .projout_L4PHIC_dataarray_data_V_d0        (projout_L4PHIC_data),               //  output  [57:0]
        .projout_L4PHIC_nentries_0_V_i             (projout_L4PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_L4PHIC_nentries_0_V_o             (projout_L4PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_L4PHIC_nentries_0_V_o_ap_vld      (projout_L4PHIC_nentries_valid[0]),  //  output
        .projout_L4PHIC_nentries_1_V_i             (projout_L4PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_L4PHIC_nentries_1_V_o             (projout_L4PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_L4PHIC_nentries_1_V_o_ap_vld      (projout_L4PHIC_nentries_valid[1]),  //  output

        .projout_L4PHID_dataarray_data_V_address0  (projout_L4PHID_address),            //  output  [7:0]
        .projout_L4PHID_dataarray_data_V_ce0       (projout_L4PHID_ce),                 //  output
        .projout_L4PHID_dataarray_data_V_we0       (projout_L4PHID_we),                 //  output
        .projout_L4PHID_dataarray_data_V_d0        (projout_L4PHID_data),               //  output  [57:0]
        .projout_L4PHID_nentries_0_V_i             (projout_L4PHID_nentries_in[0]),     //  input   [7:0]
        .projout_L4PHID_nentries_0_V_o             (projout_L4PHID_nentries_out[0]),    //  output  [7:0]
        .projout_L4PHID_nentries_0_V_o_ap_vld      (projout_L4PHID_nentries_valid[0]),  //  output
        .projout_L4PHID_nentries_1_V_i             (projout_L4PHID_nentries_in[1]),     //  input   [7:0]
        .projout_L4PHID_nentries_1_V_o             (projout_L4PHID_nentries_out[1]),    //  output  [7:0]
        .projout_L4PHID_nentries_1_V_o_ap_vld      (projout_L4PHID_nentries_valid[1]),  //  output

        .projout_L5PHIB_dataarray_data_V_address0  (projout_L5PHIB_address),            //  output  [7:0]
        .projout_L5PHIB_dataarray_data_V_ce0       (projout_L5PHIB_ce),                 //  output
        .projout_L5PHIB_dataarray_data_V_we0       (projout_L5PHIB_we),                 //  output
        .projout_L5PHIB_dataarray_data_V_d0        (projout_L5PHIB_data),               //  output  [57:0]
        .projout_L5PHIB_nentries_0_V_i             (projout_L5PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_L5PHIB_nentries_0_V_o             (projout_L5PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_L5PHIB_nentries_0_V_o_ap_vld      (projout_L5PHIB_nentries_valid[0]),  //  output
        .projout_L5PHIB_nentries_1_V_i             (projout_L5PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_L5PHIB_nentries_1_V_o             (projout_L5PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_L5PHIB_nentries_1_V_o_ap_vld      (projout_L5PHIB_nentries_valid[1]),  //  output

        .projout_L5PHIC_dataarray_data_V_address0  (projout_L5PHIC_address),            //  output  [7:0]
        .projout_L5PHIC_dataarray_data_V_ce0       (projout_L5PHIC_ce),                 //  output
        .projout_L5PHIC_dataarray_data_V_we0       (projout_L5PHIC_we),                 //  output
        .projout_L5PHIC_dataarray_data_V_d0        (projout_L5PHIC_data),               //  output  [57:0]
        .projout_L5PHIC_nentries_0_V_i             (projout_L5PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_L5PHIC_nentries_0_V_o             (projout_L5PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_L5PHIC_nentries_0_V_o_ap_vld      (projout_L5PHIC_nentries_valid[0]),  //  output
        .projout_L5PHIC_nentries_1_V_i             (projout_L5PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_L5PHIC_nentries_1_V_o             (projout_L5PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_L5PHIC_nentries_1_V_o_ap_vld      (projout_L5PHIC_nentries_valid[1]),  //  output

        .projout_L5PHID_dataarray_data_V_address0  (projout_L5PHID_address),            //  output  [7:0]
        .projout_L5PHID_dataarray_data_V_ce0       (projout_L5PHID_ce),                 //  output
        .projout_L5PHID_dataarray_data_V_we0       (projout_L5PHID_we),                 //  output
        .projout_L5PHID_dataarray_data_V_d0        (projout_L5PHID_data),               //  output  [57:0]
        .projout_L5PHID_nentries_0_V_i             (projout_L5PHID_nentries_in[0]),     //  input   [7:0]
        .projout_L5PHID_nentries_0_V_o             (projout_L5PHID_nentries_out[0]),    //  output  [7:0]
        .projout_L5PHID_nentries_0_V_o_ap_vld      (projout_L5PHID_nentries_valid[0]),  //  output
        .projout_L5PHID_nentries_1_V_i             (projout_L5PHID_nentries_in[1]),     //  input   [7:0]
        .projout_L5PHID_nentries_1_V_o             (projout_L5PHID_nentries_out[1]),    //  output  [7:0]
        .projout_L5PHID_nentries_1_V_o_ap_vld      (projout_L5PHID_nentries_valid[1]),  //  output

        .projout_L6PHIB_dataarray_data_V_address0  (projout_L6PHIB_address),            //  output  [7:0]
        .projout_L6PHIB_dataarray_data_V_ce0       (projout_L6PHIB_ce),                 //  output
        .projout_L6PHIB_dataarray_data_V_we0       (projout_L6PHIB_we),                 //  output
        .projout_L6PHIB_dataarray_data_V_d0        (projout_L6PHIB_data),               //  output  [57:0]
        .projout_L6PHIB_nentries_0_V_i             (projout_L6PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_L6PHIB_nentries_0_V_o             (projout_L6PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_L6PHIB_nentries_0_V_o_ap_vld      (projout_L6PHIB_nentries_valid[0]),  //  output
        .projout_L6PHIB_nentries_1_V_i             (projout_L6PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_L6PHIB_nentries_1_V_o             (projout_L6PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_L6PHIB_nentries_1_V_o_ap_vld      (projout_L6PHIB_nentries_valid[1]),  //  output

        .projout_L6PHIC_dataarray_data_V_address0  (projout_L6PHIC_address),            //  output  [7:0]
        .projout_L6PHIC_dataarray_data_V_ce0       (projout_L6PHIC_ce),                 //  output
        .projout_L6PHIC_dataarray_data_V_we0       (projout_L6PHIC_we),                 //  output
        .projout_L6PHIC_dataarray_data_V_d0        (projout_L6PHIC_data),               //  output  [57:0]
        .projout_L6PHIC_nentries_0_V_i             (projout_L6PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_L6PHIC_nentries_0_V_o             (projout_L6PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_L6PHIC_nentries_0_V_o_ap_vld      (projout_L6PHIC_nentries_valid[0]),  //  output
        .projout_L6PHIC_nentries_1_V_i             (projout_L6PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_L6PHIC_nentries_1_V_o             (projout_L6PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_L6PHIC_nentries_1_V_o_ap_vld      (projout_L6PHIC_nentries_valid[1]),  //  output

        .projout_L6PHID_dataarray_data_V_address0  (projout_L6PHID_address),            //  output  [7:0]
        .projout_L6PHID_dataarray_data_V_ce0       (projout_L6PHID_ce),                 //  output
        .projout_L6PHID_dataarray_data_V_we0       (projout_L6PHID_we),                 //  output
        .projout_L6PHID_dataarray_data_V_d0        (projout_L6PHID_data),               //  output  [57:0]
        .projout_L6PHID_nentries_0_V_i             (projout_L6PHID_nentries_in[0]),     //  input   [7:0]
        .projout_L6PHID_nentries_0_V_o             (projout_L6PHID_nentries_out[0]),    //  output  [7:0]
        .projout_L6PHID_nentries_0_V_o_ap_vld      (projout_L6PHID_nentries_valid[0]),  //  output
        .projout_L6PHID_nentries_1_V_i             (projout_L6PHID_nentries_in[1]),     //  input   [7:0]
        .projout_L6PHID_nentries_1_V_o             (projout_L6PHID_nentries_out[1]),    //  output  [7:0]
        .projout_L6PHID_nentries_1_V_o_ap_vld      (projout_L6PHID_nentries_valid[1]),  //  output

        .projout_D1PHIB_dataarray_data_V_address0  (projout_D1PHIB_address),            //  output  [7:0]
        .projout_D1PHIB_dataarray_data_V_ce0       (projout_D1PHIB_ce),                 //  output
        .projout_D1PHIB_dataarray_data_V_we0       (projout_D1PHIB_we),                 //  output
        .projout_D1PHIB_dataarray_data_V_d0        (projout_D1PHIB_data),               //  output  [58:0]
        .projout_D1PHIB_nentries_0_V_i             (projout_D1PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_D1PHIB_nentries_0_V_o             (projout_D1PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_D1PHIB_nentries_0_V_o_ap_vld      (projout_D1PHIB_nentries_valid[0]),  //  output
        .projout_D1PHIB_nentries_1_V_i             (projout_D1PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_D1PHIB_nentries_1_V_o             (projout_D1PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_D1PHIB_nentries_1_V_o_ap_vld      (projout_D1PHIB_nentries_valid[1]),  //  output

        .projout_D1PHIC_dataarray_data_V_address0  (projout_D1PHIC_address),            //  output  [7:0]
        .projout_D1PHIC_dataarray_data_V_ce0       (projout_D1PHIC_ce),                 //  output
        .projout_D1PHIC_dataarray_data_V_we0       (projout_D1PHIC_we),                 //  output
        .projout_D1PHIC_dataarray_data_V_d0        (projout_D1PHIC_data),               //  output  [58:0]
        .projout_D1PHIC_nentries_0_V_i             (projout_D1PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_D1PHIC_nentries_0_V_o             (projout_D1PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_D1PHIC_nentries_0_V_o_ap_vld      (projout_D1PHIC_nentries_valid[0]),  //  output
        .projout_D1PHIC_nentries_1_V_i             (projout_D1PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_D1PHIC_nentries_1_V_o             (projout_D1PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_D1PHIC_nentries_1_V_o_ap_vld      (projout_D1PHIC_nentries_valid[1]),  //  output

        .projout_D1PHID_dataarray_data_V_address0  (projout_D1PHID_address),            //  output  [7:0]
        .projout_D1PHID_dataarray_data_V_ce0       (projout_D1PHID_ce),                 //  output
        .projout_D1PHID_dataarray_data_V_we0       (projout_D1PHID_we),                 //  output
        .projout_D1PHID_dataarray_data_V_d0        (projout_D1PHID_data),               //  output  [58:0]
        .projout_D1PHID_nentries_0_V_i             (projout_D1PHID_nentries_in[0]),     //  input   [7:0]
        .projout_D1PHID_nentries_0_V_o             (projout_D1PHID_nentries_out[0]),    //  output  [7:0]
        .projout_D1PHID_nentries_0_V_o_ap_vld      (projout_D1PHID_nentries_valid[0]),  //  output
        .projout_D1PHID_nentries_1_V_i             (projout_D1PHID_nentries_in[1]),     //  input   [7:0]
        .projout_D1PHID_nentries_1_V_o             (projout_D1PHID_nentries_out[1]),    //  output  [7:0]
        .projout_D1PHID_nentries_1_V_o_ap_vld      (projout_D1PHID_nentries_valid[1]),  //  output

        .projout_D2PHIB_dataarray_data_V_address0  (projout_D2PHIB_address),            //  output  [7:0]
        .projout_D2PHIB_dataarray_data_V_ce0       (projout_D2PHIB_ce),                 //  output
        .projout_D2PHIB_dataarray_data_V_we0       (projout_D2PHIB_we),                 //  output
        .projout_D2PHIB_dataarray_data_V_d0        (projout_D2PHIB_data),               //  output  [58:0]
        .projout_D2PHIB_nentries_0_V_i             (projout_D2PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_D2PHIB_nentries_0_V_o             (projout_D2PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_D2PHIB_nentries_0_V_o_ap_vld      (projout_D2PHIB_nentries_valid[0]),  //  output
        .projout_D2PHIB_nentries_1_V_i             (projout_D2PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_D2PHIB_nentries_1_V_o             (projout_D2PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_D2PHIB_nentries_1_V_o_ap_vld      (projout_D2PHIB_nentries_valid[1]),  //  output

        .projout_D2PHIC_dataarray_data_V_address0  (projout_D2PHIC_address),            //  output  [7:0]
        .projout_D2PHIC_dataarray_data_V_ce0       (projout_D2PHIC_ce),                 //  output
        .projout_D2PHIC_dataarray_data_V_we0       (projout_D2PHIC_we),                 //  output
        .projout_D2PHIC_dataarray_data_V_d0        (projout_D2PHIC_data),               //  output  [58:0]
        .projout_D2PHIC_nentries_0_V_i             (projout_D2PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_D2PHIC_nentries_0_V_o             (projout_D2PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_D2PHIC_nentries_0_V_o_ap_vld      (projout_D2PHIC_nentries_valid[0]),  //  output
        .projout_D2PHIC_nentries_1_V_i             (projout_D2PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_D2PHIC_nentries_1_V_o             (projout_D2PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_D2PHIC_nentries_1_V_o_ap_vld      (projout_D2PHIC_nentries_valid[1]),  //  output

        .projout_D2PHID_dataarray_data_V_address0  (projout_D2PHID_address),            //  output  [7:0]
        .projout_D2PHID_dataarray_data_V_ce0       (projout_D2PHID_ce),                 //  output
        .projout_D2PHID_dataarray_data_V_we0       (projout_D2PHID_we),                 //  output
        .projout_D2PHID_dataarray_data_V_d0        (projout_D2PHID_data),               //  output  [58:0]
        .projout_D2PHID_nentries_0_V_i             (projout_D2PHID_nentries_in[0]),     //  input   [7:0]
        .projout_D2PHID_nentries_0_V_o             (projout_D2PHID_nentries_out[0]),    //  output  [7:0]
        .projout_D2PHID_nentries_0_V_o_ap_vld      (projout_D2PHID_nentries_valid[0]),  //  output
        .projout_D2PHID_nentries_1_V_i             (projout_D2PHID_nentries_in[1]),     //  input   [7:0]
        .projout_D2PHID_nentries_1_V_o             (projout_D2PHID_nentries_out[1]),    //  output  [7:0]
        .projout_D2PHID_nentries_1_V_o_ap_vld      (projout_D2PHID_nentries_valid[1]),  //  output

        .projout_D3PHIB_dataarray_data_V_address0  (projout_D3PHIB_address),            //  output  [7:0]
        .projout_D3PHIB_dataarray_data_V_ce0       (projout_D3PHIB_ce),                 //  output
        .projout_D3PHIB_dataarray_data_V_we0       (projout_D3PHIB_we),                 //  output
        .projout_D3PHIB_dataarray_data_V_d0        (projout_D3PHIB_data),               //  output  [58:0]
        .projout_D3PHIB_nentries_0_V_i             (projout_D3PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_D3PHIB_nentries_0_V_o             (projout_D3PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_D3PHIB_nentries_0_V_o_ap_vld      (projout_D3PHIB_nentries_valid[0]),  //  output
        .projout_D3PHIB_nentries_1_V_i             (projout_D3PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_D3PHIB_nentries_1_V_o             (projout_D3PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_D3PHIB_nentries_1_V_o_ap_vld      (projout_D3PHIB_nentries_valid[1]),  //  output

        .projout_D3PHIC_dataarray_data_V_address0  (projout_D3PHIC_address),            //  output  [7:0]
        .projout_D3PHIC_dataarray_data_V_ce0       (projout_D3PHIC_ce),                 //  output
        .projout_D3PHIC_dataarray_data_V_we0       (projout_D3PHIC_we),                 //  output
        .projout_D3PHIC_dataarray_data_V_d0        (projout_D3PHIC_data),               //  output  [58:0]
        .projout_D3PHIC_nentries_0_V_i             (projout_D3PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_D3PHIC_nentries_0_V_o             (projout_D3PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_D3PHIC_nentries_0_V_o_ap_vld      (projout_D3PHIC_nentries_valid[0]),  //  output
        .projout_D3PHIC_nentries_1_V_i             (projout_D3PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_D3PHIC_nentries_1_V_o             (projout_D3PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_D3PHIC_nentries_1_V_o_ap_vld      (projout_D3PHIC_nentries_valid[1]),  //  output

        .projout_D3PHID_dataarray_data_V_address0  (projout_D3PHID_address),            //  output  [7:0]
        .projout_D3PHID_dataarray_data_V_ce0       (projout_D3PHID_ce),                 //  output
        .projout_D3PHID_dataarray_data_V_we0       (projout_D3PHID_we),                 //  output
        .projout_D3PHID_dataarray_data_V_d0        (projout_D3PHID_data),               //  output  [58:0]
        .projout_D3PHID_nentries_0_V_i             (projout_D3PHID_nentries_in[0]),     //  input   [7:0]
        .projout_D3PHID_nentries_0_V_o             (projout_D3PHID_nentries_out[0]),    //  output  [7:0]
        .projout_D3PHID_nentries_0_V_o_ap_vld      (projout_D3PHID_nentries_valid[0]),  //  output
        .projout_D3PHID_nentries_1_V_i             (projout_D3PHID_nentries_in[1]),     //  input   [7:0]
        .projout_D3PHID_nentries_1_V_o             (projout_D3PHID_nentries_out[1]),    //  output  [7:0]
        .projout_D3PHID_nentries_1_V_o_ap_vld      (projout_D3PHID_nentries_valid[1]),  //  output

        .projout_D4PHIB_dataarray_data_V_address0  (projout_D4PHIB_address),            //  output  [7:0]
        .projout_D4PHIB_dataarray_data_V_ce0       (projout_D4PHIB_ce),                 //  output
        .projout_D4PHIB_dataarray_data_V_we0       (projout_D4PHIB_we),                 //  output
        .projout_D4PHIB_dataarray_data_V_d0        (projout_D4PHIB_data),               //  output  [58:0]
        .projout_D4PHIB_nentries_0_V_i             (projout_D4PHIB_nentries_in[0]),     //  input   [7:0]
        .projout_D4PHIB_nentries_0_V_o             (projout_D4PHIB_nentries_out[0]),    //  output  [7:0]
        .projout_D4PHIB_nentries_0_V_o_ap_vld      (projout_D4PHIB_nentries_valid[0]),  //  output
        .projout_D4PHIB_nentries_1_V_i             (projout_D4PHIB_nentries_in[1]),     //  input   [7:0]
        .projout_D4PHIB_nentries_1_V_o             (projout_D4PHIB_nentries_out[1]),    //  output  [7:0]
        .projout_D4PHIB_nentries_1_V_o_ap_vld      (projout_D4PHIB_nentries_valid[1]),  //  output

        .projout_D4PHIC_dataarray_data_V_address0  (projout_D4PHIC_address),            //  output  [7:0]
        .projout_D4PHIC_dataarray_data_V_ce0       (projout_D4PHIC_ce),                 //  output
        .projout_D4PHIC_dataarray_data_V_we0       (projout_D4PHIC_we),                 //  output
        .projout_D4PHIC_dataarray_data_V_d0        (projout_D4PHIC_data),               //  output  [58:0]
        .projout_D4PHIC_nentries_0_V_i             (projout_D4PHIC_nentries_in[0]),     //  input   [7:0]
        .projout_D4PHIC_nentries_0_V_o             (projout_D4PHIC_nentries_out[0]),    //  output  [7:0]
        .projout_D4PHIC_nentries_0_V_o_ap_vld      (projout_D4PHIC_nentries_valid[0]),  //  output
        .projout_D4PHIC_nentries_1_V_i             (projout_D4PHIC_nentries_in[1]),     //  input   [7:0]
        .projout_D4PHIC_nentries_1_V_o             (projout_D4PHIC_nentries_out[1]),    //  output  [7:0]
        .projout_D4PHIC_nentries_1_V_o_ap_vld      (projout_D4PHIC_nentries_valid[1]),  //  output

        .projout_D4PHID_dataarray_data_V_address0  (projout_D4PHID_address),            //  output  [7:0]
        .projout_D4PHID_dataarray_data_V_ce0       (projout_D4PHID_ce),                 //  output
        .projout_D4PHID_dataarray_data_V_we0       (projout_D4PHID_we),                 //  output
        .projout_D4PHID_dataarray_data_V_d0        (projout_D4PHID_data),               //  output  [58:0]
        .projout_D4PHID_nentries_0_V_i             (projout_D4PHID_nentries_in[0]),     //  input   [7:0]
        .projout_D4PHID_nentries_0_V_o             (projout_D4PHID_nentries_out[0]),    //  output  [7:0]
        .projout_D4PHID_nentries_0_V_o_ap_vld      (projout_D4PHID_nentries_valid[0]),  //  output
        .projout_D4PHID_nentries_1_V_i             (projout_D4PHID_nentries_in[1]),     //  input   [7:0]
        .projout_D4PHID_nentries_1_V_o             (projout_D4PHID_nentries_out[1]),    //  output  [7:0]
        .projout_D4PHID_nentries_1_V_o_ap_vld      (projout_D4PHID_nentries_valid[1])   //  output
    );

    TrackletEngineTop_0 TE_L1PHIE18_L2PHIC17 (
        .ap_clk    (fast_clk),
        .ap_rst    (1'b0),
        .ap_start  (te_start),
        .ap_done   (te_done),
        .ap_idle   (te_idle),
        .ap_ready  (te_ready),
        .bx_V      (3'b0),

        .instubinnerdata_dataarray_data_V_address0  (te_innerStubs_address),      //  output  [7:0]
        .instubinnerdata_dataarray_data_V_ce0       (te_innerStubs_ce),           //  output
        .instubinnerdata_dataarray_data_V_q0        (te_innerStubs_data),         //  input   [21:0]
        //.instubinnerdata_nentries_0_V               (te_innerStubs_nentries[0]),  //  input   [7:0]
        //.instubinnerdata_nentries_1_V               (te_innerStubs_nentries[1]),  //  input   [7:0]
        .instubinnerdata_nentries_0_V               (8'd15),  //  input   [7:0]
        .instubinnerdata_nentries_1_V               (8'b00000000),  //  input   [7:0]

        .instubouterdata_dataarray_data_V_address0  (te_outerStubs_address),         //  output  [8:0]
        .instubouterdata_dataarray_data_V_ce0       (te_outerStubs_ce),              //  output
        .instubouterdata_dataarray_data_V_q0        (te_outerStubs_data),            //  input   [15:0]
        //.instubouterdata_nentries_0_V_0             (te_outerStubs_nentries[0][0]),  //  input   [4:0]
        //.instubouterdata_nentries_0_V_1             (te_outerStubs_nentries[0][1]),  //  input   [4:0]
        //.instubouterdata_nentries_0_V_2             (te_outerStubs_nentries[0][2]),  //  input   [4:0]
        //.instubouterdata_nentries_0_V_3             (te_outerStubs_nentries[0][3]),  //  input   [4:0]
        //.instubouterdata_nentries_0_V_4             (te_outerStubs_nentries[0][4]),  //  input   [4:0]
        //.instubouterdata_nentries_0_V_5             (te_outerStubs_nentries[0][5]),  //  input   [4:0]
        //.instubouterdata_nentries_0_V_6             (te_outerStubs_nentries[0][6]),  //  input   [4:0]
        //.instubouterdata_nentries_0_V_7             (te_outerStubs_nentries[0][7]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_0             (te_outerStubs_nentries[1][0]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_1             (te_outerStubs_nentries[1][1]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_2             (te_outerStubs_nentries[1][2]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_3             (te_outerStubs_nentries[1][3]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_4             (te_outerStubs_nentries[1][4]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_5             (te_outerStubs_nentries[1][5]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_6             (te_outerStubs_nentries[1][6]),  //  input   [4:0]
        //.instubouterdata_nentries_1_V_7             (te_outerStubs_nentries[1][7]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_0             (te_outerStubs_nentries[2][0]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_1             (te_outerStubs_nentries[2][1]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_2             (te_outerStubs_nentries[2][2]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_3             (te_outerStubs_nentries[2][3]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_4             (te_outerStubs_nentries[2][4]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_5             (te_outerStubs_nentries[2][5]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_6             (te_outerStubs_nentries[2][6]),  //  input   [4:0]
        //.instubouterdata_nentries_2_V_7             (te_outerStubs_nentries[2][7]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_0             (te_outerStubs_nentries[3][0]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_1             (te_outerStubs_nentries[3][1]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_2             (te_outerStubs_nentries[3][2]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_3             (te_outerStubs_nentries[3][3]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_4             (te_outerStubs_nentries[3][4]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_5             (te_outerStubs_nentries[3][5]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_6             (te_outerStubs_nentries[3][6]),  //  input   [4:0]
        //.instubouterdata_nentries_3_V_7             (te_outerStubs_nentries[3][7]),  //  input   [4:0]
        .instubouterdata_nentries_0_V_0             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_0_V_1             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_0_V_2             (5'd1),  //  input   [4:0]
        .instubouterdata_nentries_0_V_3             (5'd1),  //  input   [4:0]
        .instubouterdata_nentries_0_V_4             (5'd1),  //  input   [4:0]
        .instubouterdata_nentries_0_V_5             (5'd1),  //  input   [4:0]
        .instubouterdata_nentries_0_V_6             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_0_V_7             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_0             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_1             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_2             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_3             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_4             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_5             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_6             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_1_V_7             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_0             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_1             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_2             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_3             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_4             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_5             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_6             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_2_V_7             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_0             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_1             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_2             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_3             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_4             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_5             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_6             (5'b00000),  //  input   [4:0]
        .instubouterdata_nentries_3_V_7             (5'b00000),  //  input   [4:0]

        .outstubpair_dataarray_data_V_address0  (te_stubPairs_address),            //  output  [7:0]
        .outstubpair_dataarray_data_V_ce0       (te_stubPairs_ce),                 //  output
        .outstubpair_dataarray_data_V_we0       (te_stubPairs_we),                 //  output
        .outstubpair_dataarray_data_V_d0        (te_stubPairs_data),               //  output  [13:0]
        .outstubpair_nentries_0_V_i             (te_stubPairs_nentries_in[0]),     //  input   [7:0]
        .outstubpair_nentries_0_V_o             (te_stubPairs_nentries_out[0]),    //  output  [7:0]
        .outstubpair_nentries_0_V_o_ap_vld      (te_stubPairs_nentries_valid[0]),  //  output
        .outstubpair_nentries_1_V_i             (te_stubPairs_nentries_in[1]),     //  input   [7:0]
        .outstubpair_nentries_1_V_o             (te_stubPairs_nentries_out[1]),    //  output  [7:0]
        .outstubpair_nentries_1_V_o_ap_vld      (te_stubPairs_nentries_valid[1])   //  output
    );

    pipe_delay # (
        .STAGES (256),
        .WIDTH (1)
    ) tc_start_delay (
        .val_in (te_start),
        .val_out (tc_start),
        .clk (fast_clk)
    );

    pipe_delay # (
        .STAGES (1),
        .WIDTH (1)
    ) tc_outerStubs_memory_delay (
        .val_in (tc_outerStubs_address[10]),
        .val_out (tc_outerStubs_memory),
        .clk (fast_clk)
    );

    pipe_delay # (
        .STAGES (1),
        .WIDTH (4)
    ) tc_stubPairs_memory_delay (
        .val_in (tc_stubPairs_address[11:8]),
        .val_out (tc_stubPairs_memory),
        .clk (fast_clk)
    );

    Memory # (
        .RAM_WIDTH (22),
        .RAM_DEPTH (256),
        .INIT_FILE ("VMStubs_VMSTE_L1PHIE18n2_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TE_INNER_STUBS (
        .addra (8'b0),
        .addrb (te_innerStubs_address),
        .dina (22'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (te_innerStubs_data)
    );

    Memory # (
        .RAM_WIDTH (16),
        .RAM_DEPTH (512),
        .INIT_FILE ("VMStubs_VMSTE_L2PHIC17n4_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TE_OUTER_STUBS (
        .addra (9'b0),
        .addrb (te_outerStubs_index),
        .dina (16'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (te_outerStubs_data)
    );

    Memory # (
        .RAM_WIDTH (36),
        .RAM_DEPTH (1024),
        .INIT_FILE ("AllStubs_AS_L1PHIEn2_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_INNER_STUBS (
        .addra (10'b0),
        .addrb (tc_innerStubs_address),
        .dina (36'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_innerStubs_data)
    );

    Memory # (
        .RAM_WIDTH (36),
        .RAM_DEPTH (1024),
        .INIT_FILE ("AllStubs_AS_L2PHIBn5_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_OUTER_STUBS_0 (
        .addra (10'b0),
        .addrb (tc_outerStubs_address[9:0]),
        .dina (36'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_outerStubs_data_0)
    );

    Memory # (
        .RAM_WIDTH (36),
        .RAM_DEPTH (1024),
        .INIT_FILE ("AllStubs_AS_L2PHICn2_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_OUTER_STUBS_1 (
        .addra (10'b0),
        .addrb (tc_outerStubs_address[9:0]),
        .dina (36'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_outerStubs_data_1)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE17_L2PHIB16_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_0 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_0)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE17_L2PHIC17_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_1 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_1)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE17_L2PHIC18_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_2 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_2)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE17_L2PHIC19_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_3 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_3)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE18_L2PHIB16_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_4 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_4)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_5 (
        .addra (te_stubPairs_address),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (te_stubPairs_data),
        .clka (fast_clk),
        .clkb (fast_clk),
        .wea (te_stubPairs_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_5)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE18_L2PHIC18_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_6 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_6)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE18_L2PHIC19_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_7 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_7)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE18_L2PHIC20_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_8 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_8)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE19_L2PHIC17_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_9 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_9)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE19_L2PHIC18_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_10 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_10)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE19_L2PHIC19_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_11 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_11)
    );

    Memory # (
        .RAM_WIDTH (14),
        .RAM_DEPTH (256),
        .INIT_FILE ("StubPairs_SP_L1PHIE19_L2PHIC20_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TC_STUB_PAIRS_12 (
        .addra (8'b0),
        .addrb (tc_stubPairs_address[7:0]),
        .dina (14'b0),
        .clka (1'b0),
        .clkb (fast_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (tc_stubPairs_data_12)
    );

    Memory # (
        .RAM_WIDTH (70),
        .RAM_DEPTH (512),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TRACKLET_PARAMETERS (
        .addra (trackletParameters_address),
        .addrb (trackletParameters_expected_address[8:0]),
        .dina (trackletParameters_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (trackletParameters_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (trackletParameters_actual_data)
    );

    Memory # (
        .RAM_WIDTH (60),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L3PHIB (
        .addra (projout_L3PHIB_address),
        .addrb (projout_L3PHIB_expected_address[7:0]),
        .dina (projout_L3PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L3PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L3PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (60),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L3PHIC (
        .addra (projout_L3PHIC_address),
        .addrb (projout_L3PHIC_expected_address[7:0]),
        .dina (projout_L3PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L3PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L3PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L4PHIB (
        .addra (projout_L4PHIB_address),
        .addrb (projout_L4PHIB_expected_address[7:0]),
        .dina (projout_L4PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L4PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L4PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L4PHIC (
        .addra (projout_L4PHIC_address),
        .addrb (projout_L4PHIC_expected_address[7:0]),
        .dina (projout_L4PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L4PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L4PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L4PHID (
        .addra (projout_L4PHID_address),
        .addrb (projout_L4PHID_expected_address[7:0]),
        .dina (projout_L4PHID_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L4PHID_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L4PHID_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L5PHIB (
        .addra (projout_L5PHIB_address),
        .addrb (projout_L5PHIB_expected_address[7:0]),
        .dina (projout_L5PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L5PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L5PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L5PHIC (
        .addra (projout_L5PHIC_address),
        .addrb (projout_L5PHIC_expected_address[7:0]),
        .dina (projout_L5PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L5PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L5PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L5PHID (
        .addra (projout_L5PHID_address),
        .addrb (projout_L5PHID_expected_address[7:0]),
        .dina (projout_L5PHID_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L5PHID_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L5PHID_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L6PHIB (
        .addra (projout_L6PHIB_address),
        .addrb (projout_L6PHIB_expected_address[7:0]),
        .dina (projout_L6PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L6PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L6PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L6PHIC (
        .addra (projout_L6PHIC_address),
        .addrb (projout_L6PHIC_expected_address[7:0]),
        .dina (projout_L6PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L6PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L6PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L6PHID (
        .addra (projout_L6PHID_address),
        .addrb (projout_L6PHID_expected_address[7:0]),
        .dina (projout_L6PHID_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_L6PHID_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L6PHID_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D1PHIB (
        .addra (projout_D1PHIB_address),
        .addrb (projout_D1PHIB_expected_address[7:0]),
        .dina (projout_D1PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D1PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D1PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D1PHIC (
        .addra (projout_D1PHIC_address),
        .addrb (projout_D1PHIC_expected_address[7:0]),
        .dina (projout_D1PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D1PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D1PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D1PHID (
        .addra (projout_D1PHID_address),
        .addrb (projout_D1PHID_expected_address[7:0]),
        .dina (projout_D1PHID_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D1PHID_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D1PHID_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D2PHIB (
        .addra (projout_D2PHIB_address),
        .addrb (projout_D2PHIB_expected_address[7:0]),
        .dina (projout_D2PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D2PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D2PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D2PHIC (
        .addra (projout_D2PHIC_address),
        .addrb (projout_D2PHIC_expected_address[7:0]),
        .dina (projout_D2PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D2PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D2PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D2PHID (
        .addra (projout_D2PHID_address),
        .addrb (projout_D2PHID_expected_address[7:0]),
        .dina (projout_D2PHID_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D2PHID_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D2PHID_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D3PHIB (
        .addra (projout_D3PHIB_address),
        .addrb (projout_D3PHIB_expected_address[7:0]),
        .dina (projout_D3PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D3PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D3PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D3PHIC (
        .addra (projout_D3PHIC_address),
        .addrb (projout_D3PHIC_expected_address[7:0]),
        .dina (projout_D3PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D3PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D3PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D3PHID (
        .addra (projout_D3PHID_address),
        .addrb (projout_D3PHID_expected_address[7:0]),
        .dina (projout_D3PHID_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D3PHID_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D3PHID_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D4PHIB (
        .addra (projout_D4PHIB_address),
        .addrb (projout_D4PHIB_expected_address[7:0]),
        .dina (projout_D4PHIB_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D4PHIB_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D4PHIB_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D4PHIC (
        .addra (projout_D4PHIC_address),
        .addrb (projout_D4PHIC_expected_address[7:0]),
        .dina (projout_D4PHIC_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D4PHIC_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D4PHIC_actual_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE (""),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D4PHID (
        .addra (projout_D4PHID_address),
        .addrb (projout_D4PHID_expected_address[7:0]),
        .dina (projout_D4PHID_data),
        .clka (fast_clk),
        .clkb (slow_clk),
        .wea (projout_D4PHID_we),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D4PHID_actual_data)
    );

    Memory # (
        .RAM_WIDTH (70),
        .RAM_DEPTH (512),
        .INIT_FILE ("TrackletParameters_TPAR_L1L2G_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) TRACKLET_PARAMETERS_EXPECTED (
        .addra (9'b0),
        .addrb (trackletParameters_expected_address[8:0]),
        .dina (70'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (trackletParameters_expected_data)
    );

    Memory # (
        .RAM_WIDTH (60),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L3PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L3PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L3PHIB_expected_address[7:0]),
        .dina (60'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L3PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (60),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L3PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L3PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L3PHIC_expected_address[7:0]),
        .dina (60'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L3PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L4PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L4PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L4PHIB_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L4PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L4PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L4PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L4PHIC_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L4PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L4PHID_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L4PHID_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L4PHID_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L4PHID_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L5PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L5PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L5PHIB_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L5PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L5PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L5PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L5PHIC_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L5PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L5PHID_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L5PHID_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L5PHID_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L5PHID_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L6PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L6PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L6PHIB_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L6PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L6PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L6PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L6PHIC_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L6PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (58),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_L6PHID_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_L6PHID_EXPECTED (
        .addra (8'b0),
        .addrb (projout_L6PHID_expected_address[7:0]),
        .dina (58'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_L6PHID_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D1PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D1PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D1PHIB_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D1PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D1PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D1PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D1PHIC_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D1PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D1PHID_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D1PHID_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D1PHID_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D1PHID_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D2PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D2PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D2PHIB_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D2PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D2PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D2PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D2PHIC_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D2PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D2PHID_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D2PHID_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D2PHID_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D2PHID_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D3PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D3PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D3PHIB_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D3PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D3PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D3PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D3PHIC_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D3PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D3PHID_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D3PHID_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D3PHID_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D3PHID_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D4PHIB_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D4PHIB_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D4PHIB_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D4PHIB_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D4PHIC_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D4PHIC_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D4PHIC_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D4PHIC_expected_data)
    );

    Memory # (
        .RAM_WIDTH (59),
        .RAM_DEPTH (256),
        .INIT_FILE ("TrackletProjections_TPROJ_L1L2G_D4PHID_04.mem"),
        .RAM_PERFORMANCE ("LOW_LATENCY"),
        .HEX (1)
    ) PROJOUT_D4PHID_EXPECTED (
        .addra (8'b0),
        .addrb (projout_D4PHID_expected_address[7:0]),
        .dina (59'b0),
        .clka (1'b0),
        .clkb (slow_clk),
        .wea (1'b0),
        .enb (1'b1),
        .rstb (1'b0),
        .regceb (1'b1),
        .doutb (projout_D4PHID_expected_data)
    );

endmodule
