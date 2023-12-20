clear
mkdir build
cd build && rm -rf *
cmake ..
make -j
./dump -u ../Test_cases/primitive_example_design_1/primitive_example_design_1.pin -o primitive_example_design_1.tcl -j primitive_example_design_1.json
tclsh primitive_example_design_1.tcl
#./pin_map -io ../Test_cases/primitive_example_design_1/post_synth_ports.json -i ../Test_cases/primitive_example_design_1/interface.json -j primitive_example_design_1.json -t ../Test_cases/Gemini_Pin_Table.csv -o primitive_example_design_1.pin
./dump -u ../Test_cases/primitive_example_design_2/primitive_example_design_2.pin -o primitive_example_design_2.tcl -j primitive_example_design_2.json
tclsh primitive_example_design_2.tcl
#./pin_map -io ../Test_cases/primitive_example_design_2/post_synth_ports.json -i ../Test_cases/primitive_example_design_2/interface.json -j primitive_example_design_2.json -t ../Test_cases/Gemini_Pin_Table.csv -o primitive_example_design_2.pin
./dump -u ../Test_cases/primitive_example_design_3/primitive_example_design_3.pin -o primitive_example_design_3.tcl -j primitive_example_design_3.json
tclsh primitive_example_design_3.tcl
#./pin_map -io ../Test_cases/primitive_example_design_3/post_synth_ports.json -i ../Test_cases/primitive_example_design_3/interface.json -j primitive_example_design_3.json -t ../Test_cases/Gemini_Pin_Table.csv -o primitive_example_design_3.pin
./dump -u ../Test_cases/primitive_example_design_4/primitive_example_design_4.pin -o primitive_example_design_4.tcl -j primitive_example_design_4.json
tclsh primitive_example_design_4.tcl
#./pin_map -io ../Test_cases/primitive_example_design_4/post_synth_ports.json -i ../Test_cases/primitive_example_design_4/interface.json -j primitive_example_design_4.json -t ../Test_cases/Gemini_Pin_Table.csv -o primitive_example_design_4.pin
./dump -u ../Test_cases/serdes/serdes.pin -o serdes.tcl -j serdes.json
tclsh serdes.tcl
#./pin_map -io ../Test_cases/serdes/post_synth_ports.json -i ../Test_cases/serdes/interface.json -j serdes.json -t ../Test_cases/Gemini_Pin_Table.csv -o serdes.pin