Target	= db.a
CC = g++
ARRU = ar -ru
SRC_CPP=myoracle.cpp
OBJ_CPP = $(SRC_CPP:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC)  -c -g $< -o $@
all: $(Target)
	cp $(Target) ../module_lib	
$(Target):$(OBJ_CPP)
	$(ARRU) $@ $^
clean:
	rm $(Target) *.o
