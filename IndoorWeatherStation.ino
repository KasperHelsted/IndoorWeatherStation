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
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

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
  display.clearDisplay();

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

  int executionTime = footer();
  display.display();
  
  if(executionTime < 100)
    delay(100 - executionTime);
}

void renderCurrentTime(){
  // Current Time()
  display.setCursor(1, 1);
  display.print("Current Time()");
}

void renderIndoorTemperature(){
  // temperature
  // humidity
  display.setCursor(1, 1);
  display.print("temperature, humidity");
}

void renderOutsideTemperature(){
  // Weather icon
  // Weather description
  // Temperature
  display.setCursor(1, 1);
  display.print("Weather icon & Stuff");
}

void renderOutsideForecast(){
  // 3 day forecast
  // Weather icon
  // From Temperature
  // To Temperature
  display.setCursor(1, 1);
  display.print("3 day forecast");
}

void renderError(){
  // Error
  // Error message
}

int loadingWidth = 0;
int changeTime = 5000;
long execTime = 0;

int footer() {
  // time - indoor temp - wifi strength
  int diff = (millis() - execTime);
  execTime = millis();

  display.drawLine(0, 64 - 10, loadingWidth, 64 - 10, WHITE);

  if(loadingWidth > 128) {
    screen = (screen + 1) % 4;
    loadingWidth = 0;
  }else {
    loadingWidth = (loadingWidth + (int)((diff/(float)changeTime)*128));
  }

  display.drawLine(0, 64 - 9, 128, 64 - 9, WHITE);
  
  display.setCursor(1, 64 - 7);
  display.print("16:47");

  renderWiFiStrength(128 - 5, 64 - 6);

  return diff;
}
