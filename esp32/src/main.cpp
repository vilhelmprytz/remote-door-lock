#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// since ESP32 does not have the Arduino built-in library, this is a fork found on GitHub
#include <ESP32Servo.h> 

// get data
#include <Api.h>

// WiFi details and backend API endpoint stored in .h file
#include <build_config.h>

Servo doorServo;
Api api;

const long pin_servo_control = GPIO_NUM_18; // GPIO to servo PWM 
const long refresh_interval = 2000;

// This is the default ADC max value on the ESP32 (12 bit ADC width);
// this width can be set (in low-level mode) from 9-12 bits, for a
// a range of max values of 512-4096
const long ADC_Max = 4096; 

// default is locked
bool lock_status = true;

void setup_servo() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // standard 50hz, modify for other servos
  doorServo.setPeriodHertz(50);

  // for the FS5103B the pulse width range is between 600 and 2400
  // use other values if you are using another servo
  doorServo.attach(pin_servo_control, 1000, 2000);
}

void wifi_begin() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void wifi_verify_conn() {
  if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("Lost WiFi connection (WL_CONNECT_FAILED), reconnecting..");
    wifi_begin();
  }
}

// init and connect to WiFi
void setup_wifi() {
  WiFi.mode(WIFI_STA);
  wifi_begin();
  WiFi.setHostname("remote-door-lock");
  Serial.print("Connecting to "); Serial.println(WIFI_SSID);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0) {
      Serial.print(F(" still trying to connect, status "));
      Serial.println(WiFi.status());
    }

    wifi_verify_conn();
  }

  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

void setup() {
  // debugging, print to us
  Serial.begin(9600);

  setup_servo();
  setup_wifi();
}

void lock() {
  doorServo.write(90);
}

void unlock() {
  doorServo.write(0);
}

void loop() {
  // verify connection not lost
  wifi_verify_conn();

  Serial.println("Refreshing data");

  // update lock status from API
  String response = api.update();

  // to determine capacity needed, I've used https://arduinojson.org/v6/assistant/
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 130;
  StaticJsonDocument<capacity> doc;
  DeserializationError err = deserializeJson(doc, response);  // data is in JSON so we need to deserialize it before using

  if (err) {
    // probably only occurs if response blob is not valid JSON
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  } else {
    if (doc["response"]["locked"] != lock_status) {
      lock_status = doc["response"]["locked"];
    }
  }

  // perform action based on lock_status
  if (lock_status) {
    Serial.println("Locking door");
    lock();
  } else {
    Serial.println("Unlocking door");
    unlock();
  }

  // wait until next refresh
  delay(refresh_interval);

  // testing
  // lock();
  // delay(5000);
  // unlock();
  // delay(5000);

	// for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	doorServo.write(pos);    // tell servo to go to position in variable 'pos'
  //   Serial.println(pos);
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }
	// for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	doorServo.write(pos);    // tell servo to go to position in variable 'pos'
  //   Serial.println(pos);
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }
}
