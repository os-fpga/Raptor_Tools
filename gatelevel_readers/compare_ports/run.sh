clear
rm -rf CMakeCache.txt CMakeFiles/ cmake_install.cmake libcompare.so Makefile compare_info src/compare_ports_wrap.cpp
cmake -b . -s .
make -j
tclsh read_json.tcl
