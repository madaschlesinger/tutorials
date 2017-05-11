
include ../make/makefile_defines.mk


HEADER_PREFIX=fibonacci
HEADERS = fibonacci/fibonacci.h


LIB1_SOURCES=fibonacci.cc
LIB1=fibonacci


LIB2_SOURCES=probe.cc
LIB2=probe
LIB2_LIBS=config++



BIN1_SOURCES=lambda.cc
BIN1=lambda
BIN1_LIBS=fibonacci

BIN2_SOURCES=variadic.cc
BIN2=variadic

# BIN3_LIBS := "3"
# BIN9_LIBS := "9"

BIN3_SOURCES=io.cc
BIN3=io
BIN3_LIBS=rt

BIN4_SOURCES=shm.cc
BIN4=shm
BIN4_LIBS=rt

BIN5_SOURCES=probe_reader.cc
BIN5=probe_reader
BIN5_LIBS=probe config++


#
# TODO - the lib has the dep - not the bin
#
BIN6_SOURCES=probe_test.cc
BIN6=probe_test
BIN6_LIBS=probe config++

BIN7_SOURCES=conf.cc
BIN7=conf
BIN7_LIBS=config++


BIN8_SOURCES=probe_config.cc
BIN8=probe_config
BIN8_LIBS=config++






include ../make/makefile_rules.mk
include ../make/makefile_targets.mk


