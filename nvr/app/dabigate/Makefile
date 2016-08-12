include ../Makefile.inc

LIBS_DIR = $(TARGET_PATH)
LIBS = -locilib -lclntsh -lnnz11 -lpthread -lactivemq-cpp $(LIBS_DIR)/app/xml.a $(LIBS_DIR)/app/appcommon.a $(LIBS_DIR)/app/appprotocol.a $(LIBS_DIR)/corelib/network.a \
		$(LIBS_DIR)/corelib/databaseoracle.a $(LIBS_DIR)/corelib/databasecommon.a $(LIBS_DIR)/corelib/corelibcommon.a $(LIBS_DIR)/corelib/corelibprotocol.a \
		$(LIBS_DIR)/library/buffer.a  $(LIBS_DIR)/library/util.a $(LIBS_DIR)/library/concurrent.a $(LIBS_DIR)/library/internal.a $(LIBS_DIR)/library/exception.a $(LIBS_DIR)/library/net.a \
		$(LIBS_DIR)/library/iostream.a

TARGET_BIN = dabigate

SRC_C = 
SRC_CPP = dabigate.cpp mtunnellistener.cpp
OBJC = $(SRC_C:.c=.o)
OBJCPP = $(SRC_CPP:.cpp=.o)

.SUFFIXES: .o .c
.c.o:
	$(CC) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG) $(INCLUDE) -c -o $@ $<

all: $(TARGET_BIN)

$(TARGET_BIN):$(OBJC) $(OBJCPP)
	$(CXX) -o $@ $(LINK_CXXFLAG) $^  -L$(LIBS_DIR) $(LIBS)

#depend:
#	mkdep $(CXXFLAG) $(SRC_C) $(SRC_CPP)

clean:
	rm -f *.o $(TARGET_BIN)
