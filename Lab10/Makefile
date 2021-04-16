# Defaults params
CC:=gcc
CXX:=g++

LIBS:=-lm

VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -g
LDFLAGS:=$(LIBS)

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

TIMING_FLAGS:=-DEN_TIME -DITERATIONS=2500000

ifndef EXTRA_FLAGS
EXTRA_FLAGS:=-DQN=22
endif

intTest: intTest.c bisection.c rootfinding.h Timers.h
	$(CC) -o $@ $(CFLAGS) $(TIMING_FLAGS) $(LDFLAGS) $(EXTRA_FLAGS) $^

intTest_mem: intTest.c bisection.c rootfinding.h Timers.h
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(EXTRA_FLAGS) $(LDFLAGS) -O1 $^

all: intTest intTest_mem
test: intTest
	./intTest > out.txt 2>&1

mem: intTest_mem
	$(VALGRIND) ./intTest_mem > mem.txt 2>&1

clean:
	-rm -f intTest
	-rm -f intTest_mem
	-rm -f out.txt
	-rm -f mem.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  mem: run valgrind tests"
	echo "  test: run executables with test cases"
	echo "  help: print this help message"

.PHONY: all
