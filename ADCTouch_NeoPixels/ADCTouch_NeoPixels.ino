#include <ADCTouch.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  #include <avr/power.h>
#endif

// Pin Definitions
#define NEOPIN 6
#define PIXELS 6
#define TEST_LED 9

#define ADC_BASE A0
#define ADC_READ A1
#define TOGGLE 40 // this is the yes-no threshold for touch

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

int FadeRate = 50;
int MaxBrightness = 60; // this could be the value0
int TotalSteps = 5;

// Values for ADCTouch
int ref0, ref1;       //reference values to remove offset
int pixelTotal;

void setup()
{ 
  // Initialize all pixels to a Random Color.
    strip.begin();
    for (int i = 0; i < PIXELS; i++) {
      strip.setPixelColor(i, random(0, MaxBrightness), random(0, MaxBrightness), random(0, 50));
    }
    strip.show();

  // Turn on the serial port
    Serial.begin(9600);

  // Turn on ADCTouch and get base values
    ref0 = ADCTouch.read(ADC_BASE, 500);    //create reference values to
    ref1 = ADCTouch.read(ADC_READ, 500);      //account for the capacitance of the pad

    pixelTotal = strip.numPixels();
  // Set up the TEST_LED
    pinMode(TEST_LED, OUTPUT);
}
 
void loop()
{
    int value0 = ADCTouch.read(ADC_BASE);   // no second parameter
    int value1 = ADCTouch.read(ADC_READ);   // --> 100 samples
    value0 -= ref0;       // remove offset
    value1 -= ref1;       // remove offset

    bool v0touched = value0 > TOGGLE;
    bool v1touched = value1 > TOGGLE;

    int selectedPixel= constrain(value1, 0, PIXELS+1);
    
    //sensorValue, sensorMin, sensorMax, 0, 255)
//    int brightness = constrain(value1, 0, 600);
//    brightness = map(brightness, 0, 600, 0, 255);
   
// Write code below here
    // wipe pixels
     for(int i=0; i<pixelTotal+1; i++) {
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
    }
     
    // set selectedPixel to random colour
    for(int i=0; i<selectedPixel; i++) {
      setRandomColour(i);
    }

    // print report to serial
    Serial.print("=========== \n");
    Serial.print("| v0 ");
    Serial.print(value0);
    Serial.print(" ");
    Serial.print(v0touched);
    Serial.print(" | v1 ");
    Serial.print(value1);
    Serial.print(v1touched);
    Serial.print(" | selectPix ");
    Serial.println(selectedPixel);
    Serial.print("\n");

    // TODO: 
    // Hook up TestLED
    // Hook up NeoPixels to brightness
    // Turn on NeoPixels
    
    delay(100);
}

void wipePixels(int LEDNumber) {
  uint8_t LED = LEDNumber;  // Pick a random pixel
  strip.setPixelColor(LED, 0, 0, 0);
  strip.show();
  delay(FadeRate);
}

void setPixelColour(int LEDNumber, int brightness) {
  uint8_t LED = LEDNumber;  // Pick a random pixel

  // Get the colors of the current pixel.
  uint32_t c = strip.getPixelColor(LED);
  uint8_t  redCur = (c >> 16) & 0xFF;
  uint8_t  greenCur = (c >>  8) & 0xFF;
  uint8_t  blueCur = c & 0xFF;
  
  //Pick a new random color
  uint8_t redNew = random(0, brightness);
  uint8_t greenNew = random(0, brightness);
  uint8_t blueNew = random(0, brightness);

    // Start fade between the two colors
  for (int i = 1; i < TotalSteps; i++)
  {
    uint8_t red = (((redCur * (TotalSteps - i)) + (redNew * i)) / TotalSteps);
    uint8_t green = (((greenCur * (TotalSteps - i)) + (greenNew * i)) / TotalSteps);
    uint8_t blue = (((blueCur * (TotalSteps - i)) + (blueNew * i)) / TotalSteps);
    // Sets the pixel to the color adjusted in the fade
    strip.setPixelColor(LED, red, green, blue/2);
    strip.show();
  }
  delay(10);
}

void setRandomColour(int LEDNumber) {
  uint8_t LED = LEDNumber;  // Pick a random pixel

  // Get the colors of the current pixel.
  uint32_t c = strip.getPixelColor(LED);
  uint8_t  redCur = (c >> 16) & 0xFF;
  uint8_t  greenCur = (c >>  8) & 0xFF;
  uint8_t  blueCur = c & 0xFF;
  
  //Pick a new random color
  uint8_t redNew = random(0, MaxBrightness);
  uint8_t greenNew = random(0, MaxBrightness);
  uint8_t blueNew = random(0, 60);

    // Start fade between the two colors
  for (int i = 1; i < TotalSteps; i++)
  {
    uint8_t red = (((redCur * (TotalSteps - i)) + (redNew * i)) / TotalSteps);
    uint8_t green = (((greenCur * (TotalSteps - i)) + (greenNew * i)) / TotalSteps);
    uint8_t blue = (((blueCur * (TotalSteps - i)) + (blueNew * i)) / TotalSteps);
    // Sets the pixel to the color adjusted in the fade
    strip.setPixelColor(LED, red, green, blue/2);
    strip.show();
  }
  delay(10);
}


