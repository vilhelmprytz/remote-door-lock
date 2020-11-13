# remote-door-lock/wifi-controller

ESP32 software. Acts as a proxy, connects to WiFi and to the Arduino Uno that controls the servo.

## Configuration

In order for the ESP32 software to work you need to create a header file with some configuration variables. Create a file in the `include` folder with the name `build_config.h`, .i.e. `include/build_config.h`.

Put the following contents in the file, modify the values of the constants. The variables are quite self explanatory.

```c++
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_ENDPOINT "http://10.0.4.102:5000/api/servo"
```
