# Defaults params
CC:=gcc
CXX:=g++

VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -g
CXXFLAGS:=$(CXXFLAGS) -Wall -std=c++98 -pedantic -g
LDFLAGS:=

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

ifndef PREFIX_CMD
# This can be set to run the program
# under a wrapper like valgrind
PREFIX_CMD:=
endif

ifndef OUTPUT_FILE
OUTPUT_FILE:=out.txt
endif

# Disable verbosity in tests if NO_VERBOSE_TESTS
# is set. This will allow us to test timing
# and not output noise and measure inaccurate numbers
# because of terminal I/O.
ifndef NO_VERBOSE_TESTS
	VERBOSE_OPTION:=-verbose
else
	VERBOSE_OPTION:=
endif

# Optimization runs
# Full optimization (not used)
TIMING_FLAGS:=-DEN_TIME -DMOVE_IO_CLOSE -DIO_REPEAT=100 -DCALC_REPEAT=20000

# Compile all c files into executables
# Create different object files for each optimization level
%_0 : %.c
	$(CC) -O0 $(TIMING_FLAGS) $(CFLAGS) $< -o $@
%_2 : %.c
	$(CC) -O2 $(TIMING_FLAGS) $(CFLAGS) $< -o $@
%_3 : %.c
	$(CC) -O3 $(TIMING_FLAGS) $(CFLAGS) $< -o $@
%_mem : %.c
	$(CC) -O1 $(CFLAGS) $< -o $@
hw8% : hw8%.c
	$(CC) -O1 $(TIMING_FLAGS) $(CFLAGS) $< -o $@


# Create the executable matrix
basic: hw8c hw8c_0 hw8c_2 hw8c_3
opt1: hw8c_opt1 hw8c_opt1_0 hw8c_opt1_2 hw8c_opt1_3
opt2: hw8c_opt2 hw8c_opt2_0 hw8c_opt2_2 hw8c_opt2_3
opt3: hw8c_opt3 hw8c_opt3_0 hw8c_opt3_2 hw8c_opt3_3
mem_build: hw8c_mem hw8c_opt1_mem hw8c_opt2_mem hw8c_opt3_mem

all: basic opt1 opt2 opt3 mem_build

.PHONY: clean all
.DEFAULT_GOAL := all

HUMAN_OPT := "" "_opt1" "_opt2" "_opt3"
COMP_OPT  := "_0" "" "_2" "_3"

test: all
	echo -e "Running all optimization tests\n" > out.txt
	for human_opt in $(HUMAN_OPT) ; do \
		for comp_opt in $(COMP_OPT) ; do \
			echo "./hw8c$${human_opt}$${comp_opt} data.txt" >> out.txt; \
			./"hw8c$${human_opt}$${comp_opt}" data.txt >> out.txt; \
			echo "ls -l hw8c$${human_opt}$${comp_opt}" >> out.txt; \
			ls -l hw8c$${human_opt}$${comp_opt} >> out.txt; \
			echo "size hw8c$${human_opt}$${comp_opt}" >> out.txt; \
			size hw8c$${human_opt}$${comp_opt} >> out.txt; \
			echo "" >> out.txt; \
		done \
	done

mem: mem_build
	echo -e "Running valgrind tests\n" > mem.txt
	for human_opt in $(HUMAN_OPT) ; do \
		echo "${VALGRIND} ./hw8c$${human_opt}$${comp_opt}_mem data.txt" >> mem.txt; \
		${VALGRIND} ./hw8c$${human_opt}$${comp_opt}_mem data.txt >> mem.txt 2>&1; \
		echo "" >> mem.txt; \
	done

clean:
	-rm -f *.o
	-rm -f  hw8c hw8c_0 hw8c_2 hw8c_3 \
			hw8c_opt1 hw8c_opt1_0 hw8c_opt1_2 hw8c_opt1_3 \
			hw8c_opt2 hw8c_opt2_0 hw8c_opt2_2 hw8c_opt2_3 \
			hw8c_opt3 hw8c_opt3_0 hw8c_opt3_2 hw8c_opt3_3 \
			hw8c_mem hw8c_opt1_mem hw8c_opt2_mem hw8c_opt3_mem
	-rm -f out.txt
	-rm -f mem.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  mem: run valgrind tests"
	echo "  test: run executables with test cases"
	echo "  help: print this help message"


