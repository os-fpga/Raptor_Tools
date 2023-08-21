SHELL := /bin/bash
ROOT_DIR ?= $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
PREFIX ?= /usr/local

print_prefix:
	@echo "Prefix is $(PREFIX)"

build: print_prefix
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCGT_GITHUB_REPO_PATH=$(CHATGPT_REPO_PATH) -S . -B build
	cd build && make -j`nproc`

install: build
	cmake --install build

clean:
	rm -rf build

deep_clean:
	rm -rf chatgpt_env.zip litex_env.zip python_3.zip build

