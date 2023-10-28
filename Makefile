# Compiler and flags
CXX = g++ -std=c++11
CXXFLAGS = -Wall -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lglut

# Object files
OBJS = Cell.o GameManager.o main.o

# Target executable
TARGET = GameOfLife

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

Cell.o: Cell.cpp Cell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

GameManager.o: GameManager.cpp GameManager.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGET).exe
