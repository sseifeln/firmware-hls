# WARNING: this will wipe out the original project by the same name

# set clock 250 MHz
set clockperiod 4

## delete old project
#delete_project trackletengine
#
## make new project
#open_project -reset trackletengine
#set_top TrackletEngineTop
#add_files ../TrackletAlgorithm/TrackletEngineTop.cpp -cflags "-std=c++11"
#
#add_files -tb ../TestBenches/TrackletEngine_test.cpp -cflags "-I../TrackletAlgorithm -std=c++11"
#add_files -tb ../emData/TE
#open_solution -reset "solution1"
#source set_fpga.tcl
#create_clock -period $clockperiod -name default
#
#csim_design -compiler gcc
#csynth_design
#cosim_design
#export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE17_L2PHIB16
open_project -reset TE_L1PHIE17_L2PHIB16
add_files ../TrackletAlgorithm/TE_L1PHIE17_L2PHIB16.cpp -cflags "-std=c++11"
set_top TE_L1PHIE17_L2PHIB16
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE17_L2PHIC17
open_project -reset TE_L1PHIE17_L2PHIC17
add_files ../TrackletAlgorithm/TE_L1PHIE17_L2PHIC17.cpp -cflags "-std=c++11"
set_top TE_L1PHIE17_L2PHIC17
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE17_L2PHIC18
open_project -reset TE_L1PHIE17_L2PHIC18
add_files ../TrackletAlgorithm/TE_L1PHIE17_L2PHIC18.cpp -cflags "-std=c++11"
set_top TE_L1PHIE17_L2PHIC18
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE17_L2PHIC19
open_project -reset TE_L1PHIE17_L2PHIC19
add_files ../TrackletAlgorithm/TE_L1PHIE17_L2PHIC19.cpp -cflags "-std=c++11"
set_top TE_L1PHIE17_L2PHIC19
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE18_L2PHIB16
open_project -reset TE_L1PHIE18_L2PHIB16
add_files ../TrackletAlgorithm/TE_L1PHIE18_L2PHIB16.cpp -cflags "-std=c++11"
set_top TE_L1PHIE18_L2PHIB16
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE18_L2PHIC17
open_project -reset TE_L1PHIE18_L2PHIC17
add_files ../TrackletAlgorithm/TE_L1PHIE18_L2PHIC17.cpp -cflags "-std=c++11"
set_top TE_L1PHIE18_L2PHIC17
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE18_L2PHIC18
open_project -reset TE_L1PHIE18_L2PHIC18
add_files ../TrackletAlgorithm/TE_L1PHIE18_L2PHIC18.cpp -cflags "-std=c++11"
set_top TE_L1PHIE18_L2PHIC18
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE18_L2PHIC19
open_project -reset TE_L1PHIE18_L2PHIC19
add_files ../TrackletAlgorithm/TE_L1PHIE18_L2PHIC19.cpp -cflags "-std=c++11"
set_top TE_L1PHIE18_L2PHIC19
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE18_L2PHIC20
open_project -reset TE_L1PHIE18_L2PHIC20
add_files ../TrackletAlgorithm/TE_L1PHIE18_L2PHIC20.cpp -cflags "-std=c++11"
set_top TE_L1PHIE18_L2PHIC20
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE19_L2PHIC17
open_project -reset TE_L1PHIE19_L2PHIC17
add_files ../TrackletAlgorithm/TE_L1PHIE19_L2PHIC17.cpp -cflags "-std=c++11"
set_top TE_L1PHIE19_L2PHIC17
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE19_L2PHIC18
open_project -reset TE_L1PHIE19_L2PHIC18
add_files ../TrackletAlgorithm/TE_L1PHIE19_L2PHIC18.cpp -cflags "-std=c++11"
set_top TE_L1PHIE19_L2PHIC18
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE19_L2PHIC19
open_project -reset TE_L1PHIE19_L2PHIC19
add_files ../TrackletAlgorithm/TE_L1PHIE19_L2PHIC19.cpp -cflags "-std=c++11"
set_top TE_L1PHIE19_L2PHIC19
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

delete_project TE_L1PHIE19_L2PHIC20
open_project -reset TE_L1PHIE19_L2PHIC20
add_files ../TrackletAlgorithm/TE_L1PHIE19_L2PHIC20.cpp -cflags "-std=c++11"
set_top TE_L1PHIE19_L2PHIC20
open_solution -reset "solution1"
source set_fpga.tcl
create_clock -period $clockperiod -name default
csynth_design
export_design -rtl verilog -format ip_catalog

exit
