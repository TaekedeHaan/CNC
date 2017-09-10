int input;
int encoderAB;

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
