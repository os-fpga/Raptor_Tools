#Copyright (c) 2021-2024 Rapid Silicon

# Use bash as the default shell
SHELL := /bin/bash

ifdef $(LC_ALL)
	undefine LC_ALL
endif

ifeq ($(CPU_CORES),)
	CPU_CORES := $(shell nproc)
	ifeq ($(CPU_CORES),)
		CPU_CORES := $(shell sysctl -n hw.physicalcpu)
	endif
	ifeq ($(CPU_CORES),)
		CPU_CORES := 2  # Good minimum assumption
	endif
endif

PREFIX ?= /usr/local
ADDITIONAL_CMAKE_OPTIONS ?=

# If 'on', then the progress messages are printed. If 'off', makes it easier
# to detect actual warnings and errors  in the build output.
RULE_MESSAGES ?= on

release: run-cmake-release
	cmake --build build -j $(CPU_CORES)

debug: run-cmake-debug
	cmake --build dbuild -j $(CPU_CORES)

run-cmake-release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -S . -B build

run-cmake-debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -S . -B dbuild

test_gen_clk:
	mkdir -p test_results/gen_clk
	build/bin/bitblast -DSYNTHESIS=1 tests/gen_clk/fabric_dut_post_route.v \
	  -v lib/pnr/genesis3/primitives.v \
	  -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ \
	  -sdf_in tests/gen_clk/fabric_dut_post_route.sdf \
	  -sdf_out test_results/gen_clk/fabric_dut_post_route.sdf \
	  -top fabric_dut -bitblast -write test_results/gen_clk/bitblasted.v
	build/bin/bitblast test_results/gen_clk/bitblasted.v -v lib/pnr/genesis3/primitives.v -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ -top fabric_dut -nonote -noinfo

test_dsp:
	mkdir -p test_results/dsp
	build/bin/bitblast -DSYNTHESIS=1 tests/dsp/fabric_GJC4_post_route.v \
	  -v lib/pnr/genesis3/primitives.v \
	  lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/primitives_mapping/DSP/rs_dsp_multxxx_post_pnr_mapping.v \
	  -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ \
	  -sdf_in tests/dsp/fabric_GJC4_post_route.sdf \
	  -sdf_out test_results/dsp/fabric_GJC4_post_route.sdf \
	  -top fabric_GJC4 -bitblast -write test_results/dsp/bitblasted.v
	build/bin/bitblast test_results/dsp/bitblasted.v -v lib/pnr/genesis3/primitives.v \
	  lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/primitives_mapping/DSP/rs_dsp_multxxx_post_pnr_mapping.v \
	  -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ -top fabric_GJC4 -nonote -noinfo

test_rom:
	mkdir -p test_results/rom
	build/bin/bitblast -DSYNTHESIS=1 tests/rom/fabric_SBox_post_route.v \
	  -v lib/pnr/genesis3/primitives.v \
	  lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/primitives_mapping/DSP/rs_dsp_multxxx_post_pnr_mapping.v \
	  lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/primitives_mapping/BRAM/rs_tdp36k_post_pnr_mapping.v \
	  -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ \
	  -sdf_in tests/rom/fabric_SBox_post_route.sdf \
	  -sdf_out test_results/rom/fabric_SBox_post_route.sdf \
	  -top fabric_SBox -bitblast -write test_results/rom/bitblasted.v
	build/bin/bitblast test_results/rom/bitblasted.v -v lib/pnr/genesis3/primitives.v \
	  lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/primitives_mapping/DSP/rs_dsp_multxxx_post_pnr_mapping.v \
      lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/primitives_mapping/BRAM/rs_tdp36k_post_pnr_mapping.v \
	  -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ \
	  -top fabric_SBox -nonote -noinfo

test: release test_gen_clk test_dsp test_rom
