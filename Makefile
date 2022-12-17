CC=g++-7
CFLAGS=-c -std=c++17 -g -O0 -I.. -I../.. -Wall
CFLAGS_RELEASE=-c -std=c++17 -O2 -I.. -I../.. -Wall
LDFLAGS=-pthread
SOURCES=$(wildcard *.cpp) ../../all.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

release: CFLAGS=$(CFLAGS_RELEASE)
release: $(EXECUTABLE)

.PHONY: clean
clean:
	find . -name a.out -delete
	find . -name "*.o" -delete
	rm -f ../../*.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
