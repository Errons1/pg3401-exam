EXETARGET ?= main
STATARGET ?= main
STATIC =
CC = gcc
# Compiler Flags for Debugging
CFG = -Og -ggdb -Wall
# Compiler Flags for release
CFR = -O3 -Wall

BINDIR = ./bin
LIBDIR = ./lib
OBJDIR = ./obj
SRCDIR = ./src

# Makes a list of all;
# .c from src.
# .c from lib.
# .a from lib.
CSRC = $(shell find $(SRCDIR) -type f -name "*.c")
CLIB = $(shell find $(LIBDIR) -type f -name "*.c")
ALIB = $(shell find $(LIBDIR) -type f -name "*.a")
#$(info $(ALIB))

# Convert all *.c names to *.o
OSRC = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CSRC))
OLIB = $(patsubst $(LIBDIR)/%.c, $(OBJDIR)/%.o, $(CLIB))
OFILES = $(OSRC) $(OLIB)
#$(info $(OSRC))

# Compiles all src/*.c to .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
#	This line is very magic at the moment
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFG)

# Compiles all lib/*.c to .o
$(OBJDIR)/%.o: $(LIBDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFG)

ifdef STATIC
# Links all the .o to static library
$(BINDIR)/$(STATARGET): $(OFILES)
	ar rcs ./bin/lib$(EXETARGET).a $^
	cp $(SRCDIR)/$(STATARGET).h ./bin/lib$(STATARGET).h
else
# Links all the .o and .a to the executable
$(BINDIR)/$(EXETARGET): $(OFILES)
	$(CC) -o $@ $^ $(ALIB) $(CFG)
endif


.PHONY: clean all crun cvrun static run init
clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*
all:
	make
crun:
	make
	cd bin; ./$(EXETARGET)
cvrun:
	make
	cd bin; valgrind --leak-check=full --track-origins=yes ./$(EXETARGET)
static:
	make STATIC=STATIC
run:
	cd bin; ./$(EXETARGET)
init:
	mkdir "./bin" "./lib" "./obj" "./src"
