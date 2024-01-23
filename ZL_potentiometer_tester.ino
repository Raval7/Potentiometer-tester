// Potentiometer tester
// Version: 1.2
// By Rafał Mlicki

// Liberies:
#include <ADCTouch.h>
#include <Adafruit_NeoPixel.h>

// Constants:
#define LED_PIN        13
#define NUMBER_OF_LEDS  2

const int potentiometerPin1 = A1;
const int potentiometerPin2 = A2; 
const int relaySwitchDelay = 250;
const int disconnectPotentiometerValue = 1000;
const int highProperValueMax = 250;
const int highProperValueMin = 200;
const int lowProperValueMax = 50;
const int lowProperValueMin = 5;

// Variables:
int mode=1;
int touchPinReference;
int touchPinValue;
int testResultUpperPosition=0;
int testResultLowerPosition=0;
Adafruit_NeoPixel ledStrip(NUMBER_OF_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Main program setup
void setup() {
  Serial.begin(9600);

  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  touchPinReference = ADCTouch.read(A3, 500);

  ledStrip.begin();
}

// Main program loop
void loop() {

  switch (mode) {
    // Potentiometer not connected
    case 1:
      Serial.println("mode=1");
      
      ledStrip.clear();
      ledStrip.setPixelColor(0, ledStrip.Color(150, 150, 0));
      ledStrip.show();

      if (analogRead(potentiometerPin1)<disconnectPotentiometerValue && analogRead(potentiometerPin2)<disconnectPotentiometerValue)
        mode=2;

      break;

    // Potentiometer test sequence
    case 2:
      Serial.println("mode=2");

      if (analogRead(potentiometerPin1)>disconnectPotentiometerValue && analogRead(potentiometerPin2)>disconnectPotentiometerValue) {
        mode=1;
        testResultUpperPosition=0;
        testResultLowerPosition=0;
        break;
      }
      
      ledStrip.clear();
      ledStrip.show();

      delay(1000);

      DriveUp();

      if (analogRead(potentiometerPin1)>highProperValueMin && analogRead(potentiometerPin1)<highProperValueMax && analogRead(potentiometerPin2)>lowProperValueMin && analogRead(potentiometerPin2)<lowProperValueMax ) {
        testResultUpperPosition=1;
      } else {
        testResultUpperPosition=2;
      }

      DriveDown();

      if (analogRead(potentiometerPin2)>highProperValueMin && analogRead(potentiometerPin2)<highProperValueMax && analogRead(potentiometerPin1)>lowProperValueMin && analogRead(potentiometerPin1)<lowProperValueMax ) {
        testResultLowerPosition=1;
      } else {
        testResultLowerPosition=2;
      }

      if (testResultUpperPosition==1 && testResultLowerPosition==1) {
        mode=3;
      }
      if (testResultUpperPosition==2 || testResultLowerPosition==2) {
        mode=4;
      }

      touchPinReference = ADCTouch.read(A3, 500);

      break;
    
    // Potentiometer test passed and touch testing
    case 3:
      Serial.println("mode=3");

      if (analogRead(potentiometerPin1)>disconnectPotentiometerValue && analogRead(potentiometerPin2)>disconnectPotentiometerValue) {
        mode=1;
        testResultUpperPosition=0;
        testResultLowerPosition=0;
        break;
      }

      ledStrip.clear();
      ledStrip.setPixelColor(0, ledStrip.Color(0, 150, 0));
      touchPinValue = ADCTouch.read(A3);
      touchPinValue -= touchPinReference;

      if (touchPinValue > 40) {
        ledStrip.setPixelColor(1, ledStrip.Color(0, 0, 150));
      }

      ledStrip.show();

      break;

    // Potentiometer test not passed
    case 4:
      Serial.println("mode=4");

      if (analogRead(potentiometerPin1)>disconnectPotentiometerValue && analogRead(potentiometerPin2)>disconnectPotentiometerValue) {
        mode=1;
        testResultUpperPosition=0;
        testResultLowerPosition=0;
        break;
      }

      ledStrip.clear();
      ledStrip.setPixelColor(0, ledStrip.Color(150, 0, 0));
      ledStrip.show();
      
      break;

    default:
      Serial.println("default");
  }  
}

// Potentiometer motor drives up
void DriveUp(){
  digitalWrite(11, HIGH);  
  delay(relaySwitchDelay);
  digitalWrite(11, LOW);
  delay(500);  
}

// Potentiometer motor drives down
void DriveDown(){
  digitalWrite(12, HIGH);  
  delay(relaySwitchDelay);
  digitalWrite(12, LOW);
  delay(500);
}
