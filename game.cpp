#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// C++ Stuff
#include <iostream>
#include <vector>

#define TOUCHED 3

#define CAP_1 1
#define CAP_2 2
#define CAP_3 4
#define CAP_4 8
#define CAP_5 16
#define CAP_6 32
#define CAP_7 64
#define CAP_8 128

typedef struct _color {
    u_int8_t red;
    u_int8_t green;
    u_int8_t blue;
} color;

void light(int section_number, color color) {
    // TODO: logic select a section 
    printf("SECTION %d LIT WITH COLOR {r: %d, g: %d, b: %d}\n", section_number, color.red, color.green, color.blue);
}

int main() {
    uint8_t touched = TOUCHED;
      
    // Sense whichever glass piece has been touched
    if (touched & CAP_1) {
       printf("CAP1 SENSED\n");

       light(1, (color){255, 0, 0});
    } 

    if (touched & CAP_2) {
       printf("CAP2 SENSED\n");

       light(1, (color){0, 128, 0});
    } 
}