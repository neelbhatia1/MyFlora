#include "init.h"

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  wifiSetup();

  bmeSetup();

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.begin();

  // Initialize SD card
  while (!SD.begin(chipSelect)) {
    Serial.println("SD card Initialization failed!");
    Serial.println(SD.card()->errorCode(), HEX);
    Serial.println(SD.card()->errorData(), HEX);
    delay(3000);
  }

  Serial.println("SD card initialized. Beginning Loop.");

}

void loop() {
  timeClient.update();

  if (csvStreaming && csvFile && csvClient && csvClient->status() == WS_CONNECTED) {
    String line = csvFile.readStringUntil('\n');
    line.trim();
    if (line.length()) {
        csvClient->text(line);
    } else {
        csvFile.close();
        csvStreaming = false;
        csvClient->text("done");
        csvClient = nullptr;
    }
    delay(2); // yield to watchdog
  }



  if (Serial.available()) {
    char command = Serial.read();
    File32 dataFile;

    switch (command) {
      case 'w': {
        Serial.println("Wiping SD card...");
        wipeSDCard();
        break;
      }
      case 'l': {
        Serial.println("Listing files on SD card...");
        SD.ls("/", LS_R);
        Serial.println("Done listing files.");
        break;
      }
      case 'd': {
        Serial.println("Logging data...");
        dataFile = SD.open("Genesis_log_1.csv", FILE_WRITE);
        if (dataFile) {
          logDataToFile(dataFile);
          dataFile.close();  // Close the file
          Serial.println("Data written to file.");
        } else {
          Serial.println("Error opening file for write.");
        }
        break;
      }
      case 'r': {
        Serial.println("Reading Genesis_log_1.csv...");
      
        dataFile = SD.open("Genesis_log_1.csv", FILE_READ);
        if (dataFile) {
          while (dataFile.available()) {
            char c = dataFile.read();
            Serial.write(c);  // Write directly to Serial
          }
          dataFile.close();
          Serial.println("\nDone reading file.");
        } else {
          Serial.println("Error opening file for read.");
        }
        break;
      }
      default: {
        Serial.println("Unknown command. Use 'w' for wipe, 'l' for list, 'd' to log data, 'r' to read logs");
      }
    }
  }
}