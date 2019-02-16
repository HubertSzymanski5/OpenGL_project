#!/bin/bash

cd build
cmake ..
make
./project1
rm -r *
echo "Zakonczono"
