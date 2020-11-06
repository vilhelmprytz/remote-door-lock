#include <build_config.h>
#include <HTTPClient.h>

class Api {
    public:

    char *status;

    String update() {
        HTTPClient http;

        // Your IP address with path or Domain name with URL path 
        http.begin(backend_endpoint);

        // Send HTTP POST request
        int httpResponseCode = http.GET();

        String payload = "{}"; 

        if (httpResponseCode>0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            payload = http.getString();
        }
        else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        // Free resources
        http.end();

        return payload;
    };
};
