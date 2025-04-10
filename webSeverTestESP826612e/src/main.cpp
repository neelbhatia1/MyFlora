// Test out a simple Web Server for the ESP8266-12E 
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ONELEGUP";
const char* password = "SloppyBoys24";

// Define the pin for the photoresistor
const int ldrPin = A0; // Photoresistor connected to analog pin A0
const int threshold = 70;
int ldrValue;
bool ledState;

// Create an instance of the server on port 80
ESP8266WebServer server(80);

// Function to handle root URL "/"
void handleRoot() {
  Serial.println("A client has connected via HTTP!"); 

  String html = "<html><body>";
  html += "<h1>ESP8266 Live Updates</h1>";
  html += "<div id='message'>Waiting for light conditions...</div>";
  html += "<script>";
  html += "function updateMessage() {";
  html += "  fetch('/status').then(response => response.text()).then(data => {";
  html += "    document.getElementById('message').innerHTML = data;";
  html += "  });";
  html += "  setTimeout(updateMessage, 500);";  // Check every 1/2 second for updates
  html += "}";
  html += "updateMessage();";  // Start the update loop
  html += "</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Function to handle the `/status` route that sends the LED status
void handleStatus() {
  String message = (ledState) ? "LED is ON (Room is dark)" : "LED is OFF (Room is bright)";
  server.send(200, "text/plain", message);  // Send the status to the client
}

void setup() {
  // Let system stabilize
  delay(2000);

  // Start the serial communication
  Serial.begin(74880);
  Serial.flush();

  // Initialize the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Connect to Wi-Fi
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("Connected to WiFi!");
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

void loop() {
  // Listen for incoming client requests
  server.handleClient();

  // Read the value from the photoresistor (0-1023)
  ldrValue = analogRead(ldrPin);

  // If it's dark (LDR value below the threshold), turn the LED on
  // If it's bright (LDR value above the threshold), turn the LED off
  if (ldrValue < threshold) {
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on
    ledState = true;
  } else {
    digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
    ledState = false;
  }

  delay(200);
}