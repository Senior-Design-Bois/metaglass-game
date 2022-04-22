

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
#define SECTION_1 1
#define SECTION_2 2
#define SECTION_3 3
#define SECTION_SIZE 20
#define SECTION_2_OFFSET 20
#define SECTION_3_OFFSET 40
////////////////////////////

//      Other Defines     //
////////////////////////////
#define MIN 1
#define MAX 4
#define PATTERNSIZE 3
////////////////////////////

//        Globals         //
////////////////////////////
bool hasPatternBeenDisplayed = false;
uint8_t counter = 0;
int userInput[PATTERNSIZE];
int randomPattern[PATTERNSIZE];
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
    
    attachInterrupt(A1, CAP1_INTERRUPT, FALLING);
    attachInterrupt(A2, CAP2_INTERRUPT, FALLING);
    attachInterrupt(A3, CAP3_INTERRUPT, FALLING);

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
          for(i = 0; i < SECTION_SIZE; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_2:
          for(i = SECTION_2_OFFSET; i < SECTION_SIZE+SECTION_2_OFFSET; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_3:
          for(i = SECTION_3_OFFSET; i < SECTION_SIZE+SECTION_3_OFFSET; i++)
          {
              pixels.setPixelColor(i, pixels.Color(OFF));
              pixels.show();
          }
          break;         
    }
}
void off()
{
    for(int i = 0; i < 3; i++)
    {
        turnOffSection(i);
    }
}
void lightSection(uint8_t section)
{
    uint8_t i = 0;
    switch(section)
    {
        case SECTION_1:
          for(i = 0; i < SECTION_SIZE; i++)
          {
              pixels.setPixelColor(i, pixels.Color(RED));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_2:
          for(i = SECTION_2_OFFSET; i < SECTION_SIZE+SECTION_2_OFFSET; i++)
          {
              pixels.setPixelColor(i, pixels.Color(WHITE));
              pixels.show();   // Send the updated pixel colors to the hardware.
          }
          break;

        case SECTION_3:
          for(i = SECTION_3_OFFSET; i < SECTION_SIZE+SECTION_3_OFFSET; i++)
          {
              pixels.setPixelColor(i, pixels.Color(GREEN));
              pixels.show();
          }
          break;         
    }
    delay(500);
    turnOffSection(section);
}

void pattern(int randomPattern[])
{
    for(int i = 0; i < PATTERNSIZE; i++)
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
    }
    delay(50);
}

void CAP1_INTERRUPT()
{
//    Serial.println("Cap1 touched");
    userInput[counter] = CAP1;
    counter++;
}

void CAP2_INTERRUPT()
{
//    Serial.println("Cap2 touched");
    userInput[counter] = CAP2;
    counter++;  
}

void CAP3_INTERRUPT()
{
//    Serial.println("Cap3 touched");
    userInput[counter] = CAP3;
    counter++;
}