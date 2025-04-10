#ifndef INIT_H
#define INIT_H

// ************************* LIBRARIES **************************
// Arduino/C++ Libraries
#include <Arduino.h>
#include <string.h>
// microSD Libraries
#include <SPI.h>
#include <SD.h>

// Function to list files in the given directory
void listFiles(File dir) {
    // Check if the directory is open
    if (!dir) {
        Serial.println("Failed to open directory.");
        return;
    }

    // List all files and directories in the directory
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
        // No more files
        break;
        }

        // Print file name and size
        if (entry.isDirectory()) {
        Serial.print("[DIR] ");
        } else {
        Serial.print("[FILE] ");
        }
        Serial.print(entry.name());
        Serial.print(" - ");
        Serial.print(entry.size());
        Serial.println(" bytes");

        entry.close();  // Close the file when done
    }
};

#endif;