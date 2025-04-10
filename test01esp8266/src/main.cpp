// A simple program to turn on an LED during Night/Dark and stay off during Day/light

#include <Arduino.h>

// Define the pin for the built-in LED (on GPIO2)
const int ldrPin = A0; // Photoresistor connected to analog pin A0
const int threshold = 70;

void setup() {
  // Initialize the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Start the serial monitor for debugging (optional)
  Serial.begin(115200);
}

void loop() {
  // Read the value from the photoresistor (0-1023)
  int ldrValue = analogRead(ldrPin);

  // Print the LDR value for debugging
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // If it's dark (LDR value below the threshold), turn the LED on
  // If it's bright (LDR value above the threshold), turn the LED off
  if (ldrValue < threshold) {
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on
  } else {
    digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
  }

  // Delay for a short period to stabilize
  delay(200);
}