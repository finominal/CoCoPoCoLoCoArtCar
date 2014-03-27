//#include <Adafruit_NeoPixel.h>



#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <BeaconEntity.h>

#define p(x) Serial.print(x)
#define pl(x) Serial.println(x)

const bool DEV = true;

const int dataPin  = 6;    // Yellow wire on Adafruit Pixels
const int clockPin = 7;    // Green wire on Adafruit Pixels
int brightness = 255; //0-255


const int countEntities = 3;//dont change this
const int StripLength = 50;
volatile int entitySize = 8;


Entity ENTITIES[countEntities];

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(StripLength, dataPin, clockPin);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(StripLength, PIN, NEO_GRB + NEO_KHZ400);

uint32_t pixels[StripLength];

const uint32_t red = Color(255,0,0);
const uint32_t green = Color(0,255,0);
const uint32_t blue = Color(0,0,255);
const uint32_t purple = Color(128,0,128);
const uint32_t yellow = Color(128,128,0);
const uint32_t black = Color(0,0,0);


void setup() {
  
  Serial.begin(115200);
  pl("*** ARDUINO RESTART ***");

  InitializeEntities();
  InitializeStrip();
  
}

void loop() {
    
  ThreeEntities(1000,30, 14); //itterations, wait, size
  Random(200, 200); //itterations, wait
  RainbowCycle(10, 20);
  ColorWipes(10);
  ThreeEntities(3000,5,14);
  Cycle(100);
  
}


void InitializeStrip()
{
  strip.begin();
  strip.show();

}

void ColorWipes(int waitMs)
{

  colorWipe(red, waitMs);
    delay(100);
  colorWipe(black, waitMs);
    delay(100);
  colorWipe(green, waitMs);
    delay(100);
  colorWipe(black, waitMs);
    delay(100);
  colorWipe(blue, waitMs);
    delay(100);
  colorWipe(black, waitMs);
    delay(100);
  colorWipe(purple, waitMs);
    delay(100);
  colorWipe(black, waitMs);
    delay(100);
}


void Random(int itterations, int wait)
{
  ShowBlack();
  int counter = 0;
  int pixel;
  while(counter < itterations)
  {
   SetAllOneColor(Color(32,0,32));
   
   pixel = random(0,StripLength-2); 
   
    //pick a random a random led and flash it
    switch(random(0,4))
    {
    case 0:
      PulsePixel(pixel, red, wait);
      break;
    case 1:
      PulsePixel(pixel, green, wait);
      break;
    case 2: 
      PulsePixel(pixel, blue, wait);
      break;
    case 3:  
     PulsePixel(pixel, purple, wait);
     break;
    case 4:
      PulsePixel(pixel, yellow, wait);
      break;
    }
    counter++;
  }
}

void SetAllOneColor(uint32_t color)
{
  for(int i = 0;i<StripLength;i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void PulsePixel(int pixel, uint32_t color, int wait)
{
    strip.setPixelColor(pixel, HalfColor(color));
    strip.setPixelColor(pixel+1, HalfColor(color));
    strip.show();
    delay(wait/2); 
    
    strip.setPixelColor(pixel, color);
    strip.setPixelColor(pixel+1, color);
    strip.show();
    delay(wait/2); 
    
    strip.setPixelColor(pixel, HalfColor(color));
    strip.setPixelColor(pixel+1, HalfColor(color));
    strip.show();
    delay(wait/2); 
}


// Create a 24 bit color value from R,G,B
uint32_t HalfColor(uint32_t color)
{
  uint8_t r, g, b;
  b = (color & 0x0000FF) /5;
  color >>= 8;
  g = (color & 0x0000FF) /5;
  color >>= 8;
  r = (color & 0x0000FF) /5;
  return Color(r, g, b);
}

void RainbowCycle(int itterations, uint8_t wait) 
{
  int counter = 0;
  
  while(counter < itterations)
  {
    int i, j;
  
    for (j=0; j < 256 * 5; j++) 
    {     // 5 cycles of all 25 colors in the wheel
      for (i=0; i < strip.numPixels(); i++) 
      {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
        strip.setPixelColor(i, WheelHalfBrightness( ((i * 256 / strip.numPixels()) + j) % 256) );
      }  
      strip.show();   // write all the pixels out
      delay(wait);
    }
    counter++;
  }
}

void SetBrightness()
{
  ENTITIES[0].color = Color(brightness,0,0); //red
  ENTITIES[0].colorHalf = Color(brightness/3,0,0); //red
  ENTITIES[2].color = Color(0,0,brightness); //blue
  ENTITIES[2].colorHalf = Color(0,0,brightness/3); //blue
  ENTITIES[1].color = Color(0,brightness,0); //green
  ENTITIES[1].colorHalf = Color(0,brightness/3,0); //green
}

void InitializeEntities()
{  
  //pl("Initialize Entities");
  SetBrightness();
  ENTITIES[0].direction = up;  
  ENTITIES[0].position = 0;
  
  ENTITIES[1].direction = up;  
  ENTITIES[1].position = StripLength*2/3*2*-1;
  
  ENTITIES[2].direction = up;  
  ENTITIES[2].position = StripLength*2/3*2*2*-1;  
}

void ThreeEntities(int itterations, int wait, int entSize)
{
  //  pl();
  //  pl("ThreeEntities");
  entitySize = entSize;
  
  int counter = 0;
  while(counter < itterations)
  {
    displayEntities();
    moveEntities();
    delay(wait);
    counter++;
  }
}

void displayEntities()
{
  clearLedBuffer();
  processEntitiesToLEDBuffer();
  pushPixelsToStrip();
}

void clearLedBuffer()
{
  //pl("  clearLedBuffer");
  for(int i = 0;i<StripLength;i++)
  {
    pixels[i] = 0;
  }
}

void processEntitiesToLEDBuffer()
{
  //pl("  processEntitiesToLEDBuffer");
  for(int i = 0; i < countEntities; i++)//for each entity
  {
    if(ENTITIES[i].position>=0)//check if position is in frame yet
    {
      if(ENTITIES[i].halfStep)
      {
        if(ENTITIES[i].direction == up)
        {
          pixels[ENTITIES[i].position-1] |= ENTITIES[i].colorHalf; //half fade the first pixel
          //if(DEV) pl(ENTITIES[i].color);
          //if(DEV) pl(pixels[ENTITIES[i].position-1]);
          for(int j = 0; j < entitySize-1; j++)//full brightness for the middle pixels
          {
            pixels[ENTITIES[i].position+j] |= ENTITIES[i].color;
          }
          pixels[ENTITIES[i].position+entitySize-1] |= ENTITIES[i].colorHalf; //half fade the last pixel
          ENTITIES[i].halfStep = false;//next step will NOT a half step
        }
        else //if(ENTITIES[i].direction == down)
        {
          pixels[ENTITIES[i].position] |= ENTITIES[i].colorHalf; //half fade the first pixel
          for(int j = 1; j < entitySize; j++)//full brightness for the middle pixels
          {
            pixels[ENTITIES[i].position+j] |= ENTITIES[i].color;
          }
          pixels[ENTITIES[i].position+entitySize] |= ENTITIES[i].colorHalf; //half fade the last pixel
          ENTITIES[i].halfStep = false;//next step will NOT be a half step
        }
      }
      else //not half step, display full unit
      {
        for(int j = 0; j < entitySize; j++)//for entity size (each pixel)
        {
          pixels[ENTITIES[i].position+j] |= ENTITIES[i].color;
        }
        ENTITIES[i].halfStep = true;//next step WILL be a half step        
      }
    }
  }
  //if(DEV) showPixels();
}

void ShowBlack()

{
  //pl("  pushPixelsToStrip");
  for(int i = 0; i<StripLength; i++)
  {
    strip.setPixelColor(i,0);
  }
    strip.show();
}

void pushPixelsToStrip()
{
  //pl("  pushPixelsToStrip");
  for(int i = 0; i<StripLength; i++)
  {
    strip.setPixelColor(i,pixels[i]);
  }
    strip.show();
}

void showPixels()
{
  //pl("ShowPixels");
  for(int i = 0; i<StripLength; i++)
  {
    pl(pixels[i]);
  }
}


void moveEntities()
{
  //pl("  moveEntities");
  for(int i = 0;i<countEntities;i++)
  {    
   if(ENTITIES[i].position <0){ENTITIES[i].position++;}//startup, enter from off screen

   else if(ENTITIES[i].halfStep == true) //only move entities that have completed a half step(false = complete)
    {
     if( ENTITIES[i].direction == up)
      {
         if(ENTITIES[i].position == StripLength - entitySize)
         {
           ENTITIES[i].direction = down;
           ENTITIES[i].position--;
         }
         else
         {
           ENTITIES[i].position++;
         }
      }
      else //diretion is down
      {
         if(ENTITIES[i].position == 0)
         {
           ENTITIES[i].direction = up;
           ENTITIES[i].position++;
         }
         else
         {
           ENTITIES[i].position--;
         }
       }
    }//end if(ENTITIES[i].halfStep == true)
   }//end for each entity
   //if(DEV)showEntityPositions();
}

void showEntityPositions()
{
  p("  ~Entity 0 Position: ");
  pl(ENTITIES[0].position);
  
  p("  ~Entity 1 Position: ");
  pl(ENTITIES[1].position);
  
  p("  ~Entity 2 Position: ");
  pl(ENTITIES[2].position);
}




// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}



/* Helper functions */

void Cycle(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 2560; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, WheelHalfBrightness( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}



/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3 , 255 - WheelPos * 3 );
  }
}


uint32_t WheelHalfBrightness(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color((WheelPos * 3)/2, (255 - WheelPos * 3)/2, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color((255 - WheelPos * 3)/2, 0, (WheelPos * 3)/2);
  } else {
   WheelPos -= 170; 
   return Color(0, (WheelPos * 3 )/2, (255 - WheelPos * 3)/2 );
  }
}


