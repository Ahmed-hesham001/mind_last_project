#ifndef MIND_PROJECT_LIB_H
#define MIND_PROJECT_LIB_H
#include <math.h>
#include <Servo.h>


// --- PIR SYSTEM ---
#define pirPin PIN_PD7
#define buzzerPin PIN_PD6

void init_pir();
void pir_sys_activate();

// --- DOOR SYSTEM (SERVO) ---
#define servoPin PIN_PB2
#define door_btn PIN_PB5

void init_servo();
void servo_sys_activate();
void close_door();

// --- TEMPERATURE SYSTEM ---
#define redPin PIN_PC2
#define greenPin PIN_PC3
#define bluePin PIN_PC4

#define FAN PIN_PB0
#define enable PIN_PB3
#define ntc_pin PIN_PC1

#define nominal_resistance 10000  // 10k nominal resistance at 25C
#define nominal_temperature 25    // 25 degrees Celsius
#define samplingrate 5            // Number of samples for averaging
#define beta 3380                 // Beta value for NTC
#define Rref 10000                // Reference resistance for NTC

void init_temperature_sys();
void temperature_sys_activate();

// --- LDR SYSTEM ---
#define ldr PIN_PC0
#define led PIN_PB1

void init_ldr();
void ldr_sys_activate();

// --- KEYPAD SYSTEM ---
#define Keypad_pin PIN_PC5

void init_keypad();
char getKey();

#endif // MIND_PROJECT_LIB_H