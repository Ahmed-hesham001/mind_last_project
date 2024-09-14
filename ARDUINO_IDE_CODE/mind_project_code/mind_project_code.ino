#include "mind_project_lib.h"
#include <avr/pgmspace.h>

// Variables
bool door_state = false;
uint8_t face_rec;
uint8_t password_flag;
String pass;
uint8_t i = 0;  // Use uint8_t instead of int for small integer values

void setup() {
  Serial.begin(9600);
  init_keypad();
  init_ldr();
  init_pir();
  init_servo();
  init_temperature_sys();
  Serial.println(F("ENTER PASS:"));
}

void loop() {
  if (door_state) {
    // Door open logic
    servo_sys_activate();
    digitalWrite(buzzerPin, LOW);
    Serial.println(F("Door is opened."));
    while (door_state) {
      temperature_sys_activate();
      ldr_sys_activate();

      // Check for button press to close the door
      if (digitalRead(door_btn) == LOW) {
        door_state = false;

        // Turn off the systems
        digitalWrite(FAN, LOW);
        digitalWrite(enable, LOW);
        digitalWrite(redPin, LOW);
        digitalWrite(bluePin, LOW);
        digitalWrite(greenPin, LOW);
        digitalWrite(led, LOW);

        Serial.println(F("Door is closed."));
        Serial.println(F("ENTER PASS:"));

        close_door();
        break;
      }
    }
  } else {
    // Face recognition
    while (Serial.available() == 0);
    face_rec = Serial.parseInt();
    if (face_rec == 1) {
      door_state = true;
    } else {

      // Keypad input for password
      char key = getKey();
      delay(150);  // Debounce

      if (key != '\0' && i < 4) {  // Ensure password index does not exceed
        pass += key;
        Serial.print(F("Key Pressed: "));
        Serial.println(key);
        i++;
      }

      if (i == 4) {
        Serial.print(F("Entered Password: "));
        Serial.println(pass);
        delay(500);

        while (Serial.available() == 0)
          ;
        password_flag = Serial.parseInt();

        // Handle password check result
        if (password_flag) {
          door_state = true;
          Serial.println(F("Password correct. Door opened."));
        } else {
          Serial.println(F("Password incorrect. Try again."));
          digitalWrite(buzzerPin, LOW);
        }

        pass = "";
        i = 0;  // Reset password input index
      } else {
        // Motion detection logic
        pir_sys_activate();
      }
    }
  }
}
