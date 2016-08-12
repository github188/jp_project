
CC = gcc
CXX = g++

ARRU = ar -ru
CXXFLAG = -Wall -static -D${PLATFORM}

#init the varables
ARCH_BIT = 32
BUILD_BIT = 32
RELEASE = 0
INIT_LIB_PATH = ../../lib
BIT32:=i686
BIT64:=x86_64

#get system bits
ARCH:=$(shell uname -m)
ifeq ($(ARCH),$(BIT64))
    ARCH_BIT = 64
    BUILD_BIT = 64
else
endif

#make the release version
ifeq ($(RELEASE),1)
    CXXFLAG += -O3 -DNDEBUG
    ver = release
else
    CXXFLAG += -g
    ver = debug
endif

#rewrite cxx flags
ifeq ($(BITS),32)
    CXXFLAG += -m32
    BUILD_BIT = 32
else
    ifeq ($(BITS),64)
        CXXFLAG += -m64
    	BUILD_BIT = 64
    else
    endif
endif

CXXFLAG_EX = $(CXXFLAG) -DSTREAM_LESS

TARGET_PATH = $(INIT_LIB_PATH)$(ver)$(BUILD_BIT)

INCLUDE = -I/usr/local/include/activemq-cpp-3.8.3
