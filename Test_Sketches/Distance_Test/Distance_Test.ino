#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Drop point
double target_latitude = 40.236684;
double target_longitude = -105.005942;
double drop_tolerance = 10.0; // meters


// Create object instances
TinyGPSPlus gps;
SoftwareSerial ss(2, 3);

void setup() {
  Serial.begin(115200); // Startup debug serial
  ss.begin(4800); // GPS software serial
  Serial.println("Distance to target:");
}

void loop() {
  // If there is a valid GPS meSerialage, check if we are in drop position
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      // Compare drop position to the current position
      double distance_to_target =
        TinyGPSPlus::distanceBetween(
          gps.location.lat(),
          gps.location.lng(),
          target_latitude, 
          target_longitude);
      Serial.println(distance_to_target);
    }
  }
}



