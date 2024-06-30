

How to check dual sensors:
Connections for Sensor 1:
  VCC to 3.3V
  GND to GND
  SDA to GPIO 21 (I2C data line)
  SCL to GPIO 22 (I2C clock line)   
  XSHUT to RX2 (e.g., GPIO 16)
Connections for Sensor 2:
  VCC to 3.3V
  GND to GND
  SDA to GPIO 21 (I2C data line)
  SCL to GPIO 22 (I2C clock line)
  XSHUT to a digital pin TX2 (e.g., GPIO 17 )

Motor Connections:
  VCC - 3.3v
  VM - Vin
  GND- GND
  
  STBY - D27
  PMWA - D13
  AIN1 - D12
  AIN2 - D14
  
  PWMB - D26
  BIN1 - D25
  BIN2 - D33
