#include "WString.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "pins_arduino.h"
#include "mind_project_lib.h"
#include <avr/pgmspace.h>

// PIR System
void init_pir() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void pir_sys_activate() {
  // Serial.println("pir active");
  delay(500);
  if (digitalRead(pirPin)) {
    digitalWrite(buzzerPin, HIGH);  // Motion detected
  } else {
    digitalWrite(buzzerPin, LOW);  // No motion
  }
}

// Door System (Servo)
Servo myservo;

void init_servo() {
  myservo.attach(servoPin);
  myservo.write(0);  // Initially close the door
  pinMode(door_btn, INPUT_PULLUP);
}

void servo_sys_activate() {
  myservo.write(90);  // Open the door
  delay(1000);        // Keep door open for 3 seconds
}

void close_door() {
  myservo.write(0);  // Close the door
  delay(1000);
}

// LDR System
void init_ldr() {
  pinMode(ldr, INPUT);
  pinMode(led, OUTPUT);
}

void ldr_sys_activate() {
  int input = analogRead(ldr);
  // Serial.print(F("light level:"));
  // Serial.println(input);  // Print LDR value for debugging
  if (input < 500) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}

// Temperature System
void init_temperature_sys() {
  // pinMode(ntc_pin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(FAN, OUTPUT);
  pinMode(enable, OUTPUT);
}

void temperature_sys_activate() {
  float average = 0;
  int speed;

  for (int i = 0; i < samplingrate; i++) {
    average += analogRead(ntc_pin);
    delay(10);  // Short delay for analog read
  }

  average /= samplingrate;
  average = (1023 / average - 1.0);
  average = Rref / average;

  float temperature = average / nominal_resistance;
  temperature = log(temperature);
  temperature /= beta;
  temperature += 1.0 / (nominal_temperature + 273.15);
  temperature = 1.0 / temperature;
  temperature -= 273.15;


  // float referenceVoltage = 5.0;  // Arduino 5V power supply

  // int analogValue = analogRead(ntc_pin);                      // Read the analog value from KY-028
  // float voltage = (analogValue / 1023.0) * referenceVoltage;  // Convert to voltage


  // // Optional: You can map the analog reading to a rough temperature range (if needed)
  // float temperature = map(analogValue, 0, 1023, 10, 70);  // Approximate temperature range


  // delay(1000);  // Wait for 1 second before taking another reading

  // int speed;
  // Control fan and LEDs based on temperature
  if (temperature < 20) {
    digitalWrite(FAN, LOW);  // Turn off fan
    analogWrite(enable, 0);
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, LOW);
  } else if (temperature >= 20 && temperature <= 30) {
    digitalWrite(FAN, HIGH);  // Turn on fan
    speed = map(temperature, 20, 30, 0, 255);
    analogWrite(enable, speed);  // Adjust fan speed
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, HIGH);
  } else if (temperature > 30) {
    digitalWrite(FAN, HIGH);   // Ensure fan is on
    analogWrite(enable, 255);  // Full speed
    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, LOW);
  }
  Serial.print(" | Temperature (approx): ");
  Serial.print(temperature);
  Serial.println(" °C");
}


// Keypad System

void init_keypad() {
  pinMode(Keypad_pin, INPUT);
}

char getKey() {
  int reading = analogRead(Keypad_pin);
  // Serial.println(reading);

  switch (reading) {
    case 292 ... 312:
      return '*';
    case 337 ... 357:
      return '0';
    case 378 ... 398:
      return '#';
    case 417 ... 437:
      return 'D';
    case 10 ... 30:
      return '7';
    case 85 ... 105:
      return '8';
    case 156 ... 176:
      return '9';
    case 214 ... 234:
      return 'C';
    case 470 ... 490:
      return '4';
    case 500 ... 520:
      return '5';
    case 530 ... 550:
      return '6';
    case 554 ... 574:
      return 'B';
    case 590 ... 610:
      return '1';
    case 611 ... 630:
      return '2';
    case 631 ... 650:
      return '3';
    case 651 ... 670:
      return 'A';
    default:
      return '\0';
  }
}
