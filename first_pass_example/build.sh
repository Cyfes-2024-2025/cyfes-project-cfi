#!/bin/bash

if [ ! -d build ]; then
    mkdir build
    cd build
    cmake ..
    cd ..
fi

cd build
make
cd ..

cp ./build/skeleton/SkeletonPass.* ./example/
cd example
make
