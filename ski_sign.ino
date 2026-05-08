#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"
#include <string.h>

#define PIN 5
#define NUMPIXELS 6

const char* ssid = SSID;
const char* password = SSID_PASS;
const char* test_api = TEST_API;
const char* api_key = API_KEY;
JsonDocument doc;
String liftStatusArr[14];


unsigned long previousMillis = 0;
const long interval = 1000 * 60;  //milliseconds to a minute

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
  JsonArray liftArray;


  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      // setup HTTP client attach api key header
      HTTPClient http;
      http.begin(test_api);
      http.addHeader("X-API-Key", api_key);

      // query http endpoint for data
      httpCode = http.GET();
      payload = http.getString();
      Serial.println(httpCode);
      // use built in error to handle a deserialization issue
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      // convert to JsonArray to iterate though and store the current status
      liftArray = doc["lift"].as<JsonArray>();
      for(int i = 0; i < liftArray.size(); i++) {
        JsonObject lift_item = liftArray[i];
        liftStatusArr[i] = lift_item["status"].as<String>();  // "open", "closed", "on_hold"
      }
      http.end();
    }

    // Set pixel color
    for (int i = 0; i < 14; i++) {
      if(liftStatusArr[i] == "open") {
        pixel.setPixelColor(i, pixel.Color(255, 0, 0));
      } else if(liftStatusArr[i] == "closed") {
        pixel.setPixelColor(i, pixel.Color(0, 255, 0));
      } else {
        pixel.setPixelColor(i, pixel.Color(153, 255, 28));
      }
    }

    pixel.show();
  }
}
