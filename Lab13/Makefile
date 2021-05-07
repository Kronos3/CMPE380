# Defaults params
CC:=gcc
CXX:=g++

LIBS:=-lpthread

VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -g -O0
LDFLAGS:=$(LIBS)

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

hw13: hw13.c Timers.h
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

all: hw13
test: all
	echo "./hw13 -t 1 -s -v" > out.txt
	./hw13 -t 1 -s -v >> out.txt 2>&1
	echo "./hw13 -t 5 -s" >> out.txt
	./hw13 -t 5 -s >> out.txt 2>&1
	echo "./hw13 -t 8 -v" >> out.txt
	./hw13 -t 8 -v >> out.txt 2>&1
	echo "./hw13 -t 0" >> out.txt
	-./hw13 -t 0 >> out.txt 2>&1
	echo "./hw13 -t 9" >> out.txt
	-./hw13 -t 9 >> out.txt 2>&1

mem: hw13
	echo "$(VALGRIND) ./hw13 -f -s -t 8" > mem.txt
	$(VALGRIND) ./hw13 -f -s -t 8 >> mem.txt 2>&1

clean:
	-rm -f hw13
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
