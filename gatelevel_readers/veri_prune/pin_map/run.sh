clear
mkdir build
cd build && rm -rf *
cmake ..
make -j
./pin_map -io ../Test_cases/serdes/post_synth_ports.json -i ../Test_cases/serdes/serdies_nl_interface.json -u ../Test_cases/serdes/Serdies_design.pin -t ../Test_cases/Gemini_Pin_Table.csv -o output.pin
./pin_map -io ../Test_cases/primitive_example_design_1/post_synth_ports.json -i ../Test_cases/primitive_example_design_1/interface.json -u ../Test_cases/primitive_example_design_1/primitive_example_design_1.pin -t ../Test_cases/Gemini_Pin_Table.csv -o primitive_example_design_1.pin
./pin_map -io ../Test_cases/primitive_example_design_2/post_synth_ports.json -i ../Test_cases/primitive_example_design_2/interface.json -u ../Test_cases/primitive_example_design_2/primitive_example_design_2.pin -t ../Test_cases/Gemini_Pin_Table.csv -o primitive_example_design_2.pin
./pin_map -io ../Test_cases/primitive_example_design_4/post_synth_ports.json -i ../Test_cases/primitive_example_design_4/interface.json -u ../Test_cases/primitive_example_design_4/primitive_example_design_4.pin -t ../Test_cases/Gemini_Pin_Table.csv -o primitive_example_design_4.pin