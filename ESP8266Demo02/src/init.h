#ifndef INIT_H
#define INIT_H

// ************************* LIBRARIES **************************
// Arduino/C++ Libraries
#include <Arduino.h>
#include <string.h>
// Sensor Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
// microSD Libraries
#include <SPI.h>
#include <SD.h>
// WiFi Libraries
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

// *************************** SENSOR ***************************
#define SEALEVELPRESSURE_HPA (1013.25) // Pressure Details for BME280
Adafruit_BME280 bme; // I2C

// ************************** microSD ***************************
const int chipSelect = D8;

// **************************** WiFi ****************************
const char *ssid     = "ESPTESTNET";
const char *password = "nbnewspot";
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

// ****************** FUNCTION DECLARATIONS ********************
void bmeSetup();
bool sdSetup();
void wifiSetup();

void listfiles(File dir, int numTabs);
void deleteAllFiles();
void logData();
void writeHeader();
String getDate();
void printFile();
int dstHandler();

// ****************** FUNCTION DEFINITIONS *********************
void bmeSetup() {
    Serial.println("\nLocating BME280 Sensor...");
    bool status;
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("BME280 found!");
}

bool sdSetup() {
    if (!SD.begin(chipSelect)) {
        Serial.println("SD Card initialization failed!");
        return false;
    }
    Serial.println("SD Card is initialized.");
    return true;
}

void wifiSetup() {
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Connected to WiFi");    

    timeClient.setTimeOffset(dstHandler());
    timeClient.begin();

}

// Function to list files in the given directory
void listFiles(File dir, int numTabs = 0) {
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

        // Print indentation for better visual structure
        for (int i = 0; i < numTabs; i++) {
            Serial.print("\t");
        }

        // Print file or directory name and size
        if (entry.isDirectory()) {
            Serial.print("[DIR] ");
            Serial.println(entry.name());

            // If it's a directory, recursively list its contents
            listFiles(entry, numTabs + 1);  // Increase indentation for subdirectories
        } else {
            Serial.print("[FILE] ");
            Serial.print(entry.name());
            Serial.print(" - ");
            Serial.print(entry.size());
            Serial.println(" bytes");
        }

        entry.close();  // Close the file when done
    }
}

void deleteAllFiles() {
  // Open the root directory
  File root = SD.open("/");
  if (!root) {
    Serial.println("Failed to open root directory.");
    return;
  }

  // Iterate through the files in the root directory
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;  // No more files
    }

    // Print file name
    Serial.print("Deleting file: ");
    Serial.println(entry.name());

    // Delete the file
    SD.remove(entry.name());

    // Close the entry file
    entry.close();
  }

  // Close the root directory
  root.close();

  Serial.println("All files deleted.");
}

void logData() {
    // Open the file in append mode to add a new line of data
    File dataFile = SD.open("csvTesting02.csv", FILE_WRITE);
    
    if (dataFile) {
        char message[100];  // Adjust size as needed
        snprintf(message, sizeof(message), "%s, %s, %.2f, %.2f, %.2f",
         getDate().c_str(), timeClient.getFormattedTime().c_str(),
         bme.readTemperature(), bme.readPressure() / 100.0F, bme.readHumidity());

        dataFile.println(message);
        dataFile.flush();  // Ensure data is written to the SD card
        // Close the file after writing
        dataFile.close();
        Serial.println("Data logged.");
    } else {
        Serial.println("Error opening data.csv for data write.");
    }
}

void writeHeader() {
    File dataFile = SD.open("csvTesting02.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println("Date, Time, Temperature, Pressure, Humidity");  // Write the CSV header
      dataFile.close();
      Serial.println("Header written to file.");
    } else {
      Serial.println("Error opening data.csv for header write.");
    }
}

String getDate() {
    // Get and print the current date
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = localtime((time_t *)&epochTime);

    int year = ptm->tm_year + 539;  // tm_year is years since 1900
    int month = ptm->tm_mon + 1;     // tm_mon is 0-based (January = 0)
    int day = ptm->tm_mday - 8;          // tm_mday is the day of the month

    String message = String(month) + "-" + String(day) + "-" + String(year) + "\n";

    return message;
}

int getMonth() {
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = localtime((time_t *)&epochTime);   
    int month = ptm->tm_mon + 1;     // tm_mon is 0-based (January = 0)
    return month;
}

void printFile() {
  // Open the file to read back the data
  File testFile = SD.open("csvTesting02.csv");
  if (testFile) {
    Serial.println("Reading from csvTesting02.csv...");
    while (testFile.available()) {
      Serial.write(testFile.read());  // Print each byte of the file to the serial monitor
    }
    testFile.close();
    Serial.println("\nFile read successfully.");
  } else {
    Serial.println("Error opening testfile.txt for reading.");
  }
}

int dstHandler() {
  int month = getMonth();
  int day = timeClient.getDay();
  
  if (month > 3 && month < 11) {
    return -7 * 3600;  // DST is active
  } 
  if (month < 3 || month > 11) {
    return -8 * 3600;  // Standard Time
  } 
  // Edge cases: Early March or Early November
  if ((month == 3 && day >= 8) || (month == 11 && day < 8)) {
    return -7 * 3600;  // DST
  } else {
    return -8 * 3600;  // Standard
  }
}

#endif