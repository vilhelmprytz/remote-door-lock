#include "WiFi.h"
#include "BuildConfig.h"

class WiFiConn {
    public:

    void wifi_begin() {
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

            // if conn failed, we need to do the "begin" again
            if (WiFi.status() == WL_CONNECT_FAILED) {
                Serial.print("WL_CONNECT_FAILED, closing and retrying, status ");
                Serial.println(WiFi.status());

                WiFi.disconnect();
                wifi_begin();
            }

            // because of this check we don't spam the message below
            if ((++i % 16) == 0) {
                Serial.print(F(" still trying to connect, status code "));
                Serial.println(WiFi.status());
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
