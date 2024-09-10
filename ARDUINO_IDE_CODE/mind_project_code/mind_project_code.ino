#include <Atmega8_IO.h>
#include "mind_project_lib.h"

// using namespace std;

// void init_keypad();
// char getKey();
// void SetCols();
// void ResetCols();
// void ResetRows();
// bool ReadRowPins(char rownum);

// const byte col=4;
// const byte row=4;

// byte rowPins[row] = {PIN_PB0, PIN_PB1, PD2, PD3}; //connect to the row pinouts of the keypad
// byte colPins[col] = {PD4, PD5, PD6, PD7}; //connect to the column pinouts of the keypad

// String Saved_Pass="4545";
// String In_Pass="";

// char i=0;

bool door_state = false;
bool face_rec   = false;
char password   = false;
bool door_key   = false;
// char choice = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_keypad();
  init_ldr();
  init_pir();
  init_servo();
  init_temperature_sys();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (face_rec) {
    door_state = true;
  } else {
    //read password

        char key=getKey();
        delay(150);

        if(key != 'Q'){
          
          if (key){

            In_Pass+=key;
            Serial.print("Key Pressed : ");
            Serial.println(key);
            i++;

          }
        }

        //sends password to python code
        if(i==4){
          Serial.println(In_Pass);
          i=0;
          In_Pass="";
        }

        //checks password
        if (password==1) {
          //open the door
          door_state = true;
        } else if(password==-1) {
          //buzzer and warning message
          door_state = false;
        }
        else{
          //No password is written
          door_state=false;

        }
    }
  

  if (door_state){
    //open the door and start all functions
    servo_sys_activate();

  }
  else {
    //pir sensor code
  }
}


// ///functions to use keypad////
// void SetCols(){
// for(int i=0;i<4;i++)
//     {

//       digitalWrite(colPins[i],HIGH);

//     }

// }

// void init_keypad() {
//   for(int i=0;i<4;i++)
//       {
//         //Should be INPUT and OUTPUT
//           pinMode(rowPins[i],INPUT);
//           pinMode(colPins[i],OUTPUT);
        
//       }

//   SetCols();
// }

// void ResetCols(){

//   for(int i=0;i<4;i++)
//     {

//       digitalWrite(colPins[i],LOW);

//     }

// }

// bool ReadRowPins(char rownum){

//   switch(rownum){
//     case 0://for reading any input in all rows
//       return (digitalRead(rowPins[0]) && 1) || (digitalRead(rowPins[1]) && 1) ||
//              (digitalRead(rowPins[2]) && 1) || (digitalRead(rowPins[3]) && 1);
    
//     case 1:
//       return digitalRead(rowPins[0]) && 1;

//     case 2:
//       return digitalRead(rowPins[1]) && 1;
      
//     case 3:
//       return digitalRead(rowPins[2]) && 1;

//     case 4:
//       return digitalRead(rowPins[3]) && 1;

//     default:
//       break;
//   }

// }

// char getKey() {
//     ResetCols();   // Reset all columns first
//     SetCols();  
  
//     while(1){
//         if (ReadRowPins(0)) {
//             ResetCols();
//             digitalWrite(colPins[0],HIGH);
//             delay(20);
//             if (ReadRowPins(1)) {
//                 delay(20);
//                 return '3';
//             } else if (ReadRowPins(2)) {
//                 delay(20);
//                 return '6';
//             } else if (ReadRowPins(3)) {
//                 delay(20);
//                 return '9';

//             } else if (ReadRowPins(4)) {
//                 delay(20);
//                 return '#';
//             } else {
//                 //Do nothing
//                 ResetCols();
//                 digitalWrite(colPins[1],HIGH);
//                delay(20);
//                 if (ReadRowPins(1)) {
//                     delay(20);
//                     return '2';

//                 } else if (ReadRowPins(2)) {
//                     delay(20);
//                     return '5';

//                 } else if (ReadRowPins(3)) {
//                     delay(20);
//                     return '8';

//                 } else if (ReadRowPins(4)) {
//                     delay(2);
//                     return '0';

//                 } else {
//                     //Do nothing
//                     ResetCols();
//                    digitalWrite(colPins[2],HIGH);
//                     delay(20);
//                     if (ReadRowPins(1)) {
//                         delay(20);
//                         return '1';

//                     } else if (ReadRowPins(2)) {
//                         delay(20);
//                         return '4';

//                     } else if (ReadRowPins(3)) {
//                         delay(20);
//                         return '7';

//                     } else if (ReadRowPins(4)) {
//                         delay(20);
//                         return '*';

//                     } else {

//                         ResetCols();
//                         digitalWrite(colPins[3],HIGH);
//                         delay(20);
//                         if (ReadRowPins(1)) {
//                             delay(20);
//                             return 'A';

//                         } else if (ReadRowPins(2)) {
//                             delay(20);
//                             return 'B';

//                         } else if (ReadRowPins(3)) {
//                             delay(20);
//                             return 'C';

//                         } else if (ReadRowPins(4)) {
//                             delay(20);
//                             return 'D';

//                         }
//                     }
//                 }




//             }
//             delay(20);
//             SetCols();
        
//         }

//         return 'Q';
//     }

        
// }

