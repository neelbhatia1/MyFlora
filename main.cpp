// NodeMCU ESP826612E, BME280
// MicroController reads data from a BME280 and sends live updates to a web server

#include "setup.h"

void setup() {
  // Let system stabilize
  delay(2000);

  // Start the serial communication
  Serial.begin(9600);
  Serial.flush();

  bmeSetup();
  webSetup();

}

void loop() {
  // Listen for incoming client requests
  server.handleClient();
  delay(500);
}