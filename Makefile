# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic
# CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

# Project files and target
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
TARGET = simulation.exe

# Default rule (alias for build)
all: build

# Build rule
build: $(TARGET)

# Link the executable
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) -lsimlib

# Run the program
run: build
	./$(TARGET) $(ARGS)

# Clean rule
clean:
	rm -f $(TARGET)
