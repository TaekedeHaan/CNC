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

volatile long encoderPosNow = 0;
long encoderPosPrev = 0;

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  attachInterrupt(0, readEncoderA, CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, readEncoderB, CHANGE);  // encoder pin on interrupt 0 - pin 2
  Serial.begin (9600);
  Serial.println("start");                // a personal quirk

}

void loop(){
  if (encoderPosNow != encoderPosPrev){
    encoderPosPrev = encoderPosNow;
    Serial.println (encoderPosNow);
  }
}
