bool needToLower = false; // CHANGE IF NEEDED

#include "WiFiHeaders.h"

using namespace std;

OTF::OpenThingsFramework *otf;

#include "MotorHeaders.h"

Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, NEOP_PIN, NEO_GRB + NEO_KHZ800); // 16 LEDs

AccelStepper xStepper = AccelStepper(AccelStepper::DRIVER, XSTEP, XDIR); // can replace AccelStepper::DRIVER with 1 if it doesn't work
AccelStepper z1Stepper = AccelStepper(AccelStepper::DRIVER, Z1STEP, Z1DIR);
AccelStepper z2Stepper = AccelStepper(AccelStepper::DRIVER, Z2STEP, Z2DIR);

bool isBusy = false;

void on_homepage(const OTF::Request &req, OTF::Response &res) {
  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/html")); 
  res.writeBodyChunk(FPSTR(ui_html)); // return the raw string defined in html_index.h
}


void on_stylesheet(const OTF::Request &req, OTF::Response &res) {
  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/css"));
  res.writeBodyChunk(FPSTR(ui_css)); // Return the raw CSS string
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
    Serial.printf("type: %s, value: %s\n", type, value);
    Serial.printf("compare: %d\n", strcmp(type, "drink"));
    if (strcmp(type, "drink") == 0) {
        int drinkType = stoi(value);
        Serial.printf("drinkType: %d, value: %s\n", drinkType, value);
        // 1: Grinch Punch
        // 2:
        // 3:
        // 4: 
        if(drinkType == 1) {
          Serial.println("making grinch punch");
          makeGrinchPunch();
        } else if (drinkType == 2) {
          Serial.println("making drink 2");
          // makeShirleyTemple();
        } else if (drinkType == 3) {
          Serial.println("making drink 3");
        } else if (drinkType == 4) {
          Serial.println("making drink 4");
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
  ring.clear(); // clear all pixels
  ring.show();

  

  // Start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi network...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500); 
  }
  Serial.print("\nConnected. Local IP: ");
  Serial.println(WiFi.localIP());

  // Create OTC Object
  otf = new OTF::OpenThingsFramework(LOCAL_SERVER_PORT, CLOUD_HOST, CLOUD_PORT, CLOUD_TOKEN, false);
  otf->localServerBegin();  // start local server

  // Set up server endpoints (APIs)
  otf->on(F("/"), on_homepage);
  otf->on(F("/get"), on_get);
  otf->on(F("/set"), on_set);
  otf->on(F("/styles.css"), on_stylesheet);

  motorSetup();
}
 
void loop(void){
  otf->loop();
}

void motorSetup() {
  Serial.println("beginning setup");

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

  rainbowCycle();
}

void xAxisHoming() {
  xStepper.setCurrentPosition(0);
  xStepper.setMaxSpeed(1600);
  xStepper.setAcceleration(1000);
  xStepper.move(20000);

  while (digitalRead(CONTACT_SWITCH) == HIGH) {
    xStepper.run();
  }
  
  delay(500);

  xStepper.move(-50);
  xStepper.setSpeed(-100);
  Serial.println(xStepper.distanceToGo());
  while(xStepper.distanceToGo() != 0) {
    xStepper.runSpeed();
  }

  xStepper.setCurrentPosition(0);
  xStepper.setMaxSpeed(3200);
  xStepper.setAcceleration(1000);
  
}

void zAxisHoming() {
  z1Stepper.setCurrentPosition(0);
  z1Stepper.setMaxSpeed(3 * STEPS_PER_ROTATION);
  z2Stepper.setCurrentPosition(0);
  z2Stepper.setMaxSpeed(3 * STEPS_PER_ROTATION);
  if(needToLower) {
    lowerPaddle();
  } else {
    z1Stepper.setCurrentPosition(LOWERED_POSITION);
    z2Stepper.setCurrentPosition(LOWERED_POSITION);
  }
}
void lowerPaddle() {
  z1Stepper.moveTo(LOWERED_POSITION);
  z1Stepper.setSpeed(3 * STEPS_PER_ROTATION); // 3 rotations per second counter clockwise
  z2Stepper.moveTo(LOWERED_POSITION);
  z2Stepper.setSpeed(3 * STEPS_PER_ROTATION);

  while(z2Stepper.distanceToGo() != 0) {
    z1Stepper.runSpeed();
    z2Stepper.runSpeed();
  }
}

void raisePaddle() {
  z1Stepper.moveTo(RAISED_POSITION);
  z1Stepper.setSpeed(-2 * STEPS_PER_ROTATION); // 2 rotations per second clockwise
  z2Stepper.moveTo(RAISED_POSITION);
  z2Stepper.setSpeed(-2 * STEPS_PER_ROTATION);

  while(z2Stepper.distanceToGo() != 0) {
    z1Stepper.runSpeed();
    z2Stepper.runSpeed();
  }
}

void dispenseDrink() {
  raisePaddle();
  delay(1000);
  lowerPaddle();
}

void timeDispenseDrink(int d) {
  raisePaddle();
  delay(d);
  lowerPaddle();
}

void moveToDrink1() { // rightmost drink
  Serial.println("moving to 1");
  xStepper.moveTo(-750);
  while(xStepper.distanceToGo() != 0) {
    xStepper.run();
  }
  Serial.println("done moving to 1");
}
void moveToDrink2() {
  Serial.println("moving to 2");
  xStepper.moveTo(-4000);
  while(xStepper.distanceToGo() != 0) {
    xStepper.run();
  }
  Serial.println("done moving to 2");
}
void moveToDrink3() {
  Serial.println("moving to 3");
  xStepper.moveTo(-7250);
  while(xStepper.distanceToGo() != 0) {
    xStepper.run();
  }
  Serial.println("done moving to 3");
}
void moveToDrink4() { // leftmost drink
  Serial.println("moving to 4");
  xStepper.moveTo(-10700);
  while(xStepper.distanceToGo() != 0) {
    xStepper.run();
  }
  Serial.println("done moving to 4");
}

void makeGrinchPunch() {
  isBusy = true;
  moveToDrink4();
  delay(100);
  dispenseDrink();
  delay(100);
  moveToDrink3();
  delay(100);
  dispenseDrink();
  delay(100);
  isBusy = false;
}

void makeShirleyTemple() {
  isBusy = true;
  moveToDrink1();
  delay(100);
  dispenseDrink();
  delay(100);
  moveToDrink2();
  delay(100);
  dispenseDrink();
  delay(100);
  isBusy = false;
}

void wipe(unsigned long color) {
  for(int i=0;i<ring.numPixels();i++) {
    ring.setPixelColor(i, color);
    delay(100);
  }
  ring.show();
}

void rainbow() {
  int i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel((i+j) & 255));
    }
    ring.show();
    delay(20);
  }
}

void rainbowCycle() {
  int i, j;
  for(j=0; j<256*3; j++) { // 3 cycles of all colors on wheel
    for(i=0; i< ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(((i * 256 / ring.numPixels()) + j) & 255));
    }
    ring.show();
    delay(20);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}