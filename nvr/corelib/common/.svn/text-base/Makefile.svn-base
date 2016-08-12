include ../Makefile.inc

SRCS = activecorelib.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: corelibcommon.a

corelibcommon.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/corelib/
	$(ARRU) $(TARGET_PATH)/corelib/corelibcommon.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/corelib/corelibcommon.a