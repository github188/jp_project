include ../Makefile.inc

SRCS = datastructure.cpp datastructuremarshaller.cpp format.cpp formatfactory.cpp formatproperty.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: corelibprotocol.a

corelibprotocol.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/corelib/
	$(ARRU) $(TARGET_PATH)/corelib/corelibprotocol.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/corelib/corelibprotocol.a