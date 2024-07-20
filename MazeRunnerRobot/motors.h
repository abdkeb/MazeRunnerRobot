#ifndef MOTORS_H
#define MOTORS_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"

void move_forward() {
  // Move Motor A forward at defined speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, left_speed_adjustment);

  // Move Motor B forward at defined speed
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, right_speed_adjustment);
}

void stop_moving() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void move_right(int duration) {
  digitalWrite(RIGHT_LED_PIN,HIGH);
  move_forward();
  delay(800 );
  unsigned long startTime = millis();
  double turn_speed = (0.2*FULLSPEED);
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA,turn_speed );

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, turn_speed);
  }
  move_forward();
  delay(580);
  stop_moving();
  digitalWrite(RIGHT_LED_PIN,LOW);
}

void move_left(int duration) {
  double turn_speed = (0.2*FULLSPEED);
  digitalWrite(LEFT_LED_PIN,HIGH);
  move_forward();
  delay(790);
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, turn_speed);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, turn_speed);
  }
  move_forward();
  delay(580);
  stop_moving();
  digitalWrite(LEFT_LED_PIN,LOW);
}

void move_backward(int duration) {
  
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, FULLSPEED);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, FULLSPEED);
  }
  stop_moving();
  
}

void motorsSetup() {
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




#endif // MOTORS_H
