#!/bin/bash

# Compile
cd build && cmake .. && make

cd -

# Run
./build/bin/autohacka editor ./impel_down.hacc
