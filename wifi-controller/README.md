# remote-door-lock/wifi-controller

ESP32 software. Acts as a proxy, connects to WiFi and to the Arduino Uno that controls the servo.

## Environment Variables

Please define these variables before building the software. All variables are required.

* `WIFI_SSID` - `SSID` to connect to.
* `WIFI_PASSWORD` - Password of the WiFi you want to connect to.
