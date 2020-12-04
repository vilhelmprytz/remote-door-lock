# remote-door-lock/esp32

ESP32 software.

## Instructions

Use VS Code with the PlatformIO extension to build the project with the required dependencies. If you clone the project and open VS Code in this folder, VS Code will automatically recommend using PlatformIO since it's listed in `.vscode/extensions.json`. Use the schematic shown in the README in the project root. The PWM for the servo should be connected to the GPIO 18.

## Configuration

For the ESP32 software to work, you need to create a header file with some configuration variables. Create a file in the `include` folder with the name `build_config.h`, i.e. `include/build_config.h`.

Put the following contents in the file, modify the values of the constants. The variables are quite self-explanatory.

```c++
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_ENDPOINT "http://10.0.1.226:5000/api/lock"
```

The `API_ENDPOINT` will be the Python backend API served at `/api`.
