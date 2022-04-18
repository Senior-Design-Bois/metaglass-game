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
#include <Adafruit_CAP1188.h>
#include <iostream>
#include <vector>
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

////////////////////////////

//        Globals         //
////////////////////////////
bool hasPatternBeenDisplayed = false;
uint8_t counter = 0;
vector<uint8_t> userInput;
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


    Wire.beginTransmission(0x28);
    Wire.write(0x1f);
    Wire.write(0x5<<4);
    Wire.endTransmission();
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
}

void pattern(vector<uint8_t> randomPattern)
{
    for(int i = 0; i < randomPattern.size(); i++)
    {
        lightSection(i+1);
        delay(1000);
    }
    hasPatternBeenDisplayed = true;
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void levelWon(uint8_t wait)
{
    pixels.begin();
    pixels.setBrightness(50);
    pixels.show(); // Initialize all pixels to 'off'
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
        for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
            pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        pixels.show();

        delay(wait);

        for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
            pixels.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

uint8_t getUserInput()
{
    uint8_t touched = cap.touched();
    vector<uint8_t> input;
    if(touched > 0)
    {
        userInput.push_back(touched);
        return input[counter];
        counter++;
    }
    return 0;
}

bool isCorrectInput(vector<uint8_t> randomPattern)
{
    uint8_t i = 0;
    bool ok = false;
    for(int i = 0; i < randomPattern.size(); i++)
    {
        ok = userInput[i] & (1 << randomPattern[i]);
        if(!ok)
            return ok;
    }
    return ok;
    //pattern();
    // if(touched & (1 << CAP1-1)) 
    // {
    //     lightSection(1); //lighting section 1
    // }
    // else if(!(touched & (1 << CAP1-1)))
    // {
    //     turnOffSection(1);
    // }

    // if(touched & (1 << CAP2-1)) 
    // {
    //     lightSection(2);
    // }
    // else if(!(touched & (1 << CAP2-1)))
    // {
    //     turnOffSection(2);
    // }

    // if(touched & (1 << CAP3-1))
    // {
    //     lightSection(3);
    // } 
    // else if(!(touched & (1 << CAP3-1)))
    // {
    //     turnOffSection(3);
    // }
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
    vector<uint8_t> test;
    for(int i = 0; i < 3; i++)
    {
        test.push_back(rand() % 3);
    }
    if(!hasPatternBeenDisplayed)
        pattern(test);

    userInput[counter] = getUserInput();
    if(userInput.size() == test.size())
    {
        if(isCorrectInput(test))
        {
            levelWon(50);
            hasPatternBeenDisplayed = false;
            delay(3000); //display level pattern for 3 seconds       
        }
    }
    delay(50);
}
   