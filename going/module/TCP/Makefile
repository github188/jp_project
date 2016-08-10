Target	= net.a
CC = g++
ARRU = ar -ru
SRC_CPP=tcp.cpp socket.cpp
OBJ_CPP = $(SRC_CPP:.cpp=.o)
CPP_FLAGS =  -c -g -Wall
.SUFFIXES: .o .cpp
.cpp.o:
	$(CC)  $(CPP_FLAGS) $< -o $@
all: $(Target)
	cp $(Target) ../module_lib	
	
$(Target):$(OBJ_CPP)
	$(ARRU) $@ $^
clean:
	rm $(Target) *.o
