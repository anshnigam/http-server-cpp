# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include

# Directories
SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Output executable
TARGET = http_server_cpp

# Default target
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile each .cpp file into a .o file in the build directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
