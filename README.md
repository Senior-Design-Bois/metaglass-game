Interactive Simon Says Game
===========================

This code is for configuring a system that allows a user to play Simon Says using a glass interface.

## Hardware

We use the [Arudio Feather](https://www.adafruit.com/product/3010) and [CAP1188 Sensor](https://learn.adafruit.com/adafruit-cap1188-breakout) to encode the logic in our demonstration. We use an [Adafruit Neopixel](https://learn.adafruit.com/adafruit-neopixel-uberguide) for our LED interfacing and display. We additionally use [ITO glass panes](https://www.adafruit.com/product/1310) from Adafruit as well.

## Libraries

External arduino libraries are used to configure the arduino Feather M0.

- Adafruit_NeoPixel.h
- Adafruit_CAP1188.h
- time.h
- cstdlib

## External Interrupts

External interrupts are used to store a user's touch.
The interrupts are triggered when the user has touched the glass interface.

## LED Display Sections

An LED strip will display a pattern through glass panes

The mapping below tells which pixels on the LED strip corresponds to which glass pane
```
pixels 28-30: section 1
pixels 33-35: section 2
pixels 39-40: section 3
pixels  9-11: section 4
pixels   4-6: section 5
pixels   0-1: section 6
```

## How to run and build
We use the [Arduino IDE](https://www.arduino.cc/en/software). There is a `.ino` file in the `/src/` folder which contains the Arduino IDE sketch to run and upload onto the Adafruit Feather.

## Instructions to Use

Plug in arduino to power source
Once powered, the user will see 5 random sections light up, sections can be repeated in the pattern.
The user will try to repeat the pattern by touching glass panes.
If the user succeeds, then a victory pattern will be displayed and a new pattern will be generated.
If the user fails, then a lose pattern will be displayed and a new pattern will be generated.
There is currently only one difficulty level.


