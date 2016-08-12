include ../Makefile.inc

SRCS = bytebuffer.cpp circularbuffer.cpp databuffer.cpp localbytearraybuffer.cpp remotebytearraybuffer.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: buffer.a

buffer.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/library/
	$(ARRU) $(TARGET_PATH)/library/buffer.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/library/buffer.a