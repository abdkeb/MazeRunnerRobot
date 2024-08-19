#ifndef ADJUST_H
#define ADJUST_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"
#include "motors.h"
#include "sensors.h"


int correctSpeed(int speed){
  if(speed <= 0 )
    return 0;
  if(speed >= 255)
    return 255;
  return speed;
}

// PID Controller for wall-following
void pid_adjust_course() {
    // Get distance measurements from sensors
    getMeasurments(); 
    updateState();
    
    // Calculate the error as the difference between right and left distances
    error = (distance_Right - distance_Left)/2;

    P = Kp * error;

    double dv = (error - previousError );
    D = Kd * dv;
    previousError = error;

    // PID output
    pidOutput = (P + D);
 
    // Adjust motor speeds based on PID output
    right_speed_adjustment = correctSpeed(RIGHT_SPEED * (1 - pidOutput));
    left_speed_adjustment = correctSpeed(LEFT_SPEED * (1 + pidOutput));
    // Move the robot forward with adjusted speeds
    // move_forward();
}

void adjust_course() {

  // If there's an obstacle in front
  while (measureForward.RangeStatus != 4 && distance_Forward < 150) {
    getMeasurments();
    stop_moving();
    delay(500);  // Short pause before turning
  }

  right_speed_adjustment = RIGHT_SPEED;
  left_speed_adjustment = LEFT_SPEED;

  double diff_left_distaces = distance_Left - prev_distance_Left;
  double diff_right_distaces = distance_Right - prev_distance_Right;
  bool closing_to_left = diff_left_distaces < 0; 
  bool closing_to_right = diff_right_distaces < 0;
  double lamda = 0.6;

  // If the robot is too close to the right wall, steer left
  switch (walls_current) {
    case both_walls:
        if (distance_Right < 180 && closing_to_right) {
          if(distance_Right < 88){
            left_speed_adjustment = 0 ;
          }else{
          left_speed_adjustment = LEFT_SPEED - lamda * (180 - distance_Right) ;
          }
        }
        if (distance_Left < 180 && closing_to_left) {
          if(distance_Right < 88){
            right_speed_adjustment = 0;

          }else{
            right_speed_adjustment = RIGHT_SPEED - lamda * (180 - distance_Left);
          }
        }
        break;

    case left_wall:
        move_forward();
        break;

    case right_wall:
        move_forward();
        break;

    default:
        break;
}

}




#endif // ADJUST_H
