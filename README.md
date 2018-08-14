# ft245-usb-relay
Command line tool for controlling FTDI FT245 based USB relay module.
It works with the following devices:
* Quimat 5V USB 4 Relay Module
* ... Please let me know if you use this library with other devices!

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

## Compilation troubleshooting

Some user experienced problem when compiling with the above mentioned command, here it is the trouble shooting:

>Calling the following command threw some error lines at me:
> $ gcc -o ft245-usb-relay -lftdi1 ft245-usb-relay.c -I /usr/include/libftdi1/
> /tmp/ccoYKXO0.o: In function main': ft245-usb-relay.c:(.text+0x3d9): undefined reference toftdi_new'
> ft245-usb-relay.c:(.text+0x422): undefined reference to ftdi_usb_open' ft245-usb-relay.c:(.text+0x437): undefined reference toftdi_get_error_string'
> ft245-usb-relay.c:(.text+0x462): undefined reference to ftdi_free' ft245-usb-relay.c:(.text+0x489): undefined reference toftdi_read_chipid'
> ft245-usb-relay.c:(.text+0x4b9): undefined reference to ftdi_read_data' ft245-usb-relay.c:(.text+0x4f0): undefined reference toftdi_free'
> ft245-usb-relay.c:(.text+0x57b): undefined reference to ftdi_free' ft245-usb-relay.c:(.text+0x59b): undefined reference toftdi_set_bitmode'
> ft245-usb-relay.c:(.text+0x5cb): undefined reference to ftdi_free' ft245-usb-relay.c:(.text+0x614): undefined reference toftdi_read_data'
> ft245-usb-relay.c:(.text+0x64f): undefined reference to ftdi_write_data' ft245-usb-relay.c:(.text+0x680): undefined reference toftdi_free'
> ft245-usb-relay.c:(.text+0x6b3): undefined reference to ftdi_free' ft245-usb-relay.c:(.text+0x6c6): undefined reference toftdi_free'
> collect2: error: ld returned 1 exit status
> The following modification did the trick for me (the project compiled & linked):
> $ gcc -o ft245-usb-relay -lftdi1 ft245-usb-relay.c $(pkg-config --cflags --libs libftdi1)

Thanks to user
@maehw
for pointing this out!

## Links
* libFTDI website: https://www.intra2net.com/en/developer/libftdi/
* FT245R datasheet: http://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT245R.pdf
