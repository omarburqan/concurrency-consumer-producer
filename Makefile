TARGET=main

# all .c files in this directory are my sources
SOURCES=$(wildcard *.cpp)

OBJS=$(SOURCES:.cpp=.o)

INC_DIR=../include
CC=g++
LDFLAGS= -g 

CFLAGS= -c -Wall -ansi $(LDFLAGS) -pedantic -Werror -Wconversion  -I$(INC_DIR)
CXXFLAGS=$(CFLAGS)


.PHONY: clean run gdb

$(TARGET): $(OBJS)
	$(CC)  $(OBJS) -o $(TARGET) -lpthread 

include .depends

.depends:
	$(CC) -MM -I$(INC_DIR) $(SOURCES) > .depends

clean:
	rm -f $(OBJS) $(TARGET) .depends

run: $(TARGET)
	./$(TARGET)

gdb: $(TARGET)
	gdb -q ./$(TARGET)

leak:
	valgrind --leak-check=full ./$(TARGET) 
