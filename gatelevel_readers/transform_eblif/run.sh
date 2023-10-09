mkdir build
cd build && rm -rf *
cmake ..
make -j
mkdir res
./transform_eblif ../tests/and2/and2_post_synth.eblif ./res/and2_post_synth_transformed.eblif
diff ./res/and2_post_synth_transformed.eblif ../tests/and2/golden/and2_post_synth_transformed.eblif
./transform_eblif ../tests/accumulator/read_netlist_blif_accumulator_post_synth.blif ./res/read_netlist_blif_accumulator_post_synth_transformed.blif
diff ./res/read_netlist_blif_accumulator_post_synth_transformed.blif ../tests/accumulator/golden/read_netlist_blif_accumulator_post_synth.blif 