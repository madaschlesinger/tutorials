

WORKSPACE_DIR=/home/adam/workspace

BUILD_DIR=./build
LIB_DIR=$(WORKSPACE_DIR)/lib
BIN_DIR=$(WORKSPACE_DIR)/bin
INC_DIR=$(WORKSPACE_DIR)/include


GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)



CC=gcc
#CFLAGS = -I. 

#CC=/usr/bin/g++
CXX=/usr/bin/g++
CCC=/usr/bin/g++

CFLAGS = -I. -I/usr/local/include -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS = -v -L/usr/local/lib -L./lib -L./build -Wl,-rpath=./build:./lib:$(WORKSPACE_DIR)/lib:/usr/local/lib
CFLAGS = -I. -g3 -DGIT_VERSION=\"$(GIT_VERSION)\"

CCFLAGS = -std=c++11 -g3 -I. -I/usr/local/include -fPIC -DGIT_VERSION=\"$(GIT_VERSION)\"


# LDFLAGS = -shared - this was not kosher.... for .so only iirc 

# -O2 -Wall -c -fmessage-length=0 -MMD -MP

N := $(shell seq 1 10)
SINGLE=3



