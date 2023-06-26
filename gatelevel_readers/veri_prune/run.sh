mkdir build
cd build && rm -rf *
cmake ..
make -j
./veri_prune ../example/iddr.v