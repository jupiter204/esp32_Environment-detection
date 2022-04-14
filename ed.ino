#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "DHTesp.h"
#include "WiFi.h"
#include "time.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define dhtpin 17

const char* ssid       = "cs_wl";
const char* password   = "6666666666";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dht;
bool OLEDStatus = true;

struct tm timeinfo;

void time_check(void){
    
    if(!getLocalTime(&timeinfo)){    
        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(2);
        display.println("Failed to obtain time , reboot ");
        display.display();
        delay(1000);
        ESP.restart();
        return;
    }
}

void output(void)
{
    TempAndHumidity values = dht.getTempAndHumidity();
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(WHITE,BLACK);
    display.println(&timeinfo, "%H:%M:%S");
    display.setCursor(0,16);
    display.println("Temp:"+ String(values.temperature,0)+"`C");
    display.setCursor(0,32);
    display.println("Humi:"+ String(values.humidity,0)+"%");
    display.display();
}

void setup()
{
    dht.setup(dhtpin, DHTesp::DHT11);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(2);
        display.setTextColor(WHITE,BLACK);
        display.println("WIFI Check");
        display.display();
        delay(500);
        
    }

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    time_check();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void loop()
{
    delay(1000);
    output();
}
