#include <Arduino.h>
#include <Servo.h>

Servo lock;         // servo object to control lock servo
int pos;            // current position of servo
int toggle = 0;     // current status of lock (locked/unlocked)
int operation = 0;  // add or remove from pos

const int servo_pin = 9;   // pin servo is connected to
const int signal_pin = 10; // pin the ESP32 is connected via

void setup() {
  // attach servo
  lock.attach(servo_pin);
  lock.write(pos);  // returns back to 0 initially

  // setup bridge (signal proxy)
  pinMode(signal_pin, INPUT);
}

void loop() {
  // read value from esp32
  toggle = digitalRead(signal_pin);

  if (toggle == HIGH) {
    operation = 1;
  } else if (toggle == LOW) {
    operation = -1;
  } else {
    operation = 0;
  }

  for (pos = 0; pos <= 180; pos += operation) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    lock.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);        // waits 15ms for the servo to reach the position
  }

  delay(15);
}
