

/*************************************************** 
  This is a library for the CAP1188 I2C/SPI 8-chan Capacitive Sensor

  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <time.h>
#include <Adafruit_CAP1188.h>
#include <iostream>
#include <cstdlib>
using namespace std;

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9

// CS pin is used for software or hardware SPI
#define CAP1188_CS  10

// These are defined for software SPI, for hardware SPI, check your 
// board's SPI pins in the Arduino documentation
#define CAP1188_MOSI  11
#define CAP1188_MISO  12
#define CAP1188_CLK  13

// For I2C, connect SDA to your Arduino's SDA pin, SCL to SCL pin
// On UNO/Duemilanove/etc, SDA == Analog 4, SCL == Analog 5
// On Leonardo/Micro, SDA == Digital 2, SCL == Digital 3
// On Mega/ADK/Due, SDA == Digital 20, SCL == Digital 21

// Use I2C, no reset pin!
Adafruit_CAP1188 cap = Adafruit_CAP1188();

// Or...Use I2C, with reset pin
//Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

// Or... Hardware SPI, CS pin & reset pin 
// Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_CS, CAP1188_RESET);

// Or.. Software SPI: clock, miso, mosi, cs, reset
//Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_CLK, CAP1188_MISO, CAP1188_MOSI, CAP1188_CS, CAP1188_RESET);

#define PIN 12
#define NUMPIXELS 60

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


//Capacitive Touch Defines//
////////////////////////////
#define CAP1 1
#define CAP2 2
#define CAP3 3
#define CAP4 4
#define CAP5 5
#define CAP6 6
#define CAP7 7
#define CAP8 8
////////////////////////////

//        Colors          //
////////////////////////////
#define OFF 0,0,0
#define RED 255,0,0
#define ORANGE 255,128,0
#define YELLOW 255,255,0
#define GREEN 0,255,0
#define BLUE 0,0,255
#define PURPLE 127,0,255
#define WHITE 255,255,255
////////////////////////////

//     Light Sections     //
////////////////////////////
//pixels 28-30: section 1
//pixels 33-35: section 2
//pixels 39-40: section 3
//pixels  9-11: section 4
//pixels   4-6: section 5
//pixels   0-1: section 6
#define SECTION_1 1
#define SECTION_2 2
#define SECTION_3 3
#define SECTION_4 4
#define SECTION_5 5
#define SECTION_6 6
#define SECTION_1_START 28
#define SECTION_2_START 33
#define SECTION_3_START 39
#define SECTION_4_START  9
#define SECTION_5_START  4
#define SECTION_6_START  0
#define SECTION_1_END   30
#define SECTION_2_END   35
#define SECTION_3_END   39
#define SECTION_4_END   11
#define SECTION_5_END    6
#define SECTION_6_END    1
#define SECTION_SIZE_OF_2 2
#define SECTION_SIZE_OF_3 3
////////////////////////////

//      Other Defines     //
////////////////////////////
#define MIN 1
#define MAX 7
#define PATTERNSIZE 5
////////////////////////////

//        Globals         //
////////////////////////////
bool hasPatternBeenDisplayed = false;
uint8_t counter = 0;
uint16_t decrementer = 0;
uint16_t userInput[PATTERNSIZE];
uint16_t randomPattern[PATTERNSIZE];
////////////////////////////
void setup() {
    Serial.begin(9600);
    Serial.println("CAP1188 test!");
    Wire.begin();
    pixels.begin();
    // Initialize the sensor, if using i2c you can pass in the i2c address
    // if (!cap.begin(0x28)) {
    if (!cap.begin(0x28)) {
      Serial.println("CAP1188 not found");
      while (true);
    }
    Serial.println("CAP1188 found!");

//Feather M0 External Interrupts//
/////////////////////////////////
    pinMode(digitalPinToInterrupt(A1), INPUT_PULLUP);
    pinMode(digitalPinToInterrupt(A2), INPUT_PULLUP);
    pinMode(digitalPinToInterrupt(A3), INPUT_PULLUP);
    pinMode(digitalPinToInterrupt(A4), INPUT_PULLUP);
    pinMode(digitalPinToInterrupt(A5), INPUT_PULLUP);
    pinMode(digitalPinToInterrupt(SCK), INPUT_PULLUP);
    
    attachInterrupt(A1, CAP1_INTERRUPT, FALLING);
    attachInterrupt(A2, CAP2_INTERRUPT, FALLING);
    attachInterrupt(A3, CAP3_INTERRUPT, FALLING);
    attachInterrupt(A4, CAP4_INTERRUPT, FALLING);
    attachInterrupt(A5, CAP5_INTERRUPT, FALLING);
    attachInterrupt(SCK, CAP6_INTERRUPT, FALLING);
////////////////////////////////
randomSeed(analogRead(9));
//CAP1188 Sensitivity Settings//
///////////////////////////////
    Wire.beginTransmission(0x28);
    Wire.write(0x1f);
    Wire.write(0x5<<4);
    Wire.endTransmission();
}
///////////////////////////////

void clearGlobals()
{
    for(int i = 0; i < PATTERNSIZE; i++)
    {
        userInput[i] = 0;
        randomPattern[i] = 0;
    }
}

void turnOffSection(uint8_t section)
{
        uint8_t i = 0;
    switch(section)
    {
        case SECTION_1:
          for(i = SECTION_1_START; i < SECTION_1_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_2:
          for(i = SECTION_2_START; i < SECTION_2_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_3:
          for(i = SECTION_3_START; i < SECTION_3_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();
          }
          break;    
        case SECTION_4:
          for(i = SECTION_4_START; i < SECTION_4_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_5:
          for(i = SECTION_5_START; i < SECTION_5_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_6:
          for(i = SECTION_6_START; i < SECTION_6_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();
          }
          break;     
    }
}
void off()
{
    for(int i = 0; i < NUMPIXELS; i++)
    {
         pixels.setPixelColor(i, pixels.Color(OFF));
         pixels.show();
    }
}
void lightSection(uint8_t section)
{
    uint8_t i = 0;
    switch(section)
    {
        case SECTION_1:
          for(i = SECTION_1_START; i < SECTION_1_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(RED));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_2:
          for(i = SECTION_2_START; i < SECTION_2_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(BLUE));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_3:
          for(i = i = SECTION_3_START; i < SECTION_3_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(GREEN));
              pixels.show();
          }
          break;  
        case SECTION_4:
          for(i = SECTION_4_START; i < SECTION_4_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(YELLOW));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_5:
          for(i = SECTION_5_START; i < SECTION_5_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(PURPLE));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_6:
          for(i = i = SECTION_6_START; i < SECTION_6_END+1; i++)
          {
              pixels.setPixelColor(i, pixels.Color(ORANGE));
              pixels.show();
          }
          break;       
    }
    delay(500);
    turnOffSection(section);
}

void pattern(uint16_t randomPattern[])
{
    for(uint8_t i = 0; i < PATTERNSIZE; i++)
    {
        lightSection(randomPattern[i]);
        delay(500);
    }
    hasPatternBeenDisplayed = true;
}

void levelWon()
{
    lightSection(1);
    delay(50);
    lightSection(2);
    delay(50);
    lightSection(3);
    delay(50);
    off();
    lightSection(1);
    lightSection(2);
    lightSection(3);
    delay(1500); //display level pattern for 3 seconds 
    off();
    delay(50);
}
void gameLost()
{
    uint8_t i;
    for(i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(RED));
        pixels.show();     
    }
    delay(50);
    off();
    for(i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(RED));
        pixels.show();     
    }
    delay(50);
    off();

    for(i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(RED));
        pixels.show();     
    }
    delay(50);
    off();
}
bool isCorrectInput()
{
    uint8_t i = 0;
    bool ok = false;
    for(int i = 0; i < PATTERNSIZE; i++)
    {
        ok = userInput[i] == randomPattern[i];
        if(!ok)
            return ok;
    }
    return ok;
}
/*  TODO INTEGRATE SIMON SAYS GAME WITH HARDWARE:
*   LOGIC: So once we get the first level's random pattern, we need to pass the stack of values
        into the pattern function. When the pattern finishes the "hasPatternBeenDisplayed" flag 
        will be set, indicating not to run the pattern again. Next we will compare the "touched" 
        variable in the "glassIsTouched" function with whatever is in the stack. If everything 
        is "ok" then we will go to the next level. If everything is not "ok" then the user will
        lose.
*/
void loop() {

    long tmp8;
    if(!hasPatternBeenDisplayed)
    {
        for(int i = 0; i < PATTERNSIZE; i++)
        {    
            randomSeed(analogRead(A0));       
            tmp8 = random(MIN, MAX);
            randomPattern[i] = tmp8;
        }
        pattern(randomPattern);
        delay(50);
    }
    
    if(counter == PATTERNSIZE)
    {
        if(isCorrectInput())
        {
            clearGlobals();
            levelWon();
            hasPatternBeenDisplayed = false;
            counter = 0; //reset counter     
        }
        else
        {
            clearGlobals();
            gameLost();
            hasPatternBeenDisplayed = false;
            counter = 0;
            decrementer = 0;
        }
    }
    delay(50);
}

void CAP1_INTERRUPT()
{
    Serial.println("Cap1 touched");
    lightSection(1);
    userInput[counter] = CAP1;
    counter++;
}

void CAP2_INTERRUPT()
{
    Serial.println("Cap2 touched");
    lightSection(2);
    userInput[counter] = CAP2;
    counter++;  
}

void CAP3_INTERRUPT()
{
    Serial.println("Cap3 touched");
    lightSection(3);
    userInput[counter] = CAP3;
    counter++;
}

void CAP4_INTERRUPT()
{
    Serial.println("Cap4 touched");
    lightSection(4);
    userInput[counter] = CAP4;
    counter++;
}

void CAP5_INTERRUPT()
{
    Serial.println("Cap5 touched");
    lightSection(5);
    userInput[counter] = CAP5;
    counter++;
}

void CAP6_INTERRUPT()
{
    Serial.println("Cap6 touched");
    lightSection(6);
    userInput[counter] = CAP6;
    counter++;
}
