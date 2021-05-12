# Defaults params
CC:=gcc
CXX:=g++

LIBS:=-lpthread
PROG=hw13
VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -g -O0
LDFLAGS:=$(LIBS)

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

$(PROG): $(PROG).c Timers.h
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

all: $(PROG)

test: all
	echo "./$(PROG) -t 1 -s -v" > out.txt
	./$(PROG) -t 1 -s -v >> out.txt 2>&1
	echo "./$(PROG) -t 5 -s" >> out.txt
	./$(PROG) -t 5 -s >> out.txt 2>&1
	echo "./$(PROG) -t 8 -v" >> out.txt
	./$(PROG) -t 8 -v >> out.txt 2>&1
	echo "./$(PROG) -t 0" >> out.txt
	-./$(PROG) -t 0 >> out.txt 2>&1
	echo "./$(PROG) -t 9" >> out.txt
	-./$(PROG) -t 9 >> out.txt 2>&1

mem: all
	echo "$(VALGRIND) ./$(PROG) -f -s -t 8" > mem.txt
	$(VALGRIND) ./$(PROG) -f -s -t 8 >> mem.txt 2>&1

clean:
	-rm -f $(PROG)
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
