# ft245-usb-relay
Command line tool for controlling FTDI FT245 based USB relay module.

## Requirements
Install the following packages: 
* libftdi1
* libftdi1-dev

## Compile
> gcc -o ft245-usb-relay -lftdi1 ft245-usb-relay.c

if, during the compilation, gcc cannot find the ftdi.h header file try to add the 
option **-I** to the gcc command line.

In my case, the gcc command that works is:
> gcc -o ft245-usb-relay -lftdi1 ft245-usb-relay.c -I /usr/include/libftdi1/

## Usage examples

Print the current state of the output:
> ft245-usb-relay

Set the output D0 to HIGH (1) (don't modify the other ouput):
> ft245-usb-relay -p 0 -e 1

Set the first 4 ouput lines (D0 to D3) to HIGH and the last 4 (D4 to D7) to LOW:
> ft245-usb-relay -o 0F

Set the first 4 ouput lines (D0 to D3) to LOW and don't modify the last 4 (D4 to D7):
> ft245-usb-relay -o 00 -m 0F
