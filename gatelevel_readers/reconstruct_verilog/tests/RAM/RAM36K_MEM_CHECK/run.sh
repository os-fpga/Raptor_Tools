rm -rf ./build
mkdir build && cd build
cp ../new_sim.v ./
g++ -o ./generate_data ../main.cpp -I../../../../src
# Generate New Random memory content
./generate_data
cpp -P -w  new_sim.v -o test_run.v
# don't forget to install iverilog : sudo apt install iverilog
iverilog -g2012 test_run.v
./a.out
