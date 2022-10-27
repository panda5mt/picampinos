#!/bin/bash
echo -e "[35m---------------- building firmware in firmware/ ---------------- [m"
cd firmware
if ./pico_build.sh; then 
echo -e "[35m---------------- build bin and uf2 files in firmware/build ---------------- [m"
else
echo -e "[31mError. None firmware is built.[m"
fi
