# Copyright (C) 2022 RapidSilicon..

# Either find yosys in system and use its path or use the given path
YOSYS_PATH ?= $(realpath $(dir $(shell which yosys))/../../..)

# Find yosys-config, throw an error if not found
YOSYS_CONFIG ?= $(YOSYS_PATH)/bin/yosys-config
ifeq (,$(wildcard $(YOSYS_CONFIG)))
$(error "Didn't find 'yosys-config' under '$(YOSYS_PATH)'")
endif

CXX ?= $(shell $(YOSYS_CONFIG) --cxx)
CXXFLAGS ?= $(shell $(YOSYS_CONFIG) --cxxflags) -Isrc/veri_prune -I../../libs/external/nlohmann_json -v
LDFLAGS ?= $(shell $(YOSYS_CONFIG) --ldflags)
LDLIBS ?= $(shell $(YOSYS_CONFIG) --ldlibs)
PLUGINS_DIR ?= $(shell $(YOSYS_CONFIG) --datdir)/plugins
DATA_DIR ?= $(shell $(YOSYS_CONFIG) --datdir)
EXTRA_FLAGS ?=

COMMON			=  $(YOSYS_PATH)/../../yosys-rs-plugin/common
GENESIS			=  $(YOSYS_PATH)/../../yosys-rs-plugin/genesis
GENESIS2		=  $(YOSYS_PATH)/../../yosys-rs-plugin/genesis2
GENESIS3		=  $(YOSYS_PATH)/../../yosys-rs-plugin/genesis3
VERILOG_MODULES	= $(COMMON)/cells_sim.v \
				  $(COMMON)/simlib.v \
				  $(GENESIS)/cells_sim.v \
				  $(GENESIS)/dsp_sim.v \
				  $(GENESIS)/ffs_map.v \
				  $(GENESIS)/dsp_map.v \
				  $(GENESIS)/dsp_final_map.v \
				  $(GENESIS)/arith_map.v \
				  $(GENESIS)/all_arith_map.v \
				  $(GENESIS)/brams_map.v \
				  $(GENESIS)/brams_map_new.v \
				  $(GENESIS)/brams_final_map.v \
				  $(GENESIS)/brams_final_map_new.v \
				  $(GENESIS)/brams.txt \
				  $(GENESIS)/brams_new.txt \
				  $(GENESIS)/brams_async.txt \
				  $(GENESIS)/TDP18K_FIFO.v \
				  $(GENESIS)/sram1024x18.v \
				  $(GENESIS)/ufifo_ctl.v \
				  $(GENESIS)/cells_sim.vhd \
				  $(GENESIS)/adder_carry.vhdl \
				  $(GENESIS)/dffnsre.vhdl \
				  $(GENESIS)/dffsre.vhdl \
				  $(GENESIS)/latchsre.vhdl \
				  $(GENESIS)/lut.vhdl \
				  $(GENESIS)/shr.vhdl \
				  $(GENESIS3)/cells_sim.vhd \
				  $(GENESIS3)/ffs_map.v \
				  $(GENESIS3)/dsp_map.v \
				  $(GENESIS3)/dsp_final_map.v \
				  $(GENESIS3)/arith_map.v \
				  $(GENESIS3)/all_arith_map.v \
				  $(GENESIS3)/brams_map.v \
				  $(GENESIS3)/brams_map_new.v \
				  $(GENESIS3)/brams_final_map.v \
				  $(GENESIS3)/brams_final_map_new.v \
				  $(GENESIS3)/brams.txt \
				  $(GENESIS3)/brams_new.txt \
				  $(GENESIS3)/brams_new_swap.txt \
				  $(GENESIS3)/brams_async.txt \
				  $(GENESIS3)/FPGA_PRIMITIVES_MODELS/blackbox_models/cell_sim_blackbox.v \
				  $(GENESIS3)/llatches_sim.v \
				  $(GENESIS3)/dsp38_map.v \
				  $(GENESIS3)/FPGA_PRIMITIVES_MODELS/sim_models/verilog/DSP38.v

# Directories
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/veri_prune/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Executable name
EXEC_NAME = veriprune

# Static library name
LIB_NAME = libveriprune.a

# Targets
all: $(BUILD_DIR)/$(EXEC_NAME)

# Build executable
$(BUILD_DIR)/$(EXEC_NAME): $(BUILD_DIR)/main.o $(LIB_DIR)/$(LIB_NAME)
	$(CXX) $(CXXFLAGS) -o $@ $< -L$(LIB_DIR) -l:$(LIB_NAME)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build static library
$(LIB_DIR)/$(LIB_NAME): $(OBJ_FILES)
	mkdir -p $(@D)
	ar rcs $@ $^

install_plugin: $(BUILD_DIR)/$(EXEC_NAME)
	install -D $< $(PLUGINS_DIR)/$<

install_modules: $(VERILOG_MODULES)
	$(foreach f,$^,install -D $(f) $(DATA_DIR)/rapidsilicon/$(f);)

.PHONY: install
install: install_plugin install_modules

valgrind_gen:
	$(MAKE) -C tests valgrind_gen YOSYS_PATH=$(YOSYS_PATH)

valgrind:
	$(MAKE) -C tests valgrind_gen2 YOSYS_PATH=$(YOSYS_PATH)

test_gen:
	$(MAKE) -C tests tests_gen YOSYS_PATH=$(YOSYS_PATH)

test:
	$(MAKE) -C tests tests_gen2 YOSYS_PATH=$(YOSYS_PATH)

# Clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(EXEC_NAME) $(LIB_DIR)/*.a
	