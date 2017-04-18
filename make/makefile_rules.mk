

# for loop....
ifdef BIN1
BINS += $(BIN1)
endif

ifdef BIN2
BINS += $(BIN2)
endif


ifdef BIN3
BINS += $(BIN3)
endif


ifdef BIN4
BINS += $(BIN4)
endif

#
# LIBS
#
ifdef LIB1
LIBS += $(LIB1)
endif

ifdef LIB2
LIBS += $(LIB2)
endif


ifdef LIB2
LIBS += $(LIB2)
endif


ifdef LIB3
LIBS += $(LIB3)
endif


ifdef LIB4
LIBS += $(LIB4)
endif



#
# build from SRCS 
#
SOURCES  	:=$(wildcard *.cc)
OBJECTS  	:= $(patsubst %.cc,$(BUILD_DIR)/%.o, $(SOURCES))
BINARYS  	:= $(patsubst %,$(BUILD_DIR)/%, $(BINS))
LIBRARYS 	:= $(patsubst %,$(BUILD_DIR)/lib%.so, $(LIBS))

INSTALL_BINARYS := $(patsubst %,$(BIN_DIR)/%, $(BINS))
INSTALL_LIBRARYS:= $(patsubst %,$(LIB_DIR)/lib%.so, $(LIBS))


BIN1_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN1_SOURCES))
BIN2_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN2_SOURCES))
BIN3_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN3_SOURCES))
BIN4_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN4_SOURCES))

LIB1_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB1_SOURCES))
LIB2_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB2_SOURCES))
LIB3_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB3_SOURCES))
LIB4_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB4_SOURCES))

BIN1_LIBRARYS := $(patsubst %.cc,$(BUILD_DIR)/lib%.so, $(BIN1_LIBS))



$(BUILD_DIR)/%.o: %.c
	@test -f $(BUID_DIR ) || mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.o: %.cc
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	$(CCC) -c -o $@ $< $(CCFLAGS)


$(BUILD_DIR)/%.o: %.cpp
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	$(CCC) -c -o $@ $< $(CCFLAGS)



#$(INSTALL_LIBRARYS) : $(LIBRARYS)
#	cp -p $^ $(LIB_DIR)

#$(INSTALL_BINARYS) : $(BINARYS)
#	cp -p $^ $(BIN_DIR)


#
# TODO - install HDR rule
#


$(WORKSPACE_DIR)/include/%.mk: %.mk
	-f $(WORKSPACE_DIR)/include|| mkdir -p $(WORKSPACE_DIR)/include
	cp -p $^ $(WORKSPACE_DIR)/include


main: build_libs build_bins
main: $(OBJECTS) $(BINARYS) $(LIBRARYS) $(HEADERS)


all : main
install : install_libs install_bins 

$(INSTALL_LIBRARYS) : $(LIBRARYS)





