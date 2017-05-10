


ifdef LIB1
$(BUILD_DIR)/lib$(LIB1).so: $(LIB1_OBJECTS)
	$(CC) -o$@ $^   -shared  $(CCFLAGS) -lstdc++
endif


ifdef LIB2
$(BUILD_DIR)/lib$(LIB2).so: $(LIB2_OBJECTS)
	$(CC) -o$@ $^  -shared $(CCFLAGS) -lstdc++
endif

#
# argh/hackme..... should write macro for -l inclusion - if defined
#
ifndef BIN1_LIBS
BIN1_LIBS=rt
endif

ifndef BIN2_LIBS
BIN2_LIBS=rt
endif

ifndef BIN3_LIBS
BIN3_LIBS=rt
endif


ifndef BIN4_LIBS
BIN4_LIBS=rt
endif


ifndef BIN5_LIBS
BIN5_LIBS=rt
endif


ifndef BIN6_LIBS
BIN6_LIBS=rt
endif


ifndef BIN7_LIBS
BIN7_LIBS=rt
endif


ifndef BIN8_LIBS
BIN8_LIBS=rt
endif


ifndef BIN9_LIBS
BIN9_LIBS=rt
endif

#
# this needs be generated per BIN
# needs work for lib dependency
# also - BIN1_LD_LIB_FLAGS -l in the variable
#

ifdef BIN1
$(BUILD_DIR)/$(BIN1): $(BIN1_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++ -v -L./lib -L./build -Wl,-rpath=./build:./lib  $(BIN1_LIBRARYS)
endif

ifdef BIN2
$(BUILD_DIR)/$(BIN2): $(BIN2_OBJECTS) 
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++ -v -L./lib -L./build -Wl,-rpath=./build:./lib  $(BIN2_LIBRARYS)
endif

ifdef BIN3
$(BUILD_DIR)/$(BIN3): $(BIN3_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++ -v -L./lib -L./build -Wl,-rpath=./build:./lib  $(BIN3_LIBRARYS)
endif

ifdef BIN4
$(BUILD_DIR)/$(BIN4): $(BIN4_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++ -v -L./lib -L./build -Wl,-rpath=./build:./lib  $(BIN4_LIBRARYS)
endif



ifdef BIN5
$(BUILD_DIR)/$(BIN5): $(BIN5_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++ -v -L./lib -L./build -Wl,-rpath=./build:./lib $(BIN5_LIBRARYS) 
endif

ifdef BIN6
$(BUILD_DIR)/$(BIN6): $(BIN6_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -v -lstdc++ $(LDFLAGS)  $(BIN6_LIBRARYS)
endif


ifdef BIN7
$(BUILD_DIR)/$(BIN7): $(BIN7_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -v -lstdc++  $(LDFLAGS) $(BIN7_LIBRARYS)
endif


ifdef BIN8
$(BUILD_DIR)/$(BIN8): $(BIN8_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++  $(LDFLAGS) -v -L./lib -L./build -Wl,-rpath=./build:./lib  $(BIN8_LIBRARYS)
endif


ifdef BIN9
$(BUILD_DIR)/$(BIN9): $(BIN9_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++ $(LDFLAGS) -v -L./lib -L./build -Wl,-rpath=./build:./lib  -l$(BIN9_LIBS)
endif


$(INSTALL_LIBRARYS) : $(LIBRARYS)
	cp -p $^ $(LIB_DIR)

$(INSTALL_BINARYS) : $(BINARYS)
	cp -p $^ $(BIN_DIR)

build_libs : $(LIBRARYS) 
build_bins : build_libs $(LIBRARYS) $(BINARYS)
main       : build_libs build_bins
install_bins : build_bins



clean:
	rm -f ./build/*
	rm -f ./bin/*
	rm -f ./lib/*.so
	rm -f core
	rm -rf .d


#
# TODO - this needs work
#


install_bins : $(INSTALL_BINARYS)
install_libs : $(INSTALL_LIBRARYS)
install_hdrs : $(INSTALL_HEADERS)

#$(INSTALL_BINS) : $(BINARYS)
#	@test -d  $(BIN_DIR) || mkdir -p $(BIN_DIR)
#	cp -p $(BINARYS) $(BIN_DIR)


#install_libs : $(LIBRARYS)
#	@test -d  $(LIB_DIR) || mkdir -p $(LIB_DIR)
#	cp -p $(LIBRARYS) $(LIB_DIR)

#install_hdrs : $(HEADERS)
#	@test -d  $(INC_DIR) || mkdir -p $(INC_DIR)
#	cp -p $(f) $(INC_DIR)

debug:
	@echo "objects: "  $(OBJECTS)
	@echo "source:  "  $(SOURCES)
	@echo "bins:    "  $(BINARYS)
	@echo "libs:    "  $(LIBRARYS)
	@echo "headers: "  $(HEADERS)
	@echo "LIB1_OBJS: "  $(LIB1_OBJECTS)
	@echo "LIB1_OBJS: "  $(LIB1_SOURCES)
	@echo "INSTALL_LIBS:" $(INSTALL_LIBRARYS)
	@echo "INSTALL_HDRS:" $(INSTALL_HEADERS)
	@echo "BIN1_OBJS:" $(BIN1_OBJECTS) 
	@echo "project_loc:" $(PROJECT_LOC) 
	@echo "workspace_loc:" $(WORKSPACE_LOC) 
	@echo "CXX: $(CXX) CC:$(CC) CCC: $(CCC)" 
	@echo "CXXFLAGS: $(CXXFLAGS) "
	@echo "CCFLAGS : $(CCFLAGS) "
	@echo "CPPFLAGS:$(CPPFLAGS) "
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)" 
	@echo "BIN5_LIBRARYS: $(BIN5_LIBRARYS)" 



debuglist:
	@echo "nlist: "  $(NLIST)
	@echo "zin1: "   $(ZIN1)
	@echo "zin2: "   $(ZIN2)
	@echo "zin2_libs: "   $(BIN2_LIBS)
	@echo "zin9_libs: "   $(BIN9_LIBS)

