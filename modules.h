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

void renderWiFiStrength(int x, int y, int color=WHITE) {
    int wifiStrength = WiFi.RSSI();

    // 1: Bad connection
    if (wifiStrength > -80)
      display.drawLine(x, y + 3, x, y + 3, WHITE);

    // 2: Okay connection
    if (wifiStrength > -70)
      display.drawLine(x + 1, y + 3, x + 1, y + 2, WHITE);

    // 3: Good connection
    if (wifiStrength > -67)
      display.drawLine(x + 2, y + 3, x + 2, y + 1, WHITE);

    // 4: Great connection
    if (wifiStrength > -30)
      display.drawLine(x + 3, y + 3, x + 3, y, WHITE);
  
    // Bottom Line
    display.drawLine(x, y + 4, x + 3, y + 4, WHITE);
}

long lastUpdateTime = 0;
void fetchData(){
  if(lastUpdateTime < millis()){
    doc.clear();
    http.begin("http://"+String(STATION)+"/");
    int lastCode = http.GET();
  
    if (lastCode > 0) {
      deserializeJson(doc, http.getString());
    }
    http.end();
    lastUpdateTime = millis() + 5000;
  }
}
