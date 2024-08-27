#!/bin/bash
mkdir -p build
cd build
export PICO_SDK_PATH=../../../pico-sdk
export PICOTOOL_FETCH_FROM_GIT_PATH=../../../picotool
cmake ..
make
