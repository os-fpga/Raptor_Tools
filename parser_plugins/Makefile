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

all:
	cmake -DCMAKE_BUILD_TYPE=Release -DCPU_CORES=$(CPU_CORES) $(ADDITIONAL_CMAKE_OPTIONS) -S . -B build
	cmake --build build -j $(CPU_CORES)
