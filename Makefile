CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
TARGET = simulation.exe

all: build

build: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) -lsimlib

run: build
	./$(TARGET) $(ARGS)

clean:
	rm -f $(TARGET)
