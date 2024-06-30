#include <Arduino.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

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
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
 
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
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

  VL53L0X_RangingMeasurementData_t measure;
   
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  double distance = measure.RangeMilliMeter;

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(distance);
  } else {
    Serial.println(" out of range ");
  }
   
    // Move Motor A forward at half speed
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 128);  // PWM range is 0-255
    // Move Motor B forward at half speed
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, 128);  // PWM range is 0-255

  if(distance < 150){
      // Stop both motors
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
  } 



  delay(100);
}
