# Compiler and flags
CC := gcc
CFLAGS := -ggdb -g3 -O0 -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion -Werror -Iinc

# Directories
SRC_DIR := src
INC_DIR := inc
BUILD_DIR := build
TESTS_DIR := tests

# Target executable name
TARGET := $(BUILD_DIR)/main

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files into final binary
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c -> .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

# Run program
run: all
	./$(TARGET)

# Phony targets (not actual files)
.PHONY: all clean run
