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

TEST_FILES := "ge0.txt" "ge1.txt" "ge2.txt" "ge3.txt" \
              "ge4.txt" "ge5.txt" "ge6.txt" "ge7.txt" \
              "ge8.txt" "ge9.txt" "ge10.txt" "ge11.txt" \
              "ge12.txt"

tests: all
	echo "Running tests ge0.txt ... ge12.txt" > out.txt
	for test_file in $(TEST_FILES) ; do \
		echo "./hw12 -i $${test_file}" >> out.txt; \
		./"hw12" -i $${test_file} >> out.txt 2>&1 || true; \
		echo "" >> out.txt; \
	done

clean:
	-rm -f rand.txt
	-rm -f hw11
	-rm -f *.o
	-rm out.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  mem: run solve under valgrind"
	echo "  tests: run the executable with the test files"
	echo "  help: print this help message"
