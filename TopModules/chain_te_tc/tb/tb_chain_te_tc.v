`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/08/2019 01:15:51 PM
// Design Name: 
// Module Name: tb_chain_te_tc
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


module tb_chain_te_tc ();

  reg clk;
  wire clk_p, clk_n;

  reg sw_N = 0;
  reg sw_E = 0;
  reg sw_S = 0;
  reg sw_W = 0;
  reg sw_C = 0;

  wire LED0;
  wire LED1;
  wire LED2;
  wire LED3;
  wire LED4;
  wire LED5;
  wire LED6;
  wire LED7;

  always begin
    #1 clk = ~clk;
  end
  assign clk_p = clk;
  assign clk_n = ~clk;

  initial begin

    clk = 1;

    sw_N <= 0;
    sw_E <= 0;
    sw_S <= 0;
    sw_W <= 0;
    sw_C <= 0;

    #10000 sw_N <= 1;
    #500 sw_N <= 0;
    #40000 sw_C <= 1;
    #500 sw_C <= 0;
    #10000 sw_N <= 1;
    #500 sw_N <= 0;

  end

  top_chain_te_tc UU_CHAIN_TE_TC (
    .clk_p (clk_p),
    .clk_n (clk_n),

    .sw_N (sw_N),
    .sw_E (sw_E),
    .sw_S (sw_S),
    .sw_W (sw_W),
    .sw_C (sw_C),

    .LED0 (LED0),
    .LED1 (LED1),
    .LED2 (LED2),
    .LED3 (LED3),
    .LED4 (LED4),
    .LED5 (LED5),
    .LED6 (LED6),
    .LED7 (LED7)
  );

endmodule
