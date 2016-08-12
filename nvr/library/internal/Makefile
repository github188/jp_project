include ../Makefile.inc

SRCS = mutex.cpp platform_lin.cpp threading.cpp ThreadList.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: internal.a

internal.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/library/
	$(ARRU) $(TARGET_PATH)/library/internal.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/library/internal.a