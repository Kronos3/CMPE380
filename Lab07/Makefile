# Defaults params
CC:=gcc
CXX:=g++

# Link in libm (math)
LIBS:=-lm

VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes
LIBS:=-lm
CFLAGS:=$(CFLAGS) -Wall -std=c99 -pedantic -g
CXXFLAGS:=$(CXXFLAGS) -Wall -std=c++98 -pedantic -g
LDFLAGS:=$(LIBS)

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
OPT_COMMON:=-DEN_TIME -DTIM_ITER=100000
O1_OPT:=-O1 $(OPT_COMMON)
O2_OPT:=-O2 $(OPT_COMMON)
O3_OPT:=-O3 $(OPT_COMMON)
FULL_OPT:=-march=native -pipe $(O2_OPT)

# FLAGS:=$(O1_OPT)

# Compile all c files into object files
%.o : %.c
	$(CC) -c $(FLAGS) $(CFLAGS) $< -o $@


hw7: hw7.o eqn2solve.o rootfinding.o
	$(CC) -o $@ $(LDFLAGS) $^

all: hw7

.PHONY: clean all
.DEFAULT_GOAL := all

tests: all
	###################################
	##           Testcases           ##
	##
	##  Tolerance: 1e-6
	##
	## - For Bisection use the interval [−2.5,2.5].
	## - For Newton’s method use two different initial guesses: −1.5 and 1.5
	## - For the Secant method use the same initial guesses as in Newton’s case and
	#    provide a suitable additional second point for initialization (justify your choice in
	#    the analysis part.)
	##################################
	$(info Test output to '$(OUTPUT_FILE)')
	echo "Running algorithm tests" > $(OUTPUT_FILE)
	echo "Running Bisection test" >> $(OUTPUT_FILE)
	$(PREFIX_CMD) ./hw7 -bisection -tolerance 1e-6 -g1 -2.5 -g2 2.5 $(VERBOSE_OPTION) >> $(OUTPUT_FILE) 2>&1

	echo -e "\nRunning Newton test #1" >> $(OUTPUT_FILE)
	$(PREFIX_CMD) ./hw7 -newton -tolerance 1e-6 -g1 -1.5 $(VERBOSE_OPTION) >> $(OUTPUT_FILE) 2>&1

	echo -e "\nRunning Newton test #2" >> $(OUTPUT_FILE)
	$(PREFIX_CMD) ./hw7 -newton -tolerance 1e-6 -g1 1.5 $(VERBOSE_OPTION) >> $(OUTPUT_FILE) 2>&1

	echo -e "\nRunning Secant test #1" >> $(OUTPUT_FILE)
	$(PREFIX_CMD) ./hw7 -secant -tolerance 1e-6 -g1 -2.5 $(VERBOSE_OPTION) >> $(OUTPUT_FILE) 2>&1

	echo -e "\nRunning Secant test #2" >> $(OUTPUT_FILE)
	$(PREFIX_CMD) ./hw7 -secant -tolerance 1e-6 -g1 96 $(VERBOSE_OPTION) >> $(OUTPUT_FILE) 2>&1

	echo -e "\nAll tests finished" >> $(OUTPUT_FILE)

opts: all
	$(info Option output to '$(OUTPUT_FILE)')
	echo "Running option tests" > $(OUTPUT_FILE)
	echo "Running './hw7 -b -tol 1E-6 -ga -2 -gb 3 -verb'" >> $(OUTPUT_FILE)
	./hw7 -b -tol 1E-6 -ga -2 -gb 3 -verb >> $(OUTPUT_FILE) 2>&1

	echo -e "\nRunning './hw7 -b -t 1E-6 -g 2 -u 3 -v'" >> $(OUTPUT_FILE)
	./hw7 -b -t 1E-6 -g 2 -u 3 -v >> $(OUTPUT_FILE) 2>&1

	echo -e "\nRunning './hw7 -b -t 1E-6 -ba 2 -bb 3 -v'" >> $(OUTPUT_FILE)
	./hw7 -b -t 1E-6 -ba 2 -bb 3 -v >> $(OUTPUT_FILE) 2>&1

mem: all
	OUTPUT_FILE=mem.txt PREFIX_CMD="$(VALGRIND)" $(MAKE) tests

clean:
	-rm -f *.o
	-rm -f hw7
	-rm -f out.txt
	-rm -f mem.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  test: run executables with test cases"
	echo "  help: print this help message"


