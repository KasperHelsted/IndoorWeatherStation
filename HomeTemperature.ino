#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include "settings.h"

#include "figures.h"

Adafruit_SSD1306 display(1);

DHT dht(DHTPIN, DHTTYPE);

HTTPClient http;
StaticJsonDocument<200> doc;

void setup()   {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  display.setTextWrap(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //Initialize to wifi
  WiFi.begin(SSID, PASSWORD);

  //Connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  dht.begin();

}

void loop() {
  display.clearDisplay();
  
  head();
  body();
  footer();

  display.display();
  //delay(10000);
  delay(500);
}

void head() {
  display.setTextColor(BLACK);
  display.fillRect(0, 0, SCREEN_WIDTH, 9, WHITE);

  long wifiStrength = WiFi.RSSI();

  display.fillRect(1, 5, 3, 3, BLACK);
  display.fillRect(2, 6, 1, 1, (wifiStrength > -70) ? BLACK : WHITE);

  display.fillRect(5, 3, 3, 5, BLACK);
  display.fillRect(6, 4, 1, 3, (wifiStrength > -69) ? BLACK : WHITE);

  display.fillRect(9, 1, 3, 7, BLACK);
  display.fillRect(10, 2, 1, 5, (wifiStrength > -59) ? BLACK : WHITE);

  display.setCursor(15, 1);
  display.println(SSID);
}

int i = 0;

void body(){
  http.begin("http://"+String(STATION)+"/");
  int httpCode = http.GET();

  if (httpCode > 0) {
    deserializeJson(doc, http.getString());
    
    String temperature = doc["temp"];
    int weather = doc["weather"];

    display.setTextColor(WHITE);
    display.drawBitmap(1, 10, getIcon(i % 76), 11, 11, WHITE);
    i++;
    display.setCursor(15, 12);
    display.println("De er " + temperature + " C");
  }
}

void footer() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(!CELSIUS);
  String denominator = (CELSIUS) ? "C" : "F";

  display.setTextColor(BLACK);
  display.fillRect(0, SCREEN_HEIGHT-9, SCREEN_WIDTH, 9, WHITE);

  display.setCursor(1, SCREEN_HEIGHT-8);
  display.print("Indoor: ");

  if (isnan(humidity) || isnan(temperature)) {
    display.println("Offline");
  }else{
    display.print(temperature, 1);
    display.print(" " + denominator + " / ");
    display.print(humidity, 0);
    display.println(" %");
  }
}
