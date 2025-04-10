#include "init.h"

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Initialize SD card
  while (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    delay(1000);
  }
  Serial.println("SD card initialized.");

  delay(2000);
  listFiles();

  
  // Open a file for writing
  FatFile dataFile = SD.open("test.txt", FILE_WRITE);

  // Check if the file opened successfully
  if (dataFile) {
    dataFile.write("Hello, SD Card!");
    dataFile.close();  // Close the file
    Serial.println("Data written to file.");
  } else {
    Serial.println("Error opening test.txt");
  }

  delay(2000);
  listFiles();

  // Now let's read from the file
  FatFile readFile = SD.open("test.txt", FILE_READ);
  if (readFile) {
    Serial.println("Reading from file:");
    while (readFile.available()) {
      Serial.write(readFile.read());
    }
    readFile.close();
    Serial.println("\n");
  } else {
    Serial.println("Error opening test.txt for reading");
  }
  
  delay(2000);
  listFiles();

  delay(2000);
  wipeSDCard();

  delay(2000);
  listFiles();
}

void loop() {
  // Nothing here for now
}