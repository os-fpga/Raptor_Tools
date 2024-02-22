create_design primitive_example_design_1
target_device GEMINI_COMPACT_104x68
add_include_path ./rtl
add_library_path ./rtl
add_library_ext .v .sv
add_design_file ./rtl/primitive_example_design_1.v
set_top_module primitive_example_design_1
analyze
synthesize delay
packing
global_placement
place
route
sta
power
bitstream 
