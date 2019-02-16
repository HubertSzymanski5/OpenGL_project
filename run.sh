#!/bin/bash

# script to compile project, run it
# to clean simply go to build and remove files there

cd build
cmake ..
make
./project1
# rm -r *
echo "Zakonczono"
