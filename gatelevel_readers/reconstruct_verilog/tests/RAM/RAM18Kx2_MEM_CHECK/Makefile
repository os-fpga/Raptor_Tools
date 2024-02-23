# Makefile

# Default target
all: build

# Build target with dependencies
build: main.cpp new_sim.v
	@echo "Running run.sh, build and test ..."
	./run.sh

# Clean target
clean:
	@echo "Cleaning..."
	# Add your cleanup commands here
	rm -rf ./build

# Set the default goal to be the "all" target
.DEFAULT_GOAL := all


