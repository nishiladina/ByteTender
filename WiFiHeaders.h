#include <Arduino.h>
#include <WiFiServer.h>
#include "OpenThingsFramework.h"
#include "ui_html.h"
#include <iostream>
#include <string>

#define CLOUD_HOST "ws.cloud.openthings.io"
#define CLOUD_PORT 80
#define LOCAL_SERVER_PORT 80
