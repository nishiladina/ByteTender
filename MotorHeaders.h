#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>

// do not change these - see wiring guide
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
#define NEOP_PIN 25

#define LOWERED_POSITION 10 * STEPS_PER_ROTATION
#define FREE_FLOW_RAISED_POSITION -3 * STEPS_PER_ROTATION
#define RAISED_POSITION -4 * STEPS_PER_ROTATION

#define DRINK1 -750 // rightmost drink
#define DRINK2 -4000
#define DRINK3 -7250
#define DRINK4 -10700 // leftmost drink

// function prototypes - need one for each drink
void makeGrinchPunch();
void makeShirleyTemple();
void makeSprite();
void makeGingerAle();
void makeHawaiianPunch();
