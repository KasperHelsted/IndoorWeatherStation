#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include "settings.h"

#include "figures.h"
#include "images.h"

//Initialze the SSD1306 screen
Adafruit_SSD1306 display(1);

//Initialze the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Different clients for handleing the HTTP calls
HTTPClient http;
StaticJsonDocument<200> doc;

void setup() {
  // Init serial and wait for serial to be ready.
  Serial.begin(115200);
  while (!Serial){}

  initScreen();

}

void initScreen() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.setTextWrap(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
}

