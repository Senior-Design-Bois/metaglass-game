# Interactive Simon Says Game

## Version 1.0.0

To compile / build

Make sure you have Cmake

## To Run

```shell
./run.sh
```

If you don't have enough permissions:

```shell
chmod +x ./run.sh
```

## Manual Build / Run

```shell
cmake -S . -B build
cmake --build build
```

To run, make sure you're inside of the build directory.

```shell
./build/Game
```

# Purpose

This code is for configuring a system that allows a user to play simon says using a glass interface.

## Libraries

External arduino libraries are used to configure the arduino Feather M0.

## External Interrupts

External interrupts are used to be able to store a user's touch.
The interrupts are triggered when the user has touched glass.

## LED Display Sections

An LED strip will display a pattern through glass panes

The mapping below tells which pixels on the LED strip corresponds to which glass pane

pixels 28-30: section 1
pixels 33-35: section 2
pixels 39-40: section 3
pixels  9-11: section 4
pixels   4-6: section 5
pixels   0-1: section 6

## Instructions

Plug in arduino to power source
Once powered, the user will see 5 random sections light up, sections can be repeated in the pattern.
The user will try to repeat the pattern by touching glass panes.
If the user succeeds, then a victory pattern will be displayed and a new pattern will be generated.
If the user fails, then a lose pattern will be displayed and a new pattern will be generated.
There is currently only one difficulty level.


