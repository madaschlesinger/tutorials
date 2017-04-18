



ifdef LIB1
$(BUILD_DIR)/lib$(LIB1).so: $(LIB1_OBJECTS)
	$(CC) -o$@ $^   $(LDFLAGS) $(CCFLAGS) -lstdc++
endif


#
# this needs be generated per BIN
# needs work for lib dependency
# also - BIN1_LD_LIB_FLAGS -l in the variable
#

ifdef BIN1
$(BUILD_DIR)/$(BIN1): $(BIN1_OBJECTS)
	$(CC) -o$@  $^ $(CCFLAGS)  -lstdc++ -v -L./lib -L./build -L/tmp -l$(BIN1_LIBS)
endif

ifdef BIN2
$(BUILD_DIR)/$(BIN2): $(BIN2_OBJECTS) 
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++ -v -L./lib -L./build
endif

ifdef BIN3
$(BUILD_DIR)/$(BIN3): $(BIN3_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++
endif

ifdef BIN4
$(BUILD_DIR)/$(BIN4): $(BIN4_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++
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

#
# TODO - this needs work
#


install_bins : $(INSTALL_BINARYS)
install_libs : $(INSTALL_LIBRARYS)

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
	@echo "BIN1_OBJS:" $(BIN1_OBJECTS) 



