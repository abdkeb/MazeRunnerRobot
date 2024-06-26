#include <Arduino.h>

// Define pin connections for Motor A
#define STBY 27   // Standby pin
#define PWMA 13   // PWM control for Motor A speed
#define AIN1 14   // Control pin 1 for Motor A direction
#define AIN2 12   // Control pin 2 for Motor A direction

// Define pin connections for Motor B
#define PWMB 26   // PWM control for Motor B speed
#define BIN1 25   // Control pin 1 for Motor B direction
#define BIN2 33   // Control pin 2 for Motor B direction

void setup() {
  // Set control pins as outputs for Motor A
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  // Set control pins as outputs for Motor B
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Ensure the motor driver is out of standby mode
  digitalWrite(STBY, HIGH);
}

void loop() {
  // Move Motor A forward at half speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 128);  // PWM range is 0-255

  // Move Motor B forward at half speed
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 128);  // PWM range is 0-255

  delay(2000);  // Run motors for 2 seconds

  // Move Motor A backward at quarter speed
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 64);  // PWM range is 0-255

  // Move Motor B backward at quarter speed
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 64);  // PWM range is 0-255

  delay(2000);  // Run motors for 2 seconds

  // Stop both motors
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(2000);  // Stop for 2 seconds
}

