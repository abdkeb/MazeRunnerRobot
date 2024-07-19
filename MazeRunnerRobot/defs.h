#ifndef DEFS_H
#define DEFS_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair

#define WALLDIST 200

#define FULLSPEED 200
// Define pin connections for Motor A
#define STBY 27   // Standby pin
#define PWMA 13   // PWM control for Motor A speed
#define AIN1 14   // Control pin 1 for Motor A direction  - left motor
#define AIN2 12   // Control pin 2 for Motor A direction

// Define pin connections for Motor B
#define PWMB 26   // PWM control for Motor B speed
#define BIN1 25   // Control pin 1 for Motor B direction  -- right motor
#define BIN2 33   // Control pin 2 for Motor B direction

#define TURN_DURATION 620

#define RIGHT_SPEED 255
#define LEFT_SPEED 255

// Define XSHUT pins
#define XSHUT_PIN1 17 //right sensor
#define XSHUT_PIN2 16 //forward sensor
#define XSHUT_PIN3 3  //left sensor

typedef enum direction {
    left,
    right
} direction;

typedef enum wall_status{
  no_walls,
  left_wall,
  right_wall,
  both_walls
} wall_status;

// Create VL53L0X objects
Adafruit_VL53L0X RightSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X ForwardSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X LeftSensor = Adafruit_VL53L0X();

double distance_Right = 0;
double distance_Forward = 0;
double distance_Left = 0;

wall_status walls_current = no_walls;
wall_status walls_prev = no_walls;

int right_turns_counter = 0;
int left_turns_counter = 0;

bool saw_wall_right = false;
bool saw_wall_left = false;

const int filter_size = 3;
int right_distances[filter_size] = {0, 0, 0};
int left_distances[filter_size] = {0, 0, 0};
int right_index = 0;
int left_index = 0;

int vec_index = 0;
std::vector<std::pair<direction, int>> turns = { {right, 2}, {left, 2} };

#endif // DEFS_H
