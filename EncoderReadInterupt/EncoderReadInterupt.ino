/* read a rotary encoder with interrupts
   Encoder hooked up with common to GROUND,
   encoder0PinA to pin 2, encoder0PinB to pin 4 (or pin 3 see below)
   it doesn't matter which encoder pin you use for A or B  

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors 
   to the A & B channel outputs 

*/ 

#define encoder0PinA  2
#define encoder0PinB  3

volatile unsigned int encoder0PosNow = 0;
volatile unsigned int encoder0PosPrev = 0;
void setup() { 


  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor

  attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
  Serial.begin (9600);
  Serial.println("start");                // a personal quirk

} 

void loop(){
  if (encoder0PosNow != encoder0PosPrev){
    encoder0PosPrev = encoder0PosNow;
    Serial.println (encoder0PosNow);
  }
}

void doEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0PosNow++;
  } else {
    encoder0PosNow--;
  }
}

/* See this expanded function to get a better understanding of the
 * meanings of the four possible (pinA, pinB) value pairs:
 */
void doEncoder_Expanded(){
  if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      encoder0PosNow = encoder0PosNow - 1;         // CCW
    } 
    else {
      encoder0PosNow = encoder0PosNow + 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      encoder0PosNow = encoder0PosNow + 1;          // CW
    } 
    else {
      encoder0PosNow = encoder0PosNow - 1;          // CCW
    }

  }
            // debug - remember to comment out
                                              // before final program run
  // you don't want serial slowing down your program if not needed
}

/*  to read the other two transitions - just use another attachInterrupt()
in the setup and duplicate the doEncoder function into say, 
doEncoderA and doEncoderB. 
You also need to move the other encoder wire over to pin 3 (interrupt 1). 
*/ 
