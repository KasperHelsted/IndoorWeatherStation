#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#include "settings.h"

#include "figures.h"

Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

void setup()   {
  Serial.begin(115200);

  display.begin();
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
  display.fillRect(0, 0, 128, 9, WHITE);

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

void body(){
  display.setTextColor(WHITE);
  display.drawBitmap(1, 10, myBitmap, 11, 11, WHITE);
  display.setCursor(15, 12);
  display.println("De go' vejr");
}

void footer() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(!CELSIUS);
  char test[] = (CELSIUS) ? "C" : "F";

  display.setTextColor(BLACK);
  display.fillRect(0, 23, 128, 9, WHITE);

  display.setCursor(1, 24);
  display.print("Indoor: ");
  if (isnan(humidity) || isnan(temperature)) {
    display.println("Offline");
  }else{
    display.print(temperature, 1);
    display.print(" "+String(test)+" / ");
    display.print(humidity, 0);
    display.println(" %");
  }
}
