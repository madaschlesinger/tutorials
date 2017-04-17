
CC=gcc
CFLAGS = -I.

CCC=g++
CCFLAGS = -std=c++11 -I.

BUILD_DIR=./build
LIB_DIR=./lib
BIN_DIR=./bin


BIN1_SOURCES=lambda.cc
BIN1=lambda

BIN2_SOURCES=variadic.cc
BIN2=variadic

# for loop....
BINS=$(BIN1) $(BIN2) $(BIN3) $(BIN4)

#
# build from SRCS 
#
SOURCES	:=$(wildcard *.cc)
OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(SOURCES))
BINARYS := $(patsubst %,$(BUILD_DIR)/%, $(BINS))

BIN1_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN1_SOURCES))
BIN2_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN2_SOURCES))



.PHONY: clean install debug main


main: $(OBJECTS) $(BINARYS)


$(BUILD_DIR)/%.o: %.c
	-f $(BUID_DIR ) || mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.o: %.cc
	test -d $(BUID_DIR ) || mkdir -p $(BUILD_DIR)
	$(CCC) -c -o $@ $< $(CCFLAGS)


#
# this needs be generated per BIN
#
$(BUILD_DIR)/$(BIN1): $(BIN1_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++


$(BUILD_DIR)/$(BIN2): $(BIN2_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++


clean:
	rm -f ./build/*
	rm -f ./bin/*
	rm -f ./lib/*.so
	rm -f core

install:
	test -d  $(BIN_DIR) || mkdir -p $(BIN_DIR)
	cp -p $(BUILD_DIR)/lambda $(BIN_DIR)
	cp -p $(BUILD_DIR)/variadic $(BIN_DIR)

debug:
	@echo $(OBJECTS)
	@echo $(SOURCES)
	@echo $(BINARYS)

