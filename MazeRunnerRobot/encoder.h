#ifndef ENCODER_H
#define ENCODER_H

#include "defs.h"

void updateEncoderA() {
  int MSB = digitalRead(encoderPinA1); // MSB = most significant bit
  int LSB = digitalRead(encoderPinA2); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB; // Converting the 2 pin value to single number
  int sum = (lastEncodedA << 2) | encoded; // Adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValueA--;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValueA++;

  lastEncodedA = encoded; // Store this value for next time
}

void updateEncoderB() {
  int MSB = digitalRead(encoderPinB1); // MSB = most significant bit
  int LSB = digitalRead(encoderPinB2); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB; // Converting the 2 pin value to single number
  int sum = (lastEncodedB << 2) | encoded; // Adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValueB--;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValueB++;

  lastEncodedB = encoded; // Store this value for next time
}

void encoderSetup() {

  pinMode(encoderPinA1, INPUT_PULLUP);
  pinMode(encoderPinA2, INPUT_PULLUP);
  pinMode(encoderPinB1, INPUT_PULLUP);
  pinMode(encoderPinB2, INPUT_PULLUP);

  // Attach interrupts for Motor A
  attachInterrupt(digitalPinToInterrupt(encoderPinA1), updateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinA2), updateEncoderA, CHANGE);

  // Attach interrupts for Motor B
  attachInterrupt(digitalPinToInterrupt(encoderPinB1), updateEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB2), updateEncoderB, CHANGE);
}


#endif // ENCODER_H
