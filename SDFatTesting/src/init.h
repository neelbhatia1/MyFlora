#ifndef INIT_H  // Include guard to prevent multiple inclusions
#define INIT_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

// Create an SdFat object
SdFat SD;

// Define the chip select pin (CS) for your SD card module
const int chipSelect = D8;

void wipeSDCard();
void listFiles();

// Function to wipe the SD card (delete all files)
void wipeSDCard() {
    Serial.println("Wiping SD card...");
  
      // Use wipe() for no dot progress indicator.
  if (!SD.format(&Serial)) {
    SD.errorHalt("Wipe failed.");
  }
  // Must reinitialize after wipe.
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!SD.begin(chipSelect, SD_SCK_MHZ(50))) {
    SD.errorHalt("Second init failed.");
  }
  Serial.println("SD card wiped.");
}
  
 // Function to list all files on the SD card
void listFiles() {
    FatFile dir;
    FatFile file;
    Serial.println("Listing files on SD card:");
    if (!dir.open("/")) {
        Serial.println("dir.open failed");
    }
    // Open next file in root.
    // Warning, openNext starts at the current position of dir so a
    // rewind may be necessary in your application.
    while (file.openNext(&dir, O_RDONLY)) {
        file.printFileSize(&Serial);
        Serial.write(' ');
        file.printModifyDateTime(&Serial);
        Serial.write(' ');
        file.printName(&Serial);
        if (file.isDir()) {
            // Indicate a directory.
            Serial.write('/');
        }
        Serial.println();
        file.close();
    }
    if (dir.getError()) {
        Serial.println("openNext failed");
    } else {
        Serial.println("Done!");
    }
}

#endif