#include "mind_project_lib.h"
#include <avr/pgmspace.h>

// Variables
uint8_t door_state = 2;
uint8_t face_rec;
uint8_t password_flag;
// String pass;
char pass[5];
uint8_t i = 0;  // Use uint8_t instead of int for small integer values

void setup() {
  Serial.begin(9600);
  init_keypad();
  init_ldr();
  init_pir();
  init_servo();
  init_temperature_sys();
}

// // void loop() {
// //   if (door_state) {
// //     // Door open logic
// //     servo_sys_activate();
// //     delay(500);
// //     digitalWrite(buzzerPin, LOW);
// //     Serial.println(F("Door is opened."));
// //     while (door_state) {
// //       temperature_sys_activate();
// //       ldr_sys_activate();

// //       // // Check for button press to close the door
// //       if (digitalRead(door_btn) == LOW) {
// //         door_state = 0;

// //         // Turn off the systems
// //         digitalWrite(FAN, LOW);
// //         digitalWrite(enable, LOW);
// //         digitalWrite(redPin, LOW);
// //         digitalWrite(bluePin, LOW);
// //         digitalWrite(greenPin, LOW);
// //         digitalWrite(led, LOW);

// //         Serial.println(F("Door is closed."));
// //         close_door();
// //         // break;
// //       }
// //     }
// //   } else {
// //     Serial.println(F("face rec flag:"));

// //     // Face recognition
// //     while (Serial.available() == 0)
// //       ;
// //     face_rec = Serial.parseInt();
// //     delay(500);

// //     Serial.println(face_rec);

// //     if (face_rec) {
// //       door_state = 1;
// //       delay(500);
// //       // Serial.println(F("face recognized"));
// //     } else {

// //       Serial.println(F("ENTER PASS:"));

// //       char key = getKey();
// //       delay(150);  // Debounce

// //       while (key != '#') {
// //         // Serial.print(key);
// //         pass += key;
// //         key = getKey();
// //         delay(150);
// //       }

// //       Serial.print(F("Entered Password: "));
// //       Serial.println(pass);
// //       delay(500);

// //       while (Serial.available() == 0)
// //         ;
// //       password_flag = Serial.parseInt();
// //       delay(500);


// //       // Handle password check result
// //       if (password_flag) {
// //         delay(150);
// //         door_state = 1;
// //         delay(500);
// //         Serial.println(F("Password correct. Door opened."));
// //       } else {
// //         delay(150);
// //         door_state = 0;
// //         Serial.println(F("Password incorrect. Try again."));

// //         digitalWrite(buzzerPin, HIGH);
// //         delay(500);
// //         digitalWrite(buzzerPin, LOW);
// //         delay(500);

// //         digitalWrite(buzzerPin, HIGH);
// //         delay(500);
// //         digitalWrite(buzzerPin, LOW);
// //         delay(500);
// //       }



// //       pass = "";
// //       i = 0;  // Reset password input index
// //     }

// //   }
// //   // pir_sys_activate();
// }
// void loop() {
//   delay(500);
//   if (door_state == 1) {
//     // Door open logic
//     servo_sys_activate();
//     delay(500);
//     digitalWrite(buzzerPin, LOW);
//     // Serial.println(F("Door is opened."));
//     while (door_state == 1) {
//       temperature_sys_activate();
//       ldr_sys_activate();

//       // Check for button press to close the door
//       delay(50);
//       if (digitalRead(door_btn) == LOW) {
//         delay(50);
//         door_state = 2;

//         // Turn off the systems
//         digitalWrite(FAN, LOW);
//         digitalWrite(enable, LOW);
//         digitalWrite(redPin, LOW);
//         digitalWrite(bluePin, LOW);
//         digitalWrite(greenPin, LOW);
//         digitalWrite(led, LOW);

//         // Serial.println(F("Door is closed."));
//         close_door();
//         break;
//       }
//     }
//   } else if (door_state == 0) {      // Only print "face rec flag:" once before checking for face recognition
//       Serial.println(F("face rec flag:"));

//       // Face recognition
//       while (Serial.available() == 0)
//         ;
//       face_rec = Serial.parseInt();
//       delay(500);

//       Serial.println(face_rec);


//       if (face_rec) {
//         door_state = 1;
//         delay(500);
//         // Serial.println(F("face recognized"));
//       } else {
//         // Serial.println(F("ENTER PASS:"));
//         while (i < 4) {
//           char key = getKey();
//           delay(200);
//           if (key != '\0'&&i<4) {
//             pass[i] = key;
//             ++i;
//           }
//           if (i == 4) {
//             pass[4] = '\0';
//             Serial.println(F("Entered Password: "));
//             Serial.println(pass);
//             delay(500);
//           }
//         }


//         while (Serial.available() == 0)
//           ;
//         password_flag = Serial.parseInt();
//         delay(500);

//         Serial.println(password_flag);

//         // Handle password check result
//         if (password_flag) {
//           delay(150);
//           door_state = 1;
//           delay(500);
//           // Serial.println(F("Password correct. Door opened."));
//         } else {
//           delay(150);
//           door_state = 2;
//           // Serial.println(F("Password incorrect. Try again."));

//           digitalWrite(buzzerPin, HIGH);
//           delay(500);
//           digitalWrite(buzzerPin, LOW);
//           delay(500);

//           digitalWrite(buzzerPin, HIGH);
//           delay(500);
//           digitalWrite(buzzerPin, LOW);
//           delay(500);
//         }

//         memset(pass, '\0', sizeof(pass));  // Clear password array
//         i = 0;                             // Reset password input index
//       }
    
//   } else if (door_state == 2) {
//     pir_sys_activate();  // Keep the PIR system active as needed
//     if (digitalRead(door_btn) == LOW) {
//       door_state = 0;
//       digitalWrite(buzzerPin, LOW);
//     }
//   }
// }

void loop() {
  delay(500);
  if (door_state == 1) {
    door_state=1;
    // Door open logic
    servo_sys_activate();
    delay(500);
    digitalWrite(buzzerPin, LOW);
    // Serial.println(F("Door is opened."));
    while (true) {
      temperature_sys_activate();
      ldr_sys_activate();

      // Check for button press to close the door
      delay(50);
      if (digitalRead(door_btn) == LOW) {
        delay(50);
        door_state = 2;

        // Turn off the systems
        digitalWrite(FAN, LOW);
        digitalWrite(enable, LOW);
        digitalWrite(redPin, LOW);
        digitalWrite(bluePin, LOW);
        digitalWrite(greenPin, LOW);
        digitalWrite(led, LOW);

        // Serial.println(F("Door is closed."));
        close_door();
        break;
      }
    }
  } else if (door_state == 0) {      // Only print "face rec flag:" once before checking for face recognition
      Serial.println(F("face rec flag:"));

      // Face recognition
      while (Serial.available() == 0)
        ;
      face_rec = Serial.parseInt();
      delay(500);

      Serial.println(face_rec);


      if (face_rec) {
        door_state = 1;
        delay(500);
        // Serial.println(F("face recognized"));
      } else {
        // Serial.println(F("ENTER PASS:"));
        while (i < 4) {
          char key = getKey();
          delay(200);
          if (key != '\0'&&i<4) {
            pass[i] = key;
            ++i;
          }
          if (i == 4) {
            pass[4] = '\0';
            Serial.println(F("Entered Password: "));
            Serial.println(pass);
            delay(500);
          }
        }


        while (Serial.available() == 0)
          ;
        password_flag = Serial.parseInt();
        delay(500);

        Serial.println(password_flag);

        // Handle password check result
        if (password_flag) {
          delay(150);
          door_state = 1;
          delay(500);
          // Serial.println(F("Password correct. Door opened."));
        } else {
          delay(150);
          door_state = 2;
          // Serial.println(F("Password incorrect. Try again."));

          digitalWrite(buzzerPin, HIGH);
          delay(500);
          digitalWrite(buzzerPin, LOW);
          delay(500);

          digitalWrite(buzzerPin, HIGH);
          delay(500);
          digitalWrite(buzzerPin, LOW);
          delay(500);
        }

        memset(pass, '\0', sizeof(pass));  // Clear password array
        i = 0;                             // Reset password input index
      }
    
  } else if (door_state == 2) {
    pir_sys_activate();  // Keep the PIR system active as needed
    if (digitalRead(door_btn) == LOW) {
      door_state = 0;
      digitalWrite(buzzerPin, LOW);
    }
  }
}