 
# Makefile to run the build flow using CMake

# Specify the default target
all: build

# Build the project using CMake
build:
	mkdir -p build
	cd build && cmake ..
	cd build && make

# Clean the build artifacts
clean:
	rm -rf build

.PHONY: all build clean
