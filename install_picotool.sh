#!/bin/bash

# 必要な依存関係をインストール
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential git

cd ~/
cd pico/

# picotoolのソースコードをクローン
git clone https://github.com/raspberrypi/picotool.git
cd picotool

# CMakeを使ってビルド
export PICO_SDK_PATH=../../pico-sdk
mkdir build
cd build
cmake ..
make

# バイナリをシステムにインストール（オプション）
#sudo make install
