#!/bin/sh

rm -rf ./build
mkdir ./build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug ..

make -j 4
