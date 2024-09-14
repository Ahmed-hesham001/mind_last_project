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
  delay(3000);
}

// LDR System
void init_ldr() {
  pinMode(ldr, INPUT);
  pinMode(led, OUTPUT);
}

void ldr_sys_activate() {
  int input = analogRead(ldr);
  Serial.print(F("light level:"));
  Serial.println(input);  // Print LDR value for debugging
  if (input < 500) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}

// Temperature System
void init_temperature_sys() {
  pinMode(ntc_pin, INPUT);

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
  Serial.print(F("Temp: "));
  Serial.print((int)temperature);
  Serial.println(F("*C"));
}


// Keypad System
// const byte col = 4;
// const byte row = 4;

// byte rowPins[row] = { PIN_PB4, PIN_PB5, PIN_PB6, PIN_PB7 };
// byte colPins[col] = { PD2, PD3, PD4, PD5 };

// // Move keymap to flash memory
// const char keymap[row][col] PROGMEM = {
//   { '7', '8', '9', '/' },
//   { '4', '5', '6', '*' },
//   { '1', '2', '3', '-' },
//   { 'c', '0', '=', '+' }
// };

// void init_keypad() {
//   for (int i = 0; i < row; i++) {
//     pinMode(rowPins[i], INPUT_PULLUP);
//   }
//   for (int i = 0; i < col; i++) {
//     pinMode(colPins[i], OUTPUT);
//     digitalWrite(colPins[i], HIGH);  // Set all columns HIGH
//     delay(20);
//   }
// }

// char getKey() {
//   for (int c = 0; c < col; c++) {
//     digitalWrite(colPins[c], LOW);  // Pull column low
//     for (int r = 0; r < row; r++) {
//       if (!digitalRead(rowPins[r])) {    // Check if row is pulled low
//         digitalWrite(colPins[c], HIGH);  // Reset column
//         delay(50);                       // Debounce delay
//         // Read keymap value from flash
//         return pgm_read_byte(&(keymap[r][c]));
//       }
//     }
//     digitalWrite(colPins[c], HIGH);  // Reset column
//   }
//   return '\0';  // No key pressed
// }

void init_keypad() {
  pinMode(Keypad_pin, INPUT);
}

char getKey() {
  int reading = analogRead(Keypad_pin);
  // Serial.println(reading);

  switch (reading) {
    case 0 ... 30:
      return '*';
    case 57 ... 77:
      return '0';
    case 110 ... 170:
      return '#';
    case 171 ... 187:
      return 'D';
    case 240 ... 260:
      return '7';
    case 270 ... 300:
      return '8';
    case 301 ... 340:
      return '9';
    case 341 ... 370:
      return 'C';
    case 371 ... 410:
      return '4';
    case 411 ... 440:
      return '5';
    case 441 ... 460:
      return '6';
    case 461 ... 480:
      return 'B';
    case 481 ... 510:
      return '1';
    case 511 ... 525:
      return '2';
    case 526 ... 540:
      return '3';
    case 541 ... 560:
      return 'A';
    default:
      return '\0';
  }
}