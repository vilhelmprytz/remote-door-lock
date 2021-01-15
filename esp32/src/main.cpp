#include <Arduino.h>
#include <WiFi.h>

// include classes
#include <Api.h>
#include <DoorServo.h>
#include <WiFiConn.h>

Api api;
DoorServo doorServo;
WiFiConn wifiConn;

const long refresh_interval = 1500;

void setup()
{
  // debugging, print to us
  Serial.begin(9600);

  wifiConn.setup();
}

void loop()
{
  // verify WiFi is alive
  wifiConn.alive();

  Serial.println("Refreshing data");

  // pass bool from API to doorServo to sync with server
  doorServo.sync(api.update());

  // wait until next refresh
  delay(refresh_interval);
}
