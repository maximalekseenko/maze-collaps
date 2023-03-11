#!/bin/bash/env bash
BUILD_DIR=./build/
SRC_DIR=./src/
DATA_DIR=./src/data/

# make build directory
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

# compile c++
g++ -std=c++20 -o $BUILD_DIR/build $SRC_DIR/*.cpp

# add data
cp -a $DATA_DIR $BUILD_DIR/data

$BUILD_DIR/build