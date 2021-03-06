# taurus

## Description
Astrology-themed visuals using the Arduino TVout library.

## Setup
### Requirements
- Arduino IDE 1.6.8 (tested on Linux x64)
- Arduino Uno, Arduino Leonardo or Arduino Mega 2560 
- [TVout 1.0.1](https://github.com/Avamander/arduino-tvout) (tested on commit `d3003a5`)

### Hardware
Setup your Arduino board like this:

![img](https://user-images.githubusercontent.com/6868935/56868337-6d058f00-69c7-11e9-8973-919bfe2ac063.png)

Additionally, add a LED on pin 13.

### Sketch
1. Install the Arduino IDE and TVout library.
2. Add `-fpermissive` to the `compiler.cpp.flags` property in `hardware/arduino/avr/platform.txt` (relative to the Arduino IDE directory).
3. Copy all `.h` and `.cpp` files in `TVout/TVoutfonts` to `TVout` (relative to the Arduino IDE `libraries` directory).
4. Compile `taurus.ino` and upload it using the Arduino IDE.

If you're having issues linking against `libreadline.so.6`, use:
```bash
$ cd /lib/x86_64-linux-gnu
$ sudo ln -s libreadline.so.7.0 libreadline.so.6
```

If uploading results in `avrdude: ser_open(): can't open device "/dev/ttyACM0": Permission denied`, try:
```bash
$ sudo chmod a+rw /dev/ttyACM0
```

## Screenshots

![img_0714](https://user-images.githubusercontent.com/6868935/42914036-79731b78-8ace-11e8-88e7-f8c045d09696.jpg)
![img_0719](https://user-images.githubusercontent.com/6868935/42914050-96a5b64c-8ace-11e8-80aa-4fc0dfabe320.jpg)
![img_1715](https://cloud.githubusercontent.com/assets/6868935/26022668/2ab70f9a-3781-11e7-98f2-d60cb15e479e.JPG)
![img_1711](https://cloud.githubusercontent.com/assets/6868935/26022669/2aeebe40-3781-11e7-96ab-6e3bf6f0a840.JPG)
![img_1710](https://cloud.githubusercontent.com/assets/6868935/26022670/2aef7b46-3781-11e7-9255-9f0aff535799.JPG)
![img_1698](https://cloud.githubusercontent.com/assets/6868935/26022671/2af458c8-3781-11e7-8d7f-3fb0c9a38bb3.JPG)

## Ideas
- Conway's Game of Life
