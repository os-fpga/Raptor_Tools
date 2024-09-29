# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alain/os-fpga/Raptor_Tools/parser_plugins

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alain/os-fpga/Raptor_Tools/parser_plugins/build

# Utility rule file for ghdl_plugin_build.

# Include any custom commands dependencies for this target.
include CMakeFiles/ghdl_plugin_build.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ghdl_plugin_build.dir/progress.make

CMakeFiles/ghdl_plugin_build: ../ghdl-yosys-plugin/ghdl.so

../ghdl-yosys-plugin/ghdl.so: ../ghdl-yosys-plugin/src/ghdl.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/alain/os-fpga/Raptor_Tools/parser_plugins/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../ghdl-yosys-plugin/ghdl.so"
	cd /home/alain/os-fpga/Raptor_Tools/parser_plugins/ghdl-yosys-plugin && echo \ \ \ \ \ \ \ Compiling\ GHDL\ plugin
	cd /home/alain/os-fpga/Raptor_Tools/parser_plugins/ghdl-yosys-plugin && make install YOSYS_PATH=/home/alain/os-fpga/Raptor_Tools/parser_plugins/synlig/third_party/yosys/ YOSYS_CONFIG=/home/alain/os-fpga/Raptor_Tools/parser_plugins/synlig/third_party/yosys/yosys-config GHDL=/home/alain/os-fpga/Raptor_Tools/parser_plugins/../HDL_simulator/build/bin/HDL_simulator/GHDL/bin/ghdl
	cd /home/alain/os-fpga/Raptor_Tools/parser_plugins/ghdl-yosys-plugin && /usr/bin/cmake -E make_directory /home/alain/os-fpga/Raptor_Tools/parser_plugins/synlig/third_party/yosys//install/share/yosys/plugins/
	cd /home/alain/os-fpga/Raptor_Tools/parser_plugins/ghdl-yosys-plugin && /usr/bin/cmake -E copy /home/alain/os-fpga/Raptor_Tools/parser_plugins/ghdl-yosys-plugin/ghdl.so /home/alain/os-fpga/Raptor_Tools/parser_plugins/synlig/third_party/yosys//install/share/yosys/plugins/

ghdl_plugin_build: CMakeFiles/ghdl_plugin_build
ghdl_plugin_build: ../ghdl-yosys-plugin/ghdl.so
ghdl_plugin_build: CMakeFiles/ghdl_plugin_build.dir/build.make
.PHONY : ghdl_plugin_build

# Rule to build all files generated by this target.
CMakeFiles/ghdl_plugin_build.dir/build: ghdl_plugin_build
.PHONY : CMakeFiles/ghdl_plugin_build.dir/build

CMakeFiles/ghdl_plugin_build.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ghdl_plugin_build.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ghdl_plugin_build.dir/clean

CMakeFiles/ghdl_plugin_build.dir/depend:
	cd /home/alain/os-fpga/Raptor_Tools/parser_plugins/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alain/os-fpga/Raptor_Tools/parser_plugins /home/alain/os-fpga/Raptor_Tools/parser_plugins /home/alain/os-fpga/Raptor_Tools/parser_plugins/build /home/alain/os-fpga/Raptor_Tools/parser_plugins/build /home/alain/os-fpga/Raptor_Tools/parser_plugins/build/CMakeFiles/ghdl_plugin_build.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ghdl_plugin_build.dir/depend

