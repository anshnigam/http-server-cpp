# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include

# Directories
SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

# Find all .cpp source files recursively
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")

# Replace 'src/' with 'build/' and '.cpp' with '.o' for object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Output executable
TARGET = http_server_cpp

# Default target
all: $(TARGET)

# Link object files into final binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files to object files, mirroring directory structure
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean

