#!/bin/bash
# for raspberry pi
sudo apt -y update
sudo apt -y install --no-install-recommends sudo make libtbb2 wget coreutils udev curl time tar nano 
sudo apt -y install git cmake libstdc++-arm-none-eabi-newlib gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential python3 

cd ~/
mkdir pico
cd pico
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
cd ..
git clone -b master https://github.com/raspberrypi/pico-examples.git

git clone https://github.com/panda5mt/picampinos.git
cd picampinos
cd firmware/
cp ../../pico-sdk/external/pico_sdk_import.cmake .
echo -e '#!/bin/bash\nmkdir -p build\ncd build\nexport PICO_SDK_PATH=../../../pico-sdk\ncmake ..\nmake' > pico_build.sh 
chmod +x pico_build.sh
cd ..
echo -e '#!/bin/bash\necho -e "\e[35m---------------- building firmware in firmware/ ---------------- \e[m"\ncd firmware\nif ./pico_build.sh; then \necho -e "\e[35m---------------- build bin and uf2 files in firmware/build ---------------- \e[m"\nelse\necho -e "\e[31mError. None firmware is built.\e[m"\nfi' > pico_build.sh 
chmod +x pico_build.sh
echo -e '#!/bin/bash\nNOW=`date "+%Y%m%d_%H%M%S"`\ngit add .\n# git commit -m "automatically uploaded at "$NOW\ngit commit -m "Automatically uploaded"\ngit push origin HEAD' > add_git.sh
chmod +x add_git.sh
git config --global core.autocrlf false


