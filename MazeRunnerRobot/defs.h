#ifndef DEFS_H
#define DEFS_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <WebSerial.h>


//debug
#define LED 2
const bool debug_mode = 0;
AsyncWebServer server(80);
const char* ssid = "Majd";          // Your WiFi SSID
const char* password = "majdkhazen123";  // Your WiFi Password
bool got_ip = false;


#define LEFT_LED_PIN 18
#define RIGHT_LED_PIN 5

#define FULLSPEED 255
// Define pin connections for Motor -------- A
#define STBY 27   // Standby pin
#define PWMA 13   // control PWM   for Motor A speed
#define AIN1 14   // Control pin 1 for Motor A direction - left motor
#define AIN2 12   // Control pin 2 for Motor A direction

// Define pin connections for Motor B
#define PWMB 26   // Control PWM   for Motor B speed
#define BIN1 25   // Control pin 1 for Motor B direction - right motor
#define BIN2 33   // Control pin 2 for Motor B direction

#define RIGHT_TURN_DURATION 490
#define LEFT_TURN_DURATION 510

#define RIGHT_SPEED 200
#define LEFT_SPEED 215
#define WALL_DIST 300
#define ADJUST_WALL_DIST 150

// Define XSHUT pins
#define XSHUT_PIN1 17 //right sensor
#define XSHUT_PIN2 16 //forward sensor
#define XSHUT_PIN3 3  //left sensor

// sessor
#define BUFF_SIZE 5

int leftSensorBuff[BUFF_SIZE]  = {0};
int rightSensorBuff[BUFF_SIZE] = {0};
int FrontSensorBuff[BUFF_SIZE] = {0};

int buffer_index = 0; 

// Encoder pins for Motor A
int encoderPinA1 = 35; // Encoder Output 'A' for motor A
int encoderPinA2 = 34; // Encoder Output 'B' for motor A
volatile int lastEncodedA = 0; // Here updated value of encoder store.
volatile long encoderValueA = 0; // Raw encoder value

// Encoder pins for Motor B
int encoderPinB1 = 4; // Encoder Output 'A' for motor B
int encoderPinB2 = 2; // Encoder Output 'B' for motor B
volatile int lastEncodedB = 0; // Here updated value of encoder store.
volatile long encoderValueB = 0; // Raw encoder value

double Kp = 0.007;
double Kd = 0;
double D = 0;
double P = 0;
double error = 0;
double pidOutput = 0;
double previousError = 0;

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

VL53L0X_RangingMeasurementData_t measureRight;
VL53L0X_RangingMeasurementData_t measureForward;
VL53L0X_RangingMeasurementData_t measureLeft;

double distance_Right = 0;
double prev_distance_Right = 0;

double distance_Forward = 0;
double prev_distance_Forward = 0;

double distance_Left = 0;
double prev_distance_Left = 0;

wall_status walls_current = no_walls;
wall_status walls_prev = no_walls;


int left_speed_adjustment  = LEFT_SPEED;
int right_speed_adjustment = RIGHT_SPEED;

int right_turns_counter = 0;
int left_turns_counter = 0;

bool saw_wall_right = false;
bool saw_wall_left = false;

int vec_index = 0;
std::vector<std::pair<direction, int>> turns = { {right, 1}, {left, 1}, {left, 1}, {right, 1} };

#endif // DEFS_H
