# https://github.com/manloloy/ECE205Course/blob/main/CourseNotes/05d-Makefiles.md
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# add cpp files you want to compile
# to the SRC variable, this line will change anytime you add a new cpp file
# to the project
SRC = src/game.cpp src/button.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp

	$(CXX) $(CXXFLAGS) -c $< -o $@	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

# Rule to build and run firsttest from firsttest.cpp
firsttest: firsttest.cpp
	$(CXX) firsttest.cpp -o firsttest $(LDFLAGS)
	./firsttest