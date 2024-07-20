#ifndef ADJUST_H
#define ADJUST_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"
#include "motors.h"

void adjust_course() {
  

  // If there's an obstacle in front
  while (measureForward.RangeStatus != 4 && distance_Forward < 150) {
    getMeasurments();
    stop_moving();
    delay(500);  // Short pause before turning
  }

  right_speed_adjustment = RIGHT_SPEED;
  left_speed_adjustment = LEFT_SPEED;

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
          move_forward();
        // if (distance_Left < ADJUST_WALL_DIST) {
        //     right_speed_adjustment = RIGHT_SPEED - 0.6 * (ADJUST_WALL_DIST - distance_Left);
        // } else {
        //     left_speed_adjustment = LEFT_SPEED - 0.6 * (ADJUST_WALL_DIST - distance_Left);
        // }
        break;

    case right_wall:
        move_forward();
        // if (distance_Right < ADJUST_WALL_DIST) {
        //     left_speed_adjustment = LEFT_SPEED - 0.6 * (ADJUST_WALL_DIST - distance_Right);
        // } else {
        //     right_speed_adjustment = RIGHT_SPEED - 0.6 * (ADJUST_WALL_DIST - distance_Right);
        // }
        break;

    default:
        // Handle the case where no walls are detected or an invalid value
        // left_speed_adjustment = LEFT_SPEED;
        // right_speed_adjustment = RIGHT_SPEED;
        break;
}

}


#endif // ADJUST_H
