SHELL := /bin/bash
ROOT_DIR ?= $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
PREFIX ?= /usr/local

print_prefix:
	@echo "Prefix is $(PREFIX)"

build: print_prefix
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -S . -B build 

install: build
	cmake --install build

clean:
	rm -rf build