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

long encoderPosPrev = 0;
volatile long encoderPosNow = 0;

long encoderPositionMinimum;
long encoderPositionMaximum;

int powerCalibration = 15; //[max = 256]

unsigned int actualSameEncoderRead;
unsigned int requiredSameEncoderRead = 120; 

#define resolutionEncoder 2400 //[ticks per rotation]
#define leadSpindle 2 //[mm/rotation]

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
  
  encoderPositionMinimum = find_position_minimum();
  encoderPositionMaximum = find_position_maximum();
  
  Serial.print("Minimum:");
  Serial.println(encoderPositionMinimum);

  Serial.print("Maximum:");
  Serial.println(encoderPositionMaximum);

  int lengthAxis = abs(encoderPositionMaximum - encoderPositionMinimum) / resolutionEncoder * leadSpindle;

  Serial.print("Difference in ticks is: ");
  Serial.println(abs(encoderPositionMaximum - encoderPositionMinimum));
  
  Serial.print("Lengtht of axis is: ");
  Serial.println(lengthAxis);
}

void loop()
{




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


long find_position_minimum(){
  Serial.println ("Searching for minimum");
  return find_position(0, powerCalibration);
}

long find_position_maximum(){
  Serial.println ("Searching for maximun");
  return find_position(powerCalibration, 0);
}    


long find_position(int analogWriteRight, int analogWriteLeft){
  analogWrite(LPWM_Output, 2 * analogWriteLeft);
  analogWrite(RPWM_Output, 2 * analogWriteRight);
  delay(100);

  analogWrite(LPWM_Output, analogWriteLeft);
  analogWrite(RPWM_Output, analogWriteRight);
  actualSameEncoderRead = 0;
  
  while (true){
    if (abs(encoderPosNow - encoderPosPrev) < 2) {
      //If the same value as previous has been read.
      actualSameEncoderRead ++;
      //Serial.println(actualSameEncoderRead);
    }
    else{
      encoderPosPrev = encoderPosNow;
      actualSameEncoderRead = 0;
    }
    if (actualSameEncoderRead > requiredSameEncoderRead){
      motor_reset();
      Serial.print("Found a value: ");
      Serial.println(encoderPosNow);
      return encoderPosNow;
    }
  }     
}

void motor_reset(){
  Serial.println ("Resetting motor");
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 0);
}

