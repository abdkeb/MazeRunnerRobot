#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"

void setupSensors() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize I2C
  Wire.begin();

  // Initialize XSHUT pins
  pinMode(XSHUT_PIN1, OUTPUT);
  pinMode(XSHUT_PIN2, OUTPUT);
  pinMode(XSHUT_PIN3, OUTPUT);

  // Reset both sensors by setting XSHUT low, then high
  digitalWrite(XSHUT_PIN1, LOW);
  digitalWrite(XSHUT_PIN2, LOW);
  digitalWrite(XSHUT_PIN3, LOW);
  delay(10);
  digitalWrite(XSHUT_PIN1, HIGH);
  delay(10);

  // Initialize right sensor
  if (!RightSensor.begin(0x30)) { // Change the address to 0x30
    Serial.println(F("Failed to boot right sensor"));
    while (1);
  }

  // Reset and initialize forward sensor
  digitalWrite(XSHUT_PIN2, HIGH);
  delay(10);
  if (!ForwardSensor.begin(0x31)) { // Change the address to 0x31
    Serial.println(F("Failed to boot forward sensor"));
    while (1);
  }

  // Reset and initialize left sensor
  digitalWrite(XSHUT_PIN3, HIGH);
  delay(10);
  if (!LeftSensor.begin(0x32)) { // Change the address to 0x32
    Serial.println(F("Failed to boot left sensor"));
    while (1);
  }

  Serial.println(F("VL53L0X three sensors setup complete"));
}




#endif // SENSORS_H
