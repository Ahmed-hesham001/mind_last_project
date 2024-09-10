#include <Atmega8_IO.h>

#define ldr PIN_PC3
#define led PIN_PB2


void setup() {
  // put your setup code here, to run once:
  pinMode(ldr, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("hello");
  int input = 0;
  int output = 0;
  input = analogRead(ldr);
  Serial.println(input);
  delay(1000);
  output = map(input, 0, 1023, 0, 255);
  analogWrite(led, output);
}
