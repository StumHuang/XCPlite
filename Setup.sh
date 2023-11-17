#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Release -S C_Demo -B build_C_Demo
cd build_C_Demo/
make clean
make
./C_Demo.out