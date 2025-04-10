#ifndef SETUP_H
#define SETUP_H

// ************************* LIBRARIES **************************
// Arduino/C++ Libraries
#include <Arduino.h>
#include <string.h>
// Sensor Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
// Web Server Libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ************************* WEB SERVER *************************
ESP8266WebServer server(80); // Create an instance of the server on port 80
const char* ssid = "ESPTESTNET";  // WiFi Details
const char* password = "nbnewspot";

// *************************** SENSOR ***************************
#define SEALEVELPRESSURE_HPA (1013.25) // Pressure Details for BME280
Adafruit_BME280 bme; // I2C

// ************************* FUNCTIONS **************************
void webSetup();
void bmeSetup();
void handleStatus();
void handleRoot();
String printValues();
void flashLED();

// UTILITY FUNCTIONS
void flashLED() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    digitalWrite(LED_BUILTIN, HIGH);
}

// SETUP FUNCTIONS
void webSetup() {
  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());      

  // Define the handler for the root URL
  server.on("/", handleRoot);
  // Serve the LED status
  server.on("/status", handleStatus);
  // Start the server
  server.begin();
  Serial.println("HTTP server started.");  
}
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
// DATA FUNCTIONS
String printValues() {
    // Convert temperature to Fahrenheit
    // Serial.print("Temperature = ");
    // Serial.print(1.8 * bme.readTemperature() + 32);
    // Serial.println(" *F");

    String message = "Temperature = " + String(bme.readTemperature()) + " °C<br>" + 
                     "Pressure = " + String(bme.readPressure() / 100.0F) + " hPa<br>" +
                     "Approx. Altitude = " + (bme.readAltitude(SEALEVELPRESSURE_HPA)) + " m<br>" + 
                     "Humidity = " + (bme.readHumidity()) + " %<br>";
    Serial.println(message);
    return message;
};

// SERVER FUNCTIONS
// Function to handle the `/status` route that sends the LED status
void handleStatus() {
    String message = printValues();
    server.send(200, "text/plain", message);  // Send the status to the client
    flashLED();
};

// Function to handle root URL "/"
void handleRoot() {
    Serial.println("A client has connected via HTTP!"); 
  
    String html = "<html><body>";
    html += "<h1>ESP8266 Live Updates</h1>";
    html += "<div id='message'>Waiting for weather conditions...</div>";
    html += "<script>";
    html += "function updateMessage() {";
    html += "  fetch('/status').then(response => response.text()).then(data => {";
    html += "    document.getElementById('message').innerHTML = data;";
    html += "  });";
    html += "  setTimeout(updateMessage, 5000);";  // Check every 5 seconds for updates
    html += "}";
    html += "updateMessage();";  // Start the update loop
    html += "</script>";
    html += "</body></html>";
  
    server.send(200, "text/html", html);
};

#endif
