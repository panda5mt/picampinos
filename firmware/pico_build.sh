#!/bin/bash
mkdir -p build
cd build
export PICO_SDK_PATH=../../../pico-sdk
cmake ..
make
