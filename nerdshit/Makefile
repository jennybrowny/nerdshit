# Makefile for SFML game
# This tells the computer how to compile your game

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# SFML libraries needed
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SOURCES = main.cpp Game.cpp Player.cpp QuizManager.cpp Button.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

# Default target
$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(SFML_LIBS)

# Compile individual object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug version
$(TARGET)-debug: CXXFLAGS += -g -DDEBUG
$(TARGET)-debug: $(OBJECTS)
	$(CXX) -o $(TARGET)-debug $(OBJECTS) $(SFML_LIBS)

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET)-debug

# Run the game after compiling
run: $(TARGET)
	./$(TARGET)

# Install SFML on different systems (for reference)
install-sfml-ubuntu:
	sudo apt-get update
	sudo apt-get install libsfml-dev

install-sfml-mac:
	brew install sfml

# Help target
help:
	@echo "Available targets:"
	@echo "  $(TARGET)     - Build the game"
	@echo "  clean         - Remove compiled files"
	@echo "  run           - Build and run the game"
	@echo "  help          - Show this help message"

.PHONY: clean run help install-sfml-ubuntu install-sfml-mac