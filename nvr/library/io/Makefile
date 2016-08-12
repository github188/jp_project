include ../Makefile.inc

SRCS = consolestreamhandler.cpp file.cpp inputstream.cpp outputstream.cpp standardinputstream.cpp standardoutputstream.cpp tcpinputstream.cpp tcpoutputstream.cpp \
	   filestreamhandler.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: iostream.a

iostream.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/library/
	$(ARRU) $(TARGET_PATH)/library/iostream.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/library/iostream.a