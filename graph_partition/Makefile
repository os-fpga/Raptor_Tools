SHELL := /bin/bash
PREFIX ?= /usr/local

ifeq ($(CPU_CORES),)
	CPU_CORES := $(shell nproc)
	ifeq ($(CPU_CORES),)
		CPU_CORES := $(shell sysctl -n hw.physicalcpu)
	endif
	ifeq ($(CPU_CORES),)
		CPU_CORES := 2  # Good minimum assumption
	endif
endif

print_prefix:
	@echo "Prefix is $(PREFIX)"

build: print_prefix
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DKAHYPAR_DOWNLOAD_TBB=On -DKAHYPAR_DOWNLOAD_BOOST=On -S . -B build 
	cmake --build build -j40

install: build
	cmake --install build

clean:
	rm -rf build


