#!/bin/sh

mkdir -p bin
mkdir -p build
mkdir -p outputs
cd build
cmake ../
make