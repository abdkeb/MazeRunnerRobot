#ifndef SENSORS_H
#define SENSORS_H


#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <vector>
#include <utility>
#include <WiFi.h>
#include <WebSerial.h>  // For std::pair
#include "defs.h"

void updateState();
void TransmitValues();
void setupSensors() {

  // Initialize I2C
  Wire.begin();

  // Initialize XSHUT pins
  pinMode(XSHUT_PIN1, OUTPUT);
  pinMode(XSHUT_PIN2, OUTPUT);
  pinMode(XSHUT_PIN3, OUTPUT);

  // Reset both sensors by setting XSHUT low, then high
  digitalWrite(XSHUT_PIN1, LOW);
  digitalWrite(XSHUT_PIN2, LOW);
  digitalWrite(XSHUT_PIN3, LOW);
  delay(10);
  digitalWrite(XSHUT_PIN1, HIGH);
  delay(10);

  // Initialize right sensor
  if (!RightSensor.begin(0x30)) { // Change the address to 0x30
    Serial.println(F("Failed to boot right sensor"));
    while (1);
  }
  // Reset and initialize forward sensor
  digitalWrite(XSHUT_PIN2, HIGH);
  delay(10);

  if (!ForwardSensor.begin(0x31)) { // Change the address to 0x31
    Serial.println(F("Failed to boot forward sensor"));
    while (1);
  }

  // Reset and initialize left sensor
  digitalWrite(XSHUT_PIN3, HIGH);
  delay(10);
  if (!LeftSensor.begin(0x32)) { // Change the address to 0x32
    Serial.println(F("Failed to boot left sensor"));
    while (1);
  }

  Serial.println(F("VL53L0X three sensors setup complete"));
}

//update the buffer according to the dostanses
void updateBuffers(){
  buffer_index = (buffer_index+1)%BUFF_SIZE;
  // if(distance_Right <= WALL_DIST){
  rightSensorBuff[buffer_index] = distance_Right; 
  leftSensorBuff[buffer_index] = distance_Left; 
  FrontSensorBuff[buffer_index] = distance_Forward;
}

void getMeasurments(){
  RightSensor.rangingTest(&measureRight, false);
  ForwardSensor.rangingTest(&measureForward, false);
  LeftSensor.rangingTest(&measureLeft, false);

  distance_Right = measureRight.RangeMilliMeter;
  distance_Forward = measureForward.RangeMilliMeter;
  distance_Left = measureLeft.RangeMilliMeter;

  updateBuffers();
  updateState(); 

  prev_distance_Right = rightSensorBuff[(buffer_index+1)%BUFF_SIZE];
  prev_distance_Forward = FrontSensorBuff[(buffer_index+1)%BUFF_SIZE];
  prev_distance_Left = leftSensorBuff[(buffer_index+1)%BUFF_SIZE];

  TransmitValues();
  // WebSerial.print("Walls status: "); WebSerial.print(walls_current);
  // WebSerial.print(" Right: "); WebSerial.print(distance_Right);
  // WebSerial.print(" Forward: "); WebSerial.print(distance_Forward);
  // WebSerial.print(" Left: "); WebSerial.println(distance_Left);

}

void countJunctions() {
  bool had_left_wall = (walls_prev == left_wall || walls_prev == both_walls);
  bool had_right_wall = (walls_prev == right_wall || walls_prev == both_walls);
  bool has_left_wall = (walls_current == left_wall || walls_current == both_walls);
  bool has_right_wall = (walls_current == right_wall || walls_current == both_walls);

  left_turns_counter += (had_left_wall && !has_left_wall);
  right_turns_counter += (had_right_wall && !has_right_wall);

  walls_prev = walls_current;
  
}

void updateState(){
  walls_prev = walls_current;

  int left_sum=0,right_sum=0; 
  for(int i=0; i<BUFF_SIZE; i++){
    left_sum +=  (leftSensorBuff[i]  <= WALL_DIST);
    right_sum += (rightSensorBuff[i] <= WALL_DIST);
  }
  
  if(left_sum == BUFF_SIZE && right_sum == BUFF_SIZE){
    walls_current = both_walls; 
  }else if(left_sum == BUFF_SIZE){
    walls_current = left_wall; 
  }else if(right_sum == BUFF_SIZE){
    walls_current = right_wall; 
  }else if(right_sum != BUFF_SIZE && left_sum != BUFF_SIZE){
	  walls_current = no_walls;
  }
  countJunctions(); 
  return;
  // digitalWrite(LEFT_LED_PIN,LOW);
  // digitalWrite(RIGHT_LED_PIN,LOW);  
}

void TransmitValues(){
      std::ostringstream oss;
      oss << "Walls status: " << walls_current << " Prev status: " << walls_prev << " Right: " << distance_Right << " Forward: " << distance_Forward << " Left: " << distance_Left << " Right Counter: " << right_turns_counter << " Left Counter: " << left_turns_counter ;
      if(debug_mode){WebSerial.println((oss.str()).c_str());}
      Serial.println((oss.str()).c_str());
}




#endif // SENSORS_H
