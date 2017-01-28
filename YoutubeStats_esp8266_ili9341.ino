#include <Arduino.h>
#include <YoutubeApi.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
//#include <ArduinoJson.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Additional UI functions
#include "GfxUi.h"

#include "Orbitron_Light_8.h"
#include "Orbitron_Light_16.h"
#include "Orbitron_Light_26.h"
#include "Orbitron_Light_50.h"
#include "icons.h"

// ILI9341
#define TFT_DC      (5)
#define TFT_CS      (15)
#define TFT_RESET   (4)
// SPI:
// SCK to 14
// MISO to 12
// MOSI to 13

//------- Replace the following! ------
char ssid[] = "SSIDname";       // your network SSID (name)
char password[] = "SSIDpassword";  // your network key
#define API_KEY "YourAPIkey"  // your google apps API Token
#define CHANNEL_ID "YourChannelID" // makes up the url of channel
//--------------------------------------

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);
GfxUi ui = GfxUi(&tft);
WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

int api_mtbs = 6000; //mean time between api requests
long api_lasttime;   //last time api request has been done

long subs = 0;

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    // Init ILI9341
    tft.begin();
    delay(1);
    //tft.setRotation(1);

    tft.fillScreen(ILI9341_RED);
    tft.drawBitmap(70, 130, wifiBitmap, 100, 70, ILI9341_WHITE);
    tft.setCursor(38, 210);
    tft.setFont(&Orbitron_Light_16);
    tft.print("Youtube Channel");
    tft.setCursor(77, 230);
    tft.print("Statistics");

    tft.setFont(&Orbitron_Light_8);
    tft.setCursor(72, 313);
    tft.print("coded by Shin2");

    // disable sleep mode for better data rate
    WiFi.setSleepMode(WIFI_NONE_SLEEP);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    

}

void loop() {
  
    
    if (millis() > api_lasttime + api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(20, 40);
      tft.setFont(&Orbitron_Light_16);
      tft.println("Subscribers");      
      tft.setCursor(20, 80);
      tft.setFont(&Orbitron_Light_50);
      tft.println(String(api.channelStats.subscriberCount));
      
      //tft.drawFastHLine(0, 95, 240, ILI9341_WHITE);

      tft.setCursor(20, 120);
      tft.setFont(&Orbitron_Light_16);
      tft.println("Total Views");
      tft.setCursor(20, 145);
      tft.setFont(&Orbitron_Light_26);
      tft.println(String(api.channelStats.viewCount));
      
      //tft.drawFastHLine(0, 160, 240, ILI9341_WHITE);

      tft.setCursor(20, 190);
      tft.setFont(&Orbitron_Light_16);
      tft.println("Total Videos");
      tft.setCursor(20, 215);
      tft.setFont(&Orbitron_Light_26);
      tft.println(String(api.channelStats.videoCount));

      tft.setCursor(20, 260);
      tft.setFont(&Orbitron_Light_16);
      tft.println("Comment Count");
      tft.setCursor(20, 285);
      tft.setFont(&Orbitron_Light_26);
      tft.println(String(api.channelStats.commentCount));
      
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      //tft.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
    api_lasttime = millis();
  }
}

