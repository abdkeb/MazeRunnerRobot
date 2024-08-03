#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include "defs.h"
#include "sensors.h"
#include "adjust.h"
#include "motors.h"
#include "debug.h"
#include "encoder.h"


void setup() {
  Serial.begin(115200);
  while (!Serial);
  if(debug_mode){
  debugSetup();
  }
  setupSensors();
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  motorsSetup();
  encoderSetup();
 
}




void loop() {
  
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  if(debug_mode && !got_ip){
    getIPAddress();
    }
  getMeasurments();
  updateState();
  countJunctions();


  // Check if we've reached the end of the turns vector
  if (vec_index >= turns.size()) {
    stop_moving();
    return;
  }

  auto current_turn = turns[vec_index];

  if (current_turn.first == right && right_turns_counter >= current_turn.second) {
    turnRight();
    right_turns_counter = 0; // Reset the counter after turning
    left_turns_counter = 0;
    vec_index++;
  } else if (current_turn.first == left && left_turns_counter >= current_turn.second) {
    turnLeft();
    left_turns_counter = 0; // Reset the counter after turning
    right_turns_counter = 0;
    vec_index++;
  } else {
    move_forward();
    adjust_course();
  }

  delay(30);
}
