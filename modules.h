#include <DHT.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include "settings.h"

//Initialze the SSD1306 screen
Adafruit_SSD1306 display(1);

//Initialze the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Different clients for handleing the HTTP calls
HTTPClient http;
StaticJsonDocument<200> doc;
