
CC=gcc
CFLAGS = -I.

CCC=g++
CCFLAGS = -std=c++11 -I.

BUILD_DIR=./build
LIB_DIR=./lib
BIN_DIR=./bin

_BIN=hello
BIN=$(patsubst %,$(BUILD_DIR)/%,$(_OBJS))
#
# build from SRCS
#
SOURCES	:=$(wildcard *.cc)
OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(SOURCES))

.PHONY: clean install debug


# _OBJS=hello.o
# OBJS=$(patsubst %,$(BUILD_DIR)/%,$(_OBJS))


$(BUILD_DIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.o: %.cc
	$(CCC) -c -o $@ $< $(CCFLAGS)

$(BUILD_DIR)/hello: $(OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++


clean:
	rm -f ./build/*
	rm -f ./bin/*
	rm -f ./lib/*.so
	rm -f core

install:
	cp -p ./build/hello ./bin

debug:
	@echo $(OBJECTS)
	@echo $(SOURCES)

