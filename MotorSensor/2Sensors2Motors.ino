#include <Wire.h>
#include <Adafruit_VL53L0X.h>
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


// Create two VL53L0X objects
Adafruit_VL53L0X sensor1 = Adafruit_VL53L0X();
Adafruit_VL53L0X sensor2 = Adafruit_VL53L0X();

// Define XSHUT pins
#define XSHUT_PIN1 17
#define XSHUT_PIN2 16

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize I2C
  Wire.begin();

  // Initialize XSHUT pins
  pinMode(XSHUT_PIN1, OUTPUT);
  pinMode(XSHUT_PIN2, OUTPUT);

  // Reset both sensors by setting XSHUT low, then high
  digitalWrite(XSHUT_PIN1, LOW);
  digitalWrite(XSHUT_PIN2, LOW);
  delay(10);
  digitalWrite(XSHUT_PIN1, HIGH);
  delay(10);

  // Initialize sensor 1
  if (!sensor1.begin(0x30)) { // Change the address to 0x30
    Serial.println(F("Failed to boot sensor 1"));
    while (1);
  }

  // Reset and initialize sensor 2
  digitalWrite(XSHUT_PIN2, HIGH);
  delay(10);
  if (!sensor2.begin(0x31)) { // Change the address to 0x31
    Serial.println(F("Failed to boot sensor 2"));
    while (1);
  }

  Serial.println(F("VL53L0X dual sensor setup complete"));
  
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

  

  VL53L0X_RangingMeasurementData_t measure1;
  VL53L0X_RangingMeasurementData_t measure2;

  double distance_1 = measure1.RangeMilliMeter;
  double distance_2 = measure2.RangeMilliMeter;

  // Read from sensor 1
  sensor1.rangingTest(&measure1, false); // Pass in 'true' to get debug data printout!
  if (measure1.RangeStatus != 4) { // Phase failures have incorrect data
    Serial.print("Sensor 1: ");
    Serial.print(distance_1);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 1: out of range");
  }

  // Read from sensor 2
  sensor2.rangingTest(&measure2, false); // Pass in 'true' to get debug data printout!
  if (measure2.RangeStatus != 4) { // Phase failures have incorrect data
    Serial.print("Sensor 2: ");
    Serial.print(distance_2);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 2: out of range");
  }

  // Move Motor A forward at half speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 128);  // PWM range is 0-255

  // Move Motor B forward at half speed
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 128);  // PWM range is 0-255


  if(distance_1 < 150)
  {
    analogWrite(PWMA, 0);
  }

  if(distance_2 < 150)
  {
    analogWrite(PWMB, 0);
  }

  
  delay(100);
}
