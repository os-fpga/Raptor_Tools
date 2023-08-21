mkdir build
cd build && rm -rf *
cmake ..
make -j
#./veri_prune ../example/OSERDES_4_to_1/o_serdes_4_to_1_gate.v oserdes_nl.v oserdes_wrapper.v
#./veri_prune ../example/io_buf.v io_buf_nl.v io_buf_wrapper.v
#./veri_prune ../example/ibuf_obuf.v ibuf_obuf_nl.v ibuf_obuf_wrapper.v
#./veri_prune ../example/DDR_BUFS/iddr_oddr_post_synth.v iddr_nl.v iddr_wrapper.v
#./veri_prune ../example/Differential/differential_gate.v difrntl_nl.v difrntl_wrapper.v
#./veri_prune ../example/i_buf_ds/ibuf_ds_post_synth.v ibuf_ds_post_synth_nl.v ibuf_ds_post_synth_wrapper.v
#./veri_prune ../example/Input_DDR/input_ddr_gate.v input_ddr_gate_nl.v input_ddr_gate_wrapper.v
#./veri_prune ../example/IO_BUFS/io_buf_post_synth.v io_buf_post_synth_nl.v io_buf_post_synth_wrapper.v
#./veri_prune ../example/o_buft/Obuft_post_synth.v Obuft_post_synth_nl.v Obuft_post_synth_wrapper.v
#./veri_prune ../example/o_buft_ds/obuft_ds_post_synth.v obuft_ds_nl.v obuft_ds_wrapper.v
#./veri_prune ../example/Output_DDR/output_ddr_gate.v output_ddr_gate_nl.v output_ddr_gate_wrapper.v
#./veri_prune ../example/SERDES_DESIGN/serdes_post_synth.v serdes_post_synth_nl.v serdes_post_synth_wrapper.v
#./veri_prune ../example/Tristate/tristate_gate.v tristate_gate_nl.v tristate_gate_wrapper.v
./veri_prune ../example/DDR_BUFS/iddr_oddr_post_synth.v iddr_nl.v iddr_wrapper.v