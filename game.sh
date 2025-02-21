#!/bin/bash

# Compile and run
cd build && cmake .. && make && cd - && ./build/bin/autohacka game ./impel_down.hacc
