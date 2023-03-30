#!/bin/bash/env bash
BASE_DIR=$(dirname "$0")
BUILD_DIR=$BASE_DIR/build/
CODE_DIR=$BASE_DIR/src/code/
DATA_DIR=$BASE_DIR/src/data/


# make build directory
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi


# xxd -i $DATA_DIR/*.png > $BUILD_DIR/data.

# # compile c++
g++ -std=c++20 -o $BUILD_DIR/build $CODE_DIR/*.cpp -fsanitize=address -fdiagnostics-color=always

# add data
# cat $BUILD_DIR/build $DATA_DIR/*.PNG > $BUILD_DIR/build2
# cp -a $DATA_DIR $BUILD_DIR/data

# run
(cd $BUILD_DIR; ./build)

(rm -r $BUILD_DIR/build)

