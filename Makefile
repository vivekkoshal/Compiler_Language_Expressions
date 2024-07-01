COMPILER = g++

SOURCE_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BIN_DIR = bin
LINKER_DIR = tests/linker

SRC_FILES = $(wildcard $(SOURCE_DIR)/*.cpp)
MAIN_FILE = $(TEST_DIR)/main.cpp

COMPILER_FLAGS = -I$(INCLUDE_DIR) -Wall -O3 -std=c++17

EXECUTABLE = $(BIN_DIR)/e++
TEST_EXECUTABLE = $(BIN_DIR)/tester

$(EXECUTABLE): $(SRC_FILES) $(MAIN_FILE)
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^
	@echo "Compilation Successful!"

clean:
	rm -f $(EXECUTABLE)

.PHONY: clean

OS_TYPE := $(shell uname -s)
ifeq ($(OS_TYPE),Darwin)
	LINKER_FILE := $(LINKER_DIR)/macos/tester_dep.o
else
	LINKER_FILE := $(LINKER_DIR)/linux/tester_dep.o
endif

tester: $(SRC_FILES) $(LINKER_FILE)
	@$(COMPILER) $(COMPILER_FLAGS) -w -o $(TEST_EXECUTABLE) $^
	@echo "Tester Compilation Successful!"
