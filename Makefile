CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include -I./src
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC = src/main.cpp \
	  src/player.cpp \
	  src/laser.cpp \
	  src/hitbox.cpp \
	  src/textManager.cpp \
	  src/enemy.cpp \
	  src/map.cpp \
	  src/score.cpp \
	  
OBJ = $(SRC:.cpp=.o)
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
