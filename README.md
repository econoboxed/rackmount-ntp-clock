# rackmount-ntp-clock

[<img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white">](https://www.arduino.cc/)
[<img src="https://img.shields.io/badge/adafruit-000000?style=for-the-badge&logo=adafruit&logoColor=white">](https://www.adafruit.com/)

This project uses an adafruit feather rp2040, an ethernet hat, and a 7-Segment Display to display the time. The project is meant to be rackmounted in 10-inch, or half rack.

## Bill of Materials:

1. [Adafruit Feather RP2040](https://www.adafruit.com/product/4884)
2. [Adafruit Ethernet Featherwing](https://www.adafruit.com/product/3201)
3. [Adafruit 7seg](https://www.adafruit.com/product/1269)
4. [Sparkfun USB-C breakout Board](https://www.sparkfun.com/products/15100)

## Assembly:

All items are meant to friction-fit into the single printed part.

Install the ethernet featherwing on the feather rp2040, and connect the 7-segment display via **both** the i2c connector, and 3.3v power (labeled '+' and '-' on the 7seg PCB).

Optionally connect "VBUS and "GND" on the sparkfun breakout board to "USB" and "GND" on the rp2040 if you wish to have a backwards-facing usb (otherwise rails may impede the usb-c cord).

Flash the rp2040 with the provided .ino file in the repository. Some libraries are required, such as Ethernet, SPI, Adafruit GFX, Adafruit_LED_Backpack_Library.

## Printing:

I printed the plate on my Prusamk3s+, the .3mf project file is included, this file includes the keystone, which I did not model in freeCAD.

The part was printed in PLA, layer height was .3mm, with a gyroid infill of 15%.

I printed with no supports, but the mounting pegs for the usb-c breakout board needed to be filed to fit.
