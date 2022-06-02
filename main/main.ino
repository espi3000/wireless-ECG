#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <Wire.h>
#include <string.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 adc;
AsyncWebServer server(80); // Create AsyncWebServer object on port 80

const char* SSID = "YOURWIFINAME";
const char* PASS = "YOURWIFIPASSWORD";

void ADC_init() {              //Initialization for the ADC, gain is set to one, giving a range of +-6V for differential measurements
    adc.setGain(GAIN_ONE);
    adc.begin();
 }

String ecgADC() {             //ECG measuring function to measure the value of the analog output. Converts data, typecasts to string and returns string
    float prev;
    float t = adc.readADC_Differential_0_1(); // Read ECG values (the default)
    if (abs(prev - t) >= 4000) {               // Some kind of filter?
        t = prev;
    }
    delay(1);

    if (isnan(t)) {    
        Serial.println("Failed to read from ECG board!");
        return "";
    } else {
        Serial.println(t);
        return String(t);
    }
    prev = t;
}

void setup() {
    // Serial port for debugging purposes
    Serial.begin(115200);
    ADC_init();
    
    bool status;
    
    
    // Initialize SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    
    WiFi.begin(SSID, PASS); // Connect to Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    
    Serial.println(WiFi.localIP());  // Print ESP32 Local IP Address
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ // Route for root / web page
      request->send(SPIFFS, "/index.html");
    });
    server.on("/ECG", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", ecgADC().c_str());
    });

    server.begin(); // Start server
}
 
void loop(){}
