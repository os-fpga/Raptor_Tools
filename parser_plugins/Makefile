#Copyright (c) 2021-2023 RapiSilicon

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

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))

INSTALL_PATH:= ${current_dir}

release: run-cmake-release

debug: run-cmake-debug

surelog:
	cd synlig_plugin/third_party/surelog ; make ; make install PREFIX=${INSTALL_PATH}/dinstall ; cd ..

run-cmake-release:
	cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
	cmake --build build
run-cmake-debug:
	cd synlig_plugin/third_party/surelog ; make debug ; make debug install PREFIX=${INSTALL_PATH}/dinstall ; cd ..
	export UHDM_INSTALL_DIR=${INSTALL_PATH}/dinstall; make -C ${current_dir}/yosys-f4pga-plugins/ install PLUGIN_LIST=systemverilog  -j$(nproc)
