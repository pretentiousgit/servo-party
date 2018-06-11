#include <ADCTouch.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

// Values for ADCTouch
int ref0, ref1;       //reference values to remove offset

#define TESTLED 9
#define ADC_BASE A0
#define ADC_READ A1
#define TOGGLE 40

void setup()
{ 
    Serial.begin(9600);
    ref0 = ADCTouch.read(ADC_BASE, 500);    //create reference values to
    ref1 = ADCTouch.read(ADC_READ, 500);      //account for the capacitance of the pad
}

void loop() 
{
    int value0 = ADCTouch.read(ADC_BASE);   //no second parameter
    int value1 = ADCTouch.read(ADC_READ);   //   --> 100 samples

    value0 -= ref0;       //remove offset
    value1 -= ref1;

    bool v0touched = value0 > TOGGLE;
    bool v1touched = value1 > TOGGLE;

    Serial.print("Start: v0touched ");
    Serial.print(v0touched);    //send (boolean) pressed or not pressed
    Serial.print("\t ");           //use if(value > threshold) to get the state of a button

    Serial.print("v1touched ");
    Serial.print(v1touched);
    Serial.print("\t\t");

    Serial.print("value0/1 raw ");
    Serial.print(value0);             //send actual reading
    Serial.print("\t");
    Serial.println(value1);

   if(v1touched){
    analogWrite(TESTLED, 255);
    //fadeIn();
   } else { 
    analogWrite(TESTLED, 0);
    //fadeOut();
   }
    delay(100);
}


void fadeIn(){ 
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(TESTLED, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

}
void fadeOut(){ 

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(TESTLED, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}

