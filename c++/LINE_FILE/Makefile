Target	= try
CC = g++
#src=$(wildcard *.c)
SRC_CPP=try.cpp main.cpp
OBJ_CPP = $(SRC_CPP:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC)  -c -g $< -o $@
all: $(Target)
	
$(Target):$(OBJ_CPP)
	$(CC) -o $@ $^
clean:
	rm $(Target) *.o
