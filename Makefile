# Build in release mode by default.
# Use "make DEBUG=1" to build in debug mode.
CC := clang++
ECHO := echo
RM := rm
CD := cd
TAR := tar

BUILD_DIR := build/GNU

OUTDIR := $(BUILD_DIR)
CFLAGS := -W -Wall -Isrc -std=c++11
DEBUG ?= 1
ifeq ($(DEBUG), 1)
OUTDIR := $(OUTDIR)/Debug
CFLAGS += -g -DDEBUG
else
OUTDIR := $(OUTDIR)/Release
CFLAGS += -s
endif
OBJDIR := $(OUTDIR)/obj

LIBS := -lGL -lGLU -lglfw -lGLEW -lSOIL -lexpat -lm -lpthread

SRC := $(wildcard src/*.cpp)
OBJFILES := $(SRC:.cpp=.o)
OBJS := $(addprefix $(OBJDIR)/, $(OBJFILES))
EXE := flatengine gears simple vbo terrain
DEPEND = .depend

all: $(EXE)

terrain: src/demo/terrain.cpp $(OBJS)
	@$(ECHO) " LD $@"
	@$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

simple: src/demo/simple.cpp $(OBJS)
	@$(ECHO) " LD $@"
	@$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

vbo: src/demo/vbo.cpp $(OBJS)
	@$(ECHO) " LD $@"
	@$(CC) -o $@ $^ $(LIBS) $(CFLAGS)
	
flatengine: src/demo/flatengine.cpp $(OBJS)
	@$(ECHO) " LD $@"
	@$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

gears: src/demo/gears.cpp $(OBJS)
	@$(ECHO) " LD $@"
	@$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

dep: $(DEPEND)

$(DEPEND):
	@$(ECHO) " MKDEP"
	@$(CC) -MM -MG $(CFLAGS) $(SRC) > $(DEPEND)

#$(EXE): $(OBJS)
#	@$(ECHO) " LD $@"
#	@$(CC) -o $(EXE) $^ $(LIBS)

$(OBJDIR)/%.o: %.cpp
	@$(ECHO) " CC $@"
	@mkdir -p ${dir $@ }
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS): | $(OBJDIR) $(OUTDIR)

$(OUTDIR):
	@mkdir -p $(OUTDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

install:

clean: FORCE
	@$(ECHO) " CLEAN object files"
	@$(RM) -rf $(OBJDIR)

realclean: clean
	@$(ECHO) " CLEAN $(EXE)"
	@$(RM) -f $(EXE)
	@$(ECHO) " CLEAN noise files"
	@$(RM) -f `find . -name "*~" -o -name "\#*"`

c: clean

rc: realclean

FORCE :
ifeq (.depend,$(wildcard .depend))
include .depend
endif


