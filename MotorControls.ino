#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define ESTOP 15 // not needed
#define XSTEP 13
#define XDIR 12
#define Z1STEP 14
#define Z1DIR 27
#define Z2STEP 33
#define Z2DIR 32
#define CONTACT_SWITCH 0 // ????

AccelStepper xStepper = AccelStepper(1, XSTEP, XDIR);
AccelStepper z1Stepper = AccelStepper(1, Z1STEP, Z1DIR);
AccelStepper z2Stepper = AccelStepper(1, Z2STEP, Z2DIR);

MultiStepper zControl = MultiStepper();

void motorSetup() {
  xStepper.setMaxSpeed(100);
  xStepper.setAcceleration(100);
  z1Stepper.setMaxSpeed(200);
  z2Stepper.setMaxSpeed(200);

  zControl.addStepper(z1Stepper);
  zControl.addStepper(z2Stepper);
}

void xAxisHoming() {
  int initialHoming = -1;
  while (digitalRead(CONTACT_SWITCH)) { // digitalRead returns 1 if not pressed, 0 if pressed
    xStepper.moveTo(initialHoming); // slowly move toward contact switch
    initialHoming--;
    xStepper.run();
    delay(5);
  }
  // now it is pressing the contact switch

  xStepper.setCurrentPosition(0);
  xStepper.setMaxSpeed(100); // must set max speed and acceleration after calling setCurrentPosition
  xStepper.setAcceleration(100);
  initialHoming = 1;

  while(!digitalRead(CONTACT_SWITCH)) { // while pressing switch
    xStepper.moveTo(initialHoming);
    initialHoming++;
    delay(5);
  }

  xStepper.setCurrentPosition(0); // this is its real home
  xStepper.setMaxSpeed(1000); // must set max speed and acceleration after calling setCurrentPosition
  xStepper.setAcceleration(500);
}

void dispenseDrink() {
  raiseDispenserPaddle();
  delay(2000);
  lowerDispenserPaddle();
}

void raiseDispenserPaddle() {
  long destination[] = {2000, 2000}; // no idea what these numbers need to be
  zControl.moveTo(destination);
  zControl.runSpeedToPosition(); // blocking
}

void lowerDispenserPaddle() {
  long destination[] = {0, 0};
  zControl.moveTo(destination);
  zControl.runSpeedToPosition(); // blocking
}