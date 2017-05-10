

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


ifdef BIN5
BINS += $(BIN5)
endif

ifdef BIN6
BINS += $(BIN6)
endif

ifdef BIN7
BINS += $(BIN7)
endif

ifdef BIN8
BINS += $(BIN8)
endif

ifdef BIN9
BINS += $(BIN9)
endif

#define lib_defined
#
#	ifdef $(eval LIB$1)
#		LIBS += $(LIB1)
#	endif
#endef

ITERATE = 1 2 3 4 5 6 7 8 9 
NLIST = $(shell for x in {1..$(words $(ITERATE))}; do echo $$x; done)
LIST = $(foreach x,5 6, LIBS += LIB$(x) := $(eval $(word $(x),$(IT))) )


#
# LIBS
#
ifdef LIB1
LIBS += $(LIB1)
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

ifdef LIB5
LIBS += $(LIB5)
endif



# TODO - should this be in rules..... - could not get this working.....
####### begin collect_libs ####### 
# define collect_libs =

# ifdef LIB$(1)
# expand_lib_var     = LIB$(1)
# LIBS += $(expand_lib_var);
# endif

# endef
# ####### end define_bin_build_targets #######

# $(foreach _,${n_targets},$(eval $(call collect_libs,$_)))

#
# build from SRCS 
#
SOURCES  	:=$(wildcard *.cc)
OBJECTS  	:= $(patsubst %.cc,$(BUILD_DIR)/%.o, $(SOURCES))
BINARYS  	:= $(patsubst %,$(BUILD_DIR)/%, $(BINS))
LIBRARYS 	:= $(patsubst %,$(BUILD_DIR)/lib%.so, $(LIBS))

INSTALL_BINARYS := $(patsubst %,$(BIN_DIR)/%, $(BINS))
INSTALL_LIBRARYS:= $(patsubst %,$(LIB_DIR)/lib%.so, $(LIBS))
INSTALL_HEADERS := $(patsubst %,$(INC_DIR)/%, $(HEADERS))


BIN1_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN1_SOURCES))
BIN2_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN2_SOURCES))
BIN3_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN3_SOURCES))
BIN4_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN4_SOURCES))
BIN5_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN5_SOURCES))
BIN6_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN6_SOURCES))
BIN7_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN7_SOURCES))
BIN8_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN8_SOURCES))
BIN9_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(BIN9_SOURCES))

LIB1_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB1_SOURCES))
LIB2_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB2_SOURCES))
LIB3_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB3_SOURCES))
LIB4_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB4_SOURCES))
LIB5_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB5_SOURCES))
LIB6_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB6_SOURCES))
LIB7_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB7_SOURCES))
LIB8_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB8_SOURCES))
LIB9_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o, $(LIB9_SOURCES))

BIN1_LIBRARYS := $(patsubst %,-l%, $(BIN1_LIBS))
BIN2_LIBRARYS := $(patsubst %,-l%, $(BIN2_LIBS))
BIN3_LIBRARYS := $(patsubst %,-l%, $(BIN3_LIBS))
BIN4_LIBRARYS := $(patsubst %,-l%, $(BIN4_LIBS))
BIN5_LIBRARYS := $(patsubst %,-l%, $(BIN5_LIBS))
BIN6_LIBRARYS := $(patsubst %,-l%, $(BIN6_LIBS))
BIN7_LIBRARYS := $(patsubst %,-l%, $(BIN7_LIBS))
BIN8_LIBRARYS := $(patsubst %,-l%, $(BIN8_LIBS))
BIN9_LIBRARYS := $(patsubst %,-l%, $(BIN9_LIBS))


####### begin define_variables ####### 
define define_variables =

ifndef BIN$(1)_LIBS
  BIN$(1)_LIBS:=
endif

ifndef BIN$(1)_LIBS
  BIN$(1)_LIBS:=
endif


endef
####### end define_variables #######

####### loop for variable definition via  define_variables ####### 
n_targets := 1 2 3 4 5 6 7 8 9 

$(foreach _,${n_targets},$(eval $(call define_variables,$_)))



MY_VARS = foo bar baz
NLIST = $(shell for x in {1..$(words $(MY_VARS))}; do echo $$x; done)
LIST = $(foreach x,5 6, BIN$(x) := $(eval $(word $(x),$(MY_VARS))) )
find_files = $(wildcard $(dir)/*)
dirs := bin lib build
files := $(foreach dir,$(dirs),$(find_files))


DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c   = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(CCFLAGS) $(TARGET_ARCH) -c
COMPILE.cc  = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(CCFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@


%o : %c

$(BUILD_DIR)/%.o: %.c $(DEPDIR)/%.d
	@test -f $(BUID_DIR ) || mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# #$(CCC) -c -o $@ $< $(CCFLAGS)

%o : %.cc
$(BUILD_DIR)/%.o: %.cc $(DEPDIR)/%.d
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)


$(BUILD_DIR)/%.o: %.cpp $(DEPDIR)/%.d
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	$(CCC) -c -o $@ $< $(CCFLAGS)


$(INC_DIR)/%.h: %.h
	@test -d $(@D) || mkdir -p $(@D)
	cp -p $^ $(@D)



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


$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES))))






