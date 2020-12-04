#include <BuildConfig.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class Api {
    public:

    char *status;

    // since http.getString() returns string and not char we just use String
    // here as well to avoid conversion, even though it's more memory consuming
    String get() {
        HTTPClient http;

        // Your IP address with path or Domain name with URL path 
        http.begin(API_ENDPOINT);

        String response;

        // Send HTTP GET request
        int httpResponseCode = http.GET();

        if (httpResponseCode == 200) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
        }
        else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        response = http.getString();

        // Free resources
        http.end();

        return response;
    };

    bool update() {
        String response = get();

        // to determine capacity needed, I've used https://arduinojson.org/v6/assistant/
        const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 130;
        StaticJsonDocument<capacity> doc;
        DeserializationError err = deserializeJson(doc, response);  // data is in JSON so we need to deserialize it before using

        if (err) {
            // most likely to occur if response blob is not valid JSON
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
        } else {
            if (doc["code"] == 200) {
                return doc["response"]["locked"];
            }

            Serial.print("HTTP Response: ");
            serializeJson(doc, Serial);
        }
        return true;    // always lock door if conn fails
    };
};
