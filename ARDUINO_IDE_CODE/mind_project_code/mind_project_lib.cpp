#include "mind_project_lib.h"


//pir

void init_pir() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}
void pir_sys_activate() {
  char MotionDetected = digitalRead(pirPin);

  if (MotionDetected) {

    digitalWrite(buzzerPin, HIGH);
    Serial.println("EMSK 7RAMY");

  } else {

    digitalWrite(buzzerPin, LOW);
  }
}


//door system

Servo myservo;

void init_servo() {
  myservo.attach(servoPin);
  myservo.write(0);
}

void servo_sys_activate() {
  myservo.write(90);
  delay(3000);
  myservo.write(0);
}

//ldr

void init_ldr() {
  pinMode(ldr, INPUT);
  pinMode(led, OUTPUT);
}
void ldr_sys_activate() {
   int input = 0;
  int output = 0;
  input = analogRead(ldr);
  Serial.println(input);
  delay(1000);
  output = map(input, 0, 1023, 0, 255);
  analogWrite(led, output);
}

//temperature

void init_temperature_sys() {
  pinMode(ntc_pin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(FAN, OUTPUT);
  pinMode(enable, OUTPUT);
}

void temperature_sys_activate() {
  int i;
  float average = 0;

  for (i = 0; i < samplingrate; i++) {
    average += analogRead(ntc_pin);
    delay(10);
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


  if (temperature < 20) {

    digitalWrite(FAN, LOW);
    analogWrite(enable, 0);

    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, LOW);

  } else if (temperature >= 20 && temperature <= 30) {

    speed = map(temperature, 20, 30, 0, 255);
    analogWrite(enable, speed);
    digitalWrite(FAN, HIGH);

    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, HIGH);

  } else if (temperature > 30) {

    digitalWrite(FAN, HIGH);
    analogWrite(enable, 255);

    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, LOW);
  }
}

//keypad

const byte col = 4;
const byte row = 4;

byte rowPins[row] = { PIN_PB0, PIN_PB1, PD2, PD3 };  //connect to the row pinouts of the keypad
byte colPins[col] = { PD4, PD5, PD6, PD7 };          //connect to the column pinouts of the keypad

String Saved_Pass = "4545";
String In_Pass = "";

char i = 0;

void SetCols() {
  for (int i = 0; i < 4; i++) {

    digitalWrite(colPins[i], HIGH);
  }
}

void init_keypad() {
  for (int i = 0; i < 4; i++) {
    //Should be INPUT and OUTPUT
    pinMode(rowPins[i], INPUT);
    pinMode(colPins[i], OUTPUT);
  }

  SetCols();
}

void ResetCols() {

  for (int i = 0; i < 4; i++) {

    digitalWrite(colPins[i], LOW);
  }
}

bool ReadRowPins(char rownum) {

  switch (rownum) {
    case 0:  //for reading any input in all rows
      return (digitalRead(rowPins[0]) && 1) || (digitalRead(rowPins[1]) && 1) || (digitalRead(rowPins[2]) && 1) || (digitalRead(rowPins[3]) && 1);

    case 1:
      return digitalRead(rowPins[0]) && 1;

    case 2:
      return digitalRead(rowPins[1]) && 1;

    case 3:
      return digitalRead(rowPins[2]) && 1;

    case 4:
      return digitalRead(rowPins[3]) && 1;

    default:
      break;
  }
}

char getKey() {
  ResetCols();  // Reset all columns first
  SetCols();

  while (1) {
    if (ReadRowPins(0)) {
      ResetCols();
      digitalWrite(colPins[0], HIGH);
      delay(20);
      if (ReadRowPins(1)) {
        delay(20);
        return '3';
      } else if (ReadRowPins(2)) {
        delay(20);
        return '6';
      } else if (ReadRowPins(3)) {
        delay(20);
        return '9';

      } else if (ReadRowPins(4)) {
        delay(20);
        return '#';
      } else {
        //Do nothing
        ResetCols();
        digitalWrite(colPins[1], HIGH);
        delay(20);
        if (ReadRowPins(1)) {
          delay(20);
          return '2';

        } else if (ReadRowPins(2)) {
          delay(20);
          return '5';

        } else if (ReadRowPins(3)) {
          delay(20);
          return '8';

        } else if (ReadRowPins(4)) {
          delay(2);
          return '0';

        } else {
          //Do nothing
          ResetCols();
          digitalWrite(colPins[2], HIGH);
          delay(20);
          if (ReadRowPins(1)) {
            delay(20);
            return '1';

          } else if (ReadRowPins(2)) {
            delay(20);
            return '4';

          } else if (ReadRowPins(3)) {
            delay(20);
            return '7';

          } else if (ReadRowPins(4)) {
            delay(20);
            return '*';

          } else {

            ResetCols();
            digitalWrite(colPins[3], HIGH);
            delay(20);
            if (ReadRowPins(1)) {
              delay(20);
              return 'A';

            } else if (ReadRowPins(2)) {
              delay(20);
              return 'B';

            } else if (ReadRowPins(3)) {
              delay(20);
              return 'C';

            } else if (ReadRowPins(4)) {
              delay(20);
              return 'D';
            }
          }
        }
      }
      delay(20);
      SetCols();
    }

    return 'Q';
  }
}
