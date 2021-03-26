# Defaults params
CC:=gcc
CXX:=g++

CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -O1 -g
CXXFLAGS:=$(CXXFLAGS) -Wall -std=c++98 -pedantic -O1 -g
LDFLAGS:=

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

# Timing macro
# FLAGS:=-DEN_TIME -DROWS=10000 -DCOLS=100 -DREPEAT_NUM=1000
FLAGS:=-DROWS=3 -DCOLS=3

# Compile all c files into object files
%.o : %.c Timers.h
ifdef VERBOSE
	echo -n "Building $<..."
endif
	$(CC) -c $(FLAGS) $(CFLAGS) $< -o $@
ifdef VERBOSE
	echo "ok"
endif

# Compile all cpp files into object files
%.o : %.cpp Timers.h
ifdef VERBOSE
	echo -n "Building $<..."
endif
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $< -o $@
ifdef VERBOSE
	echo "ok"
endif

lab_a: lab_a.o
	$(CC) -o $@ $(LDFLAGS) $^

lab_c: lab_c.o
	$(CXX) -o $@ $(LDFLAGS) $^

all: lab_a lab_c

.PHONY: clean all
.DEFAULT_GOAL := all

test: all
	# Don't clear out.txt so that we can sequentially run
	# these tests. Clear with `make clean`
	echo "NotImplemented"

clean:
	-rm -f *.o
	-rm -f lab_c lab_a
	-rm -f out.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  test: run executables with test cases"
	echo "  help: print this help message"


