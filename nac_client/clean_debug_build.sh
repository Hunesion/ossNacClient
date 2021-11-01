#!/bin/sh

rm -rf ./build
mkdir ./build
cd ./build || exit
cmake -DCMAKE_BUILD_TYPE=Debug ..

make -j 4
