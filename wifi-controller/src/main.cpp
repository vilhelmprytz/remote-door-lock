#include <Arduino.h>
#include <WiFi.h>

const long output_pin = GPIO_NUM_26;

void setup() {
  pinMode(output_pin, OUTPUT);
  digitalWrite(output_pin, LOW);
}

void loop() {
  delay(5000);
  digitalWrite(output_pin, HIGH);
  delay(5000);
  digitalWrite(output_pin, LOW);
}
