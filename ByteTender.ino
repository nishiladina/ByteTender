#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define ESTOP 15
#define XSTEP 13
#define XDIR 12
#define Z1STEP 14
#define Z1DIR 27
#define Z2STEP 33
#define Z2DIR 32

AccelStepper xStepper = AccelStepper(1, XSTEP, XDIR);
AccelStepper z1Stepper = AccelStepper(1, Z1STEP, Z1DIR);
AccelStepper z2Stepper = AccelStepper(1, Z2STEP, Z2DIR);

MultiStepper zControl = MultiStepper();

void setup() {
  xStepper.setMaxSpeed(1000);
  z1Stepper.setMaxSpeed(200);
  z2Stepper.setMaxSpeed(200);

  zControl.addStepper(z1Stepper);
  zControl.addStepper(z2Stepper);
}

void loop() {
}