#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include "arduino_secrets.h"

#define PIN 5
#define NUMPIXELS 6

const char* ssid = SSID;
const char* password = SSID_PASS;

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
  // put your main code here, to run repeatedly:
  // Turn on (white)
  for(int i = 0; i < 6; i++) {
    pixel.setPixelColor(i, pixel.Color(0, 255, 0));
  }
  
  pixel.show();
  delay(500);

  // Turn off
  // for(int i = 0; i < 6; i++) {
  //   pixel.setPixelColor(i, pixel.Color(0, 0, 0));
  // }
  
  // pixel.show();
  // delay(500);
}
