# 200 MHz
create_clock -period 5.000 -name sysclk200 -waveform {0.000 2.500} [get_ports clk_p]
# 250 MHz
create_clock -period 4.000 -name fast_clk
create_clock -period 160 -name slow_clk

set_property CONFIG_VOLTAGE 2.5 [current_design]
set_property CFGBVS VCCO [current_design]

set_property IOSTANDARD DIFF_SSTL15 [get_ports clk_p]
set_property IOSTANDARD DIFF_SSTL15 [get_ports clk_n]
set_property PACKAGE_PIN G18 [get_ports clk_n]

set_property IOSTANDARD LVCMOS18 [get_ports sw_N]
set_property PACKAGE_PIN AR40 [get_ports sw_N]
set_property IOSTANDARD LVCMOS18 [get_ports sw_E]
set_property PACKAGE_PIN AU38 [get_ports sw_E]
set_property IOSTANDARD LVCMOS18 [get_ports sw_S]
set_property PACKAGE_PIN AP40 [get_ports sw_S]
set_property IOSTANDARD LVCMOS18 [get_ports sw_W]
set_property PACKAGE_PIN AW40 [get_ports sw_W]
set_property IOSTANDARD LVCMOS18 [get_ports sw_C]
set_property PACKAGE_PIN AV39 [get_ports sw_C]

set_property IOSTANDARD LVCMOS18 [get_ports LED0]
set_property PACKAGE_PIN AM39 [get_ports LED0]
set_property IOSTANDARD LVCMOS18 [get_ports LED1]
set_property PACKAGE_PIN AN39 [get_ports LED1]
set_property IOSTANDARD LVCMOS18 [get_ports LED2]
set_property PACKAGE_PIN AR37 [get_ports LED2]
set_property IOSTANDARD LVCMOS18 [get_ports LED3]
set_property PACKAGE_PIN AT37 [get_ports LED3]
set_property IOSTANDARD LVCMOS18 [get_ports LED4]
set_property PACKAGE_PIN AR35 [get_ports LED4]
set_property IOSTANDARD LVCMOS18 [get_ports LED5]
set_property PACKAGE_PIN AP41 [get_ports LED5]
set_property IOSTANDARD LVCMOS18 [get_ports LED6]
set_property PACKAGE_PIN AP42 [get_ports LED6]
set_property IOSTANDARD LVCMOS18 [get_ports LED7]
set_property PACKAGE_PIN AU39 [get_ports LED7]
