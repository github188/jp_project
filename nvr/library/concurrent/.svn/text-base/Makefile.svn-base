include ../Makefile.inc

SRCS = lock.cpp poolthread.cpp queuedthreadpool.cpp thread.cpp threadpool.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: concurrent.a

concurrent.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/library/
	$(ARRU) $(TARGET_PATH)/library/concurrent.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/library/concurrent.a