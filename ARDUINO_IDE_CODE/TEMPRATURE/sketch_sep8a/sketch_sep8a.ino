#include <Atmega8_IO.h>
#include <math.h>

// #define ntc_pin   PIN_PC1

// #define nominal_resistance 20000
// #define nominal_temperature 25
// #define samplingrate 5
// #define beta 3950
// #define Rref 20000


// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   pinMode(ntc_pin, INPUT);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// int i;
//   float average = 0;

//   for (i = 0; i < samplingrate; i++) {
//     average += analogRead(ntc_pin);
//     delay(10);
//   }

//   average /= samplingrate;

//   float resistance = (1023.0 / average - 1.0) * Rref;

//   float temperature = resistance / nominal_resistance;
//   temperature = log(temperature);
//   temperature /= beta;
//   temperature += 1.0 / (nominal_temperature + 273.15);
//   temperature = 1.0 / temperature;
//   temperature -= 273.15;

//   Serial.print("TEMP :");
//   Serial.print(temperature);
//   Serial.println("*C");
//   delay(1000);
// }
// Constants and pin definitions
// Constants and pin definitions
// const int analogPin = PC1;       // Pin connected to the voltage divider
// const float R_fixed = 10000.0;  // Fixed resistor value (10k ohms)
// const float V_ref = 5.0;        // Reference voltage (usually 5V)

// // Steinhart-Hart equation coefficients for a typical 10k NTC thermistor
// const float A = 1.009249522e-03;
// const float B = 2.378405444e-04;
// const float C = 2.019202697e-07;

// void setup() {
//   Serial.begin(9600);  // Initialize serial communication for debugging
// }

// // Function to measure temperature using NTC thermistor
// float measureTemperature() {
//   // Read the analog value (0-1023)
//   int adcValue = analogRead(analogPin);

//   // Convert the ADC value to voltage
//   float V_thermistor = (adcValue * V_ref) / 1023.0;

//   // Calculate the resistance of the thermistor
//   float R_thermistor = (R_fixed * V_thermistor) / (V_ref - V_thermistor);

//   // Debugging output to check voltage and resistance
//   Serial.print("ADC Value: ");
//   Serial.print(adcValue);
//   Serial.print(" | Voltage across thermistor: ");
//   Serial.print(V_thermistor);
//   Serial.print(" V | Thermistor Resistance: ");
//   Serial.print(R_thermistor);
//   Serial.println(" ohms");

//   // Apply the Steinhart-Hart equation to calculate temperature in Kelvin
//   float logR = log(R_thermistor);
//   float temperatureK = 1.0 / (A + B * logR + C * logR * logR * logR);

//   // Convert Kelvin to Celsius
//   float temperatureC = temperatureK - 273.15;

//   return temperatureC;  // Return the temperature in Celsius
// }

// void loop() {
//   // Call the temperature measurement function
//   float temperature = measureTemperature();

//   // Print the temperature to the serial monitor
//   Serial.print("Temperature: ");
//   Serial.print(temperature);
//   Serial.println(" °C");

//   delay(1000);  // Wait 1 second before repeating
// }

const int SAMPLE_NUMBER = 10;

const double BALANCE_RESISTOR = 10000.0;

const double MAX_ADC = 1023.0;

const double BETA = 3974.0;

const double ROOM_TEMP = 298.15;  // room temperature in Kelvin

const double RESISTOR_ROOM_TEMP = 10000.0;

double currentTemperature = 0;

int thermistorPin = PC1;  // Where the ADC samples the resistor divider's output

void setup() {
  // Set the port speed for serial window messages
  Serial.begin(9600);
  pinMode(thermistorPin, INPUT);
}


void loop() {

  currentTemperature = readThermistor();
  delay(3000);


  if (currentTemperature > 21.0 && currentTemperature < 24.0) {
    Serial.print("It is ");
    Serial.print(currentTemperature);
    Serial.println("C.  very nice temperature.");
  } else if (currentTemperature >= 24.0) {
    Serial.print("It is ");
    Serial.print(currentTemperature);
    Serial.println("C. hot hot hot");
  } else {
    Serial.print("It is ");
    Serial.print(currentTemperature);
    Serial.println("C. saaaaa23aaaaa");
  }
}



double readThermistor() {
  // variables that live in this function
  double rThermistor = 0;         // Holds thermistor resistance value
  double tKelvin = 0;             // Holds calculated temperature
  double tCelsius = 0;            // Hold temperature in celsius
  double adcAverage = 0;          // Holds the average voltage measurement
  int adcSamples[SAMPLE_NUMBER];  // Array to hold each voltage measurement


  for (int i = 0; i < SAMPLE_NUMBER; i++) {
    adcSamples[i] = analogRead(thermistorPin);  // read from pin and store
    delay(10);                                  // wait 10 milliseconds
  }

  /* Then, we will simply average all of those samples up for a "stiffer"
    measurement. */
  for (int i = 0; i < SAMPLE_NUMBER; i++) {
    adcAverage += adcSamples[i];  // add all samples up . . .
  }
  adcAverage /= SAMPLE_NUMBER;  // . . . average it w/ divide


  rThermistor = BALANCE_RESISTOR * ((MAX_ADC / adcAverage) - 1);


  tKelvin = (BETA * ROOM_TEMP) / (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));


  tCelsius = tKelvin - 273.15;  // convert kelvin to celsius

  return tCelsius;  // Return the temperature in Celsius
}