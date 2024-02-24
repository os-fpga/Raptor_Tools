# Introduction

Welcome to the `veri_prune` Plugin for Raptor_Tools within the Yosys framework! This plugin is designed to enhance your synthesis flow by manipulating synthesized designs, particularly focusing on cutting IO primitives from design, organizing them into an interface module and connecting the edited design with the interface module. This README will guide you through the setup and usage of the plugin.

# Directory Structure

```bash
├── Makefile
├── README.md
├── src
│   └── veri_prune
│       ├── veri_prune.cpp
│       └── veri_prune.h
└── Test_cases
    ├── primitive_example_design_1
    │   ├── primitive_example_design_1.ys+
    │   ├── rtl
    │   └── synthesis
    ├── primitive_example_design_2
    │   ├── primitive_example_design_2.ys
    │   ├── rtl
    │   └── synthesis
    ├── primitive_example_design_3
    │   ├── primitive_example_design_3.ys
    │   ├── rtl
    │   └── synthesis
    ├── primitive_example_design_4
    │   ├── primitive_example_design_4.ys
    │   ├── rtl
    │   └── synthesis
    ├── primitive_example_design_5
    │   ├── primitive_example_design_5.ys
    │   ├── rtl
    │   └── synthesis
    └── primitive_example_design_6
        ├── primitive_example_design_6.ys
        ├── rtl
        └── synthesis
```

* `Makefile` sets up the environment and installs the plugin
* `src` contains the source code
* `Test_cases` contains the test cases and each test contains

    * `rtl` folder containg the RTL file
    * `synthesis` folder containing the synthesized netlist without using `veri_prune` plugin
    * A yosys (.ys) script to synthesize the design with `veri_prune` plugin

# Installation

To get started with the `veri_prune` Plugin, follow these steps:

Clone the veri_prune branch of the yosys_verific_rs repository:

```bash
git clone -b veri_prune git@github.com:os-fpga/yosys_verific_rs.git
``````
Navigate to the cloned repository:
```bash
cd yosys_verific_rs
```
Update submodules recursively:

```bash
git submodule update --init --recursive
```
Build and install the plugin:
```bash
make release
```
# Usage

Here's a basic guide on how to utilize the `veri_prune` plugin:

1. Prepare your RTL design and synthesis scripts. Write a Yosys script (.ys) for your synthesis flow. Make sure to include the `veri_prune` plugin invocation after the synthesis command.

    Example Yosys script (your_synthesis_script.ys):

    ```tcl
    read_verilog $design.v
    # Other synthesis commands...
    synth
    # add and call veri_prune plugin
    plugin -i veriprune
    veri_prune
    # Other post-processing commands...
    write_verilog synthesized_design.v
    ```

    Please check yosys scripts placed in test cases for a better understanding.

2. Execute the Yosys script using the installed Yosys binary:

    ```bash
    ${yosys} -s ${yosys_script}.ys
    ```
    Forexample, to run the `primitive_example_design_1` test case, run:
    ```bash
    cd ${primitive_example_design_1}
    ${yosys} -s primitive_example_design_1.ys
    ```
    In `yosys_verific_rs`, yosys is installed in `{root}/yosys/install/bin/yosys`.
    
3. Analyze the synthesized design output to observe the effects of the `veri_prune` plugin.

# Contributing
Contributions to the `veri_prune` Plugin and Raptor Tools are welcome! If you have ideas for improvements, bug fixes, or new features, feel free to submit a pull request or open an issue on the GitHub repository.

# Disclaimer
Please note that the `veri_prune` Plugin is still a work in progress. While we strive to ensure its functionality and reliability, there may be limitations and bugs present. Your feedback and contributions can help improve the plugin for everyone.