# Defaults params
CC:=gcc
CXX:=g++

LIBS:=-lgsl -lgslcblas

VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -g -DHW8
LDFLAGS:=$(LIBS)

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

hw12: hw12.c
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) -O1 $^

all: hw12

rand.txt: genRand
	./genRand > rand.txt

mem: rand.txt
	$(VALGRIND) ./hw12 -i rand.txt > mem.txt 2>&1

tests: all
	echo "Running tests ge0.txt ... ge12.txt" > out.txt
	TNUM=0; while [[ $$TNUM -le 12 ]] ; do \
		echo "./hw12 -i ge$${TNUM}.txt" >> out.txt; \
		./"hw12" -i ge$${TNUM}.txt >> out.txt 2>&1 || true; \
		echo "" >> out.txt; \
		((TNUM = TNUM + 1)) ; \
	done

clean:
	-rm -f rand.txt
	-rm -f hw12
	-rm -f *.o
	-rm out.txt mem.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  mem: run solve under valgrind"
	echo "  tests: run the executable with the test files"
	echo "  help: print this help message"
