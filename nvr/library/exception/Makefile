include ../Makefile.inc

SRCS = eofexception.cpp exception.cpp indexoutofboundexception.cpp ioexception.cpp nullpointerexception.cpp sysexception.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: exception.a

exception.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/library/
	$(ARRU) $(TARGET_PATH)/library/exception.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/library/exception.a