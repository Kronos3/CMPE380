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

TIMING_FLAGS:=-DEN_TIME -DTIM_ITER=10000

ifndef OPT_FLAGS
OPT_FLAGS:=-O1
endif

quadTest: quadTest.c
	$(CC) -o $@ $(CFLAGS) $(TIMING_FLAGS) $(OPT_FLAGS) $(LDFLAGS) $^

quadTest_mem: quadTest.c
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) -O1 $^

all: quadTest quadTest_mem
test: quadTest
	# Invalid params (a coeff == 0)  --- EXPECTED FAIL
	echo "./quadTest -a 0 -b 0 -c 1" > out.txt
	-./quadTest -a 0 -b 0 -c 1 >> out.txt 2>&1

	# Invalid params (missing param) --- EXPECTED FAIL
	echo -e "\n./quadTest -a 1 -b 0" >> out.txt
	-./quadTest -a 1 -b 0 >> out.txt 2>&1

	echo -e "\n./quadTest -a 1 -b 0 -c 1" >> out.txt
	./quadTest -a 1 -b 0 -c 1 >> out.txt 2>&1

	# Test out real numbers (floating)
	echo -e "\n./quadTest -a 1.2561 -b 143.35 -c 277.881" >> out.txt
	./quadTest -a 1.2561 -b 143.35 -c 277.881 >> out.txt 2>&1

mem: quadTest_mem
	echo "./quadTest -a 1 -b 0 -c 1" > mem.txt
	$(VALGRIND) ./quadTest -a 1 -b 0 -c 1 >> mem.txt 2>&1

clean:
	-rm -f quadTest
	-rm -f quadTest_mem
	-rm -f out.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  mem: run valgrind tests"
	echo "  test: run executables with test cases"
	echo "  help: print this help message"
