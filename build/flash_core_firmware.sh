#!/bin/bash -x

dfu-util -l

sleep 1

dfu-util -d 1d50:607f -a 0 -s 0x08005000:leave -D core-firmware.bin

