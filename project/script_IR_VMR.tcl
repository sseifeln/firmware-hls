# Script to generate project for IR
#   vivado_hls -f script_IR.tcl
#   vivado_hls -p inputrouter
# WARNING: this will wipe out the original project by the same name

# create new project (deleting any existing one of same name)
open_project -reset inputrouter_vmrouter

# source files
set CFLAGS {-std=c++11 -I../TrackletAlgorithm}
set_top InputRouterTop
add_files ../TrackletAlgorithm/InputRouterTop.cc -cflags "$CFLAGS"
add_files ../TrackletAlgorithm/VMRouterTop.cc -cflags "$CFLAGS"
add_files -tb ../TestBenches/IR_VMR_test.cpp -cflags "$CFLAGS"

open_solution "solution1"

# Define FPGA, clock frequency & common HLS settings.
source settings_hls.tcl

# data files
add_files -tb ../emData/
add_files -tb ../emData/VMR/VMR_L1PHIE/

create_clock -period 250MHz -name slow_clock 
create_clock -period 480MHz -name fast_clock

set nProc [exec nproc]
csim_design -compiler gcc -mflags "-j$nProc"
#csynth_design
#cosim_design 
#export_design -format ip_catalog
# Adding "-flow impl" runs full Vivado implementation, providing accurate resource use numbers (very slow).
#export_design -format ip_catalog -flow impl

exit
