# chain_te_tc

## Creating the project

After cloning this repository, the project for the `chain_te_tc` top module can be created with the following commands:

```bash
cd firmware-hls/project/
git checkout tracklet_calculator
vivado_hls -f script_TE.tcl
vivado_hls -f script_TC.tcl
cd ../TopModules/chain_te_tc/project/
vivado -mode tcl -source chain_te_tc.tcl
```

Then the project can be opened in the Vivado GUI with

```bash
vivado chain_te_tc/chain_te_tc.xpr
```
