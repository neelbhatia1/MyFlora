#include "init.h"  

void setup() {
  Serial.begin(9600);
  delay(2000);  // Wait for serial monitor to initialize
  // Initialize SD card
  while ( sdSetup() == false ) { delay(2000); }
  bmeSetup();
  wifiSetup();

  listFiles(SD.open("/"));

  deleteAllFiles();

  listFiles(SD.open("/"));

  if (!SD.exists("/csvTesting02.csv") ) {
    writeHeader();
  }

}

void loop() {
  timeClient.update();

  Serial.print("Epoch time: ");
  Serial.println(timeClient.getEpochTime());

  Serial.print("Current time: ");
  Serial.println(timeClient.getFormattedTime());

  Serial.print("Current date: ");
  Serial.println(getDate());


  for ( int i = 0; i < 5; i ++ ) {
    logData();
    delay(5000);
  }

  listFiles(SD.open("/"));
  printFile();
  
  delay(2000);
}

