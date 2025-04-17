CXX = g++
CXXFLAGS = -std=c++17 -I./src
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = src/main.cpp src/Player.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
