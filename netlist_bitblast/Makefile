#Copyright (c) 2021-2024 Rapid Silicon

# Use bash as the default shell
SHELL := /bin/bash

XVFB = xvfb-run --auto-servernum --server-args="-screen 0, 1280x1024x24"

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
# make MONACO_EDITOR=0 enables the QScintilla based Editor in place of the WebEngine based Monaco Editor
IS_WSL_OR_DOCKER := $(shell \
    if [[ -s /etc/wsl.conf ]] || \
       grep -qi "wsl2" /proc/version || \
       uname -r | grep -qi "wsl2" || \
       [ -n "$$WSL_DISTRO_NAME" ] || \
       which powershell.exe &> /dev/null || \
       [ -f "/.dockerenv" ]; then \
        echo 0; \
    else \
        echo 1; \
    fi \
)
MONACO_EDITOR ?= $(IS_WSL_OR_DOCKER)

IPA ?= 1

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
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -S . -B build

test: release
	build/bin/bitblast -DSYNTHESIS=1 tests/gen_clk/fabric_dut_post_route.v -v lib/pnr/genesis3/primitives.v -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ -top fabric_dut -bitblast -write bitblasted.v
	build/bin/bitblast bitblasted.v -v lib/pnr/genesis3/primitives.v -y lib/pnr/genesis3/mockup/FPGA_PRIMITIVES_MODELS/sim_models/verilog/ -top fabric_dut -nonote -noinfo