include ../Makefile.inc

SRCS = dbservice.cpp defaultIOHandler.cpp fileservice.cpp picturepainer.cpp servicedispatcher.cpp sqlcarrecord.cpp sqlcartrap.cpp zhuhaidatarequestlengthcodec.cpp \
	   sendingservice.cpp blacklist.cpp controllist.cpp whitelist.cpp mqservice.cpp sqldeviceheartbeat.cpp sqlcommit.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: appcommon.a

appcommon.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/app/
	$(ARRU) $(TARGET_PATH)/app/appcommon.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/app/appcommon.a
