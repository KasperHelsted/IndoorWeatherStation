#include <ESP8266WiFi.h>
#include <TimeLib.h>

#include "modules.h"
#include "font.h"
#include "LoadUniqueID.h"

#include "figures.h"
#include "images.h"

/////////////////////////////////
/////        SSD1306        /////
/////        SCL: D1        /////
/////        SDA: D2        /////
/////                       /////
/////         DHT22         /////
/////        DATA: D3       /////
/////////////////////////////////

void setup() {
  // Init serial and wait for serial to be ready.
  Serial.begin(115200);
  while (!Serial){}

  // Set system info
  http.setTimeout(1000);

  // Load Unique ID of the device
  loadUniqueID();
  
  // Init modules
  initDisplay();
  initModules();

  // Do stuff plz.
  registerAtBase();
}

void registerAtBase() {
  bool connected = false;
  String postData = "ID=" + EEPString;

  while(!connected){
    doc.clear();
    http.begin("http://"+String(STATION)+"/register");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int lastCode = http.POST(postData);
  
    if (lastCode == 200) {
      connected = true;
    }else{
      waitForConnection();
      Serial.println("COULD NOT CONNECT TO BASE");
      delay(2500);
    }
    http.end();
  }
}

void waitForConnection(){
  display.clearDisplay();

  display.setCursor(15, 20);
  display.print("Not registered");
  display.setCursor(25, 29);
  display.print("on network");
  display.setCursor(5, 38);
  display.print("Device ID: " + EEPString);
  
  
  display.display();
}

void initDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.setTextWrap(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
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
  
    display.setCursor(70, 39);
    display.print("Connecting to");
    display.setCursor(70, 48);
    display.print("WiFi");

    display.display();
    delay(2000);
  }

  //Initialize DHT module
  dht.begin();
  loadingScreen();

  display.setCursor(70, 39);
  display.print("Starting");
  display.setCursor(70, 48);
  display.print("DHT Sensor");

  display.display();
  delay(2000);
}

void loadingScreen() {
  display.clearDisplay();
  display.drawBitmap(4, 14, WiFi_Logo, WIFI_WIDTH, WIFI_HEIGHT, WHITE);

  display.setCursor(70, 21);
  display.print("Made By:");
  display.setCursor(70, 30);
  display.print("Hellsted");
}


int screen = 0;
long execTime = -1;

void loop() {
  fetchData();
  updateTemperature();

  if(execTime == -1)
    execTime = millis();
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

  footer();
  display.display();
  Serial.flush();
}

float humidity = 0;
float temperature = 0;
const String denominator = (CELSIUS) ? "°C" : "F";
long lastTemperature = 0;
void updateTemperature(){
  if(lastTemperature < millis()){
    float readHumidity = dht.readHumidity();
    float readTemperature = dht.readTemperature(!CELSIUS);
    
    if (isnan(readHumidity) || isnan(readTemperature)) {
      humidity = -1;
      temperature = -1;
      return;
    }else{
      humidity = readHumidity;
      temperature = readTemperature;
    }
    lastTemperature = millis() + 5000;
  }
}

void renderCurrentTime(){
  // Current Time()
  display.setCursor(1, 1);
  display.setTextSize(2);

  display.setCursor(15, 20);
  if(hour() < 10)
    display.print(" ");
  display.print(hour());
  display.print(":");
  display.printf("%02d", minute());
  display.print(":");
  display.printf("%02d", second());

  display.setTextSize(1);
}

void renderIndoorTemperature(){
  display.setTextSize(1);
  display.setCursor(40, 0);
  display.print("TEMPERATURE");
  display.setTextSize(2);
  display.setCursor(30, 9);
  display.print(temperature, 2);
  display.print(" " + denominator);

  display.setTextSize(1);
  display.setCursor(40, 27);
  display.print("HUMIDITY");
  display.setTextSize(2);
  display.setCursor(30, 36);
  display.print(" %");

  display.setTextSize(1);
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
void footer() {
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
  display.printf("%02d", hour());
  display.print(":");
  display.printf("%02d", minute());

  int baseOffset = 40;
  // offset might need to be 0, 7, 14 depeding on space between chars
  int offset = baseOffset + ((temperature > 100) ? 10 : (temperature > 10) ? 5 : 0);
  display.setCursor(128 - offset, 64 - 7);
  display.print(temperature, 1);
  display.print(" " + denominator);

  renderWiFiStrength(128 - 5, 64 - 6);
}
