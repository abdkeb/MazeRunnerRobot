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

// Define XSHUT pins
#define XSHUT_PIN1 17 //right sensor
#define XSHUT_PIN2 16 //forward sensor
#define XSHUT_PIN3 3  //left sensor

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

void move_forward() {
  // Move Motor A forward at defined speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, LEFT_SPEED);

  // Move Motor B forward at defined speed
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, RIGHT_SPEED);
}

void stop_moving() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void move_right(int duration) {
  move_forward();
  delay(500);
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, FULLSPEED);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, FULLSPEED);
  }
  stop_moving();
}

void move_left(int duration) {
  move_forward();
  delay(500);
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, FULLSPEED);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, FULLSPEED);
  }
  stop_moving();
}

void move_backward(int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, FULLSPEED);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, FULLSPEED);
  }
  stop_moving();
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

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

  // Set control pins as outputs for Motor A
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  // Set control pins as outputs for Motor B
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Ensure the motor driver is out of standby mode
  digitalWrite(STBY, HIGH);
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



// // Update right counter
  // if (all_right_less_than_150) {
  //   saw_wall_right = true;
  // } else if (saw_wall_right && all_right_greater_than_300) {
  //   right_turns_counter++;
  //   saw_wall_right = false;
  //   Serial.print("Right Junction Count: "); Serial.println(right_turns_counter);
  // }
  

  // Update the right distances array
  // right_distances[right_index] = distance_Right;
  // right_index = (right_index + 1) % filter_size;

  // // Update the left distances array
  // left_distances[left_index] = distance_Left;
  // left_index = (left_index + 1) % filter_size;

  // // Check if all recent right distances are > 300
  // bool all_right_greater_than_300 = true;
  // for (int i = 0; i < filter_size; i++) {
  //   if (right_distances[i] <= 300) {
  //     all_right_greater_than_300 = false;
  //     break;
  //   }
  // }

  // // Check if all recent left distances are > 300
  // bool all_left_greater_than_300 = true;
  // for (int i = 0; i < filter_size; i++) {
  //   if (left_distances[i] <= 300) {
  //     all_left_greater_than_300 = false;
  //     break;
  //   }
  // }

  // // Check if all recent right distances are < 150
  // bool all_right_less_than_150 = true;
  // for (int i = 0; i < filter_size; i++) {
  //   if (right_distances[i] >= 150) {
  //     all_right_less_than_150 = false;
  //     break;
  //   }
  // }

  // // Check if all recent left distances are < 150
  // bool all_left_less_than_150 = true;
  // for (int i = 0; i < filter_size; i++) {
  //   if (left_distances[i] >= 150) {
  //     all_left_less_than_150 = false;
  //     break;
  //   }
  // }

  // // Update right counter
  // if (all_right_less_than_150) {
  //   saw_wall_right = true;
  // } else if (saw_wall_right && all_right_greater_than_300) {
  //   right_turns_counter++;
  //   saw_wall_right = false;
  //   Serial.print("Right Junction Count: "); Serial.println(right_turns_counter);
  // }

  // // Update left counter
  // if (all_left_less_than_150) {
  //   saw_wall_left = true;
  // } else if (saw_wall_left && all_left_greater_than_300) {
  //   left_turns_counter++;
  //   saw_wall_left = false;
  //   Serial.print("Left Junction Count: "); Serial.println(left_turns_counter);
  // }
}

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

std::vector<std::pair<direction, int>> turns = { {right, 2}, {left, 2} };
int vec_index = 0;

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
