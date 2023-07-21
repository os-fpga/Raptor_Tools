# Netlist Editing

## Objective
* Make netlists containing IO primitives compatible with VPR

## Directory Structure
```bash
├── CMakeLists.txt
├── example
│   └── example.v
├── README.md
├── run.sh
└── src
    ├── main.cpp
    └── veri_prune
```

* `src` contains the source code
* `example/example.v` is the example testcase
* `run.sh` creates executable and runs it using example testcase

## How to run
* Simplest way is to run `run.sh`
    * It creates `veri_prune` executable in build directory
    * Runs th executable and generates two files `nl_to_vpr.v` and `wrapper.v` from example testcase

* After running `run.sh` for the first time, executable generated in the build folder can be run
    * Using command `./veri_prune` to run with default arguments
    * Using command `./veri_prune ${arg1} ${arg2} ${arg3}` where `${arg1}`, `${arg2}` and `${arg3}` are the names of input file, file containing netlist generated for VPR and the wrapper file