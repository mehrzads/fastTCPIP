CC=g++
CFLAGS=-c -Wall -std=c++11 -O3
LDFLAGS=  -lpthread  -lm  
SOURCES= $(wildcard src/*.cpp) $(wildcard ../Common/src/*.cpp) 
OBJECTS=$(SOURCES:.cpp=.o) 
INCLUDES= -I. -Iinclude/ -I../Common/include 

EXECUTABLE=client

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)  
	$(CC)  $(OBJECTS)  $(LDFLAGS) -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $< -o $@

clean: 
	rm -f $(EXECUTABLE) src/*.o


