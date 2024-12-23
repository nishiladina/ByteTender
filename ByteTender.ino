// https://cloud.openthings.io/forward/v1/OT4fa175d0cfd69f1457b8dba3d4d175/

bool needToLower = false; // change to true if uploading code after manually resetting paddle, false if uploading with paddle already lowered

#include "WiFiHeaders.h"
using namespace std;
OTF::OpenThingsFramework *otf;

#include "MotorHeaders.h"

#include "Settings.h"

Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, NEOP_PIN, NEO_GRB + NEO_KHZ800); // 16 LEDs

AccelStepper xStepper = AccelStepper(AccelStepper::DRIVER, XSTEP, XDIR); // can replace AccelStepper::DRIVER with 1 if it doesn't work
AccelStepper z1Stepper = AccelStepper(AccelStepper::DRIVER, Z1STEP, Z1DIR);
AccelStepper z2Stepper = AccelStepper(AccelStepper::DRIVER, Z2STEP, Z2DIR);

bool isBusy = false;

void on_homepage(const OTF::Request &req, OTF::Response &res) {
  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/html")); 
  res.writeBodyChunk(FPSTR(ui_html)); // return the raw string defined in ui_html.h
}

void on_get(const OTF::Request &req, OTF::Response &res) {
  char json[500];
  sprintf(json, "{\"busy\":%d}", isBusy);
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
    if (strcmp(type, "drink") == 0) {
        int drinkType = stoi(value);
        // 1: Grinch Punch
        // 2: Shirley Temple
        // 3: Sprite
        // 4: Ginger Ale
        if(drinkType == 1) {
          Serial.println("making Grinch Punch");
          makeGrinchPunch();
        } else if (drinkType == 2) {
          Serial.println("making Shirley Temple");
          makeShirleyTemple();
        } else if (drinkType == 3) {
          Serial.println("making Sprite");
          makeSprite();
        } else if (drinkType == 4) {
          Serial.println("making Ginger Ale");
          makeGingerAle();
        } else if (drinkType == 5) {
          Serial.println("making Hawaiian Punch");
          makeHawaiianPunch();
        }
    }
    res.writeStatus(200, "OK");
    res.writeHeader(F("content-type"), F("text/html"));
    res.writeBodyChunk(F("{\"result\": \"success\"}"));
  }
}

void setup(void){
  Serial.begin(115200);

  ring.begin();
  ring.setBrightness(32); // set a low brightness
  wipe(ring.Color(255, 0, 0)); // red while setting up

  // start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi network...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500); 
  }
  Serial.print("\nConnected. Local IP: ");
  Serial.println(WiFi.localIP());

  // create OTC Object
  otf = new OTF::OpenThingsFramework(LOCAL_SERVER_PORT, CLOUD_HOST, CLOUD_PORT, CLOUD_TOKEN, false);
  otf->localServerBegin();  // start local server

  // set up server endpoints (APIs)
  otf->on(F("/"), on_homepage);
  otf->on(F("/get"), on_get);
  otf->on(F("/set"), on_set);

  motorSetup();
}
 
void loop(void){
  otf->loop();
}

void motorSetup() {
  Serial.println("beginning motor setup");

  pinMode(TOP_BUTTON, INPUT_PULLUP);
  pinMode(MIDDLE_BUTTON, INPUT_PULLUP);
  pinMode(BOTTOM_BUTTON, INPUT_PULLUP);
  pinMode(CONTACT_SWITCH, INPUT_PULLUP);

  Serial.println("resetting z axis");
  zAxisHoming();
  Serial.println("done resetting z axis");
  Serial.println("resetting x axis");
  xAxisHoming();
  Serial.println("done resetting x axis");

  wipe(ring.Color(0, 255, 0)); // green to indicate done setting up
}

void xAxisHoming() {
  xStepper.setCurrentPosition(0);
  xStepper.setMaxSpeed(2 * STEPS_PER_ROTATION);
  xStepper.setAcceleration(1000);
  xStepper.move(20000);

  while (digitalRead(CONTACT_SWITCH) == HIGH) { // while switch not pressed 
    xStepper.run();
  }
  
  delay(500);

  xStepper.move(-50);
  xStepper.setSpeed(-100);
  while(xStepper.distanceToGo() != 0) {
    xStepper.runSpeed();
  }

  xStepper.setCurrentPosition(0);
  xStepper.setMaxSpeed(4 * STEPS_PER_ROTATION);
  xStepper.setAcceleration(1000);
  
}

void zAxisHoming() {
  z1Stepper.setCurrentPosition(0);
  z1Stepper.setMaxSpeed(3 * STEPS_PER_ROTATION);
  z2Stepper.setCurrentPosition(0);
  z2Stepper.setMaxSpeed(3 * STEPS_PER_ROTATION);
  if(needToLower) {
    lowerPaddle(LOWERED_POSITION);
  } else {
    z1Stepper.setCurrentPosition(LOWERED_POSITION);
    z2Stepper.setCurrentPosition(LOWERED_POSITION);
  }
}
void lowerPaddle(int amt) {
  z1Stepper.moveTo(amt);
  z1Stepper.setSpeed(3 * STEPS_PER_ROTATION); // 3 rotations per second counter clockwise
  z2Stepper.moveTo(amt);
  z2Stepper.setSpeed(3 * STEPS_PER_ROTATION);

  while(z2Stepper.distanceToGo() != 0) {
    z1Stepper.runSpeed();
    z2Stepper.runSpeed();
  }
}

void raisePaddle(int amt) {
  z1Stepper.moveTo(amt);
  z1Stepper.setSpeed(-2 * STEPS_PER_ROTATION); // 2 rotations per second clockwise
  z2Stepper.moveTo(amt);
  z2Stepper.setSpeed(-2 * STEPS_PER_ROTATION);

  while(z2Stepper.distanceToGo() != 0) {
    z1Stepper.runSpeed();
    z2Stepper.runSpeed();
  }
}

void dispenseDrink() {
  raisePaddle(RAISED_POSITION);
  delay(1000);
  lowerPaddle(LOWERED_POSITION);
}

void dispenseFreeFlow(int d) {
  raisePaddle(FREE_FLOW_RAISED_POSITION);
  delay(d);
  lowerPaddle(LOWERED_POSITION);
}

void moveToDrink(int drink) {
  xStepper.moveTo(drink);
  while(xStepper.distanceToGo() != 0) {
    xStepper.run();
  }
}

void makeGrinchPunch() { // Sprite and Hawaiian Punch
  beginDrink();
  moveToDrink(SPRITE);
  delay(100);
  dispenseFreeFlow(2000);
  delay(100);
  moveToDrink(HAWAIIAN_PUNCH);
  delay(100);
  dispenseFreeFlow(2000);
  delay(100);
  endDrink();
}

void makeShirleyTemple() { // Ginger Ale and Syrup
  beginDrink();
  moveToDrink(GINGER_ALE);
  delay(100);
  dispenseFreeFlow(8000);
  delay(100);
  moveToDrink(SYRUP);
  delay(100);
  dispenseDrink();
  delay(100);
  endDrink();
}

void makeSprite() {
  beginDrink();
  moveToDrink(SPRITE);
  delay(100);
  dispenseFreeFlow(5000);
  delay(100);
  endDrink();
}

void makeGingerAle() {
  beginDrink();
  moveToDrink(GINGER_ALE);
  delay(100);
  dispenseFreeFlow(5000);
  delay(100);
  endDrink();
}

void makeHawaiianPunch() {
  beginDrink();
  moveToDrink(HAWAIIAN_PUNCH);
  delay(100);
  dispenseFreeFlow(5000);
  delay(100);
  endDrink();
}

void beginDrink() {
  isBusy = true;
  wipe(ring.Color(255, 0, 0));
}

void endDrink() {
  wipe(ring.Color(0, 255, 0));
  isBusy = false;
}


void wipe(unsigned long color) {
  ring.clear();
  ring.show();
  for(int i=0;i<ring.numPixels();i++) {
    ring.setPixelColor(i, color);
    delay(100);
    ring.show();
  }
}
