#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// Create two VL53L0X objects
Adafruit_VL53L0X sensor1 = Adafruit_VL53L0X();
Adafruit_VL53L0X sensor2 = Adafruit_VL53L0X();
Adafruit_VL53L0X sensor3 = Adafruit_VL53L0X();


// Define XSHUT pins
#define XSHUT_PIN1 17 //right sensor
#define XSHUT_PIN2 16  //forward sensor
#define XSHUT_PIN3 3  //left sensor


void setup() {
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

  // Reset and initialize sensor 3
  digitalWrite(XSHUT_PIN3, HIGH);
  delay(10);
  if (!sensor3.begin(0x32)) { // Change the address to 0x32
    Serial.println(F("Failed to boot sensor 3"));
    while (1);
  }

  Serial.println(F("VL53L0X three sensors setup complete"));
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure1;
  VL53L0X_RangingMeasurementData_t measure2;
  VL53L0X_RangingMeasurementData_t measure3;


  // Read from sensor 1
  sensor1.rangingTest(&measure1, false); // Pass in 'true' to get debug data printout!
  if (measure1.RangeStatus != 4) { // Phase failures have incorrect data
    Serial.print("Sensor 1: ");
    Serial.print(measure1.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 1: out of range");
  }

  // Read from sensor 2
  sensor2.rangingTest(&measure2, false); // Pass in 'true' to get debug data printout!
  if (measure2.RangeStatus != 4) { // Phase failures have incorrect data
    Serial.print("Sensor 2: ");
    Serial.print(measure2.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 2: out of range");
  }

  // Read from sensor 3
  sensor3.rangingTest(&measure3, false); // Pass in 'true' to get debug data printout!
  if (measure3.RangeStatus != 4) { // Phase failures have incorrect data
    Serial.print("Sensor 3: ");
    Serial.print(measure3.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 3: out of range");
  }

  delay(500);
}
