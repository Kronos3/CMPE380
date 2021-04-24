# Defaults params
CC:=gcc
CXX:=g++

LIBS:=-lgsl -lgslcblas

VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -g -DHW8
LDFLAGS:=$(LIBS)

ifndef ORDER
ORDER:=3
endif

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

correction: correction.c
	$(CC) -o $@ $(CFLAGS) $^

hw11: hw11.c DynamicArrays.c DynamicArrays.h
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) -O1 $^

all: hw11 correction

base:
	./realDevice | ./detError

correct: correction
	./realDevice | ./correction | ./detError

plot: correction
	./realDevice > realdevice.txt
	./realDevice | ./correction > correction.txt
	paste realdevice.txt correction.txt > alldata.txt
	./myplot -i alldata.txt -o correction.png

# Generate out real device data
# (used for solve and mem targets)
# This is the error not the original device output
data.txt:
	./realDevice | ./diffVal > data.txt

# Determine an Nth order polynomial solution
solve: hw11 data.txt
	./hw11 -p data.txt -o $(ORDER)

# Run the above algorithm under valgrind
mem: hw11 data.txt
	echo "./hw11 -p data.txt -o $(ORDER)" > mem.txt
	$(VALGRIND) ./hw11 -p data.txt -o $(ORDER) >> mem.txt 2>&1

x:
	chmod +x ./realDevice
	chmod +x ./detError
	chmod +x ./myplot
	chmod +x ./diffVal

clean:
	-rm -f correction.txt alldata.txt
	-rm -f alldata.txt
	-rm -f data.txt mem.txt
	-rm -f correction hw11
	-rm -f correction.png

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  solve: solve for an nth order polynomial"
	echo "  mem: run solve under valgrind"
	echo "  base: show non-corrected value error"
	echo "  correct: show corrected value error"
	echo "  plot: plot the original, correct and ideal curves in correction.png"
	echo "  x: Add execute permissions to given binaries"
	echo "  help: print this help message"
	echo ""
	echo "Environment variables:"
	echo "  ORDER: int - For solve and mem. Nth order polynomial solution"
	echo "  VERBOSE: bool - Show all build commands during 'make'"
