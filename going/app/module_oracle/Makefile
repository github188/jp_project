Target	= test
CC = g++
ARRU = ar -ru
SRC_CPP=oracle_test.cpp 
LIB_FLAG= -locilib -lpthread
MODULE_LIB+=../../module/module_lib/*
#MODULE_LIB+=../../module/module_lib/log.a
#MODULE_LIB+=../../module/module_lib/db.a
#MODULE_LIB+=../../module/module_lib/xml.a
OBJ_CPP = $(SRC_CPP:.cpp=.o)
CPP_FLAGS =  -c -g -Wall
.SUFFIXES: .o .cpp
.cpp.o:
	$(CC)  $(CPP_FLAGS) $< -o $@
all: $(Target)

$(Target):$(OBJ_CPP)
	$(CC) -g $(LIB_FLAG) -o $@ $^ $(MODULE_LIB)
clean:
	rm $(Target) *.o
