#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>  // For std::pair
#include "defs.h"
#include "sensors.h"
#include "adjust.h"
#include "motors.h"



void setup() {
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  setupSensors();
  motorsSetup();
}


void countJunctions(){

  
  if( walls_current == right_wall && walls_prev != right_wall ) {
      left_turns_counter++;
  }
  if( walls_current == left_wall && walls_prev != left_wall) {
      right_turns_counter++;
  }
  if (walls_current == no_walls && walls_prev == both_walls ){
      right_turns_counter++;
      left_turns_counter++;
  }
  walls_prev = walls_current;
}

//update the buffer according to the dostanses
void updateBuffers(){
  buffer_index = (buffer_index+1)%BUFF_SIZE;
  if(distance_Right <= WALL_DIST){
    rightSensorBuff[buffer_index] = 1; 
  }else{
    rightSensorBuff[buffer_index] = 0;
  }

  if(distance_Left <= WALL_DIST){
   leftSensorBuff[buffer_index] = 1; 
  }else{
    leftSensorBuff[buffer_index] = 0;
  }
}

void updateState(){
  updateBuffers();
  walls_prev =walls_current;

  int left_sum=0,right_sum=0; 
  for(int i=0; i<BUFF_SIZE; i++){
    left_sum += leftSensorBuff[i];
    right_sum += rightSensorBuff[i];
  }
  
  if(left_sum == BUFF_SIZE && right_sum == BUFF_SIZE){
    // digitalWrite(LEFT_LED_PIN,HIGH);
    // digitalWrite(RIGHT_LED_PIN,HIGH);
    walls_current = both_walls; 
    return;
  }
  if(left_sum == BUFF_SIZE){
    // digitalWrite(LEFT_LED_PIN,HIGH);
    // digitalWrite(RIGHT_LED_PIN,LOW);
    walls_current = left_wall; 
    return;
  }
  if(right_sum == BUFF_SIZE){
    walls_current = right_wall; 
    // digitalWrite(LEFT_LED_PIN,LOW);
    // digitalWrite(RIGHT_LED_PIN,HIGH);
    return;
  }
  walls_current = no_walls;
  // digitalWrite(LEFT_LED_PIN,LOW);
  // digitalWrite(RIGHT_LED_PIN,LOW);  

}

void loop() {

  getMeasurments();

  updateState();
  countJunctions();
  Serial.print("Walls status: "); Serial.print(walls_current);
  

  // Check if we've reached the end of the turns vector
  if (vec_index >= turns.size()) {
    stop_moving();
    return;
  }

  auto current_turn = turns[vec_index];

  if (current_turn.first == right && right_turns_counter >= current_turn.second) {
    move_right(RIGHT_TURN_DURATION);
    right_turns_counter = 0; // Reset the counter after turning
    left_turns_counter = 0;
    vec_index++;
  } else if (current_turn.first == left && left_turns_counter >= current_turn.second) {
    move_left(LEFT_TURN_DURATION);
    left_turns_counter = 0; // Reset the counter after turning
    right_turns_counter = 0;
    vec_index++;
  } else {
    move_forward();
    adjust_course();
  }

  delay(10);
}