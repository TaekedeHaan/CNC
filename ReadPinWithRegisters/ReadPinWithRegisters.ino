#define inputPin 2
int input;

void setup() {
  pinMode(inputPin, INPUT_PULLUP);
  //DDRD = DDRD | B00000000;
  //DDRD = DDRD & B00000011;
  //PORTD = PORTD & B00000011;
  //PORTD = PORTD | B00000100;
  Serial.begin (9600);
  Serial.println("start");                // a personal quirk
}

void loop() {
  input = PIND;
  input = input & B00000100;
  input = input >> 2;
  Serial.println(input, BIN);
}
