#include <Arduino.h>
#include <WiFiServer.h>
#include "OpenThingsFramework.h"
#include "ui_html.h"
#include "style_css.h"
#include <iostream>
#include <string>


#define WIFI_SSID "CICS_Makerspace"
#define WIFI_PASSWORD "makerspace"
#define CLOUD_HOST "ws.cloud.openthings.io"
#define CLOUD_PORT 80
#define LOCAL_SERVER_PORT 80

// !!! Replace this by the unique token created for you !!!
#define CLOUD_TOKEN "OT4fa175d0cfd69f1457b8dba3d4d175"