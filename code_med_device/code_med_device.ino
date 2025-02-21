#include <VB_BMP280.h> 
 
const byte PIN_LED_RED = 3; 
const byte PIN_LED_GREEN = 4; 
const byte PIN_MOTOR_A = 6; 
const byte PIN_MOTOR_B = 9; 
const byte PIN_MOTOR_SLEEP = 12; 
 
const long TIMEOUT = 60000; 
 
VB_BMP280 barometer; 
 
unsigned long startTimeMotor; 
unsigned long stopTimeMotor; 
 
bool barometer_connection = false; 

byte motorState = 0; 
 
void setupBarometer() { 
  barometer.start_altitude = 0; 
 
  while (!barometer_connection) { 
    barometer_connection = barometer.begin(); 
 
    if (barometer_connection) { 
      return; 
    } else { 
      error(); 
    } 
  } 
} 
 
void setup() { 
  pinMode(PIN_MOTOR_A, OUTPUT); 
  pinMode(PIN_MOTOR_B, OUTPUT); 
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_MOTOR_SLEEP, OUTPUT); 
 
  digitalWrite(PIN_MOTOR_SLEEP, LOW); 
  setupBarometer(); 

  digitalWrite(PIN_LED_GREEN, HIGH); 
} 
 
void loop() { 
 
  if (barometer_connection) { 
    barometer.read(); 
    float pres = barometer.pres/133.32; 

    if (580 >= pres) { 
      stopMotor(); 
    } else if ((pres >= 720) && (motorState != 1)) { 
      runMotor(); 
    } 
 
    long timer = startTimeMotor - stopTimeMotor; 
    if (abs(timer) >= TIMEOUT) { 
      motorState = 1; 
      stopMotor(); 
      error(); 
    } 
  } 
} 
 
void runMotor() { 
  startTimeMotor = millis(); 
  digitalWrite(PIN_MOTOR_SLEEP, HIGH); 
  analogWrite(PIN_MOTOR_B, 255); 
  digitalWrite(PIN_MOTOR_A, LOW); 
} 
 
void stopMotor() { 
  stopTimeMotor = millis(); 
  digitalWrite(PIN_MOTOR_SLEEP, LOW); 
  analogWrite(PIN_MOTOR_B, LOW); 
  analogWrite(PIN_MOTOR_A, LOW); 
} 
 
void error() { 
  digitalWrite(PIN_LED_RED, HIGH); 
  digitalWrite(PIN_LED_GREEN, LOW); 
}