#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// #include <WebSerial.h>
#include "defs.h"
#include "sensors.h"
#include "adjust.h"
#include "motors.h"
#include "debug.h"
#include "encoder.h"
#include "comm.h"

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  // if(debug_mode){
  // debugSetup();
  // }
  // else {
  // // commSetup();
  // // commReadData();
  // }
  setupSensors();
  motorsSetup();
  encoderSetup();
}




void loop() {

  if (vec_index > turns.size()) {
    stop_moving();
    return;
  }

  getMeasurments();
  updateState();
  countJunctions();

  if (vec_index < turns.size()) {
    auto current_turn = turns[vec_index];
    if (current_turn.first == right && right_turns_counter >= current_turn.second) {
      turnRight();
      right_turns_counter = 0;  // Reset the counter after turning
      left_turns_counter = 0;
      vec_index++;
    } else if (current_turn.first == left && left_turns_counter >= current_turn.second) {
      turnLeft();
      left_turns_counter = 0;  // Reset the counter after turning
      right_turns_counter = 0;
      vec_index++;
    }
  }

  move_forward();
  if (vec_index == turns.size()) {
    if (distance_Forward < 200) {
      for (int i = 1; i < 6; i++) {
        digitalWrite(LEFT_LED_PIN, HIGH);
        digitalWrite(RIGHT_LED_PIN, LOW);
        rotate_right(500);
        digitalWrite(LEFT_LED_PIN, LOW);
        digitalWrite(RIGHT_LED_PIN, HIGH);
        rotate_left(500);
      }
      digitalWrite(LEFT_LED_PIN, LOW);
      digitalWrite(RIGHT_LED_PIN, LOW);
      stop_moving();
      vec_index++;
    }
  }
  adjust_course();

  delay(30);
}
