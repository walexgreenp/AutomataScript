# Makefile

# Compiler + Flags
CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Output Executable Name
TARGET = $(BIN_DIR)/transpile

# Find all .cpp files in src/
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)


# Rule to compie .cpp files into .o files into /bin
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build final executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Clean
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)
