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
```
## Usage

To use this project, follow the steps below:

### Step 1

Make sure you have all the necessary files ready:

- A file containing IO infrmation (post_synth_ports.json)
- A file containing interface module ports
- A user SDC file
- A pin table file

### Step 2

Run the following commands:
```
1. ./dump -u <path_to_user_sdc_file> -o <dumped_tcl> -j <dumped_json>
2. tclsh <dumped_tcl>
3. ./pin_map -io <path_to_mod_ios_file> -i <path_to_interface_file> -j <dumped_json> -t <path_to_pin_table> -o <output_sdc_file>
```

Replace the placeholders `<dumped_tcl>`, `<dumped_json>`, `<path_to_mod_ios_file>`, `<path_to_interface_file>`, `<path_to_user_sdc_file>`, `<path_to_pin_table>`, and `<output_sdc_file>` with the appropriate file paths.

* First command takes the user sdc file and the path to json as an input arguments and dumps the tcl file 
* The second command executes the tcl file to dump the json file
* Finally, the third command uses the dumped json alongwith original ports, interface ports and pin table to create the updated pin file

### Step 3

To build the project and generate the output file, use the following command:

./run.sh 


### Command Line Arguments

Use the following command line arguments to specify the paths to the necessary files:
```
- `-j` or `--json` : Path to dump json file from sdc.
- `-io` or `--ios`: Path to the modified I/Os file.
- `-i` or `--intf`: Path to the interface file.
- `-u` or `--user_sdc`: Path to the user SDC file.
- `-t` or `--ptable`: Path to the pin table file.
- `-o` or `--out`: Output SDC file path | output tcl.
```
Make sure you replace the placeholder arguments with the appropriate file paths in the actual command.






