#include "esp32-hal-gpio.h"
#ifndef MOTORS_H
#define MOTORS_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"

void move_forward();
void stop_moving();
void rotate_right(int duration);
void moveRight(int duration);
void move_left(int duration);
void turnRight();
void move_backward(int duration);

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

void rotate_left(int duration) {
  // digitalWrite(LEFT_LED_PIN, HIGH);
  // digitalWrite(RIGHT_LED_PIN, HIGH);

  unsigned long startTime = millis();
  double turn_speed = (0.2 * FULLSPEED);
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, turn_speed);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, turn_speed);
  }
  stop_moving();
  // digitalWrite(LEFT_LED_PIN, LOW);
  // digitalWrite(RIGHT_LED_PIN, LOW);
}


void rotate_right(int duration) {
  // digitalWrite(LEFT_LED_PIN, HIGH);
  // digitalWrite(RIGHT_LED_PIN, HIGH);
  unsigned long startTime = millis();
  double turn_speed = (0.2 * FULLSPEED);
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, turn_speed);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, turn_speed);
  }
  stop_moving();
  // digitalWrite(LEFT_LED_PIN, LOW);
  // digitalWrite(RIGHT_LED_PIN, LOW);
}

// Function to gradually turn right by making the left motor move faster than the right motor
void move_right() {
  // Define the speed difference between the motors
  const int leftMotorSpeed = FULLSPEED;          // Speed for the left motor
  const int rightMotorSpeed = FULLSPEED * 0.35;  // Speed for the right motor, slower to create a right turn

  // Continue turning right for the specified duration
  // Move the left motor forward at full speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, leftMotorSpeed);

  // Move the right motor forward at reduced speed
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, rightMotorSpeed);
}

void move_left() {
  // Define the speed difference between the motors
  const int leftMotorSpeed = FULLSPEED * 0.35;  // Speed for the left motor, slower to create a left turn
  const int rightMotorSpeed = FULLSPEED;        // Speed for the right motor

  // Move the left motor forward at reduced speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, leftMotorSpeed);

  // Move the right motor forward at full speed
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, rightMotorSpeed);
}


void move_left(int duration) {
  double turn_speed = (0.2 * FULLSPEED);
  digitalWrite(LEFT_LED_PIN, HIGH);
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
  digitalWrite(LEFT_LED_PIN, LOW);
}

void turnLeft() {
  digitalWrite(LEFT_LED_PIN, HIGH);
  // Constants for determining when to stop turning
  const int wallDetectionThreshold = 180;  // Distance to consider a sensor has detected a wall
  const int frontClearThreshold = 250;     // Distance to consider the front is clear
  const int closeToCornerThreshold = 100;  // Distance to consider the robot too close to the corner

  double frontDistanceChange;

  while (true) {
    getMeasurments();
    TransmitValues();
    frontDistanceChange = distance_Forward - prev_distance_Forward;
    bool leftWallDetected = (distance_Left < wallDetectionThreshold);
    bool rightWallDetected = (distance_Right < wallDetectionThreshold);
    bool frontClear = (distance_Forward > frontClearThreshold);

    // Finished the turn
    if (leftWallDetected && rightWallDetected) {
      stop_moving();  // Stop the motors
      break;          // Exit the function
    }

    // If the robot is too close to the right corner side, perform a left rotation
    // if (distance_Forward < closeToCornerThreshold && frontDistanceChange < 0 && distance_Right < 70) {
    //   stop_moving();
    //   rotate_left(200);  // Perform a full left rotation
    //   continue;
    // }

    // If the robot is too close to the left corner side, perform a right rotation
    if (distance_Forward < closeToCornerThreshold && frontDistanceChange < 0 && distance_Left < 70) {
      stop_moving();
      rotate_right(200);  // Perform a full right rotation
      continue;
    }

    if(distance_Forward < 50 ){
      move_backward(300);
    }

    if(distance_Right < 56){
      move_backward(400);
      break;
    }

    move_left();
    delay(10);
  }
  digitalWrite(LEFT_LED_PIN, LOW);
}


void turnRight() {
  digitalWrite(RIGHT_LED_PIN, HIGH);
  // Constants for determining when to stop turning
  const int wallDetectionThreshold = 180;  // Distance to consider a sensor has detected a wall
  const int frontClearThreshold = 250;     // Distance to consider the front is clear
  const int closeToCornerThreshold = 100;  // Distance to consider the robot too close to the corner

  double currentFrontDistance;
  double frontDistanceChange;

  while (true) {
    getMeasurments();
    TransmitValues();
    frontDistanceChange = distance_Forward - prev_distance_Forward;
    bool leftWallDetected = (distance_Left < wallDetectionThreshold);
    bool rightWallDetected = (distance_Right < wallDetectionThreshold);
    bool frontClear = (distance_Forward > frontClearThreshold);

    // finished the turn
    if (leftWallDetected && rightWallDetected) {
      stop_moving();  // Stop the motors
      break;          // Exit the function
    }

    if(distance_Forward < 50 ){
      move_backward(300);
    }

    if(distance_Right < 56){
      move_backward(400);
      break;
    }


    // // If the robot is too close to the left corner side, perform a right rotation
    // if (distance_Forward < closeToCornerThreshold && frontDistanceChange > 0) {
    //   stop_moving();
    //   // rotate_right(200);     // Perform a full right rotation
    //   continue;
    // }

    if (distance_Forward < closeToCornerThreshold && frontDistanceChange < 0 && distance_Right < 70) {
      stop_moving();
      rotate_left(200);  // Perform a full left rotation
      continue;
    }

    move_right();
    delay(10);
  }
  digitalWrite(RIGHT_LED_PIN, LOW);
}

// void move_right(int duration) {
//   digitalWrite(RIGHT_LED_PIN,HIGH);
//   // move_forward();
//   // delay(600 );

//   // double turn_speed = (0.2*FULLSPEED);
//   digitalWrite(AIN1, HIGH);
//   digitalWrite(AIN2, LOW);
//   analogWrite(PWMA,FULLSPEED );

//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, LOW);
//   analogWrite(PWMB, FULLSPEED/4);

//   // Wait for encoder to reach a value that represents a 90 degree turn
//   encoderValueA = 0; // Reset encoder value for motor A
//   encoderValueB = 0; // Reset encoder value for motor B
//   long targetCount_A = 1050*2; // Start with an estimated value   #almost 90 degrees with speed of 200
//   long targetCount_B = 1050/2; // Start with an estimated value   #almost 90 degrees with speed of 200
//   while (abs(encoderValueA) <= targetCount_A or abs(encoderValueB) <= targetCount_B) {
//     // Wait
//   }
//   move_forward();
//   // delay(600 );
//   // stop_moving();
//   digitalWrite(RIGHT_LED_PIN,LOW);
// }

// void move_left(int duration) {
//   double turn_speed = (0.2*FULLSPEED);
//   digitalWrite(LEFT_LED_PIN,HIGH);
//   // move_forward();
//   // delay(600);

//   digitalWrite(AIN1, HIGH);
//   digitalWrite(AIN2, LOW);
//   analogWrite(PWMA, FULLSPEED/4);

//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, LOW);
//   analogWrite(PWMB, FULLSPEED);
//   // Wait for encoder to reach a value that represents a 90 degree turn
//   encoderValueA = 0; // Reset encoder value for motor A
//   encoderValueB = 0; // Reset encoder value for motor B
//   long targetCount_A = 1100/2; // Start with an estimated value   #almost 90 degrees with speed of 200
//   long targetCount_B = 1100*2; // Start with an estimated value   #almost 90 degrees with speed of 200
//   while (abs(encoderValueA) <= targetCount_A or abs(encoderValueB) <= targetCount_B) {
//     // Wait
//   }
//   // move_forward();
//   // delay(600 );
//   stop_moving();
//   digitalWrite(LEFT_LED_PIN,LOW);
// }

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




#endif  // MOTORS_H
