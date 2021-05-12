# Defaults params
CC:=gcc
CXX:=g++

LIBS:=-lm
PROG=hw14
VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

CFLAGS:=$(CFLAGS) -Wall -std=c99 -g
LDFLAGS:=$(LIBS)

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(PROG): $(PROG).o ODEsolvers.o simulations.o
	$(CC) -o $@ $(LDFLAGS) $^

all: $(PROG)

TANK_PARAMS=-tank -x1 0 -x2 0 -step 5 -ftime 600
tank: all
	./$(PROG) $(TANK_PARAMS) -eu > t1.sim
	./$(PROG) $(TANK_PARAMS) -rk2 > t2.sim
	./$(PROG) $(TANK_PARAMS) -rk3 > t3.sim
	./$(PROG) $(TANK_PARAMS) -rk4 > t4.sim
	paste t1.sim t2.sim t3.sim t4.sim > tank.sim
	./myplot2 -tank tank.sim tank.png

SPRING_PARAMS=-spring -step 0.01 -ftime 10 -x1 1 -x2 0
damp: all
	./$(PROG) $(SPRING_PARAMS) -rk2 -damp 0.2 > damp.sim
	./myplot2 -damp damp.sim damp.png

spring: all
	./$(PROG) $(SPRING_PARAMS) -eu > s1.sim
	./$(PROG) $(SPRING_PARAMS) -rk2 > s2.sim
	./$(PROG) $(SPRING_PARAMS) -rk3 > s3.sim
	./$(PROG) $(SPRING_PARAMS) -rk4 > s4.sim
	paste s1.sim s2.sim s3.sim s4.sim > spring.sim
	./genErrors spring.sim diff.sim
	./myplot2 -spring diff.sim spring.png

tests: tank damp spring

mem: all
	echo "$(VALGRIND) ./$(PROG) $(TANK_PARAMS) -rk3" > mem.txt
	$(VALGRIND) ./$(PROG) $(TANK_PARAMS) -rk3 >> mem.txt 2>&1
	echo -e "\n$(VALGRIND) ./$(PROG) $(TANK_PARAMS) -rk4" >> mem.txt
	$(VALGRIND) ./$(PROG) $(TANK_PARAMS) -rk4 >> mem.txt 2>&1

clean:
	-rm -f $(PROG)
	-rm -f *.o
	-rm -f *.sim
	-rm -f *.png
	-rm -f mem.txt

help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and temporary files"
	echo "  tank: run tank simulation under all four simulators and generate tank.png"
	echo "  damp: run spring simulation with dampening 0.2 and generate damp.png"
	echo "  spring: run spring simulation without dampening and generate spring.png and diff.png"
	echo "  mem: run tank with RK3 and RK4 under valgrind"
	echo "  test: run tank, damp, and spring tests"
	echo "  help: print this help message"

.PHONY: all
