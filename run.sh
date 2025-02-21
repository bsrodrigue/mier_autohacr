#!/bin/bash

# Compile
cd build && cmake .. && make

cd -

# Run
./build/bin/autohacka game ./impel_down.hacc
