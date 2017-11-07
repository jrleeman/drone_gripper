/*
 * GPS_Gripper.ino
 *
 * Release the contents of a gripper when within proximity of given GPS
 * coordinates.
 * 
 * Code Repo: https://github.com/jrleeman/Drone-Gripper
 * 
 * NeceSerialary External Libraries:
 * https://github.com/mikalhart/TinyGPSPlus
 * 
*/

#include <TinyGPS++.h>
#include <Servo.h>

// Variables and defines
#define servo_pin 9 // Change for your setup/board
#define button_pin 5 // Change for your setup/board

// Drop point
double target_latitude = 40.236684;
double target_longitude = -105.005942;
double drop_tolerance = 10.0; // meters

// Variable Declarations
bool gripper_is_closed = false;
int gripper_closed_position = 60;
int gripper_open_position = 140;

// Create object instances
TinyGPSPlus gps;
Servo gripperServo;

void setup() {
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
 
  gripperServo.attach(servo_pin);
  
  Serial.begin(4800); // Startup serial

  // Start with the gripper open
  digitalWrite(LED_BUILTIN, LOW);
  gripperServo.write(gripper_open_position);
}

void loop() {
  // If there is a valid GPS meSerialage, check if we are in drop position
  while (Serial.available() > 0){
    if (gps.encode(Serial.read())){
      // Compare drop position to the current position
      double distance_to_target =
        TinyGPSPlus::distanceBetween(
          gps.location.lat(),
          gps.location.lng(),
          target_latitude, 
          target_longitude);
      if (distance_to_target<=drop_tolerance){
        // Time to drop! Toggle gripper position and shutdown.
        toggleGripper();
        shutdown();
      }
    }
  }

  // Check if the button is preSerialed to toggle the gripper state
  if (digitalRead(button_pin)==LOW) {
    delay(100); // Naive debounce
    while(digitalRead(button_pin)==LOW) {
    }
    toggleGripper();
  }
}


void toggleGripper() {
  // Change the gripper state to whatever it isn't currently.
  if (gripper_is_closed == true){
    gripperServo.write(gripper_open_position);
    digitalWrite(LED_BUILTIN, LOW);
    gripper_is_closed = false;
  }
  else {
    gripperServo.write(gripper_closed_position);
    digitalWrite(LED_BUILTIN, HIGH);
    gripper_is_closed = true;
  }
}


void shutdown(){
  // Spin forever
  while(true){
  }
}

