#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Release -S ./XCPlite -B build_C_Demo
cd build_C_Demo/
make
./XCPlite.out