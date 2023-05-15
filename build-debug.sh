#!/bin/bash/env bash
BASE_DIR=$(dirname "$0")
BUILD_DIR=$BASE_DIR/build/
CODE_DIR=$BASE_DIR/src/code/
DATA_DIR=$BASE_DIR/src/data/


# make build directory
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

# # compile c++
g++ -std=c++20 -o $BUILD_DIR/build $CODE_DIR/**.cpp -fsanitize=address -fdiagnostics-color=always

# add data
cp -a $DATA_DIR $BUILD_DIR/data

# run
(cd $BUILD_DIR; ./build)

