include ../../Makefile.inc

SRCS = databaseoracle.cpp myoracle.cpp queryresultoracle.cpp 

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: databaseoracle.a

databaseoracle.a: $(OBJS)
	mkdir -p ../$(TARGET_PATH)/corelib/
	$(ARRU) ../$(TARGET_PATH)/corelib/databaseoracle.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  ../$(TARGET_PATH)/corelib/databaseoracle.a