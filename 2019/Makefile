CC=g++-7
COMMON_CFLAGS=-c --std=c++17 -Wall -I.. -I../..
CFLAGS:=$(COMMON_CFLAGS) -g -O0
CFLAGS_RELEASE:=$(COMMON_CFLAGS) -O2
LDFLAGS=
SOURCES=code.cpp
#SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

release: CFLAGS=$(CFLAGS_RELEASE)
release: $(EXECUTABLE)

.PHONY: clean
clean:
	find . -name a.out -delete
	find . -name "*.o" -delete

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
