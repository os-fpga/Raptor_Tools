mkdir build
cd build && rm -rf *
cmake ..
make -j
mkdir res
./transform_eblif ../tests/and2/and2_post_synth.eblif ./res/and2_post_synth_transformed.eblif
diff ./res/and2_post_synth_transformed.eblif ../tests/and2/golden/and2_post_synth_transformed.eblif
./transform_eblif ../tests/accumulator/read_netlist_blif_accumulator_post_synth.blif ./res/read_netlist_blif_accumulator_post_synth_transformed.blif
diff ./res/read_netlist_blif_accumulator_post_synth_transformed.blif ../tests/accumulator/golden/read_netlist_blif_accumulator_post_synth.blif 
./transform_eblif ../tests/DSP38/dsp38_out.eblif ./res/dsp38_out_transformed.eblif
diff ./res/dsp38_out_transformed.eblif ../tests/DSP38/golden/dsp38_out_golden.eblif
