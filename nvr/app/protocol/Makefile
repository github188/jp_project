include ../Makefile.inc

SRCS = cardescriptioninfo.cpp cardescriptioninfomarshaller.cpp consumerheartbeat.cpp consumerheartbeatmarshaller.cpp consumerregisterinfo.cpp \
	   consumerregisterinfomarshaller.cpp zhuhaiformat.cpp carbypassdescinfo.cpp carbypassdescinfomarshaller.cpp deviceheartbeat.cpp deviceheartbeatmarshaller.cpp \
	   devicetimesyncinfo.cpp devicetimesyncinfomarshaller.cpp

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: appprotocol.a

appprotocol.a: $(OBJS)
	mkdir -p $(TARGET_PATH)/app/
	$(ARRU) $(TARGET_PATH)/app/appprotocol.a $(OBJS)

depend:
	mkdep $(CXXFLAG) $(SRCS)

clean: 
	rm -f *.o
	rm -f  $(TARGET_PATH)/app/appprotocol.a
