#include <Arduino.h>
#include <WiFi.h>

// since ESP32 does not have the Arduino built-in library, this is a fork found on GitHub
#include <ESP32Servo.h> 

// WiFi details and backend API endpoint stored in .h file
#include <build_config.h>

Servo doorServo;

const long pin_servo_control = GPIO_NUM_18; // GPIO to servo PWM 

// This is the default ADC max value on the ESP32 (12 bit ADC width);
// this width can be set (in low-level mode) from 9-12 bits, for a
// a range of max values of 512-4096
const long ADC_Max = 4096; 

int pos = 0;  // curr val

void setup_servo() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  doorServo.setPeriodHertz(50); // standard 50hz (euro!)

  // for the FS5103B the pulse width range is between 600 and 2400
  // use other values if you are using another servo
  doorServo.attach(pin_servo_control, 600, 2400);
}

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.print("Connecting to "); Serial.println(wifi_ssid);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0) {
      Serial.println(F(" still trying to connect"));
    }
  }

  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

void setup() {
  // debugging
  Serial.begin(9600);

  setup_servo();
  setup_wifi();
}

void lock() {
  doorServo.write(180);
}

void unlock() {
  doorServo.write(0);
}

void loop() {
  lock();
  delay(1000);
  unlock();
  delay(1000);
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
