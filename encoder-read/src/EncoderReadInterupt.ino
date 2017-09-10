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

volatile unsigned int encoderPosNow = 0;
unsigned int encoderPosPrev = 0;

int input;
int encoderAB;

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

void readEncoderA(){

  input = PIND;                   //Read out the D register
  encoderAB = input & B00001100;  //We are only interested in Pin2 and Pin3
  encoderAB = encoderAB >> 2;     //Bitshift by two to remove pin1 and pin3

  switch(encoderAB){
    case B11: encoderPosNow++;
    break;
    case B10: encoderPosNow--;
    break;
    case B01: encoderPosNow--;
    break;
    case B00: encoderPosNow++;
    break;
    default: Serial.println("Enocder read error, how could you even fuck this up?");
    break;
  }
}

void readEncoderB(){

  input = PIND;                   //Read out the D register
  encoderAB = input & B00001100;  //We are only interested in Pin2 and Pin3
  encoderAB = encoderAB >> 2;     //Bitshift by two to remove pin1 and pin3

  switch(encoderAB) {
    case B11: encoderPosNow--;
    break;
    case B10: encoderPosNow++;
    break;
    case B01: encoderPosNow++;
    break;
    case B00: encoderPosNow--;
    break;
    default: Serial.println("Enocder read error, how could you even fuck this up?");
    break;
  }
}
