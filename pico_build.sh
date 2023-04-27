#!/bin/bash
echo -e "\e[35m---------------- building firmware in firmware/ ---------------- \e[m"
cd firmware
if ./pico_build.sh; then 
echo -e "\e[35m---------------- build bin and uf2 files in firmware/build ---------------- \e[m"
else
echo -e "\e[31mError. None firmware is built.\e[m"
fi

