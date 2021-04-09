#!/bin/bash

make clean
make test OPT_FLAGS=-O0
mv out.txt out_O0.txt

make clean
make test OPT_FLAGS=-O1
mv out.txt out_O1.txt

make clean
make test OPT_FLAGS=-O2
mv out.txt out_O2.txt

make clean
make test OPT_FLAGS=-O3
mv out.txt out_O3.txt
