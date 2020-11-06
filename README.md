# vilhelmprytz/remote-door-lock

Remote door lock using WeMos D1 MINI ESP32 (Arudino framework). This is a DIY IoT solution.

## Equipment

* Wemos D1 Mini32 ESP32 or similar (with WiFi!)
* Servo Motor FS5103B or similar, see [datasheet](https://www.pololu.com/file/0J1431/FS5103B-specs.pdf)

## Instructions

The project consists of two parts. Firstly, the web application. It is meant to be ran on a server. The web application is split into a frontend which communicates with a backend API. Secondly, the software that runs on the ESP32 itself. The code and it's installation/configuration instructions can be found in the `esp32` directory. The ESP32 communicates with the same backend.
