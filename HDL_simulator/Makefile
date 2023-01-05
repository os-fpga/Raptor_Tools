SHELL := /bin/bash
PREFIX ?= /usr/local

print_prefix:
	@echo "Prefix is $(PREFIX)"

build: print_prefix
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -S . -B build 
	cmake --build build -j $(CPU_CORES)

install: build
	cmake --install build

clean:
	rm -rf build
deep_clean:
	rm -rf build sim.tar.gz
