#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

#define PIN 5
#define NUMPIXELS 6

const char* ssid = SSID;
const char* password = SSID_PASS;
const char* test_api = TEST_API;
const char* api_key = API_KEY;

unsigned long previousMillis = 0;
const long interval = 1000 * 60; //milliseconds to a minute

Adafruit_NeoPixel pixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wifi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pixel.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  
  int httpCode;
  String payload;

  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(test_api);
      http.addHeader("X-API-Key", api_key);


      httpCode = http.GET();
      Serial.printf("HTTP Code: %d\n", httpCode);
      payload = http.getString();
      Serial.println(payload);
      http.end();
    }
    
    // Turn on (white)
    for(int i = 0; i < 6; i++) {
      pixel.setPixelColor(i, pixel.Color(0, 255, 0));
    }
    
    pixel.show();

  // Turn off
  // for(int i = 0; i < 6; i++) {
  //   pixel.setPixelColor(i, pixel.Color(0, 0, 0));
  // }
  
  // pixel.show();
  // delay(500);
  
    
  }
}
