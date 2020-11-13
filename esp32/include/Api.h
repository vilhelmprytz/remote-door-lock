#include <build_config.h>
#include <HTTPClient.h>

class Api {
    public:

    char *status;

    String update() {
        HTTPClient http;

        // Your IP address with path or Domain name with URL path 
        http.begin(backend_endpoint);

        String response;

        // Send HTTP POST request
        int httpResponseCode = http.GET();

        if (httpResponseCode>0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            response = http.getString();
        }
        else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        // Free resources
        http.end();

        return response;
    };
};
