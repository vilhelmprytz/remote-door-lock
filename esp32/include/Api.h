#include <build_config.h>
#include <HTTPClient.h>

class Api {
    public:

    char *status;

    // since http.getString() returns string and not char we just use String
    // here as well to avoid conversion, even though it's more memory consuming
    String update() {
        HTTPClient http;

        // Your IP address with path or Domain name with URL path 
        http.begin(API_ENDPOINT);

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
