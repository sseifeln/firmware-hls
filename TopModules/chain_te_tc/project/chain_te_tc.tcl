create_project chain_te_tc chain_te_tc -part xc7vx690tffg1761-2
#create_project chain_te_tc chain_te_tc -part xcvu7p-flvb2104-1-e

add_files -fileset constrs_1 -norecurse ../constraints/constraints.xdc

add_files -norecurse {../source/Memory.v ../source/pipe_delay.v ../source/top_chain_te_tc.v}

create_ip -name clk_wiz -vendor xilinx.com -library ip -version 6.0 -module_name clk_wiz_0
set_property -dict [list CONFIG.PRIM_IN_FREQ {200.000} CONFIG.CLKOUT2_USED {true} CONFIG.CLK_OUT1_PORT {fast_clk} CONFIG.CLK_OUT2_PORT {slow_clk} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {250.000} CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {6.250} CONFIG.USE_LOCKED {false} CONFIG.USE_RESET {false} CONFIG.CLKIN1_JITTER_PS {50.0} CONFIG.MMCM_DIVCLK_DIVIDE {4} CONFIG.MMCM_CLKFBOUT_MULT_F {15.625} CONFIG.MMCM_CLKIN1_PERIOD {5.000} CONFIG.MMCM_CLKIN2_PERIOD {10.0} CONFIG.MMCM_CLKOUT0_DIVIDE_F {3.125} CONFIG.MMCM_CLKOUT1_DIVIDE {125} CONFIG.NUM_OUT_CLKS {2} CONFIG.CLKOUT1_JITTER {151.073} CONFIG.CLKOUT1_PHASE_ERROR {133.882} CONFIG.CLKOUT2_JITTER {306.319} CONFIG.CLKOUT2_PHASE_ERROR {133.882}] [get_ips clk_wiz_0]
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/clk_wiz_0/clk_wiz_0.xci]

set_property  ip_repo_paths  ../../../project [current_project]
update_ip_catalog

create_ip -name TE_L1PHIE17_L2PHIB16 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE17_L2PHIB16_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE17_L2PHIB16_0/TE_L1PHIE17_L2PHIB16_0.xci]

create_ip -name TE_L1PHIE17_L2PHIC17 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE17_L2PHIC17_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE17_L2PHIC17_0/TE_L1PHIE17_L2PHIC17_0.xci]

create_ip -name TE_L1PHIE17_L2PHIC18 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE17_L2PHIC18_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE17_L2PHIC18_0/TE_L1PHIE17_L2PHIC18_0.xci]

create_ip -name TE_L1PHIE17_L2PHIC19 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE17_L2PHIC19_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE17_L2PHIC19_0/TE_L1PHIE17_L2PHIC19_0.xci]

create_ip -name TE_L1PHIE18_L2PHIB16 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE18_L2PHIB16_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE18_L2PHIB16_0/TE_L1PHIE18_L2PHIB16_0.xci]

create_ip -name TE_L1PHIE18_L2PHIC17 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE18_L2PHIC17_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE18_L2PHIC17_0/TE_L1PHIE18_L2PHIC17_0.xci]

create_ip -name TE_L1PHIE18_L2PHIC18 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE18_L2PHIC18_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE18_L2PHIC18_0/TE_L1PHIE18_L2PHIC18_0.xci]

create_ip -name TE_L1PHIE18_L2PHIC19 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE18_L2PHIC19_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE18_L2PHIC19_0/TE_L1PHIE18_L2PHIC19_0.xci]

create_ip -name TE_L1PHIE18_L2PHIC20 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE18_L2PHIC20_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE18_L2PHIC20_0/TE_L1PHIE18_L2PHIC20_0.xci]

create_ip -name TE_L1PHIE19_L2PHIC17 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE19_L2PHIC17_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE19_L2PHIC17_0/TE_L1PHIE19_L2PHIC17_0.xci]

create_ip -name TE_L1PHIE19_L2PHIC18 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE19_L2PHIC18_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE19_L2PHIC18_0/TE_L1PHIE19_L2PHIC18_0.xci]

create_ip -name TE_L1PHIE19_L2PHIC19 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE19_L2PHIC19_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE19_L2PHIC19_0/TE_L1PHIE19_L2PHIC19_0.xci]

create_ip -name TE_L1PHIE19_L2PHIC20 -vendor xilinx.com -library hls -version 1.0 -module_name TE_L1PHIE19_L2PHIC20_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TE_L1PHIE19_L2PHIC20_0/TE_L1PHIE19_L2PHIC20_0.xci]

create_ip -name TrackletCalculator_L1L2G -vendor xilinx.com -library hls -version 1.0 -module_name TrackletCalculator_L1L2G_0
generate_target {instantiation_template} [get_files chain_te_tc/chain_te_tc.srcs/sources_1/ip/TrackletCalculator_L1L2G_0/TrackletCalculator_L1L2G_0.xci]

update_compile_order -fileset sources_1

set_property SOURCE_SET sources_1 [get_filesets sim_1]
add_files -fileset sim_1 -norecurse ../tb/tb_chain_te_tc.v
update_compile_order -fileset sim_1

add_files -norecurse {../memories/AllStubs_AS_L1PHIEn2_04.mem ../memories/AllStubs_AS_L2PHIBn5_04.mem ../memories/AllStubs_AS_L2PHICn2_04.mem ../memories/StubPairs_SP_L1PHIE17_L2PHIB16_04.mem ../memories/StubPairs_SP_L1PHIE17_L2PHIC17_04.mem ../memories/StubPairs_SP_L1PHIE17_L2PHIC18_04.mem ../memories/StubPairs_SP_L1PHIE17_L2PHIC19_04.mem ../memories/StubPairs_SP_L1PHIE18_L2PHIB16_04.mem ../memories/StubPairs_SP_L1PHIE18_L2PHIC17_04.mem ../memories/StubPairs_SP_L1PHIE18_L2PHIC18_04.mem ../memories/StubPairs_SP_L1PHIE18_L2PHIC19_04.mem ../memories/StubPairs_SP_L1PHIE18_L2PHIC20_04.mem ../memories/StubPairs_SP_L1PHIE19_L2PHIC17_04.mem ../memories/StubPairs_SP_L1PHIE19_L2PHIC18_04.mem ../memories/StubPairs_SP_L1PHIE19_L2PHIC19_04.mem ../memories/StubPairs_SP_L1PHIE19_L2PHIC20_04.mem ../memories/TrackletParameters_TPAR_L1L2G_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D1PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D1PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D1PHID_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D2PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D2PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D2PHID_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D3PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D3PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D3PHID_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D4PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D4PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_D4PHID_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L3PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L3PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L4PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L4PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L4PHID_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L5PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L5PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L5PHID_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L6PHIB_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L6PHIC_04.mem ../memories/TrackletProjections_TPROJ_L1L2G_L6PHID_04.mem ../memories/VMStubs_VMSTE_L1PHIE17n2_04.mem ../memories/VMStubs_VMSTE_L1PHIE17n3_04.mem ../memories/VMStubs_VMSTE_L1PHIE17n4_04.mem ../memories/VMStubs_VMSTE_L1PHIE17n5_04.mem ../memories/VMStubs_VMSTE_L1PHIE18n1_04.mem ../memories/VMStubs_VMSTE_L1PHIE18n2_04.mem ../memories/VMStubs_VMSTE_L1PHIE18n3_04.mem ../memories/VMStubs_VMSTE_L1PHIE18n4_04.mem ../memories/VMStubs_VMSTE_L1PHIE18n5_04.mem ../memories/VMStubs_VMSTE_L1PHIE19n1_04.mem ../memories/VMStubs_VMSTE_L1PHIE19n2_04.mem ../memories/VMStubs_VMSTE_L1PHIE19n3_04.mem ../memories/VMStubs_VMSTE_L1PHIE19n4_04.mem ../memories/VMStubs_VMSTE_L2PHIB16n4_04.mem ../memories/VMStubs_VMSTE_L2PHIB16n5_04.mem ../memories/VMStubs_VMSTE_L2PHIC17n3_04.mem ../memories/VMStubs_VMSTE_L2PHIC17n4_04.mem ../memories/VMStubs_VMSTE_L2PHIC17n5_04.mem ../memories/VMStubs_VMSTE_L2PHIC18n2_04.mem ../memories/VMStubs_VMSTE_L2PHIC18n3_04.mem ../memories/VMStubs_VMSTE_L2PHIC18n4_04.mem ../memories/VMStubs_VMSTE_L2PHIC19n1_04.mem ../memories/VMStubs_VMSTE_L2PHIC19n2_04.mem ../memories/VMStubs_VMSTE_L2PHIC19n3_04.mem ../memories/VMStubs_VMSTE_L2PHIC20n1_04.mem ../memories/VMStubs_VMSTE_L2PHIC20n2_04.mem}

set_property strategy Flow_PerfOptimized_high [get_runs synth_1]
set_property strategy Performance_Explore [get_runs impl_1]

exit
