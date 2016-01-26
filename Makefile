CC=clang++
CFLAGS=-c -std=c++11
LDFLAGS=
SOURCES=cpu.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=chip8

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
