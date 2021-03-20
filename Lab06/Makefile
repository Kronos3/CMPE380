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
FLAGS:=-DMOVE_FOPEN -DEN_TIME

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

hw6c: hw6c.o
	$(CC) -o $@ $(LDFLAGS) $^

hw6cpp: hw6cpp.o
	$(CXX) -o $@ $(LDFLAGS) $^

all: hw6c hw6cpp

.PHONY: clean all
.DEFAULT_GOAL := all

test: all
	# Don't clear out.txt so that we can sequentially run
	# these tests. Clear with `make clean`
	echo "==============" >> out.txt
	echo "Running ./hw6c data.txt" >> out.txt
	./hw6c data.txt >> out.txt
	echo "==============" >> out.txt
	echo "Running ./hw6cpp data.txt" >> out.txt
	./hw6cpp data.txt >> out.txt

clean:
	-rm -f *.o
	-rm -f hw6c hw6cpp
	-rm -f out.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  test: run executables with test cases"
	echo "  help: print this help message"


