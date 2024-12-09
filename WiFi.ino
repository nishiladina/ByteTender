#include <Arduino.h>
#include <WiFiServer.h>
#include "OpenThingsFramework.h"
#include "ui_html.h"

#define WIFI_SSID "TMOBILE-18DD"
#define WIFI_PASSWORD "lrtsucks"

#define CLOUD_HOST "ws.cloud.openthings.io"
#define CLOUD_PORT 80
#define LOCAL_SERVER_PORT 80

// !!! Replace this by the unique token created for you !!!
#define CLOUD_TOKEN "OT4fa175d0cfd69f1457b8dba3d4d175"

// #define BUT0 34
// #define BUT1 0
// #define BUT2 35
// #define LED 2
// #define NEOP 25
// #define BUZZER 17
// #define LIGHT_SENSOR A3

OTF::OpenThingsFramework *otf;

bool isBusy = false;

void on_homepage(const OTF::Request &req, OTF::Response &res) {
  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/html"));  
  res.writeBodyChunk(FPSTR(ui_html)); // return the raw string defined in html_index.h
}

void on_get(const OTF::Request &req, OTF::Response &res) {
  char json[500];
  sprintf(json, "{\"status\":%d}", isBusy);
  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/html"));
  res.writeBodyChunk(json);
} 

void on_set(const OTF::Request &req, OTF::Response &res) {
  char *type = req.getQueryParameter(F("type"));
  char *value = req.getQueryParameter(F("value"));
  if(!type || !value) {
    res.writeStatus(200, "OK");
    res.writeHeader(F("content-type"), F("text/html"));
    res.writeBodyChunk(F("{\"result\": \"missing type and/or value parameters\"}"));
  } else {
    // if(strcmp(type, "led")==0) {
    //   if(value[0]=='0') digitalWrite(LED, 0);
    //   else digitalWrite(LED, 1);
    // } else if(strcmp(type, "tune")==0) {
    //   playTune(value);
    // } else if(strcmp(type, "msg")==0) {
    //   lcd.clear();
    //   lcd.drawString(0, 0, value);
    //   lcd.display();
    // } else if(strcmp(type, "color")==0) {
    //   Serial.println(value);
    //   int c = strtoul(value, NULL, 16);
    //   Serial.println(c);
    //   for(int i=0;i<pixels.numPixels();i++) {
    //     pixels.setPixelColor(i, c);
    //   }
    //   pixels.show();
    // }
    res.writeStatus(200, "OK");
    res.writeHeader(F("content-type"), F("text/html"));
    res.writeBodyChunk(F("{\"result\": \"success\"}"));
  }
}

void setup(void){
  Serial.begin(115200);

  // Start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi network...");
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.print("Connected. Local IP: ");
  Serial.println(WiFi.localIP());

  // Create OTC Object
  otf = new OTF::OpenThingsFramework(LOCAL_SERVER_PORT, CLOUD_HOST, CLOUD_PORT, CLOUD_TOKEN, false);
  otf->localServerBegin();  // start local server

  // Set up server endpoints (APIs)
  otf->on(F("/"), on_homepage);
  otf->on(F("/get"), on_get);
  otf->on(F("/set"), on_set);
}
 
void loop(void){
  otf->loop();
}