# This makefile will build the Net class and its GoogleTests.

# Set the name of the project.
PROJECT_NAME := device_modeling

# Set the path to the source directory.
SRC_DIR := .

# Set the path to the build directory.
BUILD_DIR := build

# Set the path to the GoogleTest library.
GTEST_LIB := ../external/googletest/libgtest.a

# Set the value of the CMake Env variable GOOGLETEST_VERSION.
GOOGLETEST_VERSION := 1.10.0

.PHONY: all clean init

all: $(BUILD_DIR)/$(PROJECT_NAME)

$(BUILD_DIR)/$(PROJECT_NAME):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug -DGOOGLETEST_VERSION=$(GOOGLETEST_VERSION) ..
	cd $(BUILD_DIR) && make all

clean:
	rm -rf $(BUILD_DIR)

init:
	mkdir -p $(BUILD_DIR)
