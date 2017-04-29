/* read a rotary encoder with interrupts
   Encoder hooked up with common to GROUND,
   encoderPinA to pin 2, encoderPinB to pin 4 (or pin 3 see below)
   it doesn't matter which encoder pin you use for A or B

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs
*/

#define encoderPinA  2 //Red A-phase
#define encoderPinB  3 //Green B-pase

volatile unsigned int encoderPosNow = 0;
unsigned int encoderPosPrev = 0;

int input;
int encoderA;
int encoderB;

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  attachInterrupt(0, doEncoderA, CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, doEncoderB, CHANGE);  // encoder pin on interrupt 0 - pin 2
  Serial.begin (9600);
  Serial.println("start");                // a personal quirk

}

void loop(){
  if (encoderPosNow != encoderPosPrev){
    encoderPosPrev = encoderPosNow;
    Serial.println (encoderPosNow);
  }
}

void doEncoderA(){
  
  input = PIND;
  encoderA = input & B00000100;
  encoderA = encoderA >> 2;
  encoderB = input & B00001000;
  encoderB = encoderB >> 3;
  
  if (encoderA) {   // found a low-to-high on channel A
    if (encoderB) {  // check channel B to see which way
                                             // encoder is turning
      encoderPosNow++;         // CCW
    }
    else {
      encoderPosNow--;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  {
    if (encoderB) {   // check channel B to see which way
                                              // encoder is turning
      encoderPosNow--;          // CW
    }
    else {
      encoderPosNow++;          // CCW
    }
  }
}

void doEncoderB(){
  
  input = PIND;
  encoderA = input & B00000100;
  encoderA = encoderA >> 2;
  encoderB = input & B00001000;
  encoderB = encoderB >> 3;
  
  if (encoderB) {   // found a low-to-high on channel A
    if (encoderA) {  // check channel B to see which way
                                             // encoder is turning
      encoderPosNow--;         // CCW
    }
    else {
      encoderPosNow++;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  {
    if (encoderA) {   // check channel B to see which way
                                              // encoder is turning
      encoderPosNow++;          // CW
    }
    else {
      encoderPosNow--;          // CCW
    }
  }
}
