#include "init.h"

const int chipSelect = D8;  

void setup() {
  Serial.begin(9600);
  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  Serial.println("SD Card is initialized.");

  listFiles(SD.open("/"));
}

void loop() {
}

