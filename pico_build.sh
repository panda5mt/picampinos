#!/bin/bash
echo -e "[35m---------------- building firmware in firmware/ ---------------- [m"
cd firmware
./pico_build.sh
echo -e "[35m---------------- the build bin and uf2 files in firmware/build ---------------- [m"
