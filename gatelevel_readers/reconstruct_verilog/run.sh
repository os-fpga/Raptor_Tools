mkdir build
cd build && rm -rf *
cmake ..
make -j
mkdir res
./bin/finalize ../tests/and2/and2_post_synth.eblif ./res/and2_post_synth_transformed.eblif
diff ./res/and2_post_synth_transformed.eblif ../tests/and2/golden/and2_post_synth_transformed.eblif
./bin/finalize ../tests/accumulator/read_netlist_blif_accumulator_post_synth.blif ./res/read_netlist_blif_accumulator_post_synth_transformed.blif
diff ./res/read_netlist_blif_accumulator_post_synth_transformed.blif ../tests/accumulator/golden/read_netlist_blif_accumulator_post_synth.blif 
./bin/finalize ../tests/DSP38/dsp38_out.eblif ./res/dsp38_out_transformed.eblif
diff ./res/dsp38_out_transformed.eblif ../tests/DSP38/golden/dsp38_out_golden.eblif
./bin/finalize ../tests/carry/counter.eblif ./res/counter.eblif
diff ./res/counter.eblif ../tests/carry/golden/counter.eblif
./bin/finalize ../tests/RAM/TDP_RAM18KX2_primitive_inst_post_synth.eblif ./res/TDP_RAM18KX2_primitive_inst_post_synth.eblif
diff ./res/TDP_RAM18KX2_primitive_inst_post_synth.eblif ../tests/RAM/golden/TDP_RAM18KX2_primitive_inst_post_synth_golden.eblif
./bin/finalize ../tests/RAM/TDP_RAM36K_primitive_inst_post_synth.eblif ./res/TDP_RAM36K_primitive_inst_post_synth.eblif
diff ./res/TDP_RAM36K_primitive_inst_post_synth.eblif ../tests/RAM/golden/TDP_RAM36K_primitive_inst_post_synth_golden.eblif
./bin/finalize ../tests/new_DSP38/verilog/accum_output_registered_iverilog_post_rout.v ./res/accum_output_registered_iverilog_post_rout.v
diff ./res/accum_output_registered_iverilog_post_rout.v ../tests/new_DSP38/golden/accum_output_registered_iverilog_post_rout_golden.v
