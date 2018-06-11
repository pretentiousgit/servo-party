#include <ADCTouch.h>
#include <Servo.h>

#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  #include <avr/power.h>
#endif

// Pin Definitions
#define ADC_BASE A0
#define ADC_READ A1
#define TOGGLE 40 // this is the yes-no threshold for touch

#define ServoPin 9
#define TEST_LED 10

// Values for ADCTouch
int ref0, ref1;       //reference values to remove offset
Servo towerServo;

void setup()
{ 
  // Turn on the serial port
    Serial.begin(9600);

  // Turn on ADCTouch and get base values
    ref0 = ADCTouch.read(ADC_BASE, 500);    //create reference values to
    ref1 = ADCTouch.read(ADC_READ, 500);    //account for the capacitance of the pad
    
  // Attach your servo
    towerServo.attach(ServoPin);

   // Set up the TEST_LED
    pinMode(TEST_LED, OUTPUT);
}

void loop() {
    int value0 = ADCTouch.read(ADC_BASE);   // no second parameter
    int value1 = ADCTouch.read(ADC_READ);   // --> 100 samples
    value0 -= ref0;       // remove offset
    value1 -= ref1;       // remove offset

    bool v0touched = value0 > TOGGLE;
    bool v1touched = value1 > TOGGLE;

    int constrainedValue = constrain(value1, 0, 180);
    String stringOne = "constrainedVal: ";
    String report = stringOne + constrainedValue;
    
    int servoVal = map(constrainedValue, 0, 180, 0, 180);
    int ledVal = map(constrainedValue, 0, 180, 150, 255);
    
// Write code below here
    towerServo.write(servoVal);
    analogWrite(TEST_LED, 255);

// print report to serial
    Serial.print(report);
    Serial.print(" | v1 ");
    Serial.print(value1);
    Serial.print(v1touched);
    Serial.print(" | servoVal: ");
    Serial.print(servoVal);
    Serial.print(" | LEDVal: ");
    Serial.print(ledVal);
    Serial.print("\n");

    delay(15);
}


