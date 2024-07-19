#ifndef ADJUST_H
#define ADJUST_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"

void adjust_course() {
  VL53L0X_RangingMeasurementData_t measureRight;
  VL53L0X_RangingMeasurementData_t measureForward;
  VL53L0X_RangingMeasurementData_t measureLeft;

  RightSensor.rangingTest(&measureRight, false);
  ForwardSensor.rangingTest(&measureForward, false);
  LeftSensor.rangingTest(&measureLeft, false);

  distance_Right = measureRight.RangeMilliMeter;
  distance_Forward = measureForward.RangeMilliMeter;
  distance_Left = measureLeft.RangeMilliMeter;

  // If there's an obstacle in front
  while (measureForward.RangeStatus != 4 && distance_Forward < 150) {
    stop_moving();
    delay(500);  // Short pause before turning
  }

  int right_speed_adjustment = RIGHT_SPEED;
  int left_speed_adjustment = LEFT_SPEED;

  // If the robot is too close to the right wall, steer left
  switch (walls_current) {
    case both_walls:
        if (distance_Right < 180) {
            left_speed_adjustment = LEFT_SPEED - 0.6 * (180 - distance_Right);
        }
        if (distance_Left < 180) {
            right_speed_adjustment = RIGHT_SPEED - 0.6 * (180 - distance_Left);
        }
        break;

    case left_wall:
        if (distance_Left < 180) {
            right_speed_adjustment = RIGHT_SPEED - 0.3 * (180 - distance_Left);
        } else {
            left_speed_adjustment = LEFT_SPEED - 0.6 * (180 - distance_Left);
        }
        break;

    case right_wall:
        if (distance_Right < 180) {
            left_speed_adjustment = LEFT_SPEED - 0.3 * (180 - distance_Right);
        } else {
            right_speed_adjustment = RIGHT_SPEED - 0.6 * (180 - distance_Right);
        }
        break;

    default:
        // Handle the case where no walls are detected or an invalid value
        left_speed_adjustment = LEFT_SPEED;
        right_speed_adjustment = RIGHT_SPEED;
        break;
}

  // If the robot is in the middle, move straight
  analogWrite(PWMA, left_speed_adjustment);
  analogWrite(PWMB, right_speed_adjustment);
}


#endif // ADJUST_H
