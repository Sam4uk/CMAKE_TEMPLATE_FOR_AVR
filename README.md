# CMAKE TEMPLATE FOR AVR

## LICENSE

               "THE BEER-WARE LICENSE": 
                             
    git:Sam4uk wrote this file. As long as you retain this 
    notice you can do whatever you want  with this stuff. 
    If we meet some day, and  you  think  this stuff is  
    worth it, you can buy me a beer in return.           

## INTRO

This a template project for AVR microcontrollers. Using cmake tool for build, loading and burning fuses.

## HOW CUSTOMISE

### PROGRAMMER
```cmake
set(CMAKE_PROGRAMMER "USBtiny")
```

This option is set programer: usbasp, usbtiny, arduino and other.
You can see if posible values if run
```sh
avrdude -c ?
```

```cmake
set(CMAKE_PORT "usb")
```
This property set port where connected programmer

### DEVICE

```cmake
set(CMAKE_MICROCHIP "atmega48p")
```

```cmake
set(FREQUENCY "1200000") # Hz
```

~WARNING~
Use fuse bit calculator

```cmake 
set(CMAKE_E_FUSE "0xFF")
set(CMAKE_H_FUSE "0xDF")
set(CMAKE_L_FUSE "0x62")
```

### TOOLCHAIN
```cmake
set(CMAKE_C_COMPILER "avr-gcc")
set(CMAKE_CXX_COMPILER "avr-g++")
set(CMAKE_OBJCOPY "avr-objcopy")
set(CMAKE_AVRDUDE "avrdude")
```
### SOFT
```cmake
set(CMAKE_C_STANDARD "11")
set(CMAKE_CXX_STANDARD "14")
```

## HOW USE

### BUILD
Read manual for cmake
### DOUNLOAD
If project not build
```sh
mkdir build
cmake ..
cmake --build .
cmake --build . -- download
```
or
```sh
mkdir build
cmake ..
make
make download
```
If project build
```sh
make download
```
or
```sh
cmake --build . -- download
```
### BURN FUSES


