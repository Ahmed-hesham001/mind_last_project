#ifndef  MIND_PROJECT_LIB_H
#define  MIND_PROJECT_LIB_H

#include <Atmega8_IO.h>
#include <math.h>
#include <Servo.h>

using namespace std;

//pir
#define pirPin PIN_PD7
#define buzzerPin PIN_PD6

void init_pir ();
void pir_sys_activate ();

//door system
#define servoPin PIN_PB2

void init_servo ();
void servo_sys_activate();

//temperature

#define redPin PIN_PC2
#define greenPin PIN_PC3
#define bluePin PIN_PC4

#define FAN PIN_PB0
#define enable PIN_PB1

#define ntc_pin PIN_PC1
#define nominal_resistance 10000
#define nominal_temperature 25
#define samplingrate 5
#define beta 3380
#define Rref 10000

extern int speed;

void init_temperature_sys();
void temperature_sys_activate();

//ldr

#define ldr PIN_PC0
#define led PIN_PB3

void init_ldr () ;
void ldr_sys_activate ();

//keypad

extern const byte col;
extern const byte row;

extern byte rowPins[4] ;  //connect to the row pinouts of the keypad
extern byte colPins[4] ;  //connect to the column pinouts of the keypad

extern String Saved_Pass;
extern String In_Pass;

extern char i;

void init_keypad();
char getKey();
void SetCols();
void ResetCols();
void ResetRows();
bool ReadRowPins(char rownum);

#endif