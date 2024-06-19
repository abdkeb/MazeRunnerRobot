How to check solo sensor:
- use the code in the sensor_check
- connections:
  - GND - GND
  - VIN - 3.3v
  - SDA - D22
  - SCL - D21

How to check dual sensors:
Connections for Sensor 1:
  VCC to 3.3V
  GND to GND
  SDA to GPIO 21 (I2C data line)
  SCL to GPIO 22 (I2C clock line)
  XSHUT to a digital pin TX2 (e.g., GPIO 17 )
Connections for Sensor 2:
  VCC to 3.3V
  GND to GND
  SDA to GPIO 21 (I2C data line)
  SCL to GPIO 22 (I2C clock line)
  XSHUT to RX2 (e.g., GPIO 16)


Notes:
- You will have to add library Adafruit_VL53L0X.h
- set baud to: 115200


