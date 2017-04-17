#
# this needs be generated per BIN
#
ifdef BIN1
$(BUILD_DIR)/$(BIN1): $(BIN1_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++
endif

ifdef BIN2
$(BUILD_DIR)/$(BIN2): $(BIN2_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++
endif

ifdef BIN3
$(BUILD_DIR)/$(BIN3): $(BIN3_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++
endif

ifdef BIN4
$(BUILD_DIR)/$(BIN4): $(BIN4_OBJECTS)
	$(CC) -o$@ $^  $(CCFLAGS) -lstdc++
endif

clean:
	rm -f ./build/*
	rm -f ./bin/*
	rm -f ./lib/*.so
	rm -f core

#
# TODO - this needs work
#


# install: main install_bins install_libs install_hdrs
# install: main install_bins install_libs install_hdrs


install_bins : main
	@test -d  $(BIN_DIR) || mkdir -p $(BIN_DIR)
	cp -p $(BINARYS) $(BIN_DIR)


install_libs : main
	@test -d  $(LIB_DIR) || mkdir -p $(LIB_DIR)
	cp -p $(LIBRARYS) $(LIB_DIR)

install_hdrs : main
	@test -d  $(INC_DIR) || mkdir -p $(INC_DIR)
	cp -p $(HEADERS) $(INC_DIR)



debug:
	@echo "objects: "  $(OBJECTS)
	@echo "source:  "  $(SOURCES)
	@echo "bins:    "  $(BINARYS)
	@echo "libs:    "  $(LIBRARYS)
	@echo "headers: "  $(HEADERS)

