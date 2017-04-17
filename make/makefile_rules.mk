

# for loop....
#BINS=$(BIN1) $(BIN2)

ifdef BIN1
BINS += $(BIN1)
endif

ifdef BIN2
BINS += $(BIN2)
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
# build from SRCS 
#
SOURCES  :=$(wildcard *.cc)
OBJECTS  := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(SOURCES))
BINARYS  := $(patsubst %,$(BUILD_DIR)/%, $(BINS))
LIBRARYS := $(patsubst %,$(BUILD_DIR)/%, $(LIBS))


BIN1_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN1_SOURCES))
BIN2_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN2_SOURCES))
BIN3_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN3_SOURCES))
BIN4_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN4_SOURCES))


$(BUILD_DIR)/%.o: %.c
	@test -f $(BUID_DIR ) || mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.o: %.cc
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	$(CCC) -c -o $@ $< $(CCFLAGS)


$(WORKSPACE_DIR)/include/%.mk: %.mk
	-f $(WORKSPACE_DIR)/include|| mkdir -p $(WORKSPACE_DIR)/include
	cp -p $^ $(WORKSPACE_DIR)/include


.PHONY: clean install debug main all install_hdrs install_bins install_libs

all : main

main: $(OBJECTS) $(BINARYS) $(LIBRARYS) $(HEADERS)





