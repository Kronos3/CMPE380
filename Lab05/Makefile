# Defaults params
ifndef CC
CC:=gcc
endif

ifndef CFLAGS
CFLAGS:=-O1 -g -Wall -std=c99 -pedantic
endif

ifndef LDFLAGS
LDFLAGS:=
endif

# Only print everything if VERBOSE is enable
ifndef VERBOSE
.SILENT:
endif

ifndef VALGRIND
VALGRIND:=valgrind --tool=memcheck --leak-check=yes --track-origins=yes
endif

ifndef SEARCHWORD
SEARCHWORD:=space
endif

# Compile all c files into object files
%.o : %.c
ifdef VERBOSE
	echo -n "Building $<..."
endif
	$(CC) -c $(CFLAGS) $< -o $@
ifdef VERBOSE
	echo "ok"
endif


TestSearch: TestSearch.o LinkedLists.o DynamicArrays.c
	$(CC) -o $@ $(LDFLAGS) $^


.PHONY: clean all

all: TestSearch

clean:
	-rm -f *.o
	-rm -f TestSearch

mem: all
	echo "Searching for $(SEARCHWORD)" > out.txt
	./TestSearch engWords.txt $(SEARCHWORD) >> out.txt 2>&1

test: all
	echo "------- Running Search Program valid input ------" > out.txt
	./TestSearch engWords.txt $(SEARCHWORD) >> out.txt 2>&1
	echo "------- Additional test, invalid input ------" >> out.txt
	./TestSearch engWords.txt xyzzy >> out.txt 2>&1


help:
	echo "Valid targets:"
	echo "  all: build all executables"
	echo "  clean: remove all executables and object files"
	echo "  test: run executables with test cases"
	echo "  mem: run executables with valgrind"
	echo "  help: print this help message"


