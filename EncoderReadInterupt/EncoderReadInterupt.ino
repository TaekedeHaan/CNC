/* read a rotary encoder with interrupts
   Encoder hooked up with common to GROUND,
   encoderPinA to pin 2, encoderPinB to pin 4 (or pin 3 see below)
   it doesn't matter which encoder pin you use for A or B

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs

*/

#define encoderPinA  2
#define encoderPinB  3

volatile unsigned int encoderPosNow = 0;
unsigned int encoderPosPrev = 0;

void setup() {

  pinMode(encoderPinA, INPUT);
  digitalWrite(encoderPinA, HIGH);       // turn on pull-up resistor
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinB, HIGH);       // turn on pull-up resistor

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

/* See this expanded function to get a better understanding of the
 * meanings of the four possible (pinA, pinB) value pairs:
 */
void doEncoderA(){
  if (digitalRead(encoderPinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoderPinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      encoderPosNow--;         // CCW
    }
    else {
      encoderPosNow++;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  {
    if (digitalRead(encoderPinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning
      encoderPosNow++;          // CW
    }
    else {
      encoderPosNow--;          // CCW
    }
  }
}

void doEncoderB(){
  if (digitalRead(encoderPinB) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoderPinA) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      encoderPosNow++;         // CCW
    }
    else {
      encoderPosNow--;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  {
    if (digitalRead(encoderPinA) == LOW) {   // check channel B to see which way
                                              // encoder is turning
      encoderPosNow--;          // CW
    }
    else {
      encoderPosNow++;          // CCW
    }
  }
}

/*  to read the other two transitions - just use another attachInterrupt()
in the setup and duplicate the doEncoder function into say,
doEncoderA and doEncoderB.
You also need to move the other encoder wire over to pin 3 (interrupt 1).
*/

