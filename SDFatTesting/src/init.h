#ifndef INIT_H
#define INIT_H

// ************************* LIBRARIES **************************
#include <Arduino.h>
// MicroSD Libraries
#include <SPI.h>
#include <SdFat.h>
// WiFi Libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
// Sensor Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// *********************** INITIALIZATIONS ************************
// SD
SdFat SD;
const int chipSelect = D4;

// WIFI
const char *ssid     = "ESPTESTNET";
const char *password = "nbnewspot";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Sensor
#define SEALEVELPRESSURE_HPA (1013.25) // Pressure Details for BME280
Adafruit_BME280 bme; // I2C

// Date & Time
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

bool csvStreaming = false;
File32 csvFile;
AsyncWebSocketClient* csvClient = nullptr;


void bmeSetup();
void wifiSetup();
void wipeSDCard();
void listFiles();
void listMetrics();

void getDate();
int getMonth();
int dstHandler();
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
    AwsEventType type, void *arg, uint8_t *data, size_t len);
void logDataToFile(File32 &dataFile);
void copyCsvFile();




void bmeSetup() {
    Serial.println("\nLocating BME280 Sensor...");
    bool status;
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("BME280 found!");
    bme.setSampling(
        Adafruit_BME280::MODE_NORMAL,
        Adafruit_BME280::SAMPLING_X1, // Temp
        Adafruit_BME280::SAMPLING_X1, // Pressure
        Adafruit_BME280::SAMPLING_X1, // Humidity
        Adafruit_BME280::FILTER_OFF
      );
      
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
    Serial.println(WiFi.localIP());

    timeClient.setTimeOffset(dstHandler());
    timeClient.begin();

}

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

void listMetrics() {
    Serial.print(bme.readTemperature());
    Serial.print(" °C, ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(" hPa, ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
}

void getDate() {
    time_t epochTime = timeClient.getEpochTime();
    Serial.print("Epoch Time: ");
    Serial.println(epochTime);
    
    String formattedTime = timeClient.getFormattedTime();
    Serial.print("Formatted Time: ");
    Serial.println(formattedTime);  
  
    int currentHour = timeClient.getHours();
    Serial.print("Hour: ");
    Serial.println(currentHour);  
  
    int currentMinute = timeClient.getMinutes();
    Serial.print("Minutes: ");
    Serial.println(currentMinute); 
     
    int currentSecond = timeClient.getSeconds();
    Serial.print("Seconds: ");
    Serial.println(currentSecond);  
  
    String weekDay = weekDays[timeClient.getDay()];
    Serial.print("Week Day: ");
    Serial.println(weekDay);    
  
    //Get a time structure
    struct tm *ptm = gmtime ((time_t *)&epochTime); 
  
    int monthDay = ptm->tm_mday;
    Serial.print("Month day: ");
    Serial.println(monthDay);
  
    int currentMonth = ptm->tm_mon+1;
    Serial.print("Month: ");
    Serial.println(currentMonth);
  
    String currentMonthName = months[currentMonth-1];
    Serial.print("Month name: ");
    Serial.println(currentMonthName);
  
    int currentYear = ptm->tm_year+1900;
    Serial.print("Year: ");
    Serial.println(currentYear);
  
    //Print complete date:
    String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    Serial.print("Current date: ");
    Serial.println(currentDate);
  
    Serial.println("");
}

String getFormattedDate() {
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime); 
    
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;

    String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);

    return currentDate;
}

int getMonth() {
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = localtime((time_t *)&epochTime);   
    int month = ptm->tm_mon + 1;     // tm_mon is 0-based (January = 0)
    return month;
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



  

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected\n", client->id());
        client->text("Genesis");
    } 
    else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    } 
    else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;

        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
          String message;
          for (size_t i = 0; i < len; i++) {
            message += (char)data[i];
          }        
          Serial.printf("Data from client %u: %s\n", client->id(), message.c_str());
        
            if (message == "get_temp") {
                float temp = bme.readTemperature();
                client->text("temp:" + String(temp, 2));
            } else if (message == "get_humidity") {
                float hum = bme.readHumidity();
                client->text("humidity:" + String(hum, 2));
            } else if (message == "get_pressure") {
                float pres = bme.readPressure() / 100.0F;  // convert Pa to hPa
                client->text("pressure:" + String(pres, 2));
            } else if (message == "get_csv") {
                if (csvStreaming) return;

                csvFile = SD.open("/Genesis_log_1.csv");
                if (!csvFile) {
                    client->text("error: could not open file");
                    return;
                }
            
                csvStreaming = true;
                csvClient = client;
            }
            }
        }
    }

void logDataToFile(File32 &dataFile) {
    if (dataFile.size() == 0) {
        dataFile.println("Date,Time,Temperature,Humidity,Pressure");
    }

    dataFile.printf("%s,%s,%.2f,%.2f,%.2f\n", getFormattedDate().c_str(), timeClient.getFormattedTime().c_str(),
                    bme.readTemperature(), bme.readHumidity(), bme.readPressure());

    Serial.printf("%s,%s,%.2f,%.2f,%.2f\n", getFormattedDate().c_str(), timeClient.getFormattedTime().c_str(),
                    bme.readTemperature(), bme.readHumidity(), bme.readPressure());
}

void copyCsvFile() {
    File32 original = SD.open("/Genesis_log_1.csv");
    File32 copy = SD.open("/log_copy.csv", FILE_WRITE);
  
    if (!original || !copy) {
      Serial.println("Failed to open files for copying");
      return;
    }
  
    while (original.available()) {
      copy.write(original.read());
      delay(1);
    }
  
    original.close();
    copy.close();
    Serial.println("Copied to /log_copy.csv");
  }
  

#endif