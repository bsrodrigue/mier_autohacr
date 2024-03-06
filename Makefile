# Define compiler
CC = gcc
CFLAGS = -Wall -Wextra -lraylib -lm -lstdc++

# Define directories
SRC_DIR = src
BUILD_DIR = build

# Find all source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cc)
HEADER_FILES := $(wildcard $(SRC_DIR)/*.h)

# Generate object file names from source file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

TARGET = autohacka

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc $(HEADER_FILES)
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
