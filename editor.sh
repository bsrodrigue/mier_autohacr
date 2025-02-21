#!/bin/bash

# Compile and run
cd build && cmake .. && make && cd - && ./build/bin/autohacka editor ./impel_down.hacc
