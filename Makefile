CC=clang++
CFLAGS=-c -Wall -std=c++11 -g
LDFLAGS=-lSDL2
SOURCES=cpu.cpp main.cpp display.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=chip8.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXECUTABLE)
