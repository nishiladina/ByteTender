#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>

#define NEOP_PIN 25

// do not change these
#define ESTOP 15 // not needed
#define ASTEP 13
#define ADIR 12
#define BSTEP 14
#define BDIR 27
#define CSTEP 33
#define CDIR 32
#define CONTACT_SWITCH 4
#define TOP_BUTTON 34
#define MIDDLE_BUTTON 0
#define BOTTOM_BUTTON 35

#define STEPS_PER_ROTATION 800
#define LOWERED_POSITION 10 * STEPS_PER_ROTATION
#define RAISED_POSITION -4 * STEPS_PER_ROTATION

// change these depending on what motor gets plugged into A vs B vs C
#define XSTEP CSTEP
#define XDIR CDIR
#define Z1STEP ASTEP
#define Z1DIR ADIR
#define Z2STEP BSTEP
#define Z2DIR BDIR

void makeGrinchPunch();
void makeShirleyTemple();