# pi-cám-pinos
DVP CAMERA IF(8-bit parallel IF) using RP2040 with SFP Transceiver.
This project is for connecting a DVP camera, such as the OV5642, to Raspberry Pi Pico; the DVP camera is primarily an 8-bit parallel one produced by OmniVision, but it may be applicable to other image sensors with 8-bit parallel interfaces with I2C Control.

pi-cám-pinos is an anagram of 'pi pico camera'.
The name is derived from the Spanish word for 'Great spotted woodpecker'.
It is also derived from the name of a Raspberry mixed candy popular in the United States and Canada.
## Getting Started
### For Raspberry Pi 3/4
Use <code>[get_sdk.sh](get_sdk.sh)</code> to clone all Projects with Raspberry Pi Pico SDKs and Pico Examples.

```
wget https://raw.githubusercontent.com/panda5mt/picampinos/main/get_sdk.sh

chmod +x get_sdk.sh
./get_sdk.sh
```
To compile this project, just run
```
cd ~/
cd picampinos/firmware/
./pico_build.sh
```

### For Docker
Use <code>[Dockerfile](Dockerfile)</code>.
When you'd built an image, run and attach the image, type on terminal,
```
cd /root/picampinos/firmware/
./pico_build.sh
```
The docker image contains this Project, Pico SDKs, and Pico Examples.


### On your local CLI
Get ideas to see <code>[get_sdk.sh](get_sdk.sh)</code>.

## Hardware settings
This is <code>[Schematics](hardware/RP2040Board.pdf)</code>.
For instance, check comment in <code>[firmware/main.c](firmware/main.c)</code> for connection via RP2040 and DVP Camera.

This module has three options, each of which is exclusive; only one option may be used at a time.
### Using SFP module (Default)
With the SFP module, pi-cám-pinos sends image data from the DVP camera to the optical fiber at up to 100 Mbps.
### Using SPI Slave module(not tested)
### Using UART module (easiest)

