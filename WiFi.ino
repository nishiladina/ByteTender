#include <Arduino.h>
#include <WiFiServer.h>
#include "OpenThingsFramework.h"

#define LED 2

#define WIFI_SSID "TMOBILE-18DD"
#define WIFI_PASSWORD "lrtsucks"

#define CLOUD_HOST "ws.cloud.openthings.io"
#define CLOUD_PORT 80
#define LOCAL_SERVER_PORT 80

// !!! Replace this by the unique token created for you !!!
#define CLOUD_TOKEN "your_unique_token"

OTF::OpenThingsFramework *otf;

void homepage(const OTF::Request &req, OTF::Response &res) {
  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/plain"));
  res.writeBodyChunk(F("<h1>Hello World! This is a simple ESP32 cloud demo!</h1>"));
}

void updateLed(const OTF::Request &req, OTF::Response &res) {
  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/plain"));
  char *cv = req.getQueryParameter(F("value"));
  if(cv) {
    digitalWrite(LED, String(cv).toInt());
    res.writeBodyChunk(F("The LED status has been set"));
  } else {
    digitalWrite(LED, 1-digitalRead(LED));
    res.writeBodyChunk(F("The LED status has been flipped"));
  }
}

void missingPage(const OTF::Request &req, OTF::Response &res) {
  res.writeStatus(404, F("Not found"));
  res.writeHeader(F("content-type"), F("text/plain"));
  res.writeBodyChunk(F("That page does not exist"));
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi network...");
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.print("Connected. Local IP: ");
  Serial.println(WiFi.localIP());

  // Create OTC Object
  otf = new OTF::OpenThingsFramework(LOCAL_SERVER_PORT, CLOUD_HOST, CLOUD_PORT, CLOUD_TOKEN, false);

  // Set up server endpoints
  otf->on(F("/"), homepage);
  otf->on(F("/led"), updateLed);
  otf->onMissingPage(missingPage);
  otf->localServerBegin();
  Serial.println(F("Finished setup"));
}

void loop() {
  // Analogous to calling "server->handleClient".
  otf->loop();
}