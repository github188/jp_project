include ../Makefile.inc

SRCS = connection.cpp connectionhandler.cpp connectionhandlerepoll.cpp connectionmanager.cpp ioacceptor.cpp ioconnector.cpp ioevent.cpp iohandler.cpp listener.cpp tcpserver.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: network.a

network.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/corelib/
	$(ARRU) $(TARGET_PATH)/corelib/network.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/corelib/network.a