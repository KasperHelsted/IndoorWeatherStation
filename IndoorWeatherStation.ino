#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>

#include "modules.h"

#include "figures.h"
#include "images.h"

void setup() {
  // Init serial and wait for serial to be ready.
  Serial.begin(115200);
  while (!Serial){}

  initDisplay();
  initModules();
}

void initDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.setTextWrap(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

bool connectedToWifi(){
  return WiFi.status() != WL_CONNECTED;
}

void initModules(){
  //Initialize wifi module
  WiFi.begin(SSID, PASSWORD);
  // Wait for wifi to connect
  while (!connectedToWifi()) {
    loadingScreen();
    // TODO: Write Connecting to WiFi... 
    display.display();
    delay(500);
  }


  //Initialize DHT module
  dht.begin();
  loadingScreen();
  // TODO: Write Starting temperature sensor
  display.display();
}

void loadingScreen() {
  display.clearDisplay();
  display.drawBitmap(4, 14, WiFi_Logo, WIFI_WIDTH, WIFI_HEIGHT, WHITE);
}


int screen = 0;
void loop() {
  switch(screen){
    case 0:
      renderCurrentTime();
      break;
    case 1:
      renderIndoorTemperature();
      break;
    case 2:
      renderOutsideTemperature();
      break;
    case 3:
      renderOutsideForecast();
      break;
    default:
      renderError();
      break;
  }
  screen = (screen + 1) % 4;

  footer();
  display.display();
  delay(250);
}

void renderCurrentTime(){
  // Current Time()
}

void renderIndoorTemperature(){
  // temperature
  // humidity
}

void renderOutsideTemperature(){
  // Weather icon
  // Weather description
  // Temperature
}

void renderOutsideForecast(){
  // 3 day forecast
  // Weather icon
  // From Temperature
  // To Temperature
}

void renderError(){
  // Error
  // Error message
}

void footer() {
  // time - indoor temp - wifi strength
  renderWiFiStrength(1, 1);
}
