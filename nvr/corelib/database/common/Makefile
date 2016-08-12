include ../../Makefile.inc

SRCS = database.cpp field.cpp queryresult.cpp sqldelaythread.cpp sqloperation.cpp sqlstring.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: databasecommon.a

databasecommon.a: $(OBJS)
	mkdir -p ../$(TARGET_PATH)/corelib/
	$(ARRU) ../$(TARGET_PATH)/corelib/databasecommon.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  ../$(TARGET_PATH)/corelib/databasecommon.a