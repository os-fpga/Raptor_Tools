# Pin Map Tool

The Pin Map Tool is a command-line utility that updates the pin constraint file by excluding GearBox constructs. It is based on a netlist file, pin table, and a user constraint file. The updated pin constraint file will be provided to VPR.

## Getting Started

### Prerequisites

- CMake
- C++ Compiler

### Installation

```bash
clear
mkdir build
cd build && rm -rf *
cmake ..
make -j 

## Usage

To use this project, follow the steps below:

### Step 1

Make sure you have all the necessary files ready:

- A file containing IO infrmation (post_synth_ports.json)
- A file containing interface module ports
- A user SDC file
- A pin table file

### Step 2

Run the following command:

./pin_map -io <path_to_mod_ios_file> -i <path_to_interface_file> -u <path_to_user_sdc_file> -t <path_to_pin_table> -o <output_sdc_file>


Replace the placeholders `<path_to_mod_ios_file>`, `<path_to_interface_file>`, `<path_to_user_sdc_file>`, `<path_to_pin_table>`, and `<output_sdc_file>` with the appropriate file paths.

### Step 3

To build the project and generate the output file, use the following command:

./run.sh 


### Command Line Arguments

Use the following command line arguments to specify the paths to the necessary files:

- `-io` or `--ios`: Path to the modified I/Os file.
- `-i` or `--intf`: Path to the interface file.
- `-u` or `--user_sdc`: Path to the user SDC file.
- `-t` or `--ptable`: Path to the pin table file.
- `-o` or `--out`: Output SDC file path.

Make sure you replace the placeholder arguments with the appropriate file paths in the actual command.

