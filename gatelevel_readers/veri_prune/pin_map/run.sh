clear
mkdir build
cd build && rm -rf *
cmake ..
make -j
./pin_map -io ../Test_cases/serdes/post_synth_ports.json -i ../Test_cases/serdes/serdies_nl_interface.json -u ../Test_cases/serdes/Serdies_design.pin -t ../Test_cases/Gemini_Pin_Table.csv -o output.pin