#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"
#include "motors.h"
#include "sensors.h"
#include "adjust.h"


void setup() {
  setupSensors();
  motorsSetup();
}

void countJunctions(double distance_Right, double distance_Forward, double distance_Left){
  Serial.print("Right: "); Serial.print(distance_Right);
  Serial.print(" Forward: "); Serial.print(distance_Forward);
  Serial.print(" Left: "); Serial.println(distance_Left);

  walls_prev = walls_current;  

  if(distance_Right <= WALLDIST && distance_Left <= WALLDIST) {
    walls_current = both_walls;
  } else if(distance_Right <= WALLDIST && distance_Left > WALLDIST) {
    walls_current = right_wall;
  } else if(distance_Right > WALLDIST && distance_Left <= WALLDIST) {
      walls_current = left_wall;
  } else {
    walls_current = no_walls;
  }
  
  if( walls_current == right_wall && walls_prev == both_walls) {
      left_turns_counter++;
  }
  if( walls_current == left_wall && walls_prev == both_walls ) {
      right_turns_counter++;
  }
  if (walls_current == no_walls && walls_prev == both_walls){
      right_turns_counter++;
      left_turns_counter++;
  }

}


void loop() {
  distance_Right = RightSensor.readRange();
  distance_Forward = ForwardSensor.readRange();
  distance_Left = LeftSensor.readRange();

  countJunctions(distance_Right, distance_Forward, distance_Left);

  // Check if we've reached the end of the turns vector
  if (vec_index >= turns.size()) {
    stop_moving();
    return;
  }

  auto current_turn = turns[vec_index];

  if (current_turn.first == right && right_turns_counter >= current_turn.second) {
    move_right(TURN_DURATION);
    right_turns_counter = 0; // Reset the counter after turning
    vec_index++;
  } else if (current_turn.first == left && left_turns_counter >= current_turn.second) {
    move_left(TURN_DURATION);
    left_turns_counter = 0; // Reset the counter after turning
    vec_index++;
  } else {
    move_forward();
    adjust_course();
  }

  delay(100);
}
