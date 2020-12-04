#include "WiFi.h"
#include "BuildConfig.h"

class WiFiConn {
    public:

    const char* wl_status_to_string(wl_status_t status) {
        switch (status) {
            case WL_NO_SHIELD: return "WL_NO_SHIELD";
            case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
            case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
            case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
            case WL_CONNECTED: return "WL_CONNECTED";
            case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
            case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
            // this has been updated in https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/include/wl_definitions.h but not yet released AFAIK
            // case WL_WRONG_PASSWORD: return "WL_WRONG_PASSWORD";
            case WL_DISCONNECTED: return "WL_DISCONNECTED";
        }
        return "UNKNOWN_STATUS";
    }

    void wifi_begin() {
        WiFi.setHostname("remote-door-lock");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }

    void setup() {
        WiFi.disconnect();  // cleanup interface
        WiFi.mode(WIFI_STA);
        wifi_begin();

        uint8_t i = 0;
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);

            // because of this check we don't spam the message below
            if ((++i % 16) == 0) {
                Serial.print(F(" still trying to connect to "));
                Serial.print(WIFI_SSID);
                Serial.print(F(", status code: "));
                Serial.println(wl_status_to_string(WiFi.status()));

                // if conn failed, we need to do the "begin" again
                if (WiFi.status() != WL_CONNECTED) {
                    Serial.print("failed, closing and retrying, status ");
                    Serial.println(wl_status_to_string(WiFi.status()));

                    delay(500);
                    wifi_begin();
                }
            }
        }

        Serial.print(F("Connected to WiFi. My IP address is: "));
        Serial.println(WiFi.localIP());
    }

    void alive() {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.print(F("Lost WiFi connection due to: "));
            Serial.println(WiFi.status());
            setup();
        }
    }
};
