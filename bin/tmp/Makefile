# Makefile
# Compiler + Flags
CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = ../compiled_output

all = $(TARGET)

$(TARGET): $(OBJ)
	@$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ) $(TARGET)
