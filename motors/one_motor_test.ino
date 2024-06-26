#include <Arduino.h>

#define STBY 27  // Standby pin
#define PWMA 13  // PWM control for motor speed
#define AIN1 14  // Control pin 1 for motor direction
#define AIN2 12  // Control pin 2 for motor direction

void setup() {
  // Set control pins as outputs
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  // Ensure the driver is out of standby mode
  digitalWrite(STBY, HIGH);
}

void loop() {
  // Motor forward at half speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 128);  // PWM range is 0-255

  delay(2000);  // Run motor for 2 seconds

  // Motor backward at quarter speed
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
}
