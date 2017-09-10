/* read a rotary encoder with interrupts
   Encoder hooked up with common to GROUND,
   encoderPinA to pin 2, encoderPinB to pin 4 (or pin 3 see below)
   it doesn't matter which encoder pin you use for A or B

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs
*/
#include "Arduino.h"

#define encoderPinA  2 //Red A-phase
#define encoderPinB  3 //Green B-pase

int RPWM_Output = 9; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 10; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

int rightPWM;
int leftPWM;

unsigned int encoderPosPrev = 0;
volatile unsigned int encoderPosNow = 0;

unsigned int maximumValue = 65535;

void setup() {
  
  // Driver pins.
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);

  // Encoder pins.
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(0, readEncoderA, CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, readEncoderB, CHANGE);  // encoder pin on interrupt 0 - pin 2

  Serial.begin (9600);
  Serial.println("start");                // a personal quirk
}

void loop()
{
  if (encoderPosNow != encoderPosPrev){
    encoderPosPrev = encoderPosNow;
    Serial.println (encoderPosNow);
  }


  // sensor value is in the range 0 to 1023
  // the lower half of it we use for reverse rotation; the upper half for forward rotation
  if (encoderPosNow > round(maximumValue/2))
  {
    // reverse rotation
    rightPWM = map(encoderPosNow, maximumValue/2, maximumValue, 255, 0)*20;
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, rightPWM);
  }
  else
  {
    // forward rotation
    leftPWM = map(encoderPosNow, 0, maximumValue/2, 0, 255)*20;
    
    analogWrite(LPWM_Output, leftPWM);
    analogWrite(RPWM_Output, 0);
  }

}

