#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// include classes
#include <Api.h>
#include <DoorServo.h>
#include <WiFiConn.h>

Api api;
DoorServo doorServo;
WiFiConn wifiConn;

const long refresh_interval = 1500;

void setup() {
  // debugging, print to us
  Serial.begin(9600);

  doorServo.setup();
  wifiConn.setup();
}

void loop() {
  // verify WiFi is alive
  wifiConn.alive();

  Serial.println("Refreshing data");

  // raw response from API
  String response = api.update();

  // to determine capacity needed, I've used https://arduinojson.org/v6/assistant/
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 130;
  StaticJsonDocument<capacity> doc;
  DeserializationError err = deserializeJson(doc, response);  // data is in JSON so we need to deserialize it before using

  if (err) {
    // most likely to occur if response blob is not valid JSON
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  } else {
    // sync servo with API response
    doorServo.sync(doc["response"]["locked"]);
  }

  // wait until next refresh
  delay(refresh_interval);
}
