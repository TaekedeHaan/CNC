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

int encoderPosPrev = 0;
volatile int encoderPosNow = 0;

int encoderPositionMinimum;
int encoderPositionMaximum;

unsigned int actualSameEncoderRead;
unsigned int requiredSameEncoderRead = 100;

unsigned int maximumValue = 2147483648;

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
  find_position_minimum();
  find_position_maximum();
}

void loop()
{
  //if (encoderPosNow != encoderPosPrev){
  //  encoderPosPrev = encoderPosNow;
  //  Serial.println (encoderPosNow);
  //}
  // sensor value is in the range 0 to 1023
  // the lower half of it we use for reverse rotation; the upper half for forward rotation
  /*
  if (encoderPosNow < 0)
  {
    // reverse rotation
    rightPWM = map(encoderPosNow, 0, 60000, 0, 255);
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, rightPWM);
  }
  else
  {
    // forward rotation
    leftPWM = map(encoderPosNow, -60000, 0, 255, 0);

    analogWrite(LPWM_Output, leftPWM);
    analogWrite(RPWM_Output, 0);
  }
  */
}


void find_position_minimum(){
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 11);
  Serial.println ("Searching for minimum");
  actualSameEncoderRead = 0;
  while (true)
  {
    if (abs(encoderPosNow - encoderPosPrev) <= 2)
    {
      actualSameEncoderRead = actualSameEncoderRead + 1;
      Serial.println (actualSameEncoderRead);
    }
    else
    {
      actualSameEncoderRead = 0;
    }
    if (actualSameEncoderRead > requiredSameEncoderRead)
    {
      encoderPositionMinimum = encoderPosNow;
      Serial.println (encoderPositionMinimum);
      analogWrite(LPWM_Output, 0);
      analogWrite(RPWM_Output, 0);
      break;
    }
  }     
}

void find_position_maximum(){
  analogWrite(LPWM_Output, 11);
  analogWrite(RPWM_Output, 0);
  Serial.println ("Searching for maximum");
  actualSameEncoderRead = 0;
  while (true)
  {
    if (encoderPosNow == encoderPosPrev)
    {
      actualSameEncoderRead = actualSameEncoderRead + 1;
      Serial.println (actualSameEncoderRead);
    }
    else
    {
      actualSameEncoderRead = 0;
    }
      if (actualSameEncoderRead > requiredSameEncoderRead)
      {
      encoderPositionMaximum = encoderPosNow;
      Serial.println (encoderPositionMaximum);
      analogWrite(LPWM_Output, 0);
      analogWrite(RPWM_Output, 0);
      break;
    }
  }
}    
