#!/bin/bash

BUILD_DIR="build-debug"

rm -rf "$BUILD_DIR" bin
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug && cmake --build "$BUILD_DIR" -j