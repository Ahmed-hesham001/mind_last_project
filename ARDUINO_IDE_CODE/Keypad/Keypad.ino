#include <Atmega8_IO.h>
#include<Servo.h>

using namespace std;

#define servoPin PIN_PB2
#define pirPin PIN_PC0
#define buzzerPin PIN_PC1

Servo myservo;

void init_keypad();
char getKey();
void SetCols();
void ResetCols();
void ResetRows();
bool ReadRowPins(char rownum);

const byte col=4;
const byte row=4;

byte rowPins[row] = {PIN_PB0, PIN_PB1, PD2, PD3}; //connect to the row pinouts of the keypad
byte colPins[col] = {PD4, PD5, PD6, PD7}; //connect to the column pinouts of the keypad

String Saved_Pass="4545";
String In_Pass="";

char i=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  init_keypad();

  myservo.attach(servoPin);
  myservo.write(0);

  pinMode(pirPin,INPUT);
  pinMode(buzzerPin,OUTPUT);
  
}

void loop() {

  char MotionDetected=digitalRead(pirPin);

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

  if(i==4){
    Serial.println(In_Pass);
    if(In_Pass==Saved_Pass){

      Serial.println("Access Granted");
      myservo.write(90);
      delay(3000);
      myservo.write(0);

    }
    else{
      
      Serial.println("Access Denied");

    }
    i=0;
    In_Pass="";
  }

  if(MotionDetected){

    digitalWrite(buzzerPin,HIGH);
    Serial.println("EMSK 7RAMY");

  }
  else{

    digitalWrite(buzzerPin,LOW);

  }
}

///functions to use keypad////
void SetCols(){
for(int i=0;i<4;i++)
    {

      digitalWrite(colPins[i],HIGH);

    }

}

void init_keypad() {
  for(int i=0;i<4;i++)
      {
        //Should be INPUT_PULLUP and OUTPUT
          pinMode(rowPins[i],INPUT);
          pinMode(colPins[i],OUTPUT);
        
      }

  SetCols();
}

void ResetCols(){

  for(int i=0;i<4;i++)
    {

      digitalWrite(colPins[i],LOW);

    }

}

bool ReadRowPins(char rownum){

  switch(rownum){
    case 0://for reading any input in all rows
      return (digitalRead(rowPins[0]) && 1) || (digitalRead(rowPins[1]) && 1) ||
             (digitalRead(rowPins[2]) && 1) || (digitalRead(rowPins[3]) && 1);
    
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
    ResetCols();   // Reset all columns first
    SetCols();  
  
    while(1){
        if (ReadRowPins(0)) {
            ResetCols();
            digitalWrite(colPins[0],HIGH);
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
                digitalWrite(colPins[1],HIGH);
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
                   digitalWrite(colPins[2],HIGH);
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
                        digitalWrite(colPins[3],HIGH);
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

