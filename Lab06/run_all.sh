#!/bin/bash

make clean

# Test 1
COMMON_FLAGS="-DIO_REPEAT=100 -DCALC_REPEAT=20000 -DEN_TIME"
make -B FLAGS="${COMMON_FLAGS}"
echo "-------------- hw6c_1  x1, no move fopen" > out.txt
./hw6c data.txt >> out.txt
echo "" >> out.txt

# Test 2
COMMON_FLAGS="-DIO_REPEAT=200 -DCALC_REPEAT=40000 -DEN_TIME"
make -B FLAGS="${COMMON_FLAGS}"
echo "-------------- hw6c_2  x2, no move fopen" >> out.txt
./hw6c data.txt >> out.txt
echo "" >> out.txt

# Test 3
COMMON_FLAGS="-DIO_REPEAT=100 -DCALC_REPEAT=20000 -DEN_TIME -DMOVE_FOPEN"
make -B FLAGS="${COMMON_FLAGS}"
echo "-------------- hw6c_f1  x1, yes move fopen" >> out.txt
./hw6c data.txt >> out.txt
echo "" >> out.txt

# Test 4
COMMON_FLAGS="-DIO_REPEAT=200 -DCALC_REPEAT=40000 -DEN_TIME -DMOVE_FOPEN"
make -B FLAGS="${COMMON_FLAGS}"
echo "-------------- hw6c_f2  x2, yes move fopen" >> out.txt
./hw6c data.txt >> out.txt
echo "" >> out.txt

#### CPP Tests ####
COMMON_FLAGS="-DIO_REPEAT=100 -DCALC_REPEAT=20000 -DEN_TIME"
make -B FLAGS="${COMMON_FLAGS}"
echo "-------------- hw6cpp_1 x1" >> out.txt
./hw6cpp data.txt >> out.txt
echo "" >> out.txt

COMMON_FLAGS="-DIO_REPEAT=200 -DCALC_REPEAT=40000 -DEN_TIME"
make -B FLAGS="${COMMON_FLAGS}"
echo "-------------- hw6cpp_2 x2" >> out.txt
./hw6cpp data.txt >> out.txt
