mkdir build
cd build && rm -rf *
cd ../../../verific_rs/tclmain/ && make -j
cd ../../gatelevel_readers/VeriPrune/build
cmake ..
make -j
./veri_prune