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


int correctSpeed(int speed) {
  if (speed <= 0)
    return 0;
  if (speed >= 255)
    return 255;
  return speed;
}


void adjust_course() {

  // If there's an obstacle in front
  while (measureForward.RangeStatus != 4 && distance_Forward < 80) {
    getMeasurments();
    stop_moving();
    if (distance_Right > distance_Left+40) {
      digitalWrite(RIGHT_LED_PIN, HIGH);
      rotate_right(200);
      digitalWrite(RIGHT_LED_PIN, LOW);
      continue;
    }
    if ( distance_Left > distance_Right+40) {
      digitalWrite(LEFT_LED_PIN, HIGH);
      rotate_left(200);
      digitalWrite(LEFT_LED_PIN, LOW);
      continue;
    }
    delay(500);  // Short pause before turning
  }
  // if(distance_Left < 50){
  //   rotate_right(200);
  // }
  // if(distance_Right < 50){
  //   rotate_left(200);
  // }

  right_speed_adjustment = RIGHT_SPEED;
  left_speed_adjustment = LEFT_SPEED;

  double diff_left_distaces = distance_Left - prev_distance_Left;
  double diff_right_distaces = distance_Right - prev_distance_Right;
  bool closing_to_left = diff_left_distaces <= 0;
  bool closing_to_right = diff_right_distaces <= 0;
  double lamda = 0.7;

  // If the robot is too close to the right wall, steer left
  switch (walls_current) {
    case both_walls:
      if (distance_Right < 180 && closing_to_right) {
        if (distance_Right < 95) {
          left_speed_adjustment = distance_Right*0.6;
        } else {
          left_speed_adjustment = LEFT_SPEED - lamda * (180 - distance_Right);
        }
      }
      if (distance_Left < 180 && closing_to_left) {
        if (distance_Left < 95) {
          right_speed_adjustment = distance_Left*0.6;

        } else {
          right_speed_adjustment = RIGHT_SPEED - lamda * (180 - distance_Left);
        }
      }
      break;

    case left_wall:
      if (closing_to_left && distance_Left < 120) {
        right_speed_adjustment = RIGHT_SPEED - lamda * (120 - distance_Left);
      } else if (!closing_to_left && distance_Left > 120) {
        left_speed_adjustment = LEFT_SPEED - lamda * (120 - distance_Left / 2);
      }
      break;

    case right_wall:

      if (closing_to_right && distance_Right < 120) {
        left_speed_adjustment = LEFT_SPEED - lamda * (120 - distance_Right);
      } else if (!closing_to_right && distance_Right > 120) {
        right_speed_adjustment = RIGHT_SPEED - lamda * (120 - distance_Right / 2);
      }
      break;

    default:
      break;
  }
}




#endif  // ADJUST_H